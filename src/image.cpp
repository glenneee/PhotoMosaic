#include "image.h"

Image::Image(int width, int height) : w(width), h(height){};

int Image::get_w()
{
    return w;
};

int Image::get_h()
{
    return h;
};
