//
// Created by Shiva Pandiri on 5/20/2022.
//

#ifndef KALASOFT_KSIMAGELOADER_H
#define KALASOFT_KSIMAGELOADER_H

#include "Codec/CMedia/KSImage.h"
#include "ImageCodec/STBImageLoader.h"

class KSImageLoader {
public:
    static KSImage loadFromAsset(const char* path);
};


#endif //KALASOFT_KSIMAGELOADER_H
