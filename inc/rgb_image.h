#ifndef _RGB_IMAGE_H_
#define _RGB_IMAGE_H_

#include "image.h"
#include "gray_image.h" //for functions grey_to_colourful
class RGBImage : public Image
{
private:
    int ***pixels;

public:
    RGBImage();
    RGBImage(int width, int height, int ***pixels);
    ~RGBImage();
    bool LoadImage(string filename);
    void DumpImage(string filename);
    void Display_X_Server();
    void Display_ASCII();
    void Display_CMD();
    RGBImage *BoxFilter(int filter_size);
    RGBImage *Sobel_Gradient();
    RGBImage *Contrast_Stretching();
    RGBImage *Mosaic_Filter(int filter_size);
    RGBImage *LinearMotionBlur(int k_size, double angle);
    RGBImage *HistogramEqualization();
    RGBImage *GaussianBlur(int k_size, double sigma);
    RGBImage *MedianFilter(int k_size);
    RGBImage *Laplacian();
    RGBImage *Emboss();
    RGBImage *Invert();
    RGBImage *AdjustBrightness(int adjustment);
    RGBImage *SepiaTone();
    friend class PhotoMosaic;
};

#endif