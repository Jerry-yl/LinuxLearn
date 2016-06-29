/*************************************************************************
*
* �ļ����� : typedefs.h
* ��    �� : ���ݱ�׼ͷ�ļ����¶���ı������͵�ͷ�ļ���
*
* �� �� �� : Jerry
* ���ʱ�� : 2016��06��29��
* 
* Copyright NXP B.V. 2012. All rights reserved
*************************************************************************/

#ifndef _TYPE_DEFS_H_
#define _TYPE_DEFS_H_

#include "stdint.h"

#ifndef bool
	typedef uint8_t bool;
#endif

typedef int8_t                  int8;
typedef int16_t                 int16;
typedef int32_t                 int32;
typedef int64_t                 int64;
typedef uint8_t                 uint8;
typedef uint16_t               uint16;
typedef uint32_t               uint32;
typedef uint64_t               uint64;

typedef char *                  string;

#endif