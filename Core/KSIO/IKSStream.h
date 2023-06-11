//
// Created by kalasoft on 1/8/22.
//

#ifndef KALASOFT_IKSSTREAM_H
#define KALASOFT_IKSSTREAM_H


#include "stddef.h"
#include "sys/types.h"

/*
 * TODO destrucotr vir?
 * 32 && 64 bits.
 */

typedef off_t off64_t;//TODO
class IKSStream {

protected:
    void *dataSrc = nullptr;
public:
    ~IKSStream(){};
    //const;params
    virtual int   open(const char* path,int flags) = 0;
    virtual int   read(void *buf, size_t count) = 0;
    virtual off_t seek(off_t offset, int whence) = 0;
    virtual int   close() = 0;
    virtual void  setSource(void *dataSrc){this->dataSrc = dataSrc;}
    virtual long  getSize() = 0;
    virtual int64_t getCurrentPosition() = 0;
    virtual int eof() = 0;

};

//TODO convert all to fds?
class CustomIO{
public:
    //stream should not be null;
    static int read(void *stream,void *buf,size_t count)
    {
        return ((IKSStream *)stream)->read(buf,count);
    }
    static void skip(void *stream,off_t offset,int whence)
    {
          ((IKSStream *)stream)->seek(offset,whence);
    }
    static int eof(void *stream)
    {
       return ((IKSStream *)stream)->eof();
    }
    static off64_t seek(void *stream, int64_t offset, int whence)
    {
        return ((IKSStream *)stream)->seek(offset,whence);
    }
};

#endif //KALASOFT_IKSSTREAM_H
