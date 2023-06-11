//
// Created by kalasoft on 11/6/21.
//

#ifndef KALASOFT_FFMPEGLOGUTILS_H
#define KALASOFT_FFMPEGLOGUTILS_H


#include "FFMpegFileUtils.h"

class AVFormatContext;
class AVFrame;

class FFMpegLogUtils {
public:
    static void setLogCallBack();

    static void printFileFormat(AVFormatContext *fmtCtx);

    static void init();

    static void printFrameInfo(const AVFrame *frame);

    static void printBuildConfig();
};


#endif //KSPLAYER_FFMPEGLOGUTILS_H
