#include <iostream>

void spilt_pcm()
{
    FILE *audio = fopen("/Users/chan/Documents/github/learn-video/media/NocturneNo2inEflat_44.1k_s16le.pcm", "rb");
    FILE *left = fopen("/Users/chan/Documents/github/learn-video/pcm/output/left.pcm", "wb");
    FILE *right = fopen("/Users/chan/Documents/github/learn-video/pcm/output/right.pcm", "wb");

    unsigned char buffer[4];
    while (!feof(audio))
    {
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
    while (!feof(audio))
    {
        fread(buffer, 4, 1, audio);
        channel = (short *)buffer;
        *channel = (*channel) / 2;
        channel = (short *)(buffer + 2);
        *channel = (*channel) / 2;
        fwrite(buffer, 4, 1, archives);
    }

    fclose(audio);
    fclose(archives);
}

struct WAVE_HEADER
{
    char fccID[4];
    unsigned long dwSize;
    char fccType[4];
};

struct WAVE_FMT
{
    char fccID[4];
    unsigned long dwSize;
    unsigned short wFormatTag;
    unsigned short wChannels;
    unsigned long dwSamplesPerSec;
    unsigned long dwAvgBytesPerSec;
    unsigned short wBlockAlign;
    unsigned short uiBitsPerSample;
};

struct WAVE_DATA
{
    char fccID[4];
    unsigned long dwSize;
};

void pcm_2_wav()
{
    FILE *audio = fopen("/Users/chan/Documents/github/learn-video/media/NocturneNo2inEflat_44.1k_s16le.pcm", "rb");
    FILE *archives = fopen("/Users/chan/Documents/github/learn-video/pcm/output/out.wav", "wb");

    WAVE_HEADER wav_header;
    memcpy(wav_header.fccID, "RIFF", strlen("RIFF"));
    memcpy(wav_header.fccType, "WAVE", strlen("WAVE"));

    // need file size
    fseek(archives, sizeof(WAVE_HEADER), SEEK_CUR);

    unsigned short chunk = 0;
    WAVE_FMT wav_fmt;
    wav_fmt.dwSamplesPerSec = 44100;
    wav_fmt.dwAvgBytesPerSec = wav_fmt.dwSamplesPerSec * sizeof(chunk);
    wav_fmt.uiBitsPerSample = 16;
    memcpy(wav_fmt.fccID, "fmt ", strlen("fmt "));
    wav_fmt.dwSize = 16;
    wav_fmt.wBlockAlign = 2;
    wav_fmt.wChannels = 2;
    wav_fmt.wFormatTag = 1;

    fwrite(&wav_fmt, sizeof(WAVE_FMT), 1, archives);

    WAVE_DATA wav_data;

    memcpy(wav_data.fccID, "data", strlen("data"));
    wav_data.dwSize = 0;

    // need dw size
    fseek(archives, sizeof(WAVE_DATA), SEEK_CUR);

    fread(&chunk, sizeof(chunk), 1, audio);
    while (!feof(audio))
    {
        wav_data.dwSize += sizeof(chunk);
        fwrite(&chunk, sizeof(chunk), 1, archives);
        fread(&chunk, sizeof(chunk), 1, audio);
    }

    wav_header.dwSize = 44 + wav_data.dwSize;

    rewind(archives);
    fwrite(&wav_header, sizeof(WAVE_HEADER), 1, archives);
    fseek(archives, sizeof(WAVE_FMT), SEEK_CUR);
    fwrite(&wav_data, sizeof(WAVE_DATA), 1, archives);

    fclose(archives);
    fclose(audio);
}

int main(int argc, char *argv[])
{
    spilt_pcm();
    pcm_volumn_half();
    pcm_2_wav();
    return 0;
}