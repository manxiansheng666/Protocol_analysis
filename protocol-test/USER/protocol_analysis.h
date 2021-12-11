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


#pragma pack()

uint8_t if_find_frame_end(ring_buffer *ring_buffer_handle,uint8_t *content);
void door_control(uint8_t subcmd,uint8_t *data,uint8_t data_length);
#endif
