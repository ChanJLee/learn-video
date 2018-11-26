#include <stdio.h>
#include <stdlib.h>

typedef unsigned char u1;

int main(int argc, char* argv[])
{
    const char *file_name = "/Users/chan/Documents/github/learn-video/media/lena_256x256_yuv420p.yuv";
    FILE *yuv = fopen(file_name, "rb");
    FILE *y_channel = fopen("/Users/chan/Documents/github/learn-video/yuv420/output/yc", "wb");
    FILE *u_channel = fopen("/Users/chan/Documents/github/learn-video/yuv420/output/uc", "wb");
    FILE *v_channel = fopen("/Users/chan/Documents/github/learn-video/yuv420/output/vc", "wb");

    const size_t frame_size = 256 * 256;
    const size_t buffer_size = frame_size * 6.0 / 4;
    u1* buffer = (u1*) malloc(buffer_size);

    fread(buffer, buffer_size, 1, yuv);
    fwrite(buffer, frame_size, 1, y_channel);
    buffer += frame_size;
    fwrite(buffer, frame_size * 0.25, 1, u_channel);
    buffer += (size_t) (frame_size * 0.25);
    fwrite(buffer, frame_size * 0.25, 1, v_channel);

    fclose(y_channel);
    fclose(u_channel);
    fclose(v_channel);

    return 0;
}