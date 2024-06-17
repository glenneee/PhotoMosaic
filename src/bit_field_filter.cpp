// using bitfield to not to force user to passing all of the arguments
// using bitwise or to passing the options
// using bitwise and to get the info of the bitfield

#include "bit_field_filter.h"
#include "gray_image.h"
#include "rgb_image.h"

// using bitwise and to track whtat is the user's option
void loadCase(int32_t option)
{
    if (option & CASE_ONE)
        printf("Case 1 detected\n");
    if (option & CASE_TWO)
        printf("Case 2 detected\n");
    if (option & CASE_THREE)
        printf("Case 3 detected\n");
    if (option & CASE_FOUR)
        printf("Case 4 detected\n");
    printf("\n");
    printAndResult(option);
}

void printAndResult(int32_t option)
{
    static int cnt = 1;
    //GrayImage
    GrayImage *img = new GrayImage();
    img->LoadImage("Image-Folder/lena.jpg");
    //RGBImage
    RGBImage *img_2 = new RGBImage();
    img_2->LoadImage("Image-Folder/insane.jpg");
    if (option & CASE_ONE)
    {
        printf("Case 1 detection\n");
        //GrayImage
        GrayImage *img_out = img->BoxFilter(7);
        img_out->Display_CMD();
        delete img;
        img = nullptr;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->BoxFilter(7);
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_TWO)
    {
        printf("Case 2 detected\n");
        //GrayImage
        GrayImage *img_out = img->Sobel_Gradient();
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->Sobel_Gradient();
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_THREE)
    {
        printf("Case 3 detected\n");
        //GrayImage
        GrayImage *img_out = img->Contrast_Stretching();
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->Contrast_Stretching();
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_FOUR)
    {
        printf("Case 4 detected\n");
        //GrayImage
        GrayImage *img_out = img->Mosaic_Filter(3);
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->Mosaic_Filter(3);
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_FIVE)
    {
        printf("Case 5 detected\n");
        //GrayImage
        GrayImage *img_out = img->LinearMotionBlur(15, 45);
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->LinearMotionBlur(15, 45);
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_SIX)
    {
        printf("Case 6 detected\n");
        //GrayImage
        GrayImage *img_out = img->HistogramEqualization();
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->HistogramEqualization();
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_SEVEN)
    {
        printf("Case 7 detected\n");
        //GrayImage
        GrayImage *img_out = img->GaussianBlur(7 , 1.5);
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->GaussianBlur(7 , 1.5);
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_EIGHT)
    {
        printf("Case 8 detected\n");
        //GrayImage
        GrayImage *img_out = img->MedianFilter(5);
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->MedianFilter(5);
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_NINE)
    {
        printf("Case 9 detected\n");
        //GrayImage
        GrayImage *img_out = img->Laplacian();
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->Laplacian();
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_TEN)
    {
        printf("Case 10 detected\n");
        //GrayImage
        GrayImage *img_out = img->Emboss();
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->Emboss();
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_ELEVEN)
    {
        printf("Case 11 detected\n");
        //GrayImage
        GrayImage *img_out = img->Invert();
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->Invert();
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_TWELVE)
    {
        printf("Case 12 detected\n");
        //GrayImage
        GrayImage *img_out = img->AdjustBrightness(30);
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->AdjustBrightness(30);
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    if (option & CASE_THIRTEEN)
    {
        printf("Case 13 detected\n");
        //GrayImage
        GrayImage *img_out = img->SepiaTone();
        img_out->Display_CMD();
        delete img;
        img = img_out;
        //RGBImage
        RGBImage *img_2_out = img_2->SepiaTone();
        img_2_out->Display_CMD();
        delete img_2;
        img_2 = img_2_out;
    }
    img->DumpImage("img_case" + std::to_string(cnt) + ".jpg");
    delete img;
    img_2->DumpImage("img_rgb_case" + std::to_string(cnt) + ".jpg");
    delete img_2;
    cnt++;
}
