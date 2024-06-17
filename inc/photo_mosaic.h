#ifndef _PHOTO_MOSAIC_H_
#define _PHOTO_MOSAIC_H_
#include <list>
#include "rgb_image.h"

class PhotoMosaic: public RGBImage{
    private: 
    vector<RGBImage *> photos;
    vector<string> photonames;
    RGBImage *big_photo;
    vector<vector<int>> rgb_color;
    int smallest_w;
    int smallest_h;

    public:
    PhotoMosaic();
    PhotoMosaic(string fn_src, string fn_dir);
    ~PhotoMosaic();
    void Create_Image();
};

#endif