#include "AVIFile.h"

int main()
{
	CAVIFile aviFile;
	bool bRes = aviFile.LoadFile("../testfile/avi_test.avi");
	assert(bRes);

	return 1;
}