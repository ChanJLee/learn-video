#include <iostream>

void spilt_pcm()
{
    FILE *audio = fopen("/Users/chan/Documents/github/learn-video/media/NocturneNo2inEflat_44.1k_s16le.pcm", "rb");
    FILE *left = fopen("/Users/chan/Documents/github/learn-video/pcm/output/left.pcm", "wb");
    FILE *right = fopen("/Users/chan/Documents/github/learn-video/pcm/output/right.pcm", "wb");

    unsigned char buffer[4];
    while(!feof(audio)) {
        fread(buffer, 4, 1, audio);
        fwrite(buffer, 2, 1, left);
        fwrite(buffer + 2, 2, 1, right);
    }

    fclose(audio);
    fclose(left);
    fclose(right);
}

void pcm_volumn_half()
{
    FILE *audio = fopen("/Users/chan/Documents/github/learn-video/media/NocturneNo2inEflat_44.1k_s16le.pcm", "rb");
    FILE *archives = fopen("/Users/chan/Documents/github/learn-video/pcm/output/volumn_half.pcm", "wb");

    unsigned char buffer[4];
    short *channel;
    while(!feof(audio)) {
        fread(buffer, 4, 1, audio);
        channel = (short*) buffer;
        *channel = (*channel) / 2;
        channel = (short*) (buffer + 2);
        *channel = (*channel) / 2;
        fwrite(buffer, 4, 1, archives);
    }

    fclose(audio);
    fclose(archives);
}

int main(int argc, char* argv[])
{
    spilt_pcm();
    pcm_volumn_half();
    return 0;
}