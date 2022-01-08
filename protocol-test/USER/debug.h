#ifndef __DEBUG_H
#define __DEBUG_H
#include "usart.h"

//这里的printf函数大概要用到4K的flash

#define print_debug  	0

#define DebugUart huart1

#if print_debug
	#define debug_printf(format,...)	myprintf(format,##__VA_ARGS__)
#else
	#define	debug_printf(format,...)
#endif

void myprintf(const char *format,...);

#endif
