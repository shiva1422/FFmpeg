//
// Created by kalasoft on 11/6/21.
//

#ifndef KALASOFT_FFMPEGFILEUTILS_H
#define KALASOFT_FFMPEGFILEUTILS_H

#include <map>
#include "CMedia/KSImage.h"


class AVFormatContext;

class FFMpegFileUtils {
public:

    //TODO as thumnbnails as map?
    static bool getCoverPic(const char* path,std::vector<KSImage> &thumbnails,const bool bAsset = false);

};


#endif //KALASOFT_FFMPEGFILEUTILS_H
