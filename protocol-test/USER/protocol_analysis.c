#include "protocol_analysis.h"

uint8_t if_find_frame_end(ring_buffer *ring_buffer_handle,uint8_t *content)
{
	uint8_t ret;
	uint32_t offset_key;
	offset_key = Ring_Buffer_Find_Keyword(ring_buffer_handle,END,END_LENGTH);
	if(offset_key)
	{
		ret = offset_key + END_LENGTH - 1;
		Ring_Buffer_Read_String(ring_buffer_handle,content,ret);
		
		return ret;
	}
	
	//如果没有找到帧尾，并且缓冲区有数据，则应该把数据清除
	if(ring_buffer_handle->lenght)
	{
		Ring_Buffer_Clear(ring_buffer_handle);
	}
	
	return 0;
}

void door_control(uint8_t subcmd,uint8_t *data,uint8_t data_length)
{
	smd_door *door;
	switch(subcmd)
	{
		case 0x02:
			door = (smd_door *)data;
			myprintf("ID is %d,operate is %d\r\n",door->doorID,door->operate);
	}
}
