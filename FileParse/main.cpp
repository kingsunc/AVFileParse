#include <windows.h>
#include "FLVFile.h"
#include <assert.h>

// FLV ÎÄ¼þ½âÎö;
bool FLVParse_Test()
{
	CFLVFile flvFile;
	return flvFile.LoadFile("../testfile/flv_test.flv");
}

void main()
{
	// FLV;
	bool bRes = FLVParse_Test();
	assert(bRes);
}