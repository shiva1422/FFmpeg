//
//  Logger.h
//  Vulkan
//
//  Created by shiva.pandiri on 11/5/22.
//

#ifndef Logger_h
#define Logger_h

#include "cstdarg"
#include "stdio.h"

class KSLog{
    
public:
    
    static void printf(const char* format,...)
    {
        ::printf("\n");
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
    
    static void info(const char* tag,const char* format,...)
    {
        ::printf("\n I :\t%s\t:\t",tag);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
    
    static void verbose(const char* tag,const char* format,...)
    {
        ::printf("\n V : \t%s\t:\t",tag);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
    
    static void debug(const char* tag,const char* format,...)
    {
        ::printf("\n D : \t %s\t:\t",tag);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
    
    static void error(const char* tag,const char* format,...)
    {
        ::printf("\nE :\t%s\t:\t",tag);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }

    static void warn(const char* tag,const char* format,...)
    {
        ::printf("\nW :\t%s\t:\t",tag);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
};



#endif /* Logger_h */
