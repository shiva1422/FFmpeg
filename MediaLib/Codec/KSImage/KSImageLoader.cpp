//
// Created by Shiva Pandiri on 5/20/2022.
//

#include "KSImageLoader.h"

//TODO no just from IKStream or assets ,rotations,libJPEG,libPng,android loader //move?
KSImage KSImageLoader::loadFromAsset(const char* path)
{
    KSImage image;
    STBImageLoader::fromAsset(path,image);
    return image;
}
