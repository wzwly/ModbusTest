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

    unsigned char _io[3] = {0x0};
    _master.ReadCoil(0x13, 19 ,_io);
    _Slave.CheckCommModbus(&QSerial::m_gSlaveBuffer);
    _master.CheckReadCoil(0x13, _io);

    return 0;

}
