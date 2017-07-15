#ifndef __AMF_H__
#define __AMF_H__
#include "bytes.h"

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

#include <windows.h>
#include <stdint.h>

#ifdef _MSC_VER	/* MSVC */
#define snprintf _snprintf
#define strcasecmp stricmp
#define strncasecmp strnicmp
#define vsnprintf _vsnprintf
#endif

/************************************************************************************************************
*	AMF分成两种;
*	1. AMF0，基本的数据转换规则
*	2. AMF3，是AMF0的扩展
************************************************************************************************************/
// AMF分成两种：     
#ifdef __cplusplus
extern "C"
{
#endif

	// AMF0数据类型;
	typedef enum
	{
		AMF_NUMBER = 0,			// 数字(double);
		AMF_BOOLEAN,			// 布尔;
		AMF_STRING,				// 字符串;
		AMF_OBJECT,				// 对象;
		AMF_MOVIECLIP,			// 保留,未使用;
		AMF_NULL,				// null;
		AMF_UNDEFINED,			// 未定义;
		AMF_REFERENCE,			// 引用;
		AMF_ECMA_ARRAY,			// 数组;
		AMF_OBJECT_END,			// 对象结束(0x09);
		AMF_STRICT_ARRAY,		// 严格的数组;
		AMF_DATE,				// 日期;
		AMF_LONG_STRING,		// 长字符串;
		AMF_UNSUPPORTED,		// 未支持;
		AMF_RECORDSET,			// 保留,未使用;
		AMF_XML_DOC,			// xml文档;
		AMF_TYPED_OBJECT,		// 有类型的对象;
		AMF_AVMPLUS,			// 需要扩展到AMF3;
		AMF_INVALID = 0xff		// 无效的;
	}AMFDataType;

	// AMF3数据类型;
	typedef enum
	{
		AMF3_UNDEFINED = 0,		// 未定义;
		AMF3_NULL,				// null;
		AMF3_FALSE,				// false;
		AMF3_TRUE,				// true;
		AMF3_INTEGER,			// 数字int;
		AMF3_DOUBLE,			// double;
		AMF3_STRING,			// 字符串;
		AMF3_XML_DOC,			// xml文档;
		AMF3_DATE,				// 日期;
		AMF3_ARRAY,				// 数组;
		AMF3_OBJECT,			// 对象;
		AMF3_XML,				// xml;
		AMF3_BYTE_ARRAY			// 字节数组;
	} AMF3DataType;

	// AMF自定义的字符串;
	typedef struct AVal
	{
		char *av_val;
		int av_len;
	} AVal;

	// AVal的快速初始化;
#define AVC(str)		{str, sizeof(str)-1}
	// 比较AVal字符串;
#define AVMATCH(a1,a2)	((a1)->av_len == (a2)->av_len && !memcmp((a1)->av_val,(a2)->av_val,(a1)->av_len))

	struct AMFObjectProperty;

	// AMF对象, 就是由一系列的属性构成的;
	typedef struct AMFObject
	{
		int o_num;							// 属性数目;
		struct AMFObjectProperty *o_props;	// 属性数组;
	} AMFObject;

	// AMF对象的属性;
	typedef struct AMFObjectProperty
	{
		AVal p_name;			// 属性名称;
		AMFDataType p_type;		// 属性类型;
		union
		{
			double p_number;
			AVal p_aval;
			AMFObject p_object;
		} p_vu;					// 属性数值;
		short p_UTCoffset;	// UTC偏移;
	} AMFObjectProperty;

	char *AMF_EncodeString(char *output, char *outend, const AVal*  str);
	char *AMF_EncodeNumber(char *output, char *outend, double dVal);
	char *AMF_EncodeInt16(char *output, char *outend, short nVal);
	char *AMF_EncodeInt24(char *output, char *outend, int nVal);
	char *AMF_EncodeInt32(char *output, char *outend, int nVal);
	char *AMF_EncodeBoolean(char *output, char *outend, int bVal);

	/* Shortcuts for AMFProp_Encode */
	char *AMF_EncodeNamedString(char *output, char *outend, const AVal*  name, const AVal*  value);
	char *AMF_EncodeNamedNumber(char *output, char *outend, const AVal*  name, double dVal);
	char *AMF_EncodeNamedBoolean(char *output, char *outend, const AVal*  name, int bVal);

	unsigned short AMF_DecodeInt16(const char *data);
	unsigned int AMF_DecodeInt24(const char *data);
	unsigned int AMF_DecodeInt32(const char *data);
	void AMF_DecodeString(const char *data, AVal*  str);
	void AMF_DecodeLongString(const char *data, AVal*  str);
	int AMF_DecodeBoolean(const char *data);
	double AMF_DecodeNumber(const char *data);

	char *AMF_Encode(AMFObject * obj, char *pBuffer, char *pBufEnd);
	char *AMF_EncodeEcmaArray(AMFObject *obj, char *pBuffer, char *pBufEnd);
	char *AMF_EncodeArray(AMFObject *obj, char *pBuffer, char *pBufEnd);

	int AMF_Decode(AMFObject * obj, const char *pBuffer, int nSize,
		int bDecodeName);
	int AMF_DecodeArray(AMFObject * obj, const char *pBuffer, int nSize,
		int nArrayLen, int bDecodeName);
	int AMF3_Decode(AMFObject * obj, const char *pBuffer, int nSize,
		int bDecodeName);
	void AMF_Dump(AMFObject * obj);
	void AMF_Reset(AMFObject * obj);

	void AMF_AddProp(AMFObject * obj, const AMFObjectProperty * prop);
	int AMF_CountProp(AMFObject * obj);
	AMFObjectProperty *AMF_GetProp(AMFObject * obj, const AVal*  name,
		int nIndex);

	AMFDataType AMFProp_GetType(AMFObjectProperty * prop);
	void AMFProp_SetNumber(AMFObjectProperty * prop, double dval);
	void AMFProp_SetBoolean(AMFObjectProperty * prop, int bflag);
	void AMFProp_SetString(AMFObjectProperty * prop, AVal*  str);
	void AMFProp_SetObject(AMFObjectProperty * prop, AMFObject * obj);

	void AMFProp_GetName(AMFObjectProperty * prop, AVal*  name);
	void AMFProp_SetName(AMFObjectProperty * prop, AVal*  name);
	double AMFProp_GetNumber(AMFObjectProperty * prop);
	int AMFProp_GetBoolean(AMFObjectProperty * prop);

	void AMFProp_GetString(AMFObjectProperty * prop, AVal*  str);
	void AMFProp_GetObject(AMFObjectProperty * prop, AMFObject * obj);

	int AMFProp_IsValid(AMFObjectProperty * prop);

	char *AMFProp_Encode(AMFObjectProperty * prop, char *pBuffer, char *pBufEnd);
	int AMF3Prop_Decode(AMFObjectProperty * prop, const char *pBuffer,
		int nSize, int bDecodeName);
	int AMFProp_Decode(AMFObjectProperty * prop, const char *pBuffer,
		int nSize, int bDecodeName);

	void AMFProp_Dump(AMFObjectProperty * prop);
	void AMFProp_Reset(AMFObjectProperty * prop);

	typedef struct AMF3ClassDef
	{
		AVal cd_name;
		char cd_externalizable;
		char cd_dynamic;
		int cd_num;
		AVal* cd_props;
	} AMF3ClassDef;

	void AMF3CD_AddProp(AMF3ClassDef*  cd, AVal*  prop);
	AVal* AMF3CD_GetProp(AMF3ClassDef*  cd, int idx);

#ifdef __cplusplus
}
#endif

#endif				/* __AMF_H__ */