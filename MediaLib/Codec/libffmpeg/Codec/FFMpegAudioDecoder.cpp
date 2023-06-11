//
// Created by Shiva Pandiri on 6/12/2022.
//

#include <Logger.h>
#include "FFMpegAudioDecoder.h"

#define LOGTAG "FFmpegAudioDecoder"

FFMpegAudioDecoder::FFMpegAudioDecoder()
{
    this->mediaType = AVMEDIA_TYPE_AUDIO;
}

void FFMpegAudioDecoder::onFrameAvailable(const AVFrame *frame)
{
    FFMpegDecoder::onFrameAvailable(frame);
    //convert to 1024 sample MemBufs and Enque;
    KSLog::debug(LOGTAG,"audioFrameDecoded");


    ffBufIndex = 0;
    //TODO Decide Where to do the format conversion and Planar to SIngle here or in the app?
    int bytesPerSample = av_get_bytes_per_sample(sampleFmt);//cache
    if(!sampleBuf)
    {
        sampleBuf = MemAllocator::allocate(frame->channels, frameCnt * bytesPerSample);
        sampleBufIndex = 0;
        //if(!sampleBuf) //MemoryError;
    }



    /*
     * TODO memCpy;can make efficient?uint8s?copy single sample in iteration
     * also frame->nb_samples multiple of 4 so copy uint32 instead of uint8,and check with various file
     * can make MemeBuffer as IKStream for globalUsage; with position;
     */
    do{
        for(int i=0 ; i < bytesPerSample; ++i)
            for(int ch = 0;ch<frame->channels;++ch)
                sampleBuf->data[ch][sampleBufIndex * bytesPerSample + i] = frame->data[ch][ffBufIndex * bytesPerSample + i];

        ++ffBufIndex;
        ++sampleBufIndex;

        if(sampleBufIndex >= frameCnt)
        {

            sampleBufs.enque(sampleBuf);
            sampleBuf = nullptr;
            //channelCnt cache;
            sampleBuf = MemAllocator::allocate(frame->channels, frameCnt * bytesPerSample);
            sampleBufIndex = 0;
            //if(!sampleBuf) //MemoryError;

        }

    }while(ffBufIndex < frame->nb_samples);//TODO if frame has less samples,last frame?

}

void FFMpegAudioDecoder::onFileInfoAvailable(const AVFormatContext *fmtCtx)
{
    KSLog::verbose(LOGTAG,"streamCnt %d",fmtCtx->nb_streams);
}

void
FFMpegAudioDecoder::onStreamInfoAvailable(const AVFormatContext *fmtCtx,const AVStream *stream,const AVCodecContext *codecCtx)
{

    //TODO access directly through codecCtx in audio thread and uses get use below to get from outside;
    sampleFmt = codecCtx->sample_fmt;
    sampleRate = codecCtx->sample_rate;
    channelCnt = codecCtx->channels;


    KSLog::verbose(LOGTAG,"startTime %lf duration %ld in %lfsecs",(stream->start_time*stream->time_base.num*1.0/stream->time_base.den)/60.0,stream->duration,((stream->duration)*stream->time_base.num*1.0/stream->time_base.den)/60.0);
    KSLog::verbose(LOGTAG,"decoder streamInfo - sampleRate %d and channelCnt %d ",sampleRate,channelCnt);
}


//IDecoder meths override
KSBuffer *FFMpegAudioDecoder::readBuffer() {
    return sampleBufs.deque();
}

KSFrame *FFMpegAudioDecoder::readFrame() {
    return nullptr;
}

int FFMpegAudioDecoder::start() {
    return FFMpegDecoder::start();
}