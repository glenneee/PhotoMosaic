#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "data_loader.h"

using namespace std;

class Image{
protected:
int w;
int h;
static Data_Loader data_loader;

public:
Image(int width, int height);
virtual ~Image(){};
virtual bool LoadImage(string filename);
virtual void DumpImage(string filename);
virtual void Display_X_Server(){};
virtual void Display_ASCII(){};
virtual void Display_CMD(){};
int get_w();
int get_h();

};

#endif