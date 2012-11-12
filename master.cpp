#include "master.h"

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


DevMaster::DevMaster(int nAddr_, QSerialEx::TxRxBuffer* pBuffer_)
:m_nRepeatTime(300)
{
       m_pBuffer = pBuffer_;
       m_cSlaveAddr = nAddr_;
}

void DevMaster::BegineSend()
{
 #if TEST
    unsigned char* _pRecBuf = QSerial::m_gSlaveBuffer.szRxBuffer;
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;
    QSerial::m_gSlaveBuffer.iRxLen = m_pBuffer->iTxLen;
    memcpy(_pRecBuf, _pTxBuf, m_pBuffer->iTxLen);
#else

    if (QSerialEx::m_Semaphore.available())
    {
        QSerialEx::m_Semaphore.acquire();
        m_pBuffer->bTxEn = true;
        m_pBuffer->m_nEchoTimeOut = 20;
        m_pBuffer->iRxLen = 0;
        QSerialEx::m_gSlaveBuffer.iRxLen = 0;
        QSerialEx::m_Semaphore.release();
    }
#endif
}

//1
void DevMaster::ReadCoil(unsigned short wAddr_, unsigned short wQty_,unsigned char* pData_)
{
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;

    _pTxBuf[0] = m_cSlaveAddr;
    _pTxBuf[1] = 0x01;
    _pTxBuf[2] = wAddr_ >> 8;
    _pTxBuf[3] = wAddr_ & 0xff;
    _pTxBuf[4] = wQty_ >> 8;
    _pTxBuf[5] = wQty_ & 0xff;
    unsigned short _crcData = crc16(_pTxBuf, 6);
    _pTxBuf[6] = _crcData >> 8;
    _pTxBuf[7] = _crcData & 0xff;
    m_pBuffer->iTxLen = 8;
# if TEST
    BegineSend(); //发送
#else
    int _nloop = m_nRepeatTime;
    CheckStatus _sRet;
    while(_nloop > 0)
    {
        _nloop--;
        BegineSend(); //发送

        while(1)
        {
            usleep(1);
            if (m_pBuffer->m_nEchoTimeOut <= 0)
                break; //超时
            _sRet = CheckReadCoil(wQty_,pData_);
            if (CHECK_OK == _sRet) //校验正确
            {
                _nloop = 0;
                break;
            }
            else if (ECHO_ERRO == _sRet)
            {
                break;  //校验错误
            }
        }
    }
#endif
}
//1
DevMaster::CheckStatus DevMaster::CheckReadCoil(unsigned short wQty_,unsigned char* pData_)
{
    //最少5为 slave cmd addr crc0 crc1
    if (m_pBuffer->iRxLen <= 4)
        return RECEIVE_STATUS;

    unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
    if (_pRecBuf[0] == m_cSlaveAddr)
    {
        if (_pRecBuf[1] == 0x81)
            return ECHO_ERRO;//接受错误

        if (_pRecBuf[1] == 0x01)
        {
            unsigned char _byteCnt = wQty_ / 8;
            if (wQty_ % 8 != 0)
                _byteCnt++;
            int _nLen = _byteCnt + 5;
            if (m_pBuffer->iRxLen < _nLen)
                return RECEIVE_STATUS;
            unsigned short _crcData, _crcOld;
            _crcData = crc16(_pRecBuf,_nLen - 2);
            _crcOld = MakeShort(_pRecBuf[_nLen - 2],_pRecBuf[_nLen -1]);

            if (_crcData == _crcOld)
            {
                memcpy(pData_, &_pRecBuf[3], _byteCnt);
                qDebug() << "check ReadCoil Ok";
                return CHECK_OK;//接受OK
            }
            else
               return ECHO_ERRO;//接受错误
        }
    }
    return ECHO_ERRO;//接受错误
}

void DevMaster::ReadRegisters(unsigned short wAddr_, unsigned short wQty_, unsigned char* pData_)//3
{
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;

    _pTxBuf[0] = m_cSlaveAddr;
    _pTxBuf[1] = 0x03;
    _pTxBuf[2] = wAddr_ >> 8;
    _pTxBuf[3] = wAddr_ & 0xff;
    _pTxBuf[4] = wQty_ >> 8;
    _pTxBuf[5] = wQty_ & 0xff;
    unsigned short _crcData = crc16(_pTxBuf, 6);
    _pTxBuf[6] = _crcData >> 8;
    _pTxBuf[7] = _crcData & 0xff;
    m_pBuffer->iTxLen = 8;
# if TEST
    BegineSend(); //发送
#else
    int _nloop = m_nRepeatTime;
    CheckStatus _sRet;
    while(_nloop > 0)
    {
        _nloop--;
        BegineSend(); //发送

        while(1)
        {
            usleep(1);
            if (m_pBuffer->m_nEchoTimeOut <= 0)
                break; //超时
            _sRet = CheckReadRegisters(wQty_,pData_);
            if (CHECK_OK == _sRet) //校验正确
            {
                _nloop = 0;
                break;
            }
            else if (ECHO_ERRO == _sRet)
            {
                break;  //校验错误
            }
        }
    }
#endif

}

DevMaster::CheckStatus DevMaster::CheckReadRegisters(unsigned short wQty_,unsigned char* pData_) //3
{
    //最少5为 slave cmd addr crc0 crc1
    if (m_pBuffer->iRxLen <= 4)
        return RECEIVE_STATUS;

    unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
    if (_pRecBuf[0] == m_cSlaveAddr)
    {
        if (_pRecBuf[1] == 0x83)
            return ECHO_ERRO;//接受错误

        if (_pRecBuf[1] == 0x03)
        {
            unsigned char _byteCnt = _pRecBuf[2];
            int _nLen = _byteCnt + 5;
            if (m_pBuffer->iRxLen < _nLen)
                return RECEIVE_STATUS;
            unsigned short _crcData, _crcOld;
            _crcData = crc16(_pRecBuf,_nLen - 2);
            _crcOld = MakeShort(_pRecBuf[_nLen - 2],_pRecBuf[_nLen -1]);

            if (_crcData == _crcOld)
            {
                memcpy(pData_, &_pRecBuf[3], _byteCnt);
                qDebug() << "check ReadRegisters Ok";
                return CHECK_OK;//接受OK
            }
            else
               return ECHO_ERRO;//接受错误
        }
    }
    return ECHO_ERRO;//接受错误
}

void DevMaster::ForceSingleCoil(unsigned short wAddr_, bool bOnOff_) //5
{
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;
    unsigned short _wOnOff = 0x00;
    if (bOnOff_)
        _wOnOff = 0xffff;
    _pTxBuf[0] = m_cSlaveAddr;
    _pTxBuf[1] = 0x05;
    _pTxBuf[2] = wAddr_ >> 8;
    _pTxBuf[3] = wAddr_ & 0xff;
    _pTxBuf[4] = _wOnOff >> 8;
    _pTxBuf[5] = _wOnOff & 0xff;
    unsigned short _crcData = crc16(_pTxBuf, 6);
    _pTxBuf[6] = _crcData >> 8;
    _pTxBuf[7] = _crcData & 0xff;
    m_pBuffer->iTxLen = 8;
# if TEST
    BegineSend(); //发送
#else
    int _nloop = m_nRepeatTime;
    CheckStatus _sRet;
    while(_nloop > 0)
    {
        _nloop--;
        BegineSend(); //发送

        while(1)
        {
            usleep(1);
            if (m_pBuffer->m_nEchoTimeOut <= 0)
                break; //超时
            _sRet = CheckForceSingleCoil(_wOnOff);
            if (CHECK_OK == _sRet) //校验正确
            {
                _nloop = 0;
                break;
            }
            else if (ECHO_ERRO == _sRet)
            {
                break;  //校验错误
            }
        }
    }
#endif

}

 DevMaster::CheckStatus DevMaster::CheckForceSingleCoil(unsigned short wOnOff_) //5
 {
     //最少5为 slave cmd addr crc0 crc1
     if (m_pBuffer->iRxLen <= 4)
         return RECEIVE_STATUS;

     unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
     if (_pRecBuf[0] == m_cSlaveAddr)
     {
         if (_pRecBuf[1] == 0x85)
             return ECHO_ERRO;//接受错误

         if (_pRecBuf[1] == 0x05)
         {
             if (m_pBuffer->iRxLen < 8)

                 return RECEIVE_STATUS;
             unsigned short _crcData, _crcOld;
             _crcData = crc16(_pRecBuf, 6);
             _crcOld = MakeShort(_pRecBuf[6], _pRecBuf[7]);
             if (_crcData == _crcOld)
             {
                 qDebug() << "check ForceSingleCoil Ok";
                 return CHECK_OK;//接受OK
             }
             else
                return ECHO_ERRO;//接受错误
         }
     }
     return ECHO_ERRO;//接受错误
 }

//6
void DevMaster::PresetSingleRegister(unsigned short wAddr_, unsigned short wVal_)
{
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;
    _pTxBuf[0] = m_cSlaveAddr;
    _pTxBuf[1] = 0x06;
    _pTxBuf[2] = wAddr_ >> 8;
    _pTxBuf[3] = wAddr_ & 0xff;
    _pTxBuf[4] = wVal_ >> 8;
    _pTxBuf[5] = wVal_ & 0xff;
    unsigned short _crcData = crc16(_pTxBuf, 6);
    _pTxBuf[6] = _crcData >> 8;
    _pTxBuf[7] = _crcData & 0xff;
    m_pBuffer->iTxLen = 8;
# if TEST
    BegineSend(); //发送
#else
    int _nloop = m_nRepeatTime;
    CheckStatus _sRet;
    while(_nloop > 0)
    {
        _nloop--;
        BegineSend(); //发送

        while(1)
        {
            usleep(1);
            if (m_pBuffer->m_nEchoTimeOut <= 0)
                break; //超时
            _sRet = CheckPresetSingleRegister(wVal_);
            if (CHECK_OK == _sRet) //校验正确
            {
                _nloop = 0;
                break;
            }
            else if (ECHO_ERRO == _sRet)
            {
                break;  //校验错误
            }
        }
    }
#endif
}

//6
DevMaster::CheckStatus DevMaster::CheckPresetSingleRegister(unsigned short wVal_)
{
    //最少5为 slave cmd addr crc0 crc1
    if (m_pBuffer->iRxLen <= 4)
        return RECEIVE_STATUS;

    unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
    if (_pRecBuf[0] == m_cSlaveAddr)
    {
        if (_pRecBuf[1] == 0x86)
            return ECHO_ERRO;//接受错误

        if (_pRecBuf[1] == 0x06)
        {
            if (m_pBuffer->iRxLen < 8)

                return RECEIVE_STATUS;
            unsigned short _crcData, _crcOld;
            _crcData = crc16(_pRecBuf, 6);
            _crcOld = MakeShort(_pRecBuf[6], _pRecBuf[7]);
            if (_crcData == _crcOld)
            {
                qDebug() << "check PresetSingleRegister Ok";
                return CHECK_OK;//接受OK
            }
            else
               return ECHO_ERRO;//接受错误
        }
    }
    return ECHO_ERRO;//接受错误
}

 //15
void DevMaster::ForceMultipleCoils(unsigned short wAddr_, unsigned short wQty_,
                        unsigned char* pData_, unsigned char byteCnt_)
{
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;
    _pTxBuf[0] = m_cSlaveAddr;
    _pTxBuf[1] = 0x0f;
    _pTxBuf[2] = wAddr_ >> 8;
    _pTxBuf[3] = wAddr_ & 0xff;
    _pTxBuf[4] = wQty_ >> 8;
    _pTxBuf[5] = wQty_ & 0xff;

    if (byteCnt_ == 0)
    {
        byteCnt_ = wQty_ / 8;
        if (wQty_ % 8 != 0)
           byteCnt_++;
    }
    _pTxBuf[6] = byteCnt_;

    for (int _i = 0; _i < byteCnt_; ++_i)
       _pTxBuf[7 + _i] = pData_[_i];

    byteCnt_ += 7;
    unsigned short _crcData = crc16(_pTxBuf, byteCnt_);
    _pTxBuf[byteCnt_++] = _crcData >> 8;
    _pTxBuf[byteCnt_++] = _crcData & 0xff;
    m_pBuffer->iTxLen = byteCnt_;
# if TEST
    BegineSend(); //发送
#else
    int _nloop = m_nRepeatTime;
    CheckStatus _sRet;
    while(_nloop > 0)
    {
        _nloop--;
        BegineSend(); //发送
        while(1)
        {
            usleep(1);
            if (m_pBuffer->m_nEchoTimeOut <= 0)
                break; //超时
            _sRet = CheckForceMultipleCoils(wQty_);
            if (CHECK_OK == _sRet) //校验正确
            {
                _nloop = 0;
                break;
            }
            else if (ECHO_ERRO == _sRet)
            {
                break;  //校验错误
            }
        }
    }
#endif
}

//15
DevMaster::CheckStatus DevMaster::CheckForceMultipleCoils(unsigned short wQty_)
{
    //最少5为 slave cmd addr crc0 crc1
    if (m_pBuffer->iRxLen <= 4)
        return RECEIVE_STATUS;

    unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
    if (_pRecBuf[0] == m_cSlaveAddr)
    {
        if (_pRecBuf[1] == 0x8f)
            return ECHO_ERRO;//接受错误

        if (_pRecBuf[1] == 0x0f)
        {
            if (m_pBuffer->iRxLen < 8)
                return RECEIVE_STATUS;

            unsigned short _crcData, _crcOld;
            _crcData = crc16(_pRecBuf, 6);
            _crcOld = MakeShort(_pRecBuf[6], _pRecBuf[7]);
            if (_crcData == _crcOld)
            {
                qDebug() << "check ForceMultipleCoils Ok";
                return CHECK_OK;//接受OK
            }
            else
               return ECHO_ERRO;//接受错误
        }
    }
    return ECHO_ERRO;//接受错误
}


void DevMaster::PresetMultipleRegisters(unsigned short wAddr_, unsigned short wQty_,
                             unsigned char* pData_, unsigned char byteCnt_)//16
{
    unsigned char* _pTxBuf = m_pBuffer->szTxBuffer;
    _pTxBuf[0] = m_cSlaveAddr;
    _pTxBuf[1] = 0x10;
    _pTxBuf[2] = wAddr_ >> 8;
    _pTxBuf[3] = wAddr_ & 0xff;
    _pTxBuf[4] = wQty_ >> 8;
    _pTxBuf[5] = wQty_ & 0xff;
    if (byteCnt_ == 0)
    {
        byteCnt_ = wQty_ * 2;
    }
    _pTxBuf[6] = byteCnt_;

    for (int _i = 0; _i < byteCnt_; ++_i)
       _pTxBuf[7 + _i] = pData_[_i];

    byteCnt_ += 7;
    unsigned short _crcData = crc16(_pTxBuf, byteCnt_);
    _pTxBuf[byteCnt_++] = _crcData >> 8;
    _pTxBuf[byteCnt_++] = _crcData & 0xff;
    m_pBuffer->iTxLen = byteCnt_;
# if TEST
    BegineSend(); //发送
#else
    int _nloop = m_nRepeatTime;
    CheckStatus _sRet;
    while(_nloop > 0)
    {
        _nloop--;
        BegineSend(); //发送
        while(1)
        {
            usleep(1);
            if (m_pBuffer->m_nEchoTimeOut <= 0)
                break; //超时
            _sRet = CheckPresetMultipleRegisters(wQty_);
            if (CHECK_OK == _sRet) //校验正确
            {
                _nloop = 0;
                break;
            }
            else if (ECHO_ERRO == _sRet)
            {
                break;  //校验错误
            }
        }
    }
#endif

}

 DevMaster::CheckStatus DevMaster::CheckPresetMultipleRegisters(unsigned short wQty_)
 {
     //最少5为 slave cmd addr crc0 crc1
     if (m_pBuffer->iRxLen <= 4)
         return RECEIVE_STATUS;

     unsigned char* _pRecBuf = m_pBuffer->szRxBuffer;
     if (_pRecBuf[0] == m_cSlaveAddr)
     {
         if (_pRecBuf[1] == 0x90)
             return ECHO_ERRO;//接受错误

         if (_pRecBuf[1] == 0x10)
         {
             if (m_pBuffer->iRxLen < 8)
                 return RECEIVE_STATUS;

             unsigned short _crcData, _crcOld;
             _crcData = crc16(_pRecBuf, 6);
             _crcOld = MakeShort(_pRecBuf[6], _pRecBuf[7]);
             if (_crcData == _crcOld)
             {
                 qDebug() << "check PresetMultipleRegisters Ok";
                 return CHECK_OK;//接受OK
             }
             else
                return ECHO_ERRO;//接受错误
         }
     }
     return ECHO_ERRO;//接受错误
 }

