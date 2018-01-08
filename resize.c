#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    float f = atof(argv[1]);

    if (f < 0.0 || f > 100.0)
    {
        fprintf(stderr, "Can not increase file size beyond a factor of 100.0\n");
        return 2;
    }

    char *infile = argv[2];
    char *outfile = argv[3];

    FILE *inputr = fopen(infile, "r");
    if (inputr == NULL)
    {
        fprintf(stderr,"Could not open %s\n", infile);
        return 3;
    }

    FILE *outputr = fopen(outfile, "w");
    if (outputr == NULL)
    {
        fprintf(stderr,"Could not create %s\n", outfile);
        return 4;
    }

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inputr);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inputr);


    int padding1 = fmod(4 - fmod(bi.biWidth * sizeof(RGBTRIPLE), 4), 4);
    int padding2 = fmod(4 - fmod(bi.biWidth * sizeof(RGBTRIPLE) * f, 4), 4);

    if(padding2 < 0)
    {
        padding2 *= -1;
    }

    printf("padding2: %i\n", padding2);

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(inputr);
        fclose(outputr);
        fprintf(stderr, "Unsupported file format\n");
        return 5;
    }

    printf("orig biSizeImage: %i\norig width: %i\norig height: %i\n", bi.biSizeImage, bi.biWidth, bi.biHeight);
    bi.biWidth = bi.biWidth * f;
    bi.biHeight = bi.biHeight * f;
    if (bi.biHeight < 0)
    {
        bi.biSizeImage = -bi.biHeight * bi.biWidth * 3 + (padding2 * -bi.biHeight);
    }
    else
    {
        bi.biSizeImage = bi.biHeight * bi.biWidth * 3 * 3 + (padding2 * bi.biHeight);
    }
    bf.bfSize = bi.biSizeImage + 54;
    printf("new biSizeImage: %i\nnew width: %i\nnew height: %i\n", bi.biSizeImage, bi.biWidth, bi.biHeight);


    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outputr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outputr);


    if(bi.biHeight < 0)
    {
        for (int i = 0; i < -(bi.biHeight / f); i++)
        {
            for (int j = 0; j < f; j++)
            {
                for (int k = 0; k < (bi.biWidth / f); k++)
                {
                    RGBTRIPLE triple;

                    fread(&triple, sizeof(RGBTRIPLE), 1, inputr);

                    for (int u = 0; u < f; u++)
                    {
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outputr);
                    }

                }

                for (int l = 0; l < padding2; l++)
                {
                     fputc(0x00, outputr);
                }

                fseek(inputr, -(bi.biWidth / f) * 3, SEEK_CUR);
            }

            fseek(inputr, ((bi.biWidth / f) * 3), SEEK_CUR);
            fseek(inputr, padding1, SEEK_CUR);
        }
    }

    else
    {
        for (int i = 0; i < (bi.biHeight / f); i++)
        {
            for (int j = 0; j < f; j++)
            {
                for (int k = 0; k < (bi.biWidth / f); k++)
                {
                    RGBTRIPLE triple;

                    fread(&triple, sizeof(RGBTRIPLE), 1, inputr);

                    for (int u = 0; u < f; u++)
                    {
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outputr);
                    }

                }

                for (int l = 0; l < padding2; l++)
                {
                     fputc(0x00, outputr);
                }

                fseek(inputr, -((bi.biWidth / f) * 3), SEEK_CUR);
            }

            fseek(inputr, ((bi.biWidth / f) * 3), SEEK_CUR);
            fseek(inputr, padding1, SEEK_CUR);
        }
    }
    return 0;
}