#include "TagScript.h"


CTagScript::CTagScript()
{
}

CTagScript::~CTagScript()
{
}

void CTagScript::Display()
{
	// 解析类型信息;
	byte* pTemp = m_pData;
	byte* pEnd = pTemp + m_iSize;

	while (pTemp < pEnd)
	{
		byte DataType = pTemp[0];
		pTemp = pTemp + 1;		// 读出类型 后移一位;
		switch (DataType)
		{
		case AMF_NUMBER:		// 数字(double);
			break;
		case AMF_BOOLEAN:		// 布尔;
			break;
		case AMF_STRING:		// 字符串;
			{
				AVal strName;
				AMF_DecodeString((char*)pTemp, &strName);
				pTemp = pTemp + (strName.av_len + 2);

				printf("\t %s : \n", BytesToStr((byte_ptr)strName.av_val, strName.av_len).c_str());
			}
			break;
		case AMF_OBJECT:		// 对象;
		case AMF_MOVIECLIP:		// 保留,未使用;
			break;
			// AMF_NULL,				// null;
			// AMF_UNDEFINED,			// 未定义;
			// AMF_REFERENCE,			// 引用;
		case AMF_ECMA_ARRAY:			// 数组;
			{
				// 数组元素个数;
				int iArrCount = BytesToInt(pTemp, 4);
				pTemp = pTemp + 4;

				AMFObject objTemp;
				int iSize = pEnd - pTemp;
				int nRes = AMF_DecodeArray(&objTemp, (char*)pTemp, iSize, iArrCount, TRUE);
				if (nRes == -1)
				{
					return;
				}
				pTemp = pTemp + iSize;

				// 输出;
				printf("\t { \n");
				for (int i = 0; i < iArrCount; i++)
				{
					switch (objTemp.o_props[i].p_type)
					{
					case AMF_NUMBER:			// 数字(double);
						{
							printf("\t     %s : %.0f\n", BytesToStr((byte_ptr)objTemp.o_props[i].p_name.av_val, objTemp.o_props[i].p_name.av_len).c_str(), objTemp.o_props[i].p_vu.p_number);
						}
						break;
					case AMF_STRING:			// 字符串; 
						{
							std::string strValue(objTemp.o_props[i].p_vu.p_aval.av_val, objTemp.o_props[i].p_vu.p_aval.av_len);
							printf("\t     %s : %s\n", BytesToStr((byte_ptr)objTemp.o_props[i].p_name.av_val, objTemp.o_props[i].p_name.av_len).c_str(), strValue.c_str());
						}
						break;
					default:
						break;
					}
				}
				printf("\t } \n");
			}
			break;
			// AMF_OBJECT_END,			// 对象结束(0x09);
			// AMF_STRICT_ARRAY,		// 严格的数组;
			// AMF_DATE,				// 日期;
			// AMF_LONG_STRING,			// 长字符串;
			// AMF_UNSUPPORTED,			// 未支持;
			// AMF_RECORDSET,			// 保留,未使用;
			// AMF_XML_DOC,				// xml文档;
			// AMF_TYPED_OBJECT,		// 有类型的对象;
			// AMF_AVMPLUS,				// 需要扩展到AMF3;
			// AMF_INVALID = 0xff		// 无效的;
		default:
			break;
		}
	}
}
