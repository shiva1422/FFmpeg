//
// Created by Shiva Pandiri on 6/12/2022.
//

#ifndef KALASOFT_FFMPEGVIDEODECODER_H
#define KALASOFT_FFMPEGVIDEODECODER_H

#include "Codec/CMedia/KSImage.h"
#include "KSDS/BlockingQueue.h"
#include "FFMpegDecoder.h"
#include "CMedia/KSFrame.h"

template class BlockingQueue<KSFrame*>;

class FFMpegVideoDecoder : public FFMpegDecoder{

public:

    FFMpegVideoDecoder();
    //IDecoder methods implementations
    KSFrame *readFrame() override;
    KSBuffer *readBuffer() override;
    KSFrame *read() override;
    int start() override;
    bool setSource(const IKSStream *input) override;

protected:

    void onFileInfoAvailable(const AVFormatContext *fmtCtx) override;

    void onStreamInfoAvailable(const AVFormatContext *fmtCtx,const AVStream *stream,const AVCodecContext *codecCtx) override;

    void onFrameAvailable(const AVFrame *frame) override;

protected:
    AVRational timeBase{1,1},frameRate{0,0};

private:
    SwsContext *swsContext = nullptr;
    AVPixelFormat inPixFmt,outPixFmt = AV_PIX_FMT_RGBA;
    BlockingQueue<KSFrame *> frames{5};
    BlockingQueue<KSBuffer *> buffers{5};
    BlockingQueue<KSImage *> images{5};

};


#endif //KALASOFT_FFMPEGVIDEODECODER_H
