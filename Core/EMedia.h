//
// Created by pspr1 on 1/5/2022.
//

#ifndef KALASOFT_EMEDIA_H
#define KALASOFT_EMEDIA_H

enum EKSMediaType{MEDIATYPE_UNKNOWN = -1,MEDIATYPE_VIDEO,MEDIATYPE_AUDIO};
enum EKSMediaFmt{MEDIAFMT_UNKNOWN = -1,PIXELFMT_UNKNOWN = -1,PIXELFMT_RGBA8 = 0};

typedef EKSMediaFmt EKSPixelFmt;
typedef EKSMediaFmt EKSSampleFmt;

#endif //KALASOFT_EMEDIA_H
