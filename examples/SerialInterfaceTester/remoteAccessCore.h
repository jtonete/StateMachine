/*
 * File:   remoteAccessCore.c
 * Author: jeu.tonete
 *
 * Created on 11 de Dezembro de 2019, 08:59
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef REMOTEACCESSCORE_H
#define	REMOTEACCESSCORE_H

#include "StateMachine.h"
#include <string.h>

#define SYNC0	0x50
#define SYNC1	0x63

#define FRAME_TIMEOUT	6000

enum FEEDBACKSTATUS{
	WRITE_ERROR = 0xFF,
	WRITE_OK = 0x00,
};

typedef struct _sFeedbackWrite{
	uint8_t response;
} feedbackWrite;

typedef struct _sIDoutput{
	uint8_t lowByte;
	uint8_t highByte;
} IDoutput;

enum RXSTATE
{
    WORDSYNC0=0,
    WORDSYNC1,
    STOREDATASIZE,
    STOREDATA,
};

enum DATALENGTH
{
    DATALENGTH_SyncWord = 2,
    DATALENGTH_DataSize = 1,
    DATALENGTH_Tag = 1,
    DATALENGTH_CRC = 2,
    DATALENGTH_MAXDATASIZE=255,
	DATALENGTH_ID = 2,

	DATALENGTH_FEEDBACKWRITE = sizeof(feedbackWrite),
	DATALENGTH_FEEDBACK = 0,
	
};

enum TAGS
{
    GET_ID = 0x00,
	GET_1,
	GET_2,
	GET_3,
	GET_4,
	GET_5,
	GET_6,
	GET_7,
	GET_8,
	GET_9,
	GET_10,
	GET_11,
	GET_12,
	GET_13,
	GET_14,
	GET_15,

	SET_1 = 0x80,
	SET_2,
	SET_3,
	SET_4,
	SET_5,
	SET_6,
	SET_7,
	SET_8,
	SET_9,
	SET_10,
	SET_11,
	SET_12,
	SET_13,
	SET_14,
	SET_15,
	SET_16,
	SET_17,
	SET_18,
	SET_19,
	SET_20,


	CRC_ERROR = 0xFE,
	COMMAND_NOTFOUND = 0xFF,
};

typedef void ( *remoteAccessCoreTX)(unsigned char *data_out, int size);

typedef struct _sStrFinderObj {
	uint8_t enable;
	uint8_t strMatch;
	uint8_t strRef_length;
	char* strRef;
	uint8_t buffer_length;
	char* buffer;
	uint8_t timeoutEnable;
	uint32_t storedTime;
}strFinderObj;

typedef struct _sRXHANDLER { //
	uint16_t state;
	uint16_t sizeReceived;
	uint16_t inputBufferIndex;
    uint16_t rxready;
    unsigned char inputBuffer[DATALENGTH_MAXDATASIZE+3];
    unsigned char outputBuffer[DATALENGTH_MAXDATASIZE+3];
	remoteAccessCoreTX pSendData;
	uint32_t storedTime;
	uint8_t timeoutEnable;
    strFinderObj strFinder;
} remoteAccessObj;

typedef struct _sVersion{
  uint16_t product_id;
  uint16_t firmwareversion;
  uint16_t jsonversion;
  uint16_t duff;
  uint16_t checksum;
} xVersion;

#define INSTRUCTIONS_IN_ROW	64
typedef struct _sJmodemObj {
	struct _stJmodemFlags {
		uint8_t StartOfTransmition : 1; // 0
		uint8_t EndOfTransmition : 1; // 1
		uint8_t EMPTY2 : 1; // 2
		uint8_t EMPTY3 : 1; // 3
		uint8_t EMPTY4 : 1; // 4
		uint8_t EMPTY5 : 1; // 5
		uint8_t EMPTY6 : 1; // 6
		uint8_t EMPTY7 : 1; // 7
	} flags;
	
	

} xjmodem;

void remoteAccessCore_EventflagConfig(xStateMachineEventFlags *flags);
void remoteAccessCore_RXHandlerFromISR(remoteAccessObj *self, char data);
void remoteAccessCore_RXHandler(remoteAccessObj *self, char data);
void remoteAccessCore_ParseReceivedData(remoteAccessObj *self);
void remoteAccessCore_sendStruct(remoteAccessObj *self, int tag, void *ptr, int size);
uint16_t crc16(uint8_t *src, uint16_t offset, uint16_t size);

#endif	/* REMOTEACCESSCORE_H */

