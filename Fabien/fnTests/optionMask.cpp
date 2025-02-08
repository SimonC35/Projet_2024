#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <cmath>
#include <bitset>

#define maskDate 	0b10000000
#define maskLat  	0b01000000
#define maskLng  	0b00100000
#define maskAlt  	0b00010000
#define maskHdp  	0b00001000
#define maskSpd 	0b00000100
#define maskCrs 	0b00000010
#define maskSats 	0b00000001

#define maskAge 	0b10000000



using namespace std;

uint8_t options = 0b01110000;
uint8_t options2 = 0b00000000;

int main()
{
	cout << "bit Date : " << ((options & maskDate) ? "on\n" : "off\n");
	cout << "bit Lat : "  << ((options & maskLat) ? "on\n" :  "off\n");

	cout << "bit Age : "  << ((options2 & maskAge) ? "on\n" :  "off\n");

}