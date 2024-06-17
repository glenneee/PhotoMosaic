#include "photo_mosaic.h"


PhotoMosaic::PhotoMosaic(){};


PhotoMosaic::PhotoMosaic(string fn_src, string fn_dir)
{
   LoadImage(fn_src);
   data_loader.List_Directory(fn_dir, photonames);
   smallest_w = w;
   smallest_h = h;
   for (int i = 0; i < (int)photonames.size(); i++)
   {
       RGBImage *photo = new RGBImage();
       photo->LoadImage(photonames[i]);
       photos.push_back(photo);
       if (photo->get_w() < smallest_w)
       {
           smallest_w = photo->get_w();
       }
       if (photo->get_h() < smallest_h)
       {
           smallest_h = photo->get_h();
       }
       vector<int> rgb = {0, 0, 0};
       for (int i = 0; i < 3; i++)
       {
           int color_number = 0;
           for (int y = 0; y < photo->get_h(); y++)
           {
               for (int x = 0; x < photo->get_w(); x++)
               {
                   color_number = color_number + photo->pixels[y][x][i];
               }
           }
           color_number /= (photo->get_h() * photo->get_w());
           rgb[i] = color_number;
       }
       rgb_color.push_back(rgb);
   }
};


PhotoMosaic::~PhotoMosaic()
{
   for (int i = 0; i < (int)photos.size(); i++)
   {
   delete photos[i];
   }
   photos.clear(); // free stuff inside the vector
   photonames.clear();
}


void PhotoMosaic::Create_Image()
{
   for (int start_y = 0; start_y < h; start_y += smallest_h)
   {
       for (int start_x = 0; start_x < w; start_x += smallest_w)
       {
           int index = 0;
           int difference = 255 * 255 * 3;


           int local_difference = 0;
           int red = 0;
           int green = 0;
           int blue = 0;
           for (int i = 0; i < 3; i++)
           {
               int color_number = 0;
               int cnt = 0;
               for (int y = start_y; y < start_y + smallest_h; y++)
               {
                   for (int x = start_x; x < start_x + smallest_w; x++)
                   {
                       if (x < w && y < h)
                       {
                           color_number += pixels[y][x][i];
                           cnt++;
                       }
                   }
               }
               color_number /= cnt;
               if (i == 0)
                   red = color_number;
               if (i == 1)
                   green = color_number;
               if (i == 2)
                   blue = color_number;
           }
           for (int a = 0; a < (int)photos.size(); a++)
           {
               local_difference = pow((red - rgb_color[a][0]), 2) + pow((green - rgb_color[a][1]), 2) + pow((blue - rgb_color[a][2]), 2);
               if (local_difference < difference)
               {
                   index = a;
                   difference = local_difference;
               }
           }
           for (int i = 0; i < 3; i++)
           {
               for (int y = start_y; y < start_y + smallest_h; y++)
               {
                   for (int x = start_x; x < start_x + smallest_w; x++)
                   {
                       if (x < w && y < h)
                           pixels[y][x][i] = photos[index]->pixels[y - start_y][x - start_x][i];
                   }
               }
           }
       }
   }
   return;
}
