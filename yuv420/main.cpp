#include <stdio.h>
#include <stdlib.h>
#include <iostream>

typedef unsigned char u1;

void spilt_yuv();

void to_binary_yuv();

void spilt_rgb();

void rgb_2_bmp();

struct BMPHeader
{
    long size;
    long blank;
    long start;
};

struct BMPInfo
{
    long len;
    long width;
    long height;
    unsigned short color_plane;
    unsigned short color_bit;
    long zip_format;
    long real_size;
    long x_pels;
    long y_pels;
    long color_use;
    long color_important;
};

int main(int argc, char *argv[])
{
    spilt_yuv();
    to_binary_yuv();
    spilt_rgb();
    rgb_2_bmp();
    return 0;
}

void spilt_yuv()
{
    const char *file_name = "/Users/chan/Documents/github/learn-video/media/lena_256x256_yuv420p.yuv";
    FILE *yuv = fopen(file_name, "rb");
    FILE *y_channel = fopen("/Users/chan/Documents/github/learn-video/yuv420/output/yc.yuv", "wb");
    FILE *u_channel = fopen("/Users/chan/Documents/github/learn-video/yuv420/output/uc.yuv", "wb");
    FILE *v_channel = fopen("/Users/chan/Documents/github/learn-video/yuv420/output/vc.yuv", "wb");

    const size_t frame_size = 256 * 256;
    const size_t buffer_size = frame_size * 6.0 / 4;
    std::cout << "buffer size: " << buffer_size << std::endl;

    u1 *buffer = (u1 *)malloc(buffer_size);

    fread(buffer, buffer_size, 1, yuv);

    std::cout << "y channel size: " << frame_size << std::endl;
    fwrite(buffer, frame_size, 1, y_channel);

    const size_t yv_size = (size_t)(frame_size * 0.25);
    fwrite(buffer + frame_size, yv_size, 1, u_channel);

    fwrite(buffer + (size_t)(frame_size * 1.25), yv_size, 1, v_channel);

    free(buffer);
    fclose(yuv);
    fclose(y_channel);
    fclose(u_channel);
    fclose(v_channel);
}

void to_binary_yuv()
{
    const char *file_name = "/Users/chan/Documents/github/learn-video/media/lena_256x256_yuv420p.yuv";
    FILE *yuv = fopen(file_name, "rb");

    const size_t frame_size = 256 * 256;
    const size_t buffer_size = frame_size * 6.0 / 4;
    typedef unsigned char buffer_type;

    buffer_type *buffer = (buffer_type *)malloc(buffer_size);
    fread(buffer, buffer_size, 1, yuv);

    buffer_type *yv = buffer + frame_size;
    for (int i = 0; i < 256 * 256 * 0.5; ++i)
    {
        yv[i] = 128;
    }

    std::cout << "write binary file" << std::endl;
    FILE *archives = fopen("/Users/chan/Documents/github/learn-video/yuv420/output/binary.yuv", "wb");
    fwrite(buffer, buffer_size, 1, archives);
    free(buffer);
    fclose(yuv);
    fclose(archives);
}

void spilt_rgb()
{
    const char *file_name = "/Users/chan/Documents/github/learn-video/media/cie1931_500x500.rgb";
    FILE *rgb = fopen(file_name, "rb");

    const size_t frame_size = 500 * 500;
    const size_t buffer_size = frame_size * 3;

    u1 *buffer = (u1 *)malloc(buffer_size);
    fread(buffer, buffer_size, 1, rgb);

    FILE *r = fopen("/Users/chan/Documents/github/learn-video/yuv420/output/r.rgb", "wb");
    FILE *g = fopen("/Users/chan/Documents/github/learn-video/yuv420/output/g.rgb", "wb");
    FILE *b = fopen("/Users/chan/Documents/github/learn-video/yuv420/output/b.rgb", "wb");

    u1 none = 0;

    for (int i = 0; i < frame_size; ++i)
    {
        int offset = 3 * i;

        fwrite(buffer + offset, 1, 1, r);
        fwrite(&none, 1, 1, r);
        fwrite(&none, 1, 1, r);

        fwrite(&none, 1, 1, g);
        fwrite(buffer + offset + 1, 1, 1, g);
        fwrite(&none, 1, 1, g);

        fwrite(&none, 1, 1, b);
        fwrite(&none, 1, 1, b);
        fwrite(buffer + offset + 2, 1, 1, b);
    }

    fclose(r);
    fclose(g);
    fclose(b);
    fclose(rgb);
    free(buffer);
}

void rgb_2_bmp()
{
    const char *file_name = "/Users/chan/Documents/github/learn-video/media/lena_256x256_rgb24.rgb";
    FILE *rgb = fopen(file_name, "rb");

    int width = 256;
    int height = 256;
    const size_t frame_size = width * height;
    const size_t buffer_size = frame_size * 3;

    u1 *buffer = (u1 *)malloc(buffer_size);
    fread(buffer, buffer_size, 1, rgb);

    char magic[2] = {'B', 'M'};
    BMPHeader bmp_header;
    BMPInfo bmp_info;

    const size_t meta_info_size = sizeof(magic) + sizeof(BMPHeader) + sizeof(BMPInfo);
    bmp_header.size = meta_info_size + buffer_size;
    bmp_header.start = meta_info_size;

    bmp_info.len = sizeof(BMPInfo);
    bmp_info.width = width;
    bmp_info.height = -height;
    bmp_info.color_plane = 1;
    bmp_info.color_bit = 24;
    bmp_info.real_size = buffer_size;

    FILE *archives = fopen("/Users/chan/Documents/github/learn-video/yuv420/output/500x500.bmp", "wb");
    for (int i = 0; i + 2 < height * width; i += 3)
    {
        u1 temp = buffer[i];
        buffer[i] = buffer[i + 2];
        buffer[i + 2] = temp;
    }

    std::cout << "write bmp" << std::endl;
    fwrite(magic, sizeof(magic), 1, archives);
    fwrite(&bmp_header, sizeof(BMPHeader), 1, archives);
    fwrite(&bmp_info, sizeof(BMPInfo), 1, archives);
    fwrite(buffer, buffer_size, 1, archives);
    fclose(archives);
    fclose(rgb);
    free(buffer);
}