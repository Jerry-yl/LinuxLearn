/*************************************************************************
*
* 文件名称 : typedefs.h
* 功    能 : 根据标准头文件重新定义的变量类型的头文件。
*
* 开 发 者 : Jerry
* 完成时间 : 2016年06月29日
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