//
// Created by kalasoft on 12/29/21.
//

#include <Logger.h>
#include <unistd.h>
#include <cassert>
#include "FFMpegDecoder.h"
#include "../FFUtils/FFMpegUtils.h"
#include "Codec/libffmpeg/FFUtils/FFMpegLogUtils.h"

//TODO
#define INBUFSIZE 20480
#define FF_INPUT_BUFFER_PADDING_SIZE 64
#define LOGTAG "FFMpegDecoder"

/*
 * TODO
 * also buf size above for audio check
 * check callbacks accuracy,separate IOContexts for different sources in to one to FFIOCtx;
 */
static off64_t FFSeek(void *opaque, int64_t offset , int whence)
{
    return ((IKSStream *)opaque)->seek(offset,whence);

}

static off64_t FFRead(void *opaque, uint8_t *buf, int bufSize)
{
    return ((IKSStream *)opaque)->read(buf,bufSize);
}

FFMpegDecoder::FFMpegDecoder()
{

}
FFMpegDecoder::FFMpegDecoder(AVMediaType mediaType)
{
    this->mediaType = mediaType;
}



FFMpegDecoder::~FFMpegDecoder()
{
    //TODO

}

bool FFMpegDecoder::setSource(const IKSStream *input) {
    //TODO in thread//close stream
    if(!input)
        return false;

    this->input =(IKSStream *) input;
    this->input->seek(0,SEEK_SET);

    ioCtxBuf = (uint8_t *)av_malloc(INBUFSIZE + FF_INPUT_BUFFER_PADDING_SIZE);

    if(!ioCtxBuf)
    {
        KSLog::error(LOGTAG, "ioBuf alloc failed");
        goto free;
    }

    ioCtx = avio_alloc_context(ioCtxBuf, INBUFSIZE, 0, (void *) input,
                               reinterpret_cast<int (*)(void *, uint8_t *, int)>(FFRead),
                               nullptr, FFSeek);
    if(!ioCtx)
    {
        KSLog::error(LOGTAG, "IOContext alloc failed");
        goto free;

    }
    fmtCtx = avformat_alloc_context();
    if(!fmtCtx)
    {

        KSLog::error(LOGTAG, "formatCtx alloc failed");
        goto free;
    }

    fmtCtx->pb = ioCtx;
    fmtCtx->pb->eof_reached = 0;//as custom io;
    fmtCtx->flags |= AVFMT_FLAG_CUSTOM_IO;


    //TODO Check extensions,guessFormat
   // fmtCtx->iformat = av_find_input_format("mp4");
    fmtCtx->max_analyze_duration=INT64_MAX;//and one more;
    fmtCtx->probesize = INT32_MAX;

    if(avformat_open_input(&fmtCtx,"", fmtCtx->iformat, nullptr) < 0)
    {
        //close?
        KSLog::error(LOGTAG, "openInput:failed");
        goto free;//fmtCtx = null?
    }

    if(avformat_find_stream_info(fmtCtx,nullptr) < 0)
    {
        KSLog::error(LOGTAG, "findstreamInfo failed");
        goto free;
    }

    KSLog::info(LOGTAG, "fileFormat -");
    FFMpegLogUtils::printFileFormat(fmtCtx);

    // fileAVInfo.totalStreamCnt = fmtCtx->nb_streams;//later;
    // res = fillFileAVInfo(fmtCtx,fileAVInfo);
    onFileInfoAvailable(fmtCtx);
    //if false free?
    return getStreamInfoAndOpenCodec(fmtCtx);

    free:
    KSLog::error(LOGTAG, "freeing Contexts");
    if(ioCtxBuf)
        free(ioCtxBuf);
    if(fmtCtx)
        avformat_free_context(fmtCtx);
    if(ioCtx)
        avio_context_free(&ioCtx);
    //todo decide where to close; based on av_open_input above;
    // input->close();

    return false;
}

bool FFMpegDecoder::getStreamInfoAndOpenCodec(const AVFormatContext *fmtCtx) {

    if(!fmtCtx)
    {
        KSLog::error(LOGTAG, "getStreamInfoAndOpenCodec failed");
        return false;
    }
    int res;
    //this->mediaType = mediaType;
    index = av_find_best_stream((AVFormatContext *)fmtCtx, mediaType, -1, -1, &codec, 0);
    if(index < 0)
    {
        if(index == AVERROR_STREAM_NOT_FOUND)
        {
            KSLog::error(LOGTAG, "Couldnot find %s stream", av_get_media_type_string(mediaType));
        }
        else if(index == AVERROR_DECODER_NOT_FOUND)
        {
           KSLog::error(LOGTAG,"couldnot  find the codec for %s stream",av_get_media_type_string(mediaType));
        }
        //check other options if codec not found?
        goto fail;
    }

    stream = fmtCtx->streams[index];
    KSLog::verbose(LOGTAG,"find decoder %s", avcodec_get_name(stream->codecpar->codec_id));
  //  codec = avcodec_find_decoder_by_name("h264_mediacodec");//TODO fclear previouse codec
    if(!codec)//no need ? as index<0 above
    {
        codec = avcodec_find_decoder(stream->codecpar->codec_id);
    }
    if(codec)//no need as above
    {
        codecCtx = avcodec_alloc_context3(codec);
        if(!codecCtx)
        {
            KSLog::error(LOGTAG,"could not alloc codec context - ENOMEM");
            goto fail;
        }
        // codecCtx->workaround_bugs = 1;//?generally for encoders that cannot be automatically detected;

        if((res = avcodec_parameters_to_context(codecCtx,stream->codecpar)) < 0)
        {
            KSLog::error(LOGTAG,"codec params to context failed");
            goto fail;
        }

         codecCtx->pkt_timebase = stream->time_base;
        //av_log((void *)codecCtx,32,"CodecContext");
        //last options for setting private options for codec;
        //if force codec find codec (video_thread)
        if((res = avcodec_open2(codecCtx,codec, nullptr))< 0)
        {
            KSLog::error(LOGTAG,"openEncoder codec failed");
            goto fail;
        }
        //TODO Decide to alloc in init()? or elsewhere
        bOpened = true;
    }
    //TODO check other cases like INT64MIN,also for audio timebase as sampleRate?
    startTime = stream->start_time == AV_NOPTS_VALUE ? 0 : av_rescale_q(stream->start_time,stream->time_base,AVRational{1,KS_TIMEBASE});
    if(stream->duration != AV_NOPTS_VALUE)
        duration = av_rescale_q(stream->duration,stream->time_base,AVRational{1,KS_TIMEBASE});
    //TODO maybe wrong below,time base of stream?
    if(duration < 0)
        duration = av_rescale_q(fmtCtx->duration,stream->time_base,AVRational{1,KS_TIMEBASE});

    KSLog::verbose(LOGTAG,"the stream duration %lfsecs and starttime %lfsecs",duration/(1000000.0),startTime/(1000000.0));
    onStreamInfoAvailable(fmtCtx,stream,codecCtx);
    return true;

    fail:
    //TODO
    //clear codec context;
    //if(stream) stream->discard;?
    KSLog::error(LOGTAG,"getStreamInfoAndOpenCodec Failed for %s stream",av_get_media_type_string(mediaType));
    return false;
}

void *FFMpegDecoder::decodeFunc(FFMpegDecoder *decoder) {

    if(decoder)
    decoder->decode();
   // pthread_exit(NULL);//PThreadExit here or KSTHRead //TODO
   return NULL;
}

int FFMpegDecoder::start() {
    KSLog::debug("FFMPegDecoderBase starting","passed %s %d",__FUNCTION__ ,__LINE__);

    //ThreadManageMent
   // decodeThread.setThreadArgs(reinterpret_cast<void *(*)(void *)>(FFMpegDecoder::decodeFunc), this);
   // decodeThread.start();
    return 0;
}

int FFMpegDecoder::decode() {

    KSLog::debug(LOGTAG,"decode starting");
    if(!bOpened)
    {
        KSLog::error(LOGTAG,"open failed exiting decode thread");
        return -1;

    }
    frame = av_frame_alloc();
    if(!frame)
    {
        KSLog::error(LOGTAG, "frameAlloc failed");
        return -1;
    }
    if(!(packet = av_packet_alloc()))
    {
        KSLog::error(LOGTAG,"packet alloc failed");
        return -1;//free frame
    }
    //TODO remove
    /* std::map<char*,char*> metadataMap;
     FFMpegUtils::getMetaData(fmtCtx,metadataMap);
     for(auto iter = metadataMap.begin() ;iter != metadataMap.end();  ++iter)
     {
         KSLog::debug(LOGTAG," %s \t - \t %s",iter->first,iter->second);
     }*/
    //bActive Checks //TODO try open if !opened;


    int res;
    double time = 0.0;
    bool bPktRead = false,bEos = false;
    while (true)
    {
        KSLog::error("Decode","threadRunning %s",av_get_media_type_string(this->mediaType));
        //send packet to decoder;
        if(!resendPacket)
            bPktRead = readPacket();//if fails TODO

        if((res = avcodec_send_packet(codecCtx,packet))<0)
        {
            KSLog::warn("Decode","readPacketFail maybe EOS");
            //TODO res =
            //TODO resend packet if avsend fails resendPacket = true;
            if(res == AVERROR(EAGAIN))
                resendPacket = true;
            //else if  AVERROR_EOF,EINVAL,ENOMEM
            //elsif
            else
            av_packet_unref(packet);
        }
        else
            resendPacket = false;
        //receive decoded frame,TODO recieve frame first and send packet next better
        while(true)
        {
            res = avcodec_receive_frame(codecCtx,frame);
            if(res < 0)
            {
                if(res == AVERROR_EOF || res == AVERROR(EAGAIN))
                {
                    //no error
                    if(res == AVERROR_EOF)
                        bEos = true;
                }
                else
                {
                    KSLog::error(LOGTAG,"%s - recieveFrame error - %s",av_err2str(res),
                                 av_get_media_type_string(mediaType));
                }
                break;

            }

            //store frame for client frame null chck?
             onFrameAvailable(frame);
             KSLog::info(LOGTAG," decode time pts %lf secs samples %d",time + frame->pts *(double)stream->time_base.num/stream->time_base.den,frame->nb_samples);
            //used or move frame
             av_frame_unref(frame);

        }

        if(bEos)
            break;

    }
    //TODO free frame and packet
    KSLog::warn(LOGTAG,"end of decoding %s eos %d", av_get_media_type_string(this->mediaType),bEos);
    return 0;
}

bool FFMpegDecoder::readPacket() {
//TODO safe read formatContext ,for multiple streams safeRead or serparate Input files.
    while(av_read_frame(fmtCtx,packet) >= 0)
    {
        KSLog::verbose(LOGTAG,"packet read");
        if(index != packet->stream_index)
        {
            av_packet_unref(packet);
            continue;
        }
        return true;
    }
    //TODO EOF return? flush decoder with null packet after this(only if ending) else seek
    return false;

}



KSFrame * FFMpegDecoder::read() {
    //return after one Frame?decode;
    return 0;
}



void FFMpegDecoder::onFrameAvailable(const AVFrame *frame) {
    //TODO debug only
    FFMpegLogUtils::printFrameInfo(frame);
    KSLog::verbose(LOGTAG,"frame available of type %d",av_get_media_type_string(mediaType));
}

