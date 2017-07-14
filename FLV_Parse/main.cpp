#include <windows.h>
#include "FLVFile.h"
#include <assert.h>

void main()
{
	// FLV;
	CFLVFile flvFile;
	bool bRes = flvFile.LoadFile("../testfile/flv_test.flv");
	assert(bRes);
}