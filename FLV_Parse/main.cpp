#include "FLVFile.h"

int main()
{
	CFLVFile flvFile;
	bool bRes = flvFile.LoadFile("../testfile/flv_test.flv");
	//assert(bRes);

	//flvFile.Display();

	// 起始dStart~dEnd的音视频保存文件;
	double dStart = 0.8;
	double dEnd = 0.9;
	bRes = flvFile.SaveFile("../testfile/flv_out.flv", dStart, dEnd);
	//assert(bRes);

	return 1;
}