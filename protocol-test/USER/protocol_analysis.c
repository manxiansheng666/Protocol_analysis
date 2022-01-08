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

void sk_buff_init(sk_buff *skb_handle,uint8_t *buff,uint32_t buff_len)
{
	skb_handle->buff = buff;
	skb_handle->buff_len = buff_len;
	skb_handle->data_len = 0;
	skb_handle->head = buff;
	skb_handle->data = buff;
	skb_handle->tail = buff;
	skb_handle->end = buff + buff_len;
};

void skb_reserve(sk_buff *skb_handle,uint32_t len)
{
	skb_handle->data += len;
	skb_handle->tail += len;
}


uint8_t *skb_put(sk_buff *skb_handle, unsigned int len)
{
	uint8_t *ret = NULL;
	uint8_t *tail_buf = skb_handle->tail;
	tail_buf += len;
	if(tail_buf > skb_handle->end)
	{
		return ret;
	}
	ret = skb_handle->tail;
	skb_handle->tail += len;
	skb_handle->data_len  += len;
	return ret;
}


uint8_t *skb_push(sk_buff *skb_handle, unsigned int len)
{
	uint8_t *ret = NULL;
	uint8_t *data_buf = skb_handle->data;
	data_buf -= len;
	if(data_buf < skb_handle->head)
	{
		return ret;
	}
	skb_handle->data -= len;
	skb_handle->data_len  += len;
	ret = skb_handle->data;
	return ret;
}


