//
// Created by kalasoft on 11/6/21.
//

#include "FFMpegFileUtils.h"
#include "FFMpegLogUtils.h"
#include <functional>
#include <unistd.h>
#include <mutex>
#include <KSIO/IKSStream.h>
#include <Logger.h>
#include "map"
#include "vector"
extern std::once_flag avLogCallbackFlagSetupOnce;
extern "C"
{
#include <libavformat/avformat.h>
#include "libswscale/swscale.h"
#include <libavcodec/avcodec.h>
}


#define LOGTAG "FFMpegFileUtils"
#define INBUFSIZE 20480
#define FF_INPUT_BUFFER_PADDING_SIZE 64

/*
 * TODO
 * check callbacks accuracy,move to FFIOCtx;
 */
static off64_t ffSeekFD(void *opaque,int64_t offset , int whence)
{
    int fd = *(int *)opaque;
    return  lseek(fd,offset,whence);
};

static off64_t ffReadPacketFD(void *opaque, uint8_t *buf, int bufSize)
{
    KSLog::info(LOGTAG, "ffreadPacket");
    int fd = *(int *)opaque;
    bufSize=FFMIN(bufSize, INBUFSIZE);//check
    if(!bufSize)
        return AVERROR_EOF;
    int readSize = read(fd,buf,bufSize);

    if(readSize == 0)
    {
        return AVERROR_EOF;//also < o on error;
    }
    return readSize;
};


bool FFMpegFileUtils::getCoverPic(const char *path,std::vector<KSImage> &thumbnails, const bool bAsset) {
    //TODO REWRITE ENTIRE THING EFFIECIENTLY
    //TODO now only does AssetIO also cutom io might not be right for asset check ffReadPacket above
    IKSStream *input;
    if(bAsset)
    {
        //input = AssetManager::openAsset(path);//TODO
        if(!input)
        {
            KSLog::error(LOGTAG,"could not open Asset %s  %d",__FILE__,__LINE__);
            return false;
        }
    }
    bool res = false;



    //TODO openInput to separate function get FMTCtx
    AVFormatContext *fmtCtx = nullptr;
    AVIOContext *ioCtx = nullptr;


    uint8_t* ioCtxBuf = (uint8_t *)av_malloc(INBUFSIZE + FF_INPUT_BUFFER_PADDING_SIZE);

    if(!ioCtxBuf)
    {
        KSLog::error(LOGTAG, "ioBuf alloc failed");
        return false;
    }

    ioCtx = avio_alloc_context(ioCtxBuf, INBUFSIZE, 0, (void *) input,
                               reinterpret_cast<int (*)(void *, uint8_t *, int)>(CustomIO::read),
                               nullptr, CustomIO::seek);
    if(!ioCtx)
    {
        KSLog::error(LOGTAG, "IOContext alloc failed");
        return false;

    }
    fmtCtx = avformat_alloc_context();
    if(!fmtCtx)
    {

        KSLog::error(LOGTAG, "formatCtx alloc failed");
        return false;
    }

    fmtCtx->pb = ioCtx;
    fmtCtx->pb->eof_reached = 0;//as custom io;
    fmtCtx->flags |= AVFMT_FLAG_CUSTOM_IO;


    //TODO Check extensions
    fmtCtx->iformat = av_find_input_format("mp3");
    //fmtCtx->max_analyze_duration=INT64_MAX;
    //fmtCtx->max_ts_probe=INT32_MAX;

    if(avformat_open_input(&fmtCtx,"",NULL,NULL) < 0)
    {
        KSLog::error(LOGTAG, "openInput:failed");
        return false;
    }

    if(avformat_find_stream_info(fmtCtx,NULL) < 0)
    {
        KSLog::error(LOGTAG, "findstreamInfo failed");
        return false;
    }

    KSLog::info(LOGTAG, "fileFormat -");
    FFMpegLogUtils::printFileFormat(fmtCtx);

    // fileAVInfo.totalStreamCnt = fmtCtx->nb_streams;//later;

  //  res = fillFileAVInfo(fmtCtx,fileAVInfo);

  AVPacket *pkt = av_packet_alloc();
  AVFrame * frame = av_frame_alloc();
  if(!pkt)
      return false;//clear all initialiations
      int ret;
      AVStream *stream = nullptr;
      const AVCodec *codec = nullptr;
       AVCodecContext *codecCtx = nullptr;
  for(int i = 0;i<fmtCtx->nb_streams;++i)
  {
      //TODO only videoStream check
      //clear codec context close etc each iteration;

      stream = fmtCtx->streams[i];
      if(stream->disposition & AV_DISPOSITION_ATTACHED_PIC) {
          //has thumbnail
          if ((ret = av_packet_ref(pkt, &stream->attached_pic)) < 0)
              continue;
          codec = avcodec_find_decoder(stream->codecpar->codec_id);

          if (codec)//no need as above
          {
              codecCtx = avcodec_alloc_context3(codec);
              if (!codecCtx) {
                  KSLog::error(LOGTAG, "could not alloc codec context - ENOMEM");
                  continue;
              }
              // codecCtx->workaround_bugs = 1;//?generally for encoders that cannot be automatically detected;

              if ( avcodec_parameters_to_context(codecCtx, stream->codecpar) < 0) {
                  KSLog::error(LOGTAG, "codec params to context failed");
                  continue;
              }
              //av_log((void *)codecCtx,32,"CodecContext");
              //last options for setting private options for codec;
              if ((res = avcodec_open2(codecCtx, codec, nullptr)) < 0) {
                  KSLog::error(LOGTAG, "openEncoder codec failed");
                  continue;
              }
              AVPacket *sentPkt = nullptr;
              bool decoded = false;
              while (true) {
                  if (decoded)
                      break;
                  if (avcodec_send_packet(codecCtx, pkt) < 0) {
                      KSLog::warn(LOGTAG, "readPacketFail maybe EOS");
                      //TODO res =
                      break;
                  }
                  sentPkt = pkt;
                  pkt = nullptr;
                  //receive decoded frame
                  while (true) {
                      res = avcodec_receive_frame(codecCtx, frame);
                      if (res < 0) {
                          if (res == AVERROR_EOF || res == AVERROR(EAGAIN)) {
                              //no error
                              res = 0;
                          }
                              //declare EOS ?
                          else
                              KSLog::error("Decode Packet................", "Error decodeing - %s",
                                           av_err2str(res));

                          break;

                      }
                      SwsContext *swsContext = sws_getContext(frame->width, frame->height,
                                                              static_cast<AVPixelFormat>(frame->format), frame->width, frame->height, AV_PIX_FMT_RGB0, SWS_BICUBLIN, NULL, NULL, NULL);
                      KSImage image;
                      image.width = frame->width;
                      image.height = frame->height;
                      image.pixelFmt = PIXELFMT_RGBA8;
                      image.data = (uint8_t *)malloc(frame->width * frame->height *4);
                      if(!image.data)
                          return false;
                      //TODO all imageparams
                      int outLineSizes[4] = {frame->width * 4, 0, 0, 0};//TODO 4 numchannels
                      uint8_t* out[4] ={image.data,NULL,NULL,NULL};
                      if(swsContext)
                      {

                          sws_scale(swsContext, frame->data, frame->linesize, 0, image.height, out, outLineSizes);
                      }
                      if(image.isValid())
                          thumbnails.push_back(std::move(image));//TODO
                          else
                              KSLog::error(LOGTAG,"getCOverpics could not convert image");
                      av_frame_unref(frame);
                      pkt = sentPkt;
                      av_packet_unref(pkt);
                      decoded = true;
                      break;


                      //clear all

                  }
              }


          }
      }
  }

  delete input;
  input = nullptr;
  return true;

    free:
    KSLog::info(LOGTAG, "freeing Contexts");
    if(ioCtxBuf)
        free(ioCtxBuf);
    if(fmtCtx)
        avformat_free_context(fmtCtx);
    if(ioCtx)
        avio_context_free(&ioCtx);


    //close(fd);//TODO decide

    return false;
}

