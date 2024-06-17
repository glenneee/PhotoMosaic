#ifndef _GRAY_IMAGE_H_
#define _GRAY_IMAGE_H_

#include "image.h"

class GrayImage : public Image
{
private:
    int **pixels;

public:
    GrayImage();
    GrayImage(int width, int height, int **pixels);
    ~GrayImage();
    bool LoadImage(string filename);
    void DumpImage(string filename);
    void Display_X_Server();
    void Display_ASCII();
    void Display_CMD();
    GrayImage *BoxFilter(int k_size);
    GrayImage *Sobel_Gradient();
    GrayImage *Contrast_Stretching();
    GrayImage *Mosaic_Filter(int filter_size);
    GrayImage *LinearMotionBlur(int k_size, double angle);
    GrayImage *HistogramEqualization();
    GrayImage *GaussianBlur(int k_size, double sigma);
    GrayImage *MedianFilter(int k_size);
    GrayImage *Laplacian();
    GrayImage *Emboss();
    GrayImage *Invert();
    GrayImage *AdjustBrightness(int adjustment);
    GrayImage *SepiaTone();
};


#endif