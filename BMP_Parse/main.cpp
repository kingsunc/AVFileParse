#include "BMPFile.h"

int main()
{
	CBMPFile flvFile;
	bool bRes = flvFile.LoadFile("../testfile/bmp_test.bmp");
	assert(bRes);

	flvFile.Display();

	return 1;
}