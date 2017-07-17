#include "FLVFile.h"

int main()
{
	CFLVFile flvFile;
	bool bRes = flvFile.LoadFile("../testfile/flv_test.flv");
	assert(bRes);

	flvFile.Display();

	return 1;
}