#include "WAVFile.h"

int main()
{
	CWAVFile wavFile;
	bool bRes = wavFile.LoadFile("../testfile/wav_test.wav");
	assert(bRes);

	wavFile.Display();

	return 1;
}