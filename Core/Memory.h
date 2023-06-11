//
// Created by kalasoft on 1/9/22.
//

#ifndef KALASOFT_MEMORY_H
#define KALASOFT_MEMORY_H

#include <cstdint>
#include "cstdlib"
#include "memory"
class KSBuffer{
public:

    int channelCnt = 0;
    int sizes[8];
    uint8_t* data[8];

    ~KSBuffer()
    {
        //TODO
        for(int i = 0 ; i< channelCnt ;++i)
        {
            if(data[i])
                free(data[i]);
            data[i] = NULL;

        }
    }
};
class MemAllocator {

public:
    //create memory buffer of  channelCnt channels and  byteCount bytes for each channel;
    static KSBuffer* allocate(int channelCnt, int byteCount)
    {
        auto buffer = new(std::nothrow) KSBuffer();
        if(buffer)
        {
            buffer->channelCnt = channelCnt;
            for(int i = 0;i < channelCnt;++i)
            {
                //TODO check;
                buffer->data[i] = (uint8_t *)(malloc(byteCount));
                buffer->sizes[i] = byteCount;
            }
        }
        return buffer;
    }
};



#endif //KALASOFT_MEMORY_H
