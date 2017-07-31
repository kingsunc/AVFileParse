#include "BMPFile.h"

int main()
{
	unsigned ch1 = 'B';
	unsigned ch2 = 'P';
	CBMPFile bmpFile;
	bool bRes = bmpFile.LoadFile("../testfile/bmp_test.bmp");
	assert(bRes);

	bmpFile.Display();

	return 1;
}