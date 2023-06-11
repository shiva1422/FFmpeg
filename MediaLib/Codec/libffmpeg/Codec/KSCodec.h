//
// Created by Shiva Pandiri on 6/9/2022.
//

#ifndef KALASOFT_KSCODEC_H
#define KALASOFT_KSCODEC_H
#include "IDecoder.h"
//TODO inline or no abstract
class KSCodec {
public:
    //Audio only Decoder
    static IDecoder* createAudioDecoder();

    //Video Only decoder
    static IDecoder* createVideoDecoder();
};


#endif //KALASOFT_KSCODEC_H
