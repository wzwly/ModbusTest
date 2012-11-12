#include "modbus.h"
#include "slave.h"

#include <QWSServer>
#include <QSocketNotifier>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <termios.h>	//串口头文件
#include <assert.h>

#define szModbusCom "tty0"
QSerial::TxRxBuffer QSerial::m_gTxRxBuffer;

QSerial::QSerial(Modbus* pSlave_, QObject * p_)
{
    m_nFdModbus = -1;
    m_pSlave = pSlave_;
    //InitModbus();
    m_nTimer = startTimer(1); //开启定时器，1ms一次
}

QSerial::~QSerial()
{
    Close(m_nFdModbus);
}

void QSerial::InitModbus()
{
    if ((m_nFdModbus = Open(szModbusCom, O_RDWR | O_NOCTTY)) < 0)
    {
        assert(false);
        perror(szModbusCom);
        return ;
    }
    struct termios _opt;

    bzero(&_opt, sizeof(_opt)); //清除结构体以放入新的序列埠设定值
    Tcgetattr(m_nFdModbus, &_opt);
    _opt.c_cc[VMIN] = 1;
    _opt.c_iflag = 0;
    _opt.c_oflag = 0;
    _opt.c_lflag = 0;
    _opt.c_cflag	= B115200 | CS8 | PARENB | CLOCAL | CREAD;
    if (Tcsetattr(m_nFdModbus, TCSANOW, &_opt))
    {
        perror("tcsetattr   error");
        assert(false);
        exit(1);
    }   
    //注册响应
    QSocketNotifier* notify = new QSocketNotifier(m_nFdModbus, QSocketNotifier::Read, this);
    connect(notify, SIGNAL(activated(int)), this, SLOT(OnReceiveChar()));
}


void QSerial::OnReceiveChar()
{
    if (!m_gTxRxBuffer.bRxEn)
        return;

    unsigned char _cRead[10];
    int _ret = Read(m_nFdModbus, _cRead, 10);

    if (_ret <= 0)
        return;

    for (int _i = 0; _i < _ret; ++_i)
    {
        m_gTxRxBuffer.szRxBuffer[m_gTxRxBuffer.iRxLen++] = _cRead[_i];
    }
    m_nTemMs = 10;

    if (m_pSlave)
    {
        //here need to notify the view to deal package
        m_pSlave->CheckCommModbus(&m_gTxRxBuffer);
    }    
}


void QSerial::SendBuffer()
{
    int _nSendLen = m_gTxRxBuffer.iTxLen;
    if (_nSendLen >0)
    {
        int _nRet = Write(m_nFdModbus,m_gTxRxBuffer.szTxBuffer ,_nSendLen);
        if (_nRet != _nSendLen)
        {
            //deal send erro
        }
    }
}


void QSerial::timerEvent(QTimerEvent *event_)
{
    //in receive mode
   // if (m_nTemMs > 0 && m_gTxRxBuffer.iRxLen > 0)
   // {
   //     --m_nTemMs;
   //     if (m_nTemMs == 0)
   //     {
    //       m_gTxRxBuffer.iRxLen = 0; //receive time out, start a new package
   //     }
   // }

    if (m_gTxRxBuffer.m_nEchoTimeOut > 0)
        m_gTxRxBuffer.m_nEchoTimeOut--;
}
