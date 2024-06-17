#include "rgb_image.h"

RGBImage::RGBImage() : Image(0, 0), pixels(nullptr){};
RGBImage::RGBImage(int width, int height, int ***pixels) : Image(width, height), pixels(pixels){};
RGBImage::~RGBImage()
{
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            delete[] pixels[y][x];
        }
        delete[] pixels[y];
    }
    delete[] pixels;
}

bool RGBImage::LoadImage(string filename)
{
    pixels = data_loader.Load_RGB(filename, &w, &h);
    return (pixels != nullptr);
};
void RGBImage::DumpImage(string filename)
{
    data_loader.Dump_RGB(w, h, pixels, filename);
    return;
};
void RGBImage::Display_X_Server()
{
    data_loader.Display_RGB_X_Server(w, h, pixels);
    return;
};
void RGBImage::Display_ASCII()
{
    data_loader.Display_RGB_ASCII(w, h, pixels);
    return;
};
void RGBImage::Display_CMD()
{
    string filename = "temp.jpg";
    data_loader.Dump_RGB(w, h, pixels, filename);
    data_loader.Display_RGB_CMD(filename);
    string rm_cmd = "rm " + filename;
    system(rm_cmd.c_str());
    return;
}


RGBImage *RGBImage::BoxFilter(int filter_size)
{
    int ***p = new int **[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }
    int s = filter_size / 2;
    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                int cnt = 0;
                int color_number = 0;
                for (int y_k = y - s; y_k <= y + s; y_k++)
                {
                    for (int x_k = x - s; x_k <= x + s; x_k++)
                    {
                        if (y_k >= 0 && y_k < h && x_k >= 0 && x_k < w)
                        {
                            color_number += pixels[y_k][x_k][i];
                            cnt++;
                        }
                    }
                }
                color_number /= cnt;
                p[y][x][i] = color_number;
            }
        }
    }
    RGBImage *boxfilter = new RGBImage(w, h, p);
    return boxfilter;
}

RGBImage *RGBImage::Sobel_Gradient()
{
    int ***p = new int **[h];
    const int k_x[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
    const int k_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    
    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }
    
    for (int i = 0; i < 3; i++)
    {
        for (int y = 1; y < h - 1; y++)
        {
            for (int x = 1; x < w - 1; x++)
            {
                int gx = 0;
                int gy = 0;
                
                for (int ky = -1; ky <= 1; ky++)
                {
                    for (int kx = -1; kx <= 1; kx++)
                    {
                        gx += pixels[y + ky][x + kx][i] * k_x[ky + 1][kx + 1];
                        gy += pixels[y + ky][x + kx][i] * k_y[ky + 1][kx + 1];
                    }
                }
                
                int gradient = static_cast<int>(sqrt(gx * gx + gy * gy));
                p[y][x][i] = std::min(255, std::max(0, gradient));
            }
        }
    }
    
    RGBImage *sobel_gradient = new RGBImage(w, h, p);
    return sobel_gradient;
}

RGBImage *RGBImage::Contrast_Stretching()
{
    int ***pix = new int **[h];
    for (int y = 0; y < h; y++)
    {
        pix[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            pix[y][x] = new int[3];
        }
    }
    int min; // declare min and max as "static" to walk-around the valgrind bug
    int max;
    min = 255;
    max = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                if (pixels[y][x][i] < min)
                    min = pixels[y][x][i];
                if (pixels[y][x][i] > max)
                    max = pixels[y][x][i];
            }
        }
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                pix[y][x][i] = ((pixels[y][x][i] - min) * 255) / (max - min);
            }
        }
    }
    RGBImage *contrast_stretching = new RGBImage(w, h, pix);
    return contrast_stretching;
}

RGBImage *RGBImage::Mosaic_Filter(int filter_size)
{
    int ***p = new int **[h];

    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < h; y += filter_size)
        {
            for (int x = 0; x < w; x += filter_size)
            {
                int cnt = 0;
                int color = 0;
                for (int k_y = y; k_y < y + filter_size; k_y++)
                {
                    for (int k_x = x; k_x < x + filter_size; k_x++)
                    {
                        if (k_y < h && k_x < w)
                        {
                            color += pixels[k_y][k_x][i];
                            cnt++;
                        }
                    }
                }
                color /= cnt;
                for (int k_y = y; k_y < y + filter_size; k_y++)
                {
                    for (int k_x = x; k_x < x + filter_size; k_x++)
                    {
                        if (k_y < h && k_x < w)
                        {
                            p[k_y][k_x][i] = color;
                        }
                    }
                }
            }
        }
    }
    RGBImage *mosaic_filter = new RGBImage(w, h, p);
    return mosaic_filter;
}

RGBImage *RGBImage::LinearMotionBlur(int k_size, double angle)
{
    int ***p = new int **[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }

    double radian = angle * M_PI / 180.0;
    int half = k_size / 2;
    double cos_angle = cos(radian);
    double sin_angle = sin(radian);

    // Create motion blur kernel
    double *kernel = new double[k_size];
    for (int i = 0; i < k_size; i++)
    {
        kernel[i] = 1.0 / k_size;
    }

    // Apply the kernel to each color channel
    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                double pixel_value = 0.0;
                for (int t = -half; t <= half; t++)
                {
                    int y_pos = y + t * sin_angle;
                    int x_pos = x + t * cos_angle;
                    if (y_pos >= 0 && y_pos < h && x_pos >= 0 && x_pos < w)
                    {
                        pixel_value += pixels[y_pos][x_pos][i] * kernel[half + t];
                    }
                }
                p[y][x][i] = static_cast<int>(pixel_value);
            }
        }
    }

    delete[] kernel;

    RGBImage *motion_blur = new RGBImage(w, h, p);
    return motion_blur;
}
RGBImage *RGBImage::HistogramEqualization()
{
    int ***p = new int **[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }

    for (int i = 0; i < 3; i++)
    {
        // Step 1: Compute the histogram
        int histogram[256] = {0};
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                histogram[pixels[y][x][i]]++;
            }
        }

        // Step 2: Compute the cumulative distribution function (CDF)
        int cdf[256] = {0};
        cdf[0] = histogram[0];
        for (int j = 1; j < 256; ++j)
        {
            cdf[j] = cdf[j - 1] + histogram[j];
        }

        // Step 3: Normalize the CDF
        int cdf_min = 0;
        for (int j = 0; j < 256; ++j)
        {
            if (cdf[j] != 0)
            {
                cdf_min = cdf[j];
                break;
            }
        }

        int cdf_max = cdf[255];
        double scale_factor = 255.0 / (cdf_max - cdf_min);
        int equalized[256] = {0};
        for (int j = 0; j < 256; ++j)
        {
            equalized[j] = std::round((cdf[j] - cdf_min) * scale_factor);
            if (equalized[j] < 0)
            {
                equalized[j] = 0;
            }
            if (equalized[j] > 255)
            {
                equalized[j] = 255;
            }
        }

        // Step 4: Apply the equalized histogram to the image
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                p[y][x][i] = equalized[pixels[y][x][i]];
            }
        }
    }

    RGBImage *equalized_image = new RGBImage(w, h, p);
    return equalized_image;
}

RGBImage *RGBImage::GaussianBlur(int k_size, double sigma)
{
    int ***p = new int **[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }

    double **kernel = new double *[k_size];
    for (int i = 0; i < k_size; i++)
    {
        kernel[i] = new double[k_size];
    }

    int half = k_size / 2;
    double sum = 0.0;

    // Generate Gaussian kernel
    for (int y = -half; y <= half; y++)
    {
        for (int x = -half; x <= half; x++)
        {
            double value = exp(-(x*x + y*y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
            kernel[y + half][x + half] = value;
            sum += value;
        }
    }

    // Normalize the kernel
    for (int y = 0; y < k_size; y++)
    {
        for (int x = 0; x < k_size; x++)
        {
            kernel[y][x] /= sum;
        }
    }

    // Apply the Gaussian blur
    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                double pixel_value = 0.0;
                for (int ky = -half; ky <= half; ky++)
                {
                    for (int kx = -half; kx <= half; kx++)
                    {
                        int y_pos = y + ky;
                        int x_pos = x + kx;
                        if (y_pos >= 0 && y_pos < h && x_pos >= 0 && x_pos < w)
                        {
                            pixel_value += pixels[y_pos][x_pos][i] * kernel[ky + half][kx + half];
                        }
                    }
                }
                p[y][x][i] = static_cast<int>(pixel_value);
            }
        }
    }

    // Clean up
    for (int i = 0; i < k_size; i++)
    {
        delete[] kernel[i];
    }
    delete[] kernel;

    RGBImage *gaussian_blur = new RGBImage(w, h, p);
    return gaussian_blur;
}
RGBImage *RGBImage::MedianFilter(int k_size)
{
    int ***p = new int **[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }

    int half = k_size / 2;
    std::vector<int> window;

    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                window.clear();
                for (int ky = -half; ky <= half; ky++)
                {
                    for (int kx = -half; kx <= half; kx++)
                    {
                        int y_pos = y + ky;
                        int x_pos = x + kx;
                        if (y_pos >= 0 && y_pos < h && x_pos >= 0 && x_pos < w)
                        {
                            window.push_back(pixels[y_pos][x_pos][i]);
                        }
                    }
                }
                std::nth_element(window.begin(), window.begin() + window.size() / 2, window.end());
                p[y][x][i] = window[window.size() / 2];
            }
        }
    }

    RGBImage *median_filtered = new RGBImage(w, h, p);
    return median_filtered;
}

RGBImage *RGBImage::Laplacian()
{
    int ***p = new int **[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }

    const int kernel[3][3] = {
        { 0, -1,  0},
        {-1,  4, -1},
        { 0, -1,  0}
    };

    for (int i = 0; i < 3; i++)
    {
        for (int y = 1; y < h - 1; y++)
        {
            for (int x = 1; x < w - 1; x++)
            {
                int pixel_value = 0;
                for (int ky = -1; ky <= 1; ky++)
                {
                    for (int kx = -1; kx <= 1; kx++)
                    {
                        pixel_value += pixels[y + ky][x + kx][i] * kernel[ky + 1][kx + 1];
                    }
                }
                p[y][x][i] = std::min(255, std::max(0, pixel_value));
            }
        }
    }

    RGBImage *laplacian_image = new RGBImage(w, h, p);
    return laplacian_image;
}

RGBImage *RGBImage::Emboss()
{
    int ***p = new int **[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }

    const int kernel[3][3] = {
        {-2, -1,  0},
        {-1,  1,  1},
        { 0,  1,  2}
    };

    for (int i = 0; i < 3; i++)
    {
        for (int y = 1; y < h - 1; y++)
        {
            for (int x = 1; x < w - 1; x++)
            {
                int pixel_value = 0;
                for (int ky = -1; ky <= 1; ky++)
                {
                    for (int kx = -1; kx <= 1; kx++)
                    {
                        pixel_value += pixels[y + ky][x + kx][i] * kernel[ky + 1][kx + 1];
                    }
                }
                p[y][x][i] = std::min(255, std::max(0, pixel_value + 128)); // Add 128 to adjust for the mid-tone shift
            }
        }
    }

    RGBImage *emboss_image = new RGBImage(w, h, p);
    return emboss_image;
}

RGBImage *RGBImage::Invert()
{
    int ***p = new int **[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                p[y][x][i] = 255 - pixels[y][x][i];
            }
        }
    }

    RGBImage *inverted_image = new RGBImage(w, h, p);
    return inverted_image;
}

RGBImage *RGBImage::AdjustBrightness(int adjustment)
{
    int ***p = new int **[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                p[y][x][i] = std::min(255, std::max(0, pixels[y][x][i] + adjustment));
            }
        }
    }

    RGBImage *brightness_adjusted = new RGBImage(w, h, p);
    return brightness_adjusted;
}
RGBImage *RGBImage::SepiaTone()
{
    int ***p = new int **[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int *[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = new int[3];
        }
    }

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int r = pixels[y][x][0];
            int g = pixels[y][x][1];
            int b = pixels[y][x][2];

            int tr = std::min(255, std::max(0, static_cast<int>(r * 0.393 + g * 0.769 + b * 0.189)));
            int tg = std::min(255, std::max(0, static_cast<int>(r * 0.349 + g * 0.686 + b * 0.168)));
            int tb = std::min(255, std::max(0, static_cast<int>(r * 0.272 + g * 0.534 + b * 0.131)));

            p[y][x][0] = tr;
            p[y][x][1] = tg;
            p[y][x][2] = tb;
        }
    }

    RGBImage *sepia_tone_image = new RGBImage(w, h, p);
    return sepia_tone_image;
}
