//
// Created by pspr1 on 5/20/2022.
//

#include <Logger.h>
#include "FFMpegUtils.h"

void FFMpegUtils::getMetaData(AVFormatContext *fmtCtx,std::map<char*,char*> &metadataMap)
{
    AVDictionary *dictionary = fmtCtx->metadata;
    AVDictionaryEntry *mapEntry = nullptr;
    while(mapEntry = av_dict_get(dictionary,"",mapEntry,AV_DICT_IGNORE_SUFFIX))
    {
        metadataMap.insert({mapEntry->key,mapEntry->value});
    }
}

void FFMpegUtils::showAvailableCodecs()
{
    AVCodec *iteration= nullptr;
    KSLog::info("FFMpegUtils","CodecNames :: start............");

    while(true)
    {
        AVCodec* presentCodec = const_cast<AVCodec *>(av_codec_iterate((void **) (&iteration)));


        if(presentCodec)//for  video 0;
        {
            const char* mediaType;
            if(presentCodec->type==AVMEDIA_TYPE_AUDIO) {
                mediaType = "audio";
            }
            else if(presentCodec->type==AVMEDIA_TYPE_VIDEO)
            {
                mediaType="video";
            }
            else
                mediaType="unknown";//text,data,nb

            KSLog::info("FFMpegUtils","decoder: %s , encoder:%s ,CodecName : %s ,MediaType is %s ", av_codec_is_decoder(presentCodec) ? "YES" : "NO", av_codec_is_encoder(presentCodec) ? "YES" : "NO", presentCodec->name,mediaType);
            // AVCodecDescriptor *descriptor=(AVCodecDescriptor *)avcodec_descriptor_get(presentCodec->id);
            //CodecLogI("descriptor name %s and long name : %s",descriptor->name,descriptor->long_name);


        }
        else
            break;


    }
    KSLog::info("FFMpegUtils","CodecNames :: end.........................");
}
