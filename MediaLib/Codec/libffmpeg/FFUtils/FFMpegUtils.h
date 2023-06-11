//
// Created by pspr1 on 5/20/2022.
//

#ifndef KALASOFT_FFMPEGUTILS_H
#define KALASOFT_FFMPEGUTILS_H

extern "C"
{
#include <libavcodec/codec.h>
#include <libavcodec/avcodec.h>
#include "libavformat/avformat.h"
//#include "libswscale/swscale.h"
//#include <libavutil/channel_layout.h>
//#include <libavutil/imgutils.h>
//#include "libavutil/frame.h"
//#include <libavutil/common.h>
};
#include "map"
class FFMpegUtils {
public:
    static void getMetaData(AVFormatContext *fmtCtx,std::map<char*,char*> &metaMap);
    static void showAvailableCodecs();
};


#endif //KALASOFT_FFMPEGUTILS_H
