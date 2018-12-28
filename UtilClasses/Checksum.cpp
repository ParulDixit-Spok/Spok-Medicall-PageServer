
// checksum.cpp

// disable deprecation
#pragma warning(disable:4996)


#include "Checksum.h"
#include "windows.h"


ULONG CRC32Table[256];


	// Reflection is a requirement for the official CRC-32 standard.
	// You can create CRCs without it, but they won't conform to the standard.
	ULONG
Reflect(ULONG ref, char ch)
	{
	// Used only by Init_CRC32_Table()

	ULONG value(0);

	// Swap bit 0 for bit 7
	// bit 1 for bit 6, etc.
	for(int i = 1; i < (ch + 1); i++)
		{
		if(ref & 1)
			value |= 1 << (ch - i);
		ref >>= 1;
		}

	return value;
	}


	// Call this function only once to initialize the CRC table.
	void
InitCRC32Table(void)
	{
	// This is the official polynomial used by CRC-32
	// in PKZip, WinZip and Ethernet.
	ULONG ulPolynomial = 0x04c11db7;

	// 256 values representing ASCII character codes.
	for(int i = 0; i <= 0xFF; i++)
		{
		CRC32Table[i]=Reflect(i, 8) << 24;

		for (int j = 0; j < 8; j++)
			{
			CRC32Table[i] = (CRC32Table[i] << 1) ^ (CRC32Table[i] & (1 << 31) ? ulPolynomial : 0);
			}

		CRC32Table[i] = Reflect(CRC32Table[i], 32);
		}
	}


	// Once the lookup table has been filled in by the two functions above,
	// this function creates all CRCs using only the lookup table.
	unsigned int
GetCRC32(char *text, int length)
	{
	static BOOL FirstTimeThru = TRUE;

	// Be sure to use unsigned variables,
	// because negative values introduce high bits
	// where zero bits are required.

	// Start out with all bits set high.
	ULONG  ulCRC(0xffffffff);
	unsigned char* buffer;

	if(FirstTimeThru)
		{
		FirstTimeThru = FALSE;

		InitCRC32Table();
		}

	// Save the text in the buffer.
	buffer = (unsigned char*) (LPCTSTR) text;

	// Perform the algorithm on each character
	// in the string, using the lookup table values.
	while(length--)
		{
		ulCRC = (ulCRC >> 8) ^ CRC32Table[(ulCRC & 0xFF) ^ *buffer++];
		}

	// Exclusive OR the result with the beginning value.
	return ulCRC ^ 0xffffffff;
	}
