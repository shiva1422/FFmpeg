//
// Created by kalasoft on 12/29/21.
//

#ifndef KALASOFT_FFMPEGDECODER_H
#define KALASOFT_FFMPEGDECODER_H


#include "KSIO/IKSStream.h"
#include "IDecoder.h"
#include <DMedia.h>

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

/*TODO make decoder interface and set audio and video types in the sub implementations
 * TODO startTime Considerations?end frame handling.
 * Make header not visible to outside
 * ***Keep Common Parts in FFMpegDecoder and create SubClasses for audio and videoDecoder.
 */
class FFMpegDecoder : public IDecoder{/*FFMpegMediaStreamDecoder only single stream*/

public:

    FFMpegDecoder();
    FFMpegDecoder(AVMediaType mediaType);
    virtual ~FFMpegDecoder();
    int start();


    //TODO closeInput
    //bool openInputWithFD(const int fd);
    //keep src to only one as below ,implement reader for fd,asset and other externally;
    bool setSource(const IKSStream *input);

    //StreamInfo
    //in KSTIMEBASE - not thread safe ,try to get before starting decoder
    int64_t getStartTime() override{return startTime;};
    int64_t getDuration() override{return duration;}

    KSFrame *readFrame() override;
    KSFrame *read() override;
    KSBuffer *readBuffer() override;;

protected:
    //Input
    virtual void onFileInfoAvailable(const AVFormatContext* fmtCtx)=0;
    virtual void onStreamInfoAvailable(const AVFormatContext *fmtCtx,const AVStream *stream,const AVCodecContext *codecCtx) =0;

    AVMediaType mediaType = AVMEDIA_TYPE_UNKNOWN;
protected:
    //Output
    //TODO onFrameAvailable virtual or function for each mediatype frame availabel?;
    virtual void onFrameAvailable(const AVFrame *frame);

protected:

   // KSThread decodeThread;
    static void* decodeFunc(FFMpegDecoder *audioDecoder);
    int decode();
    bool readPacket();

private:
    bool getStreamInfoAndOpenCodec(const AVFormatContext *fmtCtx);

private:

    //all params may be not requried used directly based on stream Index;
    //iolock for multiple streams? or sepearate files,open same file multiple times;
    AVFormatContext     *fmtCtx = nullptr;
    AVStream            *stream = nullptr;
    AVCodecContext      *codecCtx = nullptr;
    const AVCodec       *codec = nullptr;
    AVPacket            *packet = nullptr;
    AVFrame             *frame = nullptr;
    int64_t             startTime= KS_NOPTS_VALUE,duration = KS_NOPTS_VALUE;
    int                 index = -1;
    //TODO check set at all places /opened codec?
    bool                bOpened = false;
    bool                resendPacket = false;

    //IO - emliminate by convert fd to path?
    IKSStream *input = nullptr;
    AVIOContext         *ioCtx = nullptr;
    uint8_t             *ioCtxBuf = nullptr;


};


#endif //KALASOFT_FFMPEGDECODER_H
