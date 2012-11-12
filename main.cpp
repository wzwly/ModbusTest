#include <QtGui/QApplication>
#include "mainwindow.h"


#include "slave.h"
#include "master.h"

//QSerial::TxRxBuffer QSerial::m_gMasterBuffer;
//QSerial::TxRxBuffer QSerial::m_gSlaveBuffer;
int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();
    DevMaster _master(1,&QSerial::m_gMasterBuffer);
    DevSlave  _Slave(1);

#if TEST

#if 0  // 5 test Write
    _master.ForceSingleCoil(0x101, 1);
    _Slave.CheckCommModbus(&QSerial::m_gSlaveBuffer);
    _master.CheckForceSingleCoil(1);
#endif

#if 0  // 6 test Write
    _master.PresetSingleRegister(0x102, 0x3e65);
    _Slave.CheckCommModbus(&QSerial::m_gSlaveBuffer);
    _master.CheckPresetSingleRegister(1);
#endif

#if 0  // 15 test Write
    unsigned char _out[5] = {0x33, 0xd7, 0xc8, 0xb9, 0xaa};
    _master.ForceMultipleCoils(0x101, 8, _out);
    _Slave.CheckCommModbus(&QSerial::m_gSlaveBuffer);
    _master.CheckForceMultipleCoils(1);
#endif

#if 1  // 16 test Write
    unsigned short _outReg[5] = {0xee33, 0xccd7, 0xddc8, 0xffb9, 0x45aa};
    _master.PresetMultipleRegisters(0x100, 0x5, (unsigned char*)_outReg);
    _Slave.CheckCommModbus(&QSerial::m_gSlaveBuffer);
    _master.CheckPresetMultipleRegisters(1);
#endif

#if 0  // 1 test Read
    unsigned char _io[3] = {0x0};
    _master.ReadCoil(0x0, 19 ,_io);
    _Slave.CheckCommModbus(&QSerial::m_gSlaveBuffer);
    _master.CheckReadCoil(19, _io);
#endif

#if 1  // 3 test Read
    unsigned char _reg[32] = {0x0};
    _master.ReadRegisters(0x100, 10 , _reg);
    _Slave.CheckCommModbus(&QSerial::m_gSlaveBuffer);
    _master.CheckReadRegisters(10, _reg);
#endif

#else
    QSerial _Serial(&_Slave, NULL);
    _Serial.StartThread(QThread::NormalPriority);

# if 0 //5
     _master.ForceSingleCoil(0x101, 1);
#endif

#if 0  // 6 test Write
    _master.PresetSingleRegister(0x102, 0x3e65);
#endif

#if 0  // 15 test Write
    unsigned char _out[5] = {0x33, 0xd7, 0xc8, 0xb9, 0xaa};
    _master.ForceMultipleCoils(0x101, 8, _out);
#endif

#if 1  // 16 test Write
    unsigned short _outReg[5] = {0xee33, 0xccd7, 0xddc8, 0xffb9, 0x45aa};
    _master.PresetMultipleRegisters(0x101, 0x5, (unsigned char*)_outReg);
#endif


#if 0  // 1 test Read
    unsigned char _io[3] = {0x0};
    _master.ReadCoil(0x100, 0 ,_io);
#endif

#if 1  // 3 test Read
    unsigned char _reg[32] = {0x0};
    _master.ReadRegisters(0x101, 10 , _reg);
#endif

    _Serial.ExitThread();
    while(!_Serial.IsExit())
    sleep(1);
#endif
    return 0;

}
