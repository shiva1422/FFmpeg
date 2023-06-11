//
// Created by kalasoft on 11/6/21.
//

#include <cstdarg>
#include <mutex>
#include "Logger.h"
#include "FFMpegLogUtils.h"
#include "FFMpegFileUtils.h"
extern "C"
{
  #include "libavformat/avformat.h"
  #include "libavcodec/avcodec.h"
};
#include "FFMpegUtils.h"

static void avLogCallBack(void *ptr,int level,const char *fmt,va_list v1)//clalback av_dump_format;
{
    if(level>av_log_get_level())
        return;
    va_list v12;
    char line[1024];
    static int print_prefix=1;
    va_copy(v12,v1);
    av_log_format_line(ptr,level,fmt,v12,line,sizeof(line),&print_prefix);//level LOG_LEVEL?
    va_end((v12));
    KSLog::info("KSFFMpegLog ","%s",line);
}

void FFMpegLogUtils::setLogCallBack()
{
    //for other check this aptly;
    av_log_set_level(32);
    av_log_set_callback(avLogCallBack);
    KSLog::error("FFMpegLogUtils","setCallback");
}

void FFMpegLogUtils::printFileFormat(AVFormatContext *fmtCtx)
{
    //move to FFMpeg::init();
    FFMpegLogUtils::init();

    //ANDROID_LOG_INFO??setCallback check
    av_dump_format(fmtCtx,34,"",0);
}

void FFMpegLogUtils::init()
{
    ////TODO check to see if right and Move to FFMPEG::init;
    FFMpegLogUtils::setLogCallBack();
    FFMpegUtils::showAvailableCodecs();//TODO all initailization from one libffmpeg::init();
//    assert(false);

}

void FFMpegLogUtils::printFrameInfo(const AVFrame *frame)
{
    //TODO llds
    KSLog::verbose("FFFrameInfo :","pts - %lld\tbestEfforTimeStamp - %lld\t",frame->pts,frame->best_effort_timestamp);
}

void FFMpegLogUtils::printBuildConfig()
{
    KSLog::verbose("AVCodec Config","%s",avcodec_configuration());
}
