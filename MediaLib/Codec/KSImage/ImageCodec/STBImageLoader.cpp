//
// Created by Shiva Pandiri on 5/21/2022.
//

#include "new"
#include <KSIO/IKSStream.h>
#include <CMedia/KSImage.h>
#include <Logger.h>

extern "C"
{
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

}
#include "STBImageLoader.h"
#define LOGTAG "STBImage"

stbi_io_callbacks assetIOCallbacks ={.read = reinterpret_cast<int (*)(void *, char *,int)>(CustomIO::read),.skip=reinterpret_cast<void (*)(void *, int)>(CustomIO::skip),.eof = CustomIO::eof};

//TODO return bool
void STBImageLoader::fromAsset(const char *path,KSImage &image)
{
    //TODO desired channels,setting all params of image correctly and set PIXEL format accordingly
    IKSStream *stream = nullptr ;//AssetManager::openAsset(path);
    if(stream)
    {
        //stbi_set_flip_vertically_on_load(true);
        stbi_info_from_callbacks(&assetIOCallbacks, stream, &image.width, &image.height, &image.chCnt);
        stream->seek(0,SEEK_SET);
        image.data = stbi_load_from_callbacks(&assetIOCallbacks,stream, &image.width, &image.height, &image.chCnt, 4);
        stream->close();
        delete stream;
        stream = nullptr;
        if(image.data == NULL)
        {
            KSLog::error(LOGTAG,"%s KS_DECODE ERROR",path);
            return;
        }
        //TODO other formats if based on STB
        image.pixelFmt = PIXELFMT_RGBA8;
    }
    else
        KSLog::error(LOGTAG,"couldn't open asset");
}
