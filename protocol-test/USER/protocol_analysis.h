#ifndef __PROTOCOL_ANALYSIS_H
#define __PROTOCOL_ANALYSIS_H

#include "stdint.h"
#include "ring_buffer.h"
#include "debug.h"

#define END 		0x5A5A
#define END_LENGTH	2

#define HEAD		0xA5A5



#pragma pack(1)

typedef struct
{
	uint16_t bigin;
	uint8_t version;
	uint8_t msgID;
	uint32_t timePoint;
	uint8_t msgType;
	uint8_t command;
	uint8_t subCmd;
	uint8_t dataLength;
	uint8_t data[];
}message_head;

typedef struct
{
	uint8_t doorID;
	uint8_t operate;
}smd_door;

typedef struct
{
	uint8_t sum;
	uint16_t end;
}frame_end;
#pragma pack()

typedef struct 
{
	uint32_t buff_len;
	uint32_t data_len;
	uint8_t *tail;
	uint8_t *end;
	uint8_t *head;
	uint8_t *data;
	uint8_t *buff;
}sk_buff;



uint8_t if_find_frame_end(ring_buffer *ring_buffer_handle,uint8_t *content);
void door_control(uint8_t subcmd,uint8_t *data,uint8_t data_length);

void sk_buff_init(sk_buff *skb_handle,uint8_t *buff,uint32_t buff_len);
void skb_reserve(sk_buff *skb_handle,uint32_t len);
uint8_t *skb_put(sk_buff *skb_handle, unsigned int len);
uint8_t *skb_push(sk_buff *skb_handle, unsigned int len);
#endif
