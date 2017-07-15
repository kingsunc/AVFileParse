#ifndef __BYTES_H__
#define __BYTES_H__
#include <wchar.h>
#include <stdio.h>
#include <assert.h>

#ifdef _WIN32
/* Windows is little endian only */
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __BYTE_ORDER __LITTLE_ENDIAN
#define __FLOAT_WORD_ORDER __BYTE_ORDER

typedef unsigned char		byte;
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;

typedef byte*				byte_ptr;

#else /* !_WIN32 */

#include <sys/param.h>

#if defined(BYTE_ORDER) && !defined(__BYTE_ORDER)
#define __BYTE_ORDER    BYTE_ORDER
#endif

#if defined(BIG_ENDIAN) && !defined(__BIG_ENDIAN)
#define __BIG_ENDIAN	BIG_ENDIAN
#endif

#if defined(LITTLE_ENDIAN) && !defined(__LITTLE_ENDIAN)
#define __LITTLE_ENDIAN	LITTLE_ENDIAN
#endif

#endif /* !_WIN32 */
#include <string>

/* define default endianness */
#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN	1234
#endif

#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN	4321
#endif

#ifndef __BYTE_ORDER
#warning "Byte order not defined on your system, assuming little endian!"
#define __BYTE_ORDER	__LITTLE_ENDIAN
#endif

/* ok, we assume to have the same float word order and byte order if float word order is not defined */
#ifndef __FLOAT_WORD_ORDER
#warning "Float word order not defined, assuming the same as byte order!"
#define __FLOAT_WORD_ORDER	__BYTE_ORDER
#endif

#if !defined(__BYTE_ORDER) || !defined(__FLOAT_WORD_ORDER)
#error "Undefined byte or float word order!"
#endif

#if __FLOAT_WORD_ORDER != __BIG_ENDIAN && __FLOAT_WORD_ORDER != __LITTLE_ENDIAN
#error "Unknown/unsupported float word order!"
#endif

#if __BYTE_ORDER != __BIG_ENDIAN && __BYTE_ORDER != __LITTLE_ENDIAN
#error "Unknown/unsupported byte order!"
#endif

// 将pData后的iCount字节转换为数字
int BytesToInt(byte_ptr pData, int iCount);
// 将pData后的iCount字节转换为字符串;
std::string BytesToStr(byte_ptr pData, int iCount);

#endif