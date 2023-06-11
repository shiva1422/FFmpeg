//
// Created by Shiva Pandiri on 6/13/2022.
//

#ifndef KALASOFT_FFFRAMEUTILS_H
#define KALASOFT_FFFRAMEUTILS_H

#include "Memory.h"
#include "CMedia/KSImage.h"
extern "C"
{
#include <libavcodec/codec.h>
#include <libavcodec/avcodec.h>
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include <libavutil/channel_layout.h>
#include <libavutil/imgutils.h>
#include "libavutil/frame.h"
#include <libavutil/common.h>
};
class FFFrameUtils {
public:

    /*
     * convert AVFrame to KSBuffer ,the buffer would be RGBA8888 interleaved always.params should be null checked
     * later support other formats if needed
     * TODO swscontext can be createdd within this need not from outside
     */
    static bool avframeToBuffer(const AVFrame *frame, KSBuffer *buf,  const SwsContext *ctx, const char *tag);

    /*
     * This will not unref the frame ,check if SwsContext should be passed as well for iterative usage.
     * returned image will always be RGBA8888 for now.
     */
    static KSImage *ffFrametoImage(const AVFrame *frame, const char *tag = "");


    static KSFrame *convertAVFrameToKSFrame(const AVFrame *frame, SwsContext *ctx = nullptr, const char* tag = "");
};


#endif //KALASOFT_FFFRAMEUTILS_H
