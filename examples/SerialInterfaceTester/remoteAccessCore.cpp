/*
 * File:   remoteAccessCore.c
 * Author: jeu.tonete
 *
 * Created on 11 de Dezembro de 2019, 08:53
 */

#include "remoteAccessCore.h"

#define VERSION_FIRMWARE	0x0001
#define PRODUCT_ID		0xCAFE
#define JSON_VERSION	    	0

const xVersion version = {
	.product_id      = PRODUCT_ID,
    .firmwareversion = VERSION_FIRMWARE,
    .jsonversion     = JSON_VERSION,
    .duff            = 0xBEAF,
    .checksum        = 0xC0CA
};

feedbackWrite fbWrite __attribute__((packed));
xjmodem jmodem __attribute__((packed));
xStateMachineEventFlags *remoteAccess_MSeventflags __attribute__((packed));

/*
 -------------------------------------------------------------------------------
 |   syncword   |  datalength   |      CMD      |     DATA      |     CRC      |
 |   2  bytes   |    1  byte    |    1  byte    |               |   2  bytes   |              
 -------------------------------------------------------------------------------
                                 \____________________________________________/
                                                       |
                                                       V
                                                    PAYLOAD                   */
void remoteAccessCore_EventflagConfig(xStateMachineEventFlags *flags)
{
    remoteAccess_MSeventflags = flags;
}

void remoteAccessCore_RXHandlerFromISR(remoteAccessObj *self, char data)
{
    
    if(self->timeoutEnable == true)
    {
        if( (vStateMachineMillis() - self->storedTime) > FRAME_TIMEOUT)
        {
            self->timeoutEnable = false;
            self->state = WORDSYNC0;
        }
    }
    
    switch (self->state)
    {
        case WORDSYNC0:
            /* First sync byte trigger */
            self->rxready = false;
            if (data == SYNC0)
            {
                self->state = WORDSYNC1;
            }
            break;
        case WORDSYNC1:
            /* Second sync byte trigger */
            if (data == SYNC1)
            {
                self->state = STOREDATASIZE;
                self->timeoutEnable = true;
                self->storedTime = vStateMachineMillis();
            } else
            {
                self->state = WORDSYNC0;
            }
            break;
        case STOREDATASIZE:
            /* Store the data length starting from next data */
            if ( ((unsigned int)data >= ((unsigned int)DATALENGTH_DataSize+(unsigned int)DATALENGTH_CRC)) ||
                 ((unsigned int)data <= (unsigned int)DATALENGTH_MAXDATASIZE) )
            {
                self->sizeReceived = (unsigned int)(data & 0xFF);
                self->inputBufferIndex = 0;
                self->inputBuffer[self->inputBufferIndex++] = data;
                self->state = STOREDATA;
            } else
            {
                self->state = WORDSYNC0;
            }
            break;
        case STOREDATA:
            /* Store the payload */
            self->inputBuffer[self->inputBufferIndex++] = data;
            if ((self->inputBufferIndex-1) == self->sizeReceived)
            {
                self->state = WORDSYNC0;
                self->rxready = true;
                self->timeoutEnable = false;
            }
            break;
        
        default:
            self->state = WORDSYNC0;
            break;
    }
}

void remoteAccessCore_RXHandler(remoteAccessObj *self, char data)
{
    int i;
    char test0, test1;
    
    if(self->strFinder.enable == true)
    {
        for(i = 0; i < (self->strFinder.buffer_length-1); i++)
        {
            self->strFinder.buffer[i] = self->strFinder.buffer[i+1];
        }
        self->strFinder.buffer[i] = data;
        
        i=0;
        while(true)
        {
            test0 = self->strFinder.buffer[i];
            test1 = self->strFinder.strRef[i];
            if(test0 == test1)
            {
                i++;
                if(i >= self->strFinder.strRef_length)
                {
                    //self->strFinder.strMatch = true;
                    self->strFinder.timeoutEnable = true;
                    self->strFinder.storedTime = vStateMachineMillis();
                    break;
                }
            }else{
                self->strFinder.timeoutEnable = false;
                break;
            }
        }
    }
    
    if(self->timeoutEnable == true)
    {
        if( (vStateMachineMillis() - self->storedTime) > FRAME_TIMEOUT)
        {
            self->timeoutEnable = false;
            self->state = WORDSYNC0;
        }
    }
    
    switch (self->state)
    {
        case WORDSYNC0:
            /* First sync byte trigger */
            self->rxready = false;
            if (data == SYNC0)
            {
                self->state = WORDSYNC1;
                
            }
            break;
        case WORDSYNC1:
            /* Second sync byte trigger */
            if (data == SYNC1)
            {
                self->state = STOREDATASIZE;
                self->timeoutEnable = true;
                self->storedTime = vStateMachineMillis();
            } else
            {
                self->state = WORDSYNC0;
            }
            break;
        case STOREDATASIZE:
            /* Store the data length starting from next data */
            if ( ((unsigned int)data >= ((unsigned int)DATALENGTH_DataSize+(unsigned int)DATALENGTH_CRC)) ||
                  ((unsigned int)data <= (unsigned int)DATALENGTH_MAXDATASIZE) )
            {
                self->sizeReceived = (unsigned int)(data & 0xFF);
                self->inputBufferIndex = 0;
                self->inputBuffer[self->inputBufferIndex++] = data;
                self->state = STOREDATA;
            } else
            {
                self->state = WORDSYNC0;
            }
            break;
        case STOREDATA:
            /* Store the payload */
            self->inputBuffer[self->inputBufferIndex++] = data;
            
            if ((self->inputBufferIndex-1) == self->sizeReceived)
            {
                self->state = WORDSYNC0;
                self->rxready = true;
                self->timeoutEnable = false;
            }
            break;
        
        default:
            self->state = WORDSYNC0;
            break;
    }
}

void remoteAccessCore_ParseReceivedData(remoteAccessObj *self)
{
    static IDoutput toSend;
    /*char tempNum[ADDR_PHONENUMBER_SIZE+1];
    char tempAddr[UBX_APNADDR_SIZE+1];*/
    char *pch;
    int temp=0;
    uint16_t utemp = 0;
    
    uint16_t i=0, j=0, k=0;
    uint32_t iterator;
//    static int mgntest = 0;
    uint16_t checksum = 0;
    
    if (crc16(self->inputBuffer, 0, self->inputBufferIndex) == 0)
    {
        switch (self->inputBuffer[1])
        {
            
            case GET_ID:
                toSend.lowByte = (uint8_t)((version.product_id & 0xFF00)>>8);
                toSend.highByte = (uint8_t)(version.product_id & 0xFF);
                remoteAccessCore_sendStruct(self, GET_ID, &toSend, DATALENGTH_ID);
                break;

            case SET_1:
                remoteAccess_MSeventflags->flags = SET_1;
                fbWrite.response = WRITE_OK;
                remoteAccessCore_sendStruct(self, SET_1, &fbWrite, DATALENGTH_FEEDBACKWRITE);
                break;

            case SET_2:
                remoteAccess_MSeventflags->flags = SET_2;
                fbWrite.response = WRITE_OK;
                remoteAccessCore_sendStruct(self, SET_2, &fbWrite, DATALENGTH_FEEDBACKWRITE);
                break;

            default:
                fbWrite.response = WRITE_ERROR;
                remoteAccessCore_sendStruct(self, COMMAND_NOTFOUND, &fbWrite, DATALENGTH_FEEDBACKWRITE);
                break;
        }
    }else
    {
        // When CRC check fail
        remoteAccessCore_sendStruct(self, CRC_ERROR, NULL, DATALENGTH_FEEDBACK);
    }
}

void remoteAccessCore_sendStruct(remoteAccessObj *self, int tag, void *ptr, int size){
    int crc;
    int i=0;
    self->outputBuffer[i++] = SYNC0;
    self->outputBuffer[i++] = SYNC1;
    self->outputBuffer[i++] = size+DATALENGTH_CRC+DATALENGTH_Tag;
    self->outputBuffer[i++] = tag;

    if(size != 0)
    {
        memcpy(&self->outputBuffer[i], ptr, size);
        i += size;
    }

    crc = crc16(self->outputBuffer, DATALENGTH_SyncWord, size + DATALENGTH_Tag + DATALENGTH_DataSize);

    self->outputBuffer[i++] = (unsigned char)(crc);
    self->outputBuffer[i++] = (unsigned char)(crc>>8);
    
    if(self->pSendData != NULL)
    {
        self->pSendData(self->outputBuffer, i);
    }
//    write_UART1(self->outputBuffer,i);
}

uint16_t crc16(uint8_t *src, uint16_t offset, uint16_t size)
{
    uint16_t iCRC16 = 0xFFFF;
    uint16_t polCRC16 = 0x8408;
    uint8_t data;
    uint16_t temp = 0;
    uint16_t j,i;
    for (i = 0; i < size; i++)
    {
        data = src[offset+i];
        for (j = 0; j < 8; j++)
        {
            temp = (uint16_t)(data ^ iCRC16);
            iCRC16 >>= 1;
            if ((temp & 0x01) > 0)
            {
                iCRC16 = (uint16_t)(iCRC16 ^ polCRC16);
            }
            data >>= 1;
        }
    }
    return iCRC16;
}
