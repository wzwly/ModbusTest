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


static unsigned short crc16(unsigned char *puchMsg, unsigned int nDataLen)
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


DevSlave::DevSlave(int nAddr_)
{
       m_pBuffer = NULL;
       m_cSlaveAddr = nAddr_;
       for (int _i  = 0; _i < 1024; ++_i)
           m_arrayData[_i] = _i + 0x3300;
}

void DevSlave::GetRegVal(unsigned short addr_,unsigned short *pData_)
{
    if (addr_ >= 1024)
    { *pData_ = 0;   return;}

    *pData_ = m_arrayData[addr_];
}

 void DevSlave::SetRegVal(unsigned short addr_,unsigned short wData_)
 {
     m_arrayData[addr_] = wData_;
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
                        ReadCoil();//读取线圈状态(读取点 16位以内)
                    }
                    else if(_pRecBuf[1] == 3)
                    {
                        ReadRegisters();//读取保持寄存器(一个或多个)
                    }
                    else if(_pRecBuf[1] == 5)
                    {
                        ForceSingleCoil();//强制单个线圈
                    }
                    else if(_pRecBuf[1] == 6)
                    {
                        PresetSingleRegister();
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
                       ForceMultipleCoils();
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
                       PresetMultipleRegisters();
                   }
               }
               m_pBuffer->iRxLen = 0;
           }
           break;

       default:
           break;

       }
}

//1
void DevSlave::ReadCoil(void)
{
    unsigned char  _byteCnt;
    unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;

    unsigned short _wAddr = MakeShort(_pRecBuf[2], _pRecBuf[3]);
    unsigned short _bitCnt = MakeShort(_pRecBuf[4], _pRecBuf[5]);
    _byteCnt =  _bitCnt / 8;
    if (_bitCnt % 8 != 0)
        _byteCnt++;

    unsigned short _MaxAddr = _wAddr + _bitCnt;
    unsigned short _tempData;

    bool _bExit = false;
    for(int _k = 0; _k < _byteCnt; ++_k)
    {
        //字节位置
        _pTxBuf[_k + 3] = 0;
        for(int _i = 0; _i < 8; ++_i)
        {
            GetRegVal(_wAddr++,&_tempData);
            if (_tempData > 0)
             _pTxBuf[_k + 3] |= 0x01 << _i;

            if(_wAddr >= _MaxAddr)
            {	//读完
                _bExit = true;
                qDebug() << "I receive get cmd 01";
                break;
            }
        }
        if(_bExit)
            break;
    }

    _pTxBuf[0] = m_cSlaveAddr;
    _pTxBuf[1] = 0x01;
    _pTxBuf[2] = _byteCnt;

    _byteCnt += 3;
    unsigned short _crcData = crc16(_pTxBuf,_byteCnt);
    _pTxBuf[_byteCnt++] = _crcData >> 8;
    _pTxBuf[_byteCnt++] = _crcData & 0xff;
    m_pBuffer->iTxLen = _byteCnt;
    BegineSend();
}


//读寄存器 3
void DevSlave::ReadRegisters()
{
    unsigned char _byteCnt;
    unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;

    unsigned short _wAddr = MakeShort(_pRecBuf[2],_pRecBuf[3]);
    unsigned short _ReadCnt = MakeShort(_pRecBuf[4],_pRecBuf[5]);
    _byteCnt = _ReadCnt * 2;

    unsigned short _tempData;
    for(int _i=0;_i < _byteCnt; _i += 2)
    {
        GetRegVal(_wAddr++,&_tempData);
        _pTxBuf[_i+3] = _tempData >> 8;
        _pTxBuf[_i+4] = _tempData & 0xff;
    }
    qDebug() << "read data ok , cmd 3";
    _pTxBuf[0] = m_cSlaveAddr;
    _pTxBuf[1] = 0x03;
    _pTxBuf[2] = _byteCnt;
    _byteCnt += 3;
    unsigned short _crcData = crc16(_pTxBuf,_byteCnt);
    _pTxBuf[_byteCnt++] = _crcData >> 8;
    _pTxBuf[_byteCnt++] = _crcData & 0xff;
    m_pBuffer->iTxLen = _byteCnt;
    BegineSend();
}


//强制单个线圈 5
void DevSlave::ForceSingleCoil()
{
    unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;

    unsigned short _wAddr = MakeShort(_pRecBuf[2],_pRecBuf[3]);
    unsigned short _onOff = MakeShort(_pRecBuf[4],_pRecBuf[5]);

    unsigned short _tempData;
    if( _onOff & 0xffff)
    {	//设为ON
        _tempData = 1;
    }
    else if( _onOff == 0x0000)
    {	//设为OFF
        _tempData = 0;
    }

    SetRegVal(_wAddr, _onOff);
    qDebug() <<"Set the Coil Val";

    for(int _i=0; _i< m_pBuffer->iRxLen; ++_i)
    {
        _pTxBuf[_i] = _pRecBuf[_i];
    }
    m_pBuffer->iTxLen = m_pBuffer->iRxLen;
    BegineSend();
}


void DevSlave::PresetSingleRegister()//6
{
    //unsigned char  _byteCnt;
    unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;

    unsigned short _wAddr = MakeShort(_pRecBuf[2],_pRecBuf[3]);
    unsigned short _tempData = MakeShort(_pRecBuf[4], _pRecBuf[5]);

    SetRegVal(_wAddr, _tempData);
    qDebug()<< "write ok ,cmd 6";

    for(int _i=0; _i< m_pBuffer->iRxLen; ++_i)
    {
        _pTxBuf[_i] = _pRecBuf[_i];
    }
    m_pBuffer->iTxLen = m_pBuffer->iRxLen;
    BegineSend();
}

void DevSlave::ForceMultipleCoils() //15
{
    unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;

    unsigned short _wAddr = MakeShort(_pRecBuf[2],_pRecBuf[3]);
    unsigned short _setCnt = MakeShort(_pRecBuf[4],_pRecBuf[5]);
    unsigned char _byteCnt = _pRecBuf[6];

    unsigned int _iSet = 0;
    bool _bExit = false;
    for (int _i = 0; _i < _byteCnt; ++_i)
    {
        unsigned char _ch = _pRecBuf[7 + _i];
        for(int _k = 0; _k < 8; ++_k)
        {

            if (_ch & (0x01 << _k))
            {
                //Set _wAddr + _iSet whith ON
                SetRegVal(_wAddr + _iSet, 0xffff);
            }
            else
            {
                //Set _wAddr + _iSet whith OFF
                SetRegVal(_wAddr + _iSet, 0x00);
            }

            _iSet++;
            if (_iSet >= _setCnt)
            {_bExit = true; break;}

        }
        if (_bExit)
            break;
    }
    qDebug() <<"Set the Coil Val";

    _pTxBuf[0] = m_cSlaveAddr;
    _pTxBuf[1] = 0x0f;
    _pTxBuf[2] = _wAddr >> 8;
    _pTxBuf[3] = _wAddr & 0xff;
    _pTxBuf[4] = _setCnt >> 8;
    _pTxBuf[5] = _setCnt & 0xff;
    unsigned short _crcData = crc16(_pTxBuf,6);
    _pTxBuf[6] = _crcData >> 8;
    _pTxBuf[7] = _crcData & 0xff;
    m_pBuffer->iTxLen = 8;
    BegineSend();
}

//设置多个寄存器16
void DevSlave::PresetMultipleRegisters()
{
    //unsigned char  _byteCnt;
    unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;

    unsigned short _tempData;
    unsigned short _wAddr = MakeShort(_pRecBuf[2],_pRecBuf[3]);
    unsigned short _setCnt = MakeShort(_pRecBuf[4],_pRecBuf[5]);
    //_byteCnt = _pRecBuf[6];

    for(int _i=0; _i < _setCnt;++_i)
    {
        _tempData = MakeShort(_pRecBuf[_i*2 + 7], _pRecBuf[_i*2 + 8]);
        SetRegVal(_wAddr + _i,_tempData);       
    }
    qDebug()<< "write ok ,cmd 16";
    _pTxBuf[0] = m_cSlaveAddr;
    _pTxBuf[1] = 0x10;
    _pTxBuf[2] = _wAddr >> 8;
    _pTxBuf[3] = _wAddr & 0xff;
    _pTxBuf[4] = _setCnt >> 8;
    _pTxBuf[5] = _setCnt & 0xff;
    unsigned short _crcData = crc16(_pTxBuf,6);
    _pTxBuf[6] = _crcData >> 8;
    _pTxBuf[7] = _crcData & 0xff;
    m_pBuffer->iTxLen = 8;
    BegineSend();
}


void DevSlave::BegineSend()
{
    unsigned char* _pRecBuf = QSerial::m_gMasterBuffer.szRxBuffer;
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;
    QSerial::m_gMasterBuffer.iRxLen = m_pBuffer->iTxLen;
    memcpy(_pRecBuf, _pTxBuf, m_pBuffer->iTxLen);
}
