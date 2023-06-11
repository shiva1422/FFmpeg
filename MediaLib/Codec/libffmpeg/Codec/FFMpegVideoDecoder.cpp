//
// Created by Shiva Pandiri on 6/12/2022.
//

#include <Logger.h>
#include "FFMpegVideoDecoder.h"
#include "../FFUtils/FFFrameUtils.h"
#include "CMedia/KSFrame.h"

#define TAGLOG "FFmpegVideoDecoder"



FFMpegVideoDecoder::FFMpegVideoDecoder()
{
    this->mediaType = AVMEDIA_TYPE_VIDEO;
}
void FFMpegVideoDecoder::onFrameAvailable(const AVFrame *frame)
{
    KSLog::verbose(TAGLOG,"frameAvailable");
    /*if (decoder_reorder_pts == -1) {
        frame->pts = frame->best_effort_timestamp;
    } else if (!decoder_reorder_pts) {
        frame->pts = frame->pkt_dts;
    }*/
    //TODO Frame order,first frame,isKeyFrame?
    FFMpegDecoder::onFrameAvailable(frame);
    //TODO frameDrop here; or after conversion or taking conversion time in
    /*
     * if (got_picture) {
        double dpts = NAN;

        if (frame->pts != AV_NOPTS_VALUE)
            dpts = av_q2d(is->video_st->time_base) * frame->pts;

        frame->sample_aspect_ratio = av_guess_sample_aspect_ratio(fmtCtx, stream, frame);

        if (bframedrop || (bframedrop && get_master_sync_type(is) != AV_SYNC_VIDEO_MASTER)) {
            if (frame->pts != AV_NOPTS_VALUE) {
                double diff = dpts - get_master_clock(is);
                if (!isnan(diff) && fabs(diff) < AV_NOSYNC_THRESHOLD &&
                    diff - is->frame_last_filter_delay < 0 &&
                    is->viddec.pkt_serial == is->vidclk.serial &&
                    is->videoq.nb_packets) {
                    is->frame_drops_early++;
                    av_frame_unref(frame);
                    got_picture = 0;
                }
     */
    //TODOconvert AVFrame to KSFrame/Texture here for now KSImage , also swsContext get only if formats change;
    //TODO parameterize Scaling type,check filters
    if(!swsContext)
    swsContext = sws_getContext(frame->width, frame->height, inPixFmt, frame->width, frame->height,outPixFmt,SWS_BILINEAR,NULL,NULL,NULL);
    //TODO if format changes need to free and get new swscontex;
    KSFrame *ksFrame = FFFrameUtils::convertAVFrameToKSFrame(frame,swsContext,__FUNCTION__ );
    if(ksFrame)
    {
        //TODO no need double direct multiplay with KSTIMEBASE
         ksFrame->duration = (frameRate.num && frameRate.den) ? av_q2d((AVRational){frameRate.den, frameRate.num}) * KS_TIMEBASE : 0;
         ksFrame->pts = (frame->pts == AV_NOPTS_VALUE) ? KS_NOPTS_VALUE : frame->pts * av_q2d(timeBase) * KS_TIMEBASE;
         KSLog::debug(TAGLOG,"pts %ld timebase %ld/%ld %ld ksFrame->pts %lf",frame->pts,timeBase.num ,timeBase.den,ksFrame->pts/1000000.0);
         frames.enque(ksFrame);
    }

    else
        KSLog::error(TAGLOG,"error convert AVToKSFrame");
}

void FFMpegVideoDecoder::onFileInfoAvailable(const AVFormatContext *fmtCtx)
{

}

void FFMpegVideoDecoder::onStreamInfoAvailable(const AVFormatContext *fmtCtx,const AVStream *stream,const AVCodecContext *codecCtx)
{
    //TODO check if get changed in onFrameAvailbale
    if(codecCtx->pix_fmt == AV_PIX_FMT_NONE)
    {
        KSLog::warn(TAGLOG,"unKnownPixelformat %s", av_get_media_type_string(mediaType));
    }
     KSLog::debug(TAGLOG,"videoStreamInfo available");
     inPixFmt = codecCtx->pix_fmt;
     frameRate = av_guess_frame_rate((AVFormatContext *)fmtCtx,(AVStream *)stream,NULL);
     timeBase = stream->time_base;
}
KSFrame *FFMpegVideoDecoder::readFrame()
{
    KSLog::verbose(TAGLOG,"readFrame");
    return frames.deque();
}

KSBuffer *FFMpegVideoDecoder::readBuffer() {
    return buffers.deque();
}

KSFrame *FFMpegVideoDecoder::read() {
    return FFMpegDecoder::read();
}

int FFMpegVideoDecoder::start() {
    return FFMpegDecoder::start();
}

bool FFMpegVideoDecoder::setSource(const IKSStream *input) {
    return FFMpegDecoder::setSource(input);
}




