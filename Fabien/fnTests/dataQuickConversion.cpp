#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <iostream>
#include <cmath>

#define valueCount 3


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

void checkFullArray(uint8_t arr[], size_t size)
{

    for (size_t i = 0; i < size + 1; i++)
    {
        fprintf(stderr,"%02X ",arr[i]);
    }
    fprintf(stderr,"\n\n");

}


int main()
{


    int valueArray[valueCount];

    valueArray[0] = (int) ((float)lat() * pow(10,6));
    valueArray[1] = (int) ((float)lng() * pow(10,6)); 
    valueArray[2] = (int) ((float)meters() * pow(10,2));

    uint8_t fullArray[valueCount * 4];

    for (size_t i = 0; i < valueCount; i++)
    {
        fullArray[i*4]   = (valueArray[i] >> 24) & 0xFF;
        fullArray[i*4+1] = (valueArray[i] >> 16) & 0xFF;
        fullArray[i*4+2] = (valueArray[i] >> 8)  & 0xFF;
        fullArray[i*4+3] = (valueArray[i])       & 0xFF;
        //checkFullArray(fullArray ,i*4+3);
    }
/*
    for (size_t i = 0; i < 12; i++)
    {
        printf("%02X ",fullArray[i]);
    }
*/

    send(12, fullArray, 1, 1);

    return 0;
}