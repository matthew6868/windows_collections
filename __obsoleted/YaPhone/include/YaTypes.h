/**	@file	YaTypes.h
 *	\brief	
 *	
 *	\author xuyao<matthew6868@163.com>
 *
 *	\revised history[version,who,when,what]
 *		v1.0.0,xuyao,2007-02-13,created.
 */

#ifndef __YATYPES_H__
#define __YATYPES_H__

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN
#endif
#undef BIG_ENDIAN

#ifdef WINVER
#undef WINVER
#define WINVER 0x0400
#endif
#define WINVER 0x0400

#ifndef _X86_
#define _X86_
#endif

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windef.h>
#include <winbase.h>
#include <time.h>

typedef char                    YaInt8;
typedef unsigned char           YaUint8;
typedef short                   YaInt16;
typedef unsigned short          YaUint16;
typedef int                     YaInt32;
typedef unsigned int            YaUint32;
typedef __int64                 YaInt64;
typedef unsigned __int64        YaUint64;

typedef int                     YaInt;
typedef unsigned int            YaUint;
typedef void                    YaVoid;
typedef int                     YaBool;
typedef char                    YaChar;
typedef unsigned char           YaByte;
typedef short                   YaShort;
typedef unsigned short          YaWord;
typedef long                    YaLong;
typedef unsigned long           YaDword;
typedef float                   YaFloat;
typedef double                  YaDouble;

typedef unsigned int            YaSocket;
typedef HANDLE                  YaHandle;

typedef int						YaStatus;

#define YA_S_OK             0 
#define YA_E_FAIL			-1	/* There was an error, but we don't know specifics */
#define YA_E_OUTOFRESOURCES -2	/* no resource left for this operation */
#define YA_E_BADPARAM		-3  /* parameter value invalid */
#define YA_E_NULLPTR		-4  /* required pointer parameter was a NULL pointer */
#define YA_E_OUTOFRANGE		-5  /* parameter out of range */
#define YA_E_DESTRUCTED		-6  /* operation attempted on a destructed object */
#define YA_E_NOTSUPPORTED	-7  /* request not supported under current configuration */
#define YA_E_UNINITIALIZED	-8  /* object uninitialized */


#ifndef TRUE
#define TRUE			1
#endif

#ifndef FALSE
#define FALSE			0
#endif

#ifndef NULL
#define NULL			0
#endif

#ifndef SOCKER_ERROR
#define SOCKER_ERROR	-1
#endif

#define IN
#define OUT
#define INOUT

#define YAAPI               __declspec(dllexport)
#define YACALLCONV          __cdecl/* __stdcall */

#endif	/* __YATYPES_H__ */
