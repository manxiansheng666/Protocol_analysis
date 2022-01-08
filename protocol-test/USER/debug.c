#include "debug.h"
#include "stdarg.h"

uint8_t _dbg_TXBuff[100];
void myprintf(const char *format,...)
{
	uint32_t length;
	va_list args;

	va_start(args, format);
	length = vsnprintf((char *)_dbg_TXBuff, sizeof(_dbg_TXBuff)+1, (char *)format, args);
	va_end(args);
	
//	HAL_UART_Transmit_DMA(&DebugUart,_dbg_TXBuff,length);
	HAL_UART_Transmit(&DebugUart,_dbg_TXBuff,length,2000);
}
