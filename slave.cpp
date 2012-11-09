#include "slave.h"

//字地址 0 - 255 (只取低8位)
//位地址 0 - 255 (只取低8位)

/* CRC 高位字节值表 */
static const unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;

/* CRC低位字节值表*/
static const unsigned auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ;


unsigned short crc16(unsigned char *puchMsg, unsigned int nDataLen)
{
    unsigned char uchCRCHi = 0xFF ; /* 高CRC字节初始化 */
    unsigned char uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */
    unsigned int uIndex ;   /* CRC循环中的索引 */
    while (nDataLen--)      /* 传输消息缓冲区 */
    {
        uIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
        uchCRCLo = auchCRCLo[uIndex] ;
    }
    return (uchCRCHi << 8 | uchCRCLo) ;
}
/*
//读线圈状态
void ReadCoil(void)
{
    uint8 addr;
    uint8 tempAddr;
    uint8 byteCount;
    uint8 bitCount;
    uint16 crcData;
    uint8 position;
    uint8 i,k;
    uint16 tempData;
    uint8  exit = 0;

    //addr = (receBuf[2]<<8) + receBuf[3];
    //tempAddr = addr & 0xfff;
    addr = receBuf[3];
    tempAddr = addr;

    //bitCount = (receBuf[4]<<8) + receBuf[5];	//读取的位个数
    bitCount = receBuf[5];

    byteCount = bitCount / 8;					//字节个数
    if(bitCount%8 != 0)
        byteCount++;

    for(k=0;k<byteCount;k++)
    {//字节位置
        position = k + 3;
        sendBuf[position] = 0;
        for(i=0;i<8;i++)
        {
            getCoilVal(tempAddr,&tempData);

            sendBuf[position] |= tempData << i;
            tempAddr++;
            if(tempAddr >= addr+bitCount)
            {	//读完
                exit = 1;
                break;
            }
        }
        if(exit == 1)
            break;
    }

    sendBuf[0] = localAddr;
    sendBuf[1] = 0x01;
    sendBuf[2] = byteCount;
    byteCount += 3;
    crcData = crc16(sendBuf,byteCount);
    sendBuf[byteCount] = crcData >> 8;
    byteCount++;
    sendBuf[byteCount] = crcData & 0xff;
    sendCount = byteCount + 1;

    beginSend();
}//void readCoil(void)

//读寄存器
void readRegisters(void)
{
    uint8 addr;
    uint8 tempAddr;
    uint16 crcData;
    uint8 readCount;
    uint8 byteCount;
    uint16 i;
    uint16 tempData = 0;

    //addr = (receBuf[2]<<8) + receBuf[3];
    //tempAddr = addr & 0xfff;
    addr = receBuf[3];
    tempAddr = addr;

    //readCount = (receBuf[4]<<8) + receBuf[5];	//要读的个数
    readCount = receBuf[5];

    byteCount = readCount * 2;

    for(i=0;i<byteCount;i+=2,tempAddr++)
    {
        getRegisterVal(tempAddr,&tempData);
        sendBuf[i+3] = tempData >> 8;
        sendBuf[i+4] = tempData & 0xff;
    }

    sendBuf[0] = localAddr;
    sendBuf[1] = 3;
    sendBuf[2] = byteCount;
    byteCount += 3;
    crcData = crc16(sendBuf,byteCount);
    sendBuf[byteCount] = crcData >> 8;
    byteCount++;
    sendBuf[byteCount] = crcData & 0xff;

    sendCount = byteCount + 1;
    beginSend();
}//void readRegisters(void)


//强制单个线圈
void forceSingleCoil(void)
{
    uint8 addr;
    uint8 tempAddr;
    uint16 tempData;
    uint8  onOff;
    uint8 i;

    //addr = (receBuf[2]<<8) + receBuf[3];
    //tempAddr = addr & 0xfff;
    addr = receBuf[3];
    tempAddr = addr;

    //onOff = (receBuf[4]<<8) + receBuf[5];
    onOff = receBuf[4];

    //if(onOff == 0xff00)
    if(onOff == 0xff)
    {	//设为ON
        tempData = 1;
    }
    //else if(onOff == 0x0000)
    else if(onOff == 0x00)
    {	//设为OFF
        tempData = 0;
    }

    setCoilVal(tempAddr,tempData);

    for(i=0;i<receCount;i++)
    {
        sendBuf[i] = receBuf[i];
    }
    sendCount = receCount;
    beginSend();
}//void forceSingleCoil(void)


//设置多个寄存器
void presetMultipleRegisters(void)
{
    uint8 addr;
    uint8 tempAddr;
    uint8 byteCount;
    uint8 setCount;
    uint16 crcData;
    uint16 tempData;
    uint8 i;

    //addr = (receBuf[2]<<8) + receBuf[3];
    //tempAddr = addr & 0xfff;
    addr = receBuf[3];
    tempAddr = addr & 0xff;

    //setCount = (receBuf[4]<<8) + receBuf[5];
    setCount = receBuf[5];
    byteCount = receBuf[6];

    for(i=0;i<setCount;i++,tempAddr++)
    {
        tempData = (receBuf[i*2+7]<<8) + receBuf[i*2+8];

        setRegisterVal(tempAddr,tempData);
    }

    sendBuf[0] = localAddr;
    sendBuf[1] = receBuf[1];
    sendBuf[2] = addr >> 8;
    sendBuf[3] = addr & 0xff;
    sendBuf[4] = setCount >> 8;
    sendBuf[5] = setCount & 0xff;
    crcData = crc16(sendBuf,6);
    sendBuf[6] = crcData >> 8;
    sendBuf[7] = crcData & 0xff;
    sendCount = 8;
    beginSend();
}//void presetMultipleRegisters(void)

*/


/*
//取线圈状态 返回0表示成功
uint16 getCoilVal(uint16 addr,uint16 *tempData)
{
    uint16 result = 0;
    uint16 tempAddr;

    tempAddr = addr & 0xfff;
    //只取低8位地址
    switch(tempAddr & 0xff)
    {
        case 0:
            *tempData = led0;
                break;
        case 1:
                *tempData = led1;
                break;
        case 2:
                *tempData = led2;
                break;
        case 3:
                *tempData = led3;
                break;
        case 4:
                *tempData = led4;
                break;
        case 5:
                *tempData = led5;
                break;
        case 6:
                *tempData = led6;
                break;
        case 7:
                *tempData = led7;
                break;
        case 8:
                break;
        case 9:
                break;
        case 10:
                break;
        case 11:
                break;
        case 12:
                break;
        case 13:
                break;
        case 14:
                break;
        case 15:
                break;
        case 16:
                break;
        default:
                break;
    }

    return result;
}//uint16 getCoilVal(uint16 addr,uint16 *data)


//设定线圈状态 返回0表示成功
uint16 setCoilVal(uint16 addr,uint16 tempData)
{
    uint16 result = 0;
    uint16 tempAddr;

    tempAddr = addr & 0xfff;


    switch(tempAddr & 0xff)
    {
        case 0:
                led0 = tempData;
                break;
        case 1:
                led1 = tempData;
                break;
        case 2:
                led2 = tempData;
                break;
        case 3:
                led3 = tempData;
                break;
        case 4:
                led4 = tempData;
                break;
        case 5:
                led5 = tempData;
                break;
        case 6:
                led6 = tempData;
                break;
        case 7:
                led7 = tempData;
                break;
        case 8:
                break;
        case 9:
                break;
        case 10:
                break;
        case 11:
                break;
        case 12:
                break;
        case 13:
                break;
        case 14:
                break;
        case 15:
                break;
        case 16:
                break;
        default:
                break;
    }


    return result;
}//uint16 setCoilVal(uint16 addr,uint16 data)

//取寄存器值 返回0表示成功
uint16 getRegisterVal(uint16 addr,uint16 *tempData)
{
    uint16 result = 0;
    uint16 tempAddr;

    tempAddr = addr & 0xfff;

    switch(tempAddr & 0xff)
    {
        case 0:
                break;
        case 1:
                break;
        case 2:
                break;
        case 3:
                break;
        case 4:
                break;
        case 5:
                break;
        case 6:
                break;
        case 7:
                break;
        case 8:
                break;
        case 9:
                break;
        case 10:
                break;
        case 11:
                break;
        case 12:
                break;
        case 13:
                break;
        case 14:
                break;
        case 15:
                break;
        case 16:
                *tempData = a;
                break;
        default:
                break;
    }

    return result;
}//uint16 getRegisterVal(uint16 addr,uint16 &data)

//设置寄存器值 返回0表示成功
uint16 setRegisterVal(uint16 addr,uint16 tempData)
{
    uint16 result = 0;
    uint16 tempAddr;

    tempAddr = addr & 0xfff;

    switch(tempAddr & 0xff)
    {
        case 0:
                break;
        case 1:
                break;
        case 2:
                break;
        case 3:
                break;
        case 4:
                break;
        case 5:
                break;
        case 6:
                break;
        case 7:
                break;
        case 8:
                break;
        case 9:
                break;
        case 10:
                break;
        case 11:
                break;
        case 12:
                break;
        case 13:
                break;
        case 14:
                break;
        case 15:
                break;
        case 16:
                a = tempData;
                break;
        default:
                break;
    }

    return result;
}//uint8 setRegisterVal(uint16 addr,uint16 data)
*/


DevSlave::DevSlave(int nAddr_)
{
       m_pBuffer = NULL;
       m_cSlaveAddr = nAddr_;
}

void DevSlave::CheckCommModbus(QSerial::TxRxBuffer* pBuffer_)
{
    if (m_pBuffer != pBuffer_)
        m_pBuffer = pBuffer_;

    if (m_pBuffer->iRxLen <= 4)//最少5为 slave cmd addr crc0 crc1
        return;

    unsigned short _crcData, _crcOld;
    unsigned short _wTemp;
    unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;

    switch(_pRecBuf[1])
    {
    case 1://读取线圈状态(读取点 16位以内)
    case 3://读取保持寄存器(一个或多个)
    case 5://强制单个线圈
    case 6://设置单个寄存器

        if(m_pBuffer->iRxLen >= 8)
        {    //接收完成一组数据
            //应该关闭接收

            if(_pRecBuf[0] == m_cSlaveAddr)
            {
                _crcData = crc16(_pRecBuf,6);
                _crcOld = _pRecBuf[7] + (_pRecBuf[6] << 8);
                if(_crcOld == _crcData)//校验正确
                {
                    if(_pRecBuf[1] == 1)
                    {
                        //readCoil();//读取线圈状态(读取点 16位以内)
                    }
                    else if(_pRecBuf[1] == 3)
                    {
                        //readRegisters();//读取保持寄存器(一个或多个)
                    }
                    else if(_pRecBuf[1] == 5)
                    {
                        //forceSingleCoil();//强制单个线圈
                    }
                    else if(_pRecBuf[1] == 6)
                    {
                        //presetMultipleRegisters();
                    }
                }
            }
            m_pBuffer->iRxLen = 0;
        }
        break;

       case 15://设置多个线圈
           _wTemp = _pRecBuf[6];
           _wTemp += 9;  //数据个数

           if(m_pBuffer->iRxLen >= _wTemp)
           {
               if(_pRecBuf[0] == m_cSlaveAddr)
               {
                   _crcData = crc16(_pRecBuf,_wTemp - 2);
                   _crcOld = _pRecBuf[_wTemp - 1] + (_pRecBuf[_wTemp- 2] << 8);
                   if(_crcOld == _crcData)//校验正确
                   {
                       //forceMultipleCoils();
                   }
               }
               m_pBuffer->iRxLen = 0;
           }
           break;

       case 16://设置多个寄存器
           _wTemp = (_pRecBuf[4]<< 8) + _pRecBuf[5];
           _wTemp = _wTemp * 2;	//数据个数
           _wTemp += 9;

           if(m_pBuffer->iRxLen >= _wTemp)
           {
               if(_pRecBuf[0] == m_cSlaveAddr)
               {
                   _crcData = crc16(_pRecBuf,_wTemp - 2);
                   _crcOld = _pRecBuf[_wTemp - 1] + (_pRecBuf[_wTemp- 2] << 8);
                   if(_crcOld == _crcData)//校验正确
                   {
                       //presetMultipleRegisters();
                   }
               }
               m_pBuffer->iRxLen = 0;
           }
           break;

       default:
           break;

       }
}

