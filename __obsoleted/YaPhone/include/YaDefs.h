/**	@file	YaDefs.h
 *	\brief	
 *	
 *	\author xuyao<matthew6868@163.com>
 *
 *	\revised history[version,who,when,what]
 *		v1.0.0,xuyao,2007-02-13,created.
 */

#ifndef __YADEFS_H__
#define __YADEFS_H__

#include "YaAfx.h"
#include "YaTypes.h"

typedef enum
{
	YA_ADDRESS_TYPE_IP	= 0x1 ,
	YA_ADDRESS_TYPE_PHONE,
	YA_ADDRESS_TYPE_URI,
	YA_ADDRESS_TYPE_EMAIL,
}EYaAddressType;

#endif	/* __YADEFS_H__ */