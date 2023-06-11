//
// Created by Shiva Pandiri on 6/9/2022.
//

#ifndef KALASOFT_IDECODER_H
#define KALASOFT_IDECODER_H

#include <KSIO/IKSStream.h>
#include <CMedia/KSFrame.h>
#include "Memory.h"

class IDecoder{

public:

    virtual bool setSource(const IKSStream *reader) = 0;

    virtual KSFrame* readFrame() =0;

    virtual KSFrame* read() = 0;

    virtual int start() = 0;//

    virtual KSBuffer* readBuffer() = 0;//TOMemBuffer;

    virtual int64_t getStartTime() = 0;

    //totol mediafile duration in US
    virtual int64_t getDuration() = 0;
};
#endif //KALASOFT_IDECODER_H

