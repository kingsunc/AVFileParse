#include "MP4File.h"
#include <assert.h>

void main()
{
	CMP4File mp4File;
	bool bRes= mp4File.LoadFile("../testfile/mp4_test.mp4");
	assert(bRes);
}