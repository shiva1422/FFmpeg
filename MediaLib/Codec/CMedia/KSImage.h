//
// Created by Shiva Pandiri on 5/21/2022.
//

#ifndef KALASOFT_KSIMAGE_H
#define KALASOFT_KSIMAGE_H

#include <cstdint>
#include "DMedia.h"
#include "EMedia.h"

//TODO DESts // force inline
class KSImage {
public:
   inline bool isValid()
    {
        //TODO check more
        return data != nullptr;
    }
public:
    uint8_t *data = nullptr;
    int chCnt = 4 ,width = 1,height =1;
    EKSPixelFmt pixelFmt = PIXELFMT_UNKNOWN;

};
#endif //KALASOFT_KSIMAGE_H
