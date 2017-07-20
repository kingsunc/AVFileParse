#pragma once
#include <string>
#include <vector>
using namespace std;

typedef unsigned char byte;
typedef byte* byteptr;

struct Parameter
{
	byteptr	pData;
	int		iSize;
};

class BaseBox;
typedef vector<BaseBox*>	VecBoxs;
typedef vector<string>		VecStrs;
typedef vector<Parameter>	VecParameters;

#define min(a,b)            (((a) < (b)) ? (a) : (b))

#define BOXSIZE_SIZE			4			// box size: 4 bytes;
#define BOXTYPE_SIZE			4			// box type: 4 bytes;
#define BOXHEADER_SIZE			(BOXSIZE_SIZE + BOXTYPE_SIZE)		

struct box_header
{
	byte BoxSize[BOXSIZE_SIZE];
	byte BoxType[BOXTYPE_SIZE];
};

/********************************************************************************************
**		字段名称			|	长度(bytes)	|		有关描述
--------------------------------------------------------------------------------------------
**		boxsize				|	4			|		box的长度
**		boxtype				|	4			|		box的类型
********************************************************************************************/
class BaseBox
{
public:
	BaseBox();
	virtual ~BaseBox();

	// 根据文件信息获取 对象结构体;
	static BaseBox* GetBoxFromFile(byteptr &pData);

	// 将pData后的iCount字节转换为数字
	// bOffset 转换后pData是否后移iCount个字节;
	// bBigEndian 大端排序;
	static int BytesToIntEx(byteptr &pData, int iCount, bool bOffset = true, bool bBigEndian = true);

	static int BytesToInt(byte arrByte[], int iCount);

	// 将pData后的iCount字节转换为字符串;
	// bOffset 转换后pData是否后移iCount个字节;
	static std::string BytesToStrEx(byteptr &pData, int iCount, bool bOffset = true);

	static std::string BytesToStr(byte arrByte[], int iCount);

	static std::string BytesToStr(char* pByte, int iCount);

	// iM, iN分别对应整数、小数位数;
	static double BytesToDoubleEx(byteptr &pData, int iM, int iN, bool bOffset = true);

	// iM, iN分别对应整数、小数位数;
	static double BytesToDouble(byte arrByte[], int iM, int iN);

	// 将iPos后的iCount位数转化为数字(此处按比特处理);
	static int BitsToIntEx(byteptr &pData, int iPos, int iCount, bool bOffset = true);

	// 解析完毕后, 返回解析的总字节数;
	virtual int ParseBox(byteptr &pData);

protected:

	virtual int ParseHeader(byteptr &pData);
	virtual int ParseAttrs(byteptr &pData);
	virtual int ParseChild(byteptr &pData);

	static BaseBox* CreateObject();

public:
	int			m_iBoxSize;				// Box 大小;
	string		m_strBoxType;			// Box 类型;

	VecBoxs		m_vecBoxs;				// 子 Box;
};