//
// Created by Shiva Pandiri on 6/13/2022.
//

#include <CMedia/KSFrame.h>
#include <Logger.h>
#include "FFFrameUtils.h"
#include "CMedia/KSImage.h"

#define TAGLOG "FFFrameUtils"

bool FFFrameUtils::avframeToBuffer(const AVFrame *frame, KSBuffer *buf, const SwsContext *ctx, const char *tag)
{
    int outLineSizes[4] = {frame->width * 4, 0, 0, 0};
    uint8_t* out[4] ={buf->data[0],NULL,NULL,NULL};
    sws_scale((SwsContext *)ctx, frame->data, frame->linesize, 0, frame->height, out, outLineSizes);
    //TODO return as requred;
    return true;
}

KSImage *FFFrameUtils::ffFrametoImage(const AVFrame *frame, const char *tag)
{
    return nullptr;
}

KSFrame *FFFrameUtils::convertAVFrameToKSFrame(const AVFrame *frame, SwsContext *ctx, const char *tag)
{
    bool clearCtx = false;
    KSFrame *ksFrame = nullptr;

    if(!ctx)
    {
        //TODO params to scalar
        ctx = sws_getContext(frame->width, frame->height, (AVPixelFormat)frame->format, frame->width, frame->height,AV_PIX_FMT_RGBA,SWS_BILINEAR,NULL,NULL,NULL);
        clearCtx = true;
    }
    if(ctx)
    {
        KSBuffer *buffer = MemAllocator::allocate(1,frame->width * frame->height * 4);//TODO size dynamic
        //TODO check if decoder can be forced to give out interleaved frame.

        if(buffer)
        {
            int outLineSizes[4] = {frame->width * 4, 0, 0, 0};
            uint8_t* out[4] ={buffer->data[0],NULL,NULL,NULL};

            //TODO in shaders when possible
            //return <=0 ?
            int height = sws_scale((SwsContext *)ctx, frame->data, frame->linesize, 0, frame->height, out, outLineSizes);
            //if expecter != h free buffer return null
            KSLog::debug(TAGLOG,"required height = %d ,scaled height = %d",frame->height,height);

            ksFrame = new(std::nothrow) KSFrame(buffer,MEDIATYPE_VIDEO,PIXELFMT_RGBA8);
            if(ksFrame)
            {
                ksFrame->setPrivData("height",frame->height);
                ksFrame->setPrivData("width",frame->width);
                //TODO ;aspectration,bytePos

            }
        }
    }
    else
        KSLog::error(TAGLOG,"error SWSCTX");


    if(clearCtx)
        sws_freeContext(ctx);

    return ksFrame;
}
