#include <iostream>

FILE *h264 = NULL;

typedef enum
{
    NALU_TYPE_SLICE = 1,
    NALU_TYPE_DPA = 2,
    NALU_TYPE_DPB = 3,
    NALU_TYPE_DPC = 4,
    NALU_TYPE_IDR = 5,
    NALU_TYPE_SEI = 6,
    NALU_TYPE_SPS = 7,
    NALU_TYPE_PPS = 8,
    NALU_TYPE_AUD = 9,
    NALU_TYPE_EOSEQ = 10,
    NALU_TYPE_EOSTREAM = 11,
    NALU_TYPE_FILL = 12,
} NaluType;

typedef enum
{
    NALU_PRIORITY_DISPOSABLE = 0,
    NALU_PRIRITY_LOW = 1,
    NALU_PRIORITY_HIGH = 2,
    NALU_PRIORITY_HIGHEST = 3
} NaluPriority;

typedef struct
{
    int startcodeprefix_len; //! 4 for parameter sets and first slice in picture, 3 for everything else (suggested)
    unsigned len;            //! Length of the NAL unit (Excluding the start code, which does not belong to the NALU)
    unsigned max_size;       //! Nal Unit bufferfer size
    int forbidden_bit;       //! should be always FALSE
    int nal_reference_idc;   //! NALU_PRIORITY_xxxx
    int nal_unit_type;       //! NALU_TYPE_xxxx
    char *buffer;            //! contains the first byte followed by the EBSP
} NALU_t;

typedef unsigned char u1;

static bool find_code2(u1 *buffer)
{
    return buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 1;
}

static bool find_code3(u1 *buffer)
{
    return buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0 && buffer[3] == 1;
}

int get_annexb_NALU(NALU_t *nalu)
{
    u1 *buffer = (u1 *)malloc(nalu->max_size);
    fread(buffer, 3, 1, h264);

    int pos = 0;
    if (find_code2(buffer))
    {
        nalu->startcodeprefix_len = 3;
        pos = 3;
        goto find_end;
    }

    fread(buffer + 3, 1, 1, h264);
    if (find_code3(buffer))
    {
        nalu->startcodeprefix_len = 4;
        pos = 4;

        goto find_end;
    }

    free(buffer);
    return -1;

find_end:
    int rewind = 0;
    while (1)
    {
        if (feof(h264))
        {
            nalu->len = (pos - 1) - nalu->startcodeprefix_len;
            memcpy(nalu->buffer, &buffer[nalu->startcodeprefix_len], nalu->len);
            nalu->forbidden_bit = nalu->buffer[0] & 0x80;     //1 bit
            nalu->nal_reference_idc = nalu->buffer[0] & 0x60; // 2 bit
            nalu->nal_unit_type = (nalu->buffer[0]) & 0x1f;   // 5 bit
            free(buffer);
            return pos - 1;
        }

        buffer[pos++] = fgetc(h264);
        if (find_code3(&buffer[pos - 4]))
        {
            rewind = -4;
            break;
        }

        if (find_code2(&buffer[pos - 3]))
        {
            rewind = -3;
            break;
        }
    }

    fseek(h264, rewind, SEEK_CUR);

    nalu->len = (pos + rewind) - nalu->startcodeprefix_len;
    memcpy(nalu->buffer, &buffer[nalu->startcodeprefix_len], nalu->len);
    nalu->forbidden_bit = nalu->buffer[0] & 0x80;     //1 bit
    nalu->nal_reference_idc = nalu->buffer[0] & 0x60; // 2 bit
    nalu->nal_unit_type = (nalu->buffer[0]) & 0x1f;   // 5 bit
    free(buffer);
    return pos + rewind;
}

int main(int argc, char *argv[])
{
    h264 = fopen("/Users/chan/Documents/github/learn-video/media/sintel.h264", "rb");
    printf("-----+-------- NALU Table ------+---------+\n");
	printf("| NUM |    POS  |    IDC |  TYPE |   LEN   |\n");
	printf("-----+---------+--------+-------+---------+\n");
    NALU_t nalu;
    nalu.max_size = 100000;
    nalu.buffer = (char *)malloc(nalu.max_size);
    int data_offset = 0;
    int nal_num = 0;

    while (!feof(h264))
    {
        int data_lenth;
        data_lenth = get_annexb_NALU(&nalu);

        char type_str[20] = {0};
        switch (nalu.nal_unit_type)
        {
        case NALU_TYPE_SLICE:
            sprintf(type_str, "SLICE");
            break;
        case NALU_TYPE_DPA:
            sprintf(type_str, "DPA");
            break;
        case NALU_TYPE_DPB:
            sprintf(type_str, "DPB");
            break;
        case NALU_TYPE_DPC:
            sprintf(type_str, "DPC");
            break;
        case NALU_TYPE_IDR:
            sprintf(type_str, "IDR");
            break;
        case NALU_TYPE_SEI:
            sprintf(type_str, "SEI");
            break;
        case NALU_TYPE_SPS:
            sprintf(type_str, "SPS");
            break;
        case NALU_TYPE_PPS:
            sprintf(type_str, "PPS");
            break;
        case NALU_TYPE_AUD:
            sprintf(type_str, "AUD");
            break;
        case NALU_TYPE_EOSEQ:
            sprintf(type_str, "EOSEQ");
            break;
        case NALU_TYPE_EOSTREAM:
            sprintf(type_str, "EOSTREAM");
            break;
        case NALU_TYPE_FILL:
            sprintf(type_str, "FILL");
            break;
        }

        char idc_str[20] = {0};
        switch (nalu.nal_reference_idc >> 5)
        {
        case NALU_PRIORITY_DISPOSABLE:
            sprintf(idc_str, "DISPOS");
            break;
        case NALU_PRIRITY_LOW:
            sprintf(idc_str, "LOW");
            break;
        case NALU_PRIORITY_HIGH:
            sprintf(idc_str, "HIGH");
            break;
        case NALU_PRIORITY_HIGHEST:
            sprintf(idc_str, "HIGHEST");
            break;
        }

        fprintf(stdout, "%5d| %8d| %7s| %6s| %8d|\n", nal_num, data_offset, idc_str, type_str, nalu.len);
        data_offset = data_offset + data_lenth;

        nal_num++;
    }

    free(nalu.buffer);
    fclose(h264);
    return 0;
}