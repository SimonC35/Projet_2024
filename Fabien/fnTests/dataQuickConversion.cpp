#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <cmath>

#define valueCount 9

int valueArray[] = {48723912, -3458392, 2856};



void send(uint8_t datalen, uint8_t *data, uint8_t port, bool confirmed)
{
    printf("\n\n");
    for (size_t i = 0; i < datalen;i++)
    {
        printf("%02X", *data);
        data++;
        if (i%4 == 0) printf("\n");
    }


}

void intToUint8(const int a, uint8_t *array)
{
    array[0] = (a >> 24) & 0xFF;
    array[1] = (a >> 16) & 0xFF;
    array[2] = (a >>  8) & 0xFF;
    array[3] = (a >>  0) & 0xFF;
}

double lng()
{

    return 3.430201;
}

double lat()
{
    return 42.304930;
}

double meters()
{
    return 23.59;
}


int main()
{


    int valueArray[valueCount];


    printf("%ld\n\n\n",sizeof(short int));


    uint8_t fullArray[valueCount];

    for (size_t i = 0; i < valueCount; i++)
    {
        fullArray[i*4]   = (valueArray[i] >> 24) & 0xFF;
        fullArray[i*4+1] = (valueArray[i] >> 16) & 0xFF;
        fullArray[i*4+2] = (valueArray[i] >> 8)  & 0xFF;
        fullArray[i*4+3] = (valueArray[i])       & 0xFF;
    }

    for (size_t i = 0; i < 12; i++)
    {
        printf("%02X ",fullArray[i]);
    }


    //send(12, fullArray, 1, 1);

    return 0;
}