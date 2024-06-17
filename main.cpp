#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "photo_mosaic.h"
#include "bit_field_filter.h"
#include <iostream>
#define ENABLE_X_SERVER true

int main(int argc, char *argv[])
{
    Image *img1 = new GrayImage();
    cout << "Loading gray image...\n";
    img1->LoadImage("Image-Folder/mnist/img_100.jpg");
    cout << "Dumping gray image...\n";
    img1->DumpImage("img1.jpg");
    cout << "Display gray image...\n";
    if (ENABLE_X_SERVER)
    {
        img1->Display_X_Server();
    }
    cout << "Display gray image in console...\n";
    img1->Display_CMD();

    delete img1;
    
    Image *img2 = new RGBImage();
    cout << "Loading RGB image...\n";
    img2->LoadImage("Image-Folder/lena.jpg");
    cout << "Dumping RGB image...\n";
    img2->DumpImage("img2.jpg");
    cout << "Display RGB image...\n";
    if (ENABLE_X_SERVER)
    {
        img2->Display_X_Server();
    }
    cout << "Display RGB image in console...\n";
    img2->Display_CMD();

    delete img2;
    // some bit field filter design driven code here
    //------------------------------
    // test1:
    // uint8_t option = 0b00001001;
    printf("test1:\n");
    // loadCase(option);
    loadCase(CASE_ONE);

    // test2:
    printf("test2:\n");
    loadCase(CASE_TWO);

    // test3:
    printf("test3:\n");
    loadCase(CASE_THREE);

    // test4:
    printf("test4:\n");
    loadCase(CASE_FOUR);

    // test5:
    printf("test5:\n");
    loadCase(CASE_FIVE);

    //test6:
    printf("test6:\n");
    loadCase(CASE_SIX);

    //test7:
    printf("test7:\n");
    loadCase(CASE_SEVEN);

    //test8:
    printf("test8:\n");
    loadCase(CASE_EIGHT);

    //test9:
    printf("test9:\n");
    loadCase(CASE_NINE);

    //test10:
    printf("test10:\n");
    loadCase(CASE_TEN);

    //test11:
    printf("test11:\n");
    loadCase(CASE_ELEVEN);

    //test12:
    printf("test12:\n");
    loadCase(CASE_TWELVE);

    //test13:
    printf("test13:\n");
    loadCase(CASE_THIRTEEN);

    // loadCase(CASE_ONE | CASE_TWO | CASE_THREE | CASE_FOUR);
    //------------------------------
    //  some photo mosaic driven code here
    printf("Starting PhotoMosaic...\n");
    PhotoMosaic *photomosaic = new PhotoMosaic("Image-Folder/cat.jpg", "Image-Folder/cifar10");
    photomosaic->Create_Image();
    photomosaic->DumpImage("catmosaic.jpg");
    cout << "photomosaic\n";
    photomosaic->Display_CMD();
    if (ENABLE_X_SERVER)
    {
        photomosaic->Display_X_Server();
    }
    delete photomosaic;
    // more ...
    return 0;
}
