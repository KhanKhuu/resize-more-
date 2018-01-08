#include <cs50.h>
#include <math.h>
#include <stdio.h>

#include "bmp.h"

int main(void)
{
    float f = 5;
    float padding = ((4 - 3 % 4) % 4);
    float padding2 = fmod(fmod(4 - (3 * f), 4), 4);
    printf("padding1 is %f\npadding2 is %f\n", padding, padding2);

    int padding3 = ((4 - 3) % 4) % 4;
    int padding4 = ((4 - (3 * 5)) % 4) % 4;
    printf("padding3 is %i\npadding4 is %i\n", padding3, padding4);
    return 0;
}

