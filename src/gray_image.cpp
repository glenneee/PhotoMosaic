#include "gray_image.h"

GrayImage::GrayImage() : Image(0, 0), pixels(nullptr){};

GrayImage::GrayImage(int width, int height, int **pixels) : Image(width, height), pixels(pixels){};

GrayImage::~GrayImage()
{
    for (int y = 0; y < h; ++y)
    {
        delete[] pixels[y];
    }
    delete[] pixels;
};
bool GrayImage::LoadImage(string filename)
{
    pixels = data_loader.Load_Gray(filename, &w, &h);
    return (pixels != nullptr);
};
void GrayImage::DumpImage(string filename)
{
    data_loader.Dump_Gray(w, h, pixels, filename);
    return;
};
void GrayImage::Display_X_Server()
{
    data_loader.Display_Gray_X_Server(w, h, pixels);
    return;
};
void GrayImage::Display_ASCII()
{
    data_loader.Display_Gray_ASCII(w, h, pixels);
    return;
};
void GrayImage::Display_CMD()
{
    string filename = "temp.jpg";
    data_loader.Dump_Gray(w, h, pixels, filename);
    data_loader.Display_Gray_CMD(filename);
    string rm_cmd = "/usr/bin/rm " + filename;
    system(rm_cmd.c_str());
    return;
};

GrayImage *GrayImage::BoxFilter(int k_size)
{
    int **p = new int *[h];
    static int s = k_size / 2;
    for (int y = 0; y < h; y++)
    {
        p[y] = new int[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = 0;
            int cnt = 0;
            for (int y_k = y - s; y_k <= y + s; y_k++)
            {
                for (int x_k = x - s; x_k <= x + s; x_k++)
                {
                    if (y_k >= 0 && y_k < h && x_k >= 0 && x_k < w)
                    {
                        cnt++;
                        p[y][x] += pixels[y_k][x_k];
                    }
                }
            }
            p[y][x] /= cnt;
        }
    }
    GrayImage *boxfilter = new GrayImage(w, h, p);
    return boxfilter;
}

GrayImage *GrayImage::Sobel_Gradient()
{
    int **p = new int *[h];
    const int k_x[3][3] = {{-1, 0, 1}, {2, 0, -2}, {-1, 0, 1}};
    const int k_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (int y = 0; y < h; y++)
    {
        p[y] = new int[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = 0;
            int gx = 0;
            int gy = 0;
            for (int y_k = y - 1; y_k <= y + 1; y_k++)
            {
                for (int x_k = x - 1; x_k <= x + 1; x_k++)
                {
                    if (y_k >= 0 && y_k < h && x_k >= 0 && x_k < w)
                    {
                        gx += pixels[y_k][x_k] * k_x[y_k - y + 1][x_k - x + 1];
                        gy += pixels[y_k][x_k] * k_y[y_k - y + 1][x_k - x + 1];
                    }
                }
            }
            p[y][x] = sqrt(gx * gx + gy * gy);
        }
    }
    GrayImage *sobel_gradient = new GrayImage(w, h, p);
    return sobel_gradient;
}

GrayImage *GrayImage::Contrast_Stretching()
{
    int **pix = new int *[h];
    static int min; // declare min and max as "static" to walk-around the valgrind bug
    static int max;
    min = 255;
    max = 0;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (pixels[y][x] < min)
                min = pixels[y][x];
            if (pixels[y][x] > max)
                max = pixels[y][x];
        }
    }
    for (int y = 0; y < h; y++)
    {
        pix[y] = new int[w];
        for (int x = 0; x < w; x++)
        {
            pix[y][x] = ((pixels[y][x] - min) * 255) / (max - min);
        }
    }
    GrayImage *contrast_stretching = new GrayImage(w, h, pix);
    return contrast_stretching;
}

GrayImage *GrayImage::Mosaic_Filter(int filter_size)
{
    int **p = new int *[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int[w];
    }
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
                        color += pixels[k_y][k_x];
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
                        p[k_y][k_x] = color;
                    }
                }
            }
        }
    }
    GrayImage *mosaic_filter = new GrayImage(w, h, p);
    return mosaic_filter;
}
GrayImage *GrayImage::LinearMotionBlur(int k_size, double angle)
{

    int **p = new int *[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int[w];
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

    // Apply the kernel to the image
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
                    pixel_value += pixels[y_pos][x_pos] * kernel[half + t];
                }
            }
            p[y][x] = static_cast<int>(pixel_value);
        }
    }

    delete[] kernel;

    GrayImage *motion_blur = new GrayImage(w, h, p);
    return motion_blur;
}
GrayImage *GrayImage::HistogramEqualization()
{
    // Step 1: Compute the histogram
    int histogram[256] = {0};
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            histogram[pixels[y][x]]++;
        }
    }

    // Step 2: Compute the cumulative distribution function (CDF)
    int cdf[256] = {0};
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; ++i)
    {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    // Step 3: Normalize the CDF
    int cdf_min = 0;
    for (int i = 0; i < 256; ++i)
    {
        if (cdf[i] != 0)
        {
            cdf_min = cdf[i];
            break;
        }
    }

    int cdf_max = cdf[255];
    double scale_factor = 255.0 / (cdf_max - cdf_min);
    int equalized[256] = {0};
    for (int i = 0; i < 256; ++i)
    {
        equalized[i] = std::round((cdf[i] - cdf_min) * scale_factor);
        if (equalized[i] < 0)
        {
            equalized[i] = 0;
        }
        if (equalized[i] > 255)
        {
            equalized[i] = 255;
        }
    }

    // Step 4: Apply the equalized histogram to the image
    int **p = new int *[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int[w];
        for (int x = 0; x < w; x++)
        {
            p[y][x] = equalized[pixels[y][x]];
        }
    }
    GrayImage *equalized_image = new GrayImage(w, h, p);
    return equalized_image;
}
GrayImage *GrayImage::GaussianBlur(int k_size, double sigma)
{
    int **p = new int *[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int[w];
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
            double value = exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
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
                        pixel_value += pixels[y_pos][x_pos] * kernel[ky + half][kx + half];
                    }
                }
            }
            p[y][x] = static_cast<int>(pixel_value);
        }
    }

    // Clean up
    for (int i = 0; i < k_size; i++)
    {
        delete[] kernel[i];
    }
    delete[] kernel;

    GrayImage *gaussian_blur = new GrayImage(w, h, p);
    return gaussian_blur;
}

GrayImage *GrayImage::MedianFilter(int k_size)
{
    int **p = new int *[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int[w];
    }

    int half = k_size / 2;
    std::vector<int> window;

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
                        window.push_back(pixels[y_pos][x_pos]);
                    }
                }
            }
            std::nth_element(window.begin(), window.begin() + window.size() / 2, window.end());
            p[y][x] = window[window.size() / 2];
        }
    }
    GrayImage *median_filtered = new GrayImage(w, h, p);
    return median_filtered;
}


    GrayImage *GrayImage::Laplacian()
    {
        int **p = new int *[h];
        for (int y = 0; y < h; y++)
        {
            p[y] = new int[w];
        }

        const int kernel[3][3] = {
            {0, -1, 0},
            {-1, 4, -1},
            {0, -1, 0}};

        for (int y = 1; y < h - 1; y++)
        {
            for (int x = 1; x < w - 1; x++)
            {
                int pixel_value = 0;
                for (int ky = -1; ky <= 1; ky++)
                {
                    for (int kx = -1; kx <= 1; kx++)
                    {
                        pixel_value += pixels[y + ky][x + kx] * kernel[ky + 1][kx + 1];
                    }
                }
                p[y][x] = std::min(255, std::max(0, pixel_value));
            }
        }

        GrayImage *laplacian_image = new GrayImage(w, h, p);
        return laplacian_image;
    }

   GrayImage *GrayImage::Emboss()
{
    int **p = new int *[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int[w];
    }

    const int kernel[3][3] = {
        {-2, -1,  0},
        {-1,  1,  1},
        { 0,  1,  2}
    };

    for (int y = 1; y < h - 1; y++)
    {
        for (int x = 1; x < w - 1; x++)
        {
            int pixel_value = 0;
            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
                    pixel_value += pixels[y + ky][x + kx] * kernel[ky + 1][kx + 1];
                }
            }
            p[y][x] = std::min(255, std::max(0, pixel_value + 128)); // Add 128 to adjust for the mid-tone shift
        }
    }

    GrayImage *emboss_image = new GrayImage(w, h, p);
    return emboss_image;
}

GrayImage *GrayImage::Invert()
{
    int **p = new int *[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int[w];
    }

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            p[y][x] = 255 - pixels[y][x];
        }
    }

    GrayImage *inverted_image = new GrayImage(w, h, p);
    return inverted_image;
}

GrayImage *GrayImage::AdjustBrightness(int adjustment)
{
    int **p = new int *[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int[w];
    }

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            p[y][x] = std::min(255, std::max(0, pixels[y][x] + adjustment));
        }
    }

    GrayImage *brightness_adjusted = new GrayImage(w, h, p);
    return brightness_adjusted;
}
GrayImage *GrayImage::SepiaTone()
{
    int **p = new int *[h];
    for (int y = 0; y < h; y++)
    {
        p[y] = new int[w];
    }

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int pixel_value = pixels[y][x];
            int sepia_value = std::min(255, std::max(0, static_cast<int>(pixel_value * 0.393 + pixel_value * 0.769 + pixel_value * 0.189)));
            p[y][x] = sepia_value;
        }
    }

    GrayImage *sepia_tone_image = new GrayImage(w, h, p);
    return sepia_tone_image;
}
