//
// Created by Shiva Pandiri on 12/31/21.
//

#ifndef KALASOFT_KSFRAME_H
#define KALASOFT_KSFRAME_H

#include <DMedia.h>
#include <Commons.h>
#include "EMedia.h"
#include <Memory.h>
#include "unordered_map"
#include "string"
//vDes


class KSFrame {

public:

    KSFrame(){}

    KSFrame(KSBuffer *buf,EKSMediaType mType,EKSMediaFmt fmt)
    {
        this->buffer = buf;
        this->mediaType =  mType;
        this->format = fmt;
    }
    //pixel/sample format
    int getFormat() const{return format;};
    //audio/video etc
    int getMediaType() const {return mediaType;};

    void setType(EKSMediaType mediaType){ this->mediaType = mediaType;}

    void setFormat(EKSMediaFmt mediaFmt){ this->format = mediaFmt;}

    KSBuffer* getBuffer(){return  buffer;}

    void setPrivData(std::string key,int val)
    {
        privData.insert({key,val});
    }

    std::unordered_map<std::string,int> getPrivData(){return privData;}

public:
    int64_t pts = KS_NOPTS_VALUE,duration = 0;
    int flags;
protected:
    std::unordered_map<std::string,int> privData;
    KSBuffer *buffer    = nullptr;
    EKSMediaType mediaType = MEDIATYPE_UNKNOWN;
    EKSMediaFmt  format = MEDIAFMT_UNKNOWN;

};


#endif //KALASOFT_KSFRAME_H
