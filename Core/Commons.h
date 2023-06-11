//
// Created by shiva.pandiri on 12/18/21.
//

#ifndef KALASOFT_COMMONS_H
#define KALASOFT_COMMONS_H

#include "stdint.h"

#define KSFORCEINLINE inline __attribute__((always_inline))
#define KSINLINE inline
#define KSPRINTF printf

/***TODO move to kBits.h or other bitoperations**/
KSFORCEINLINE static bool isLittleEndian()
{
    unsigned int i = 1;
    char *c = (char*)&i;
    return (*(c) ? true : false);
}

KSFORCEINLINE static void swapEndian32(uint8_t *val)
{
    //TODO check is best way?
    uint8_t tmp ;
    //swap 1 and 4 bytes
    tmp = *(val);
    *(val) = *(val + 3);
    *(val + 3) = tmp;
    //swap 2 and 3
    val += 1;
    tmp = *(val);
    *(val) = *(val + 1);
    *(val + 1) = tmp;
}

KSFORCEINLINE static void swapEndian16(uint8_t *val)
{
    uint8_t tmp;
    tmp = *(val);
    *(val) = *(val+1);
    *(val+1) = tmp;
}
/** KBits.h**/

#endif //KALASOFT_COMMONS_H
