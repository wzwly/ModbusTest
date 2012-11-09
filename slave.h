#ifndef SLAVE_H
#define SLAVE_H

#include "serial.h"

class DevSlave
{
public:
    DevSlave(int nAddr_ = 1);

public:
    void CheckCommModbus(QSerial::TxRxBuffer* pBuffer_);

private:
    void ReadCoil(void);

    //===========================
    void GetCoilVal(unsigned short addr_,unsigned char *pData_);
private:
    QSerial::TxRxBuffer* m_pBuffer;
    unsigned char m_cSlaveAddr;
};


void ReadRegisters(void);
void ForceSingleCoil(void);
void PresetSingleRegister(void);
void PresetMultipleRegisters(void);
void ForceMultipleCoils(void);

unsigned short GetRegisterVal(unsigned short addr_,unsigned short *pData_);
unsigned short setRegisterVal(unsigned short addr_,unsigned short wData_);

unsigned short setCoilVal(unsigned short addr_,unsigned short wData_);

#endif // SLAVE_H
