#include <cstdlib>
#include <cstdio>
#include <cstdint>

uint8_t options = {0};

size_t valueCount = 3;

void iTouint8(const int a, uint8_t *array)
{
    array[0] = (a >> 24) & 0xFF;
    array[1] = (a >> 16) & 0xFF;
    array[2] = (a >>  8) & 0xFF;
    array[3] = (a >>  0) & 0xFF;
}

int main()
{

    int valueArray[] = {48723912, -3458392, 2856};

    uint8_t array[4];

    uint8_t fullArray[12];

    int k = 0;
    for (int i = 0; i < valueCount; i++)
    {
        iTouint8(valueArray[i], array);
        for (int j =0;j < 4; j++)
        {
            fullArray[k] = array[j];
            k++;
        }
    }

    for (int i = 0; i < 12; i++)
    {
        if (i%4 == 0) printf("\n");
        printf("%02X",fullArray[i]);
    }
    return 0;
}