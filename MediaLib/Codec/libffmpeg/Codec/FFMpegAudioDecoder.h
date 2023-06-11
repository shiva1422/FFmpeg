

//
// Created by Shiva Pandiri on 6/12/2022.
//

#ifndef KALASOFT_FFMPEGAUDIODECODER_H
#define KALASOFT_FFMPEGAUDIODECODER_H


#include "FFMpegDecoder.h"
#include "KSDS/BlockingQueue.h"
#include "Memory.h"


class FFMpegAudioDecoder : public FFMpegDecoder{

public:

    FFMpegAudioDecoder();

public:

    //Implement methods from IDecoder
    KSBuffer *readBuffer() override;
    KSFrame *readFrame() override;


    int start() override;

private:
    void onFileInfoAvailable(const AVFormatContext *fmtCtx) override;
    void onStreamInfoAvailable(const AVFormatContext *fmtCtx,const AVStream *stream,const AVCodecContext *codecCtx) override;
    void onFrameAvailable(const AVFrame *frame) override;


private:

    BlockingQueue<KSBuffer *> sampleBufs{5};//TODO KSFrame

    KSBuffer *sampleBuf = nullptr;
    AVSampleFormat sampleFmt;
    int sampleRate = 44100,channelCnt = 2;
    //default frameSize;
    const int frameCnt = 1024;
    //store//for now audio only;
    //temporary Store;

    int sampleBufIndex = 0;
    int ffBufIndex = 0;

};


#endif //KALASOFT_FFMPEGAUDIODECODER_H
