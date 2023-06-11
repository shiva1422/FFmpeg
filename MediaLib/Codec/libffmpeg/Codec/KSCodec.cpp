//
// Created by Shiva Pandiri on 6/9/2022.
//

#include "IDecoder.h"
#include "KSCodec.h"
#include "memory"
#include "FFMpegDecoder.h"
#include "FFMpegAudioDecoder.h"
#include "FFMpegVideoDecoder.h"

IDecoder *KSCodec::createAudioDecoder()
{

    return static_cast<IDecoder *>(new(std::nothrow) FFMpegAudioDecoder());
}

IDecoder *KSCodec::createVideoDecoder()
{
    return new (std::nothrow) FFMpegVideoDecoder();
}
