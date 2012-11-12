#ifndef MASTER_H
#define MASTER_H


#include <QDebug>
#include "serialex.h"

class DevMaster
{
public:
    enum CheckStatus
    {
        CHECK_OK = 0,
        ECHO_ERRO,
        RECEIVE_STATUS,
    };
    DevMaster(int nAddr_ = 1, QSerialEx::TxRxBuffer* pBuffer_ = NULL);

public:



public:
    void ReadCoil(unsigned short wAddr_, unsigned short wQty_, unsigned char* pData_);    //1
    void ReadRegisters(unsigned short wAddr_, unsigned short wQty_, unsigned char* pData_);//3
    void ForceSingleCoil(unsigned short wAddr_, bool bOnOff_); //5
    void PresetSingleRegister(unsigned short wAddr_, unsigned short wVal_); //6
    void ForceMultipleCoils(unsigned short wAddr_, unsigned short wQty_, unsigned char* pData_, unsigned char byteCnt_ = 0); //15
    void PresetMultipleRegisters(unsigned short wAddr_, unsigned short wQty_, unsigned char* pData_, unsigned char byteCnt_ = 0);//16
    //
    CheckStatus CheckReadCoil(unsigned short wQty_,unsigned char* pData_); //1
    CheckStatus CheckReadRegisters(unsigned short wQty_,unsigned char* pData_); //3
    CheckStatus CheckForceSingleCoil(unsigned short wOnOff_); //5
    CheckStatus CheckPresetSingleRegister(unsigned short wVal_); //6
    CheckStatus CheckForceMultipleCoils(unsigned short wQty_);//15
    CheckStatus CheckPresetMultipleRegisters(unsigned short wQty_);//16
private:
    //===========================
    unsigned short MakeShort(unsigned char H_, unsigned int L)
    {
        unsigned short _ret = H_ & 0xff;
        return ((_ret << 8) + L);
    };
    void BegineSend();
private:
    QSerialEx::TxRxBuffer* m_pBuffer;
    unsigned char m_cSlaveAddr;
    const int m_nRepeatTime;
    /*
     01(0x01 读线圈) 读去多个线圈状态，每个线圈是一个bit位
     请求码格式：【Dev】[0x01][地址高字节][地址低字节][读取数量高字节][读取数量低字节][Crc低字节][Crc高字节]
     响应码格式：【Dev】[0x01][字节数][输入状态N*1个字节][Crc低字节][Crc高字节]
     异常响应格式：【Dev】[0x82][异常码][Crc低字节][Crc高字节]

     备注：
     --N = 输入数量/8,如果余数不为零，则N=N+1；
     --输入数量为1-2000，超过算是错误；
     例子：
     【Dev】[0x01][00][0x13][00][0x13][CrcH][CrcL]
     【Dev】[0x01][0x03][0xCD][0x6b][0x05][CrcL][CrcL]
     */

     /*
     03(0x03 读保持寄存器)读连续块中保持寄存器，一个或者多个
     请求码格式：【Dev】[0x03][地址高字节][地址低字节][读取数量高字节][读取数量低字节][Crc低字节][Crc高字节]
     响应码格式：【Dev】[0x03][字节数][读取数量N*2个字节][Crc低字节][Crc高字节]
     异常响应格式：【Dev】[0x83][异常码][Crc低字节][Crc高字节]

     例子：
     【Dev】[0x03][00][0x6B][00][0x03][CrcH][CrcL]
     【Dev】[0x03][0x06][0x02][0x2B][0x00][00][0x64][CrcL][CrcL]
     */

     /*
     05(0x05 写单个线圈) 将远程设备的单个输出写为ON 0x00或者 OFF 0xff
     请求码格式：【Dev】[0x05][地址高字节][地址低字节][0xFF/00][00/0xFF][Crc低字节][Crc高字节]
     响应码格式：【Dev】[0x05][地址高字节][地址低字节][0xFF/00][00/0xFF][Crc低字节][Crc高字节]
     异常响应格式：【Dev】[0x85][异常码][Crc低字节][Crc高字节]

     备注：
     --ff00\FFFF表示On，0000表示offf；
     --输入数量为1-ffff，超过算是错误；
     例子：写为ON
     【Dev】[0x05][00][0xAC][FF][0x00][CrcH][CrcL]
     【Dev】[0x05][00][0xAC][FF][0x00][CrcH][CrcL]
     */

     /*
     06(0x06 写单个寄存器) 写单个保持寄存器
     请求码格式：【Dev】[0x06][地址高字节][地址低字节][数据H][数据L][Crc低字节][Crc高字节]
     响应码格式：【Dev】[0x06][地址高字节][地址低字节][数据H][数据L][Crc低字节][Crc高字节]
     异常响应格式：【Dev】[0x86][异常码][Crc低字节][Crc高字节]

     例子：
     【Dev】[0x06][00][0x01][00][0x03][CrcH][CrcL]
     【Dev】[0x06][00][0x01][00][0x03][CrcH][CrcL]
     */

     /*
     15(0x0f 写多个线圈) 编号1的线圈 寻址为0
     请求码格式：【Dev】[0x0f][地址高字节][地址低字节][输出数量高字节][输出数量低字节][字节数][N*1字节数据][Crc低字
     节][Crc高字节]
     响应码格式：【Dev】[0x0f][地址高字节][地址低字节][输出数量高字节][输出数量低字节][Crc低字节][Crc高字节]
     异常响应格式：【Dev】[0x8f][异常码][Crc低字节][Crc高字节]

     例子：
     【Dev】[0x0f][00][0x013][00][0x0a][0x02][0xcd][0x01][CrcH][CrcL]
     【Dev】[0x0f][00][0x013][00][0x0a][CrcH][CrcL]
     说明，从地址20开始写入10个线圈：
     27 26 25 24 23 22 21 20  —— —— —— —— —— —— 29 28
     1  1  0  0  1  1  0  1   0  0  0  0  0  0  0   1  = CD 01
     */


     /*
     16(0x10 写多个寄存器)最多写入1-123个寄存器
     请求码格式：【Dev】[0x10][地址高字节][地址低字节][寄存器数量高字节][寄存器数量低字节][字节数][N*2字节数据][Crc低字
     节][Crc高字节]
     响应码格式：【Dev】[0x10][地址高字节][地址低字节][寄存器数量高字节][寄存器数量低字节][Crc低字节][Crc高字节]
     异常响应格式：【Dev】[0x90][异常码][Crc低字节][Crc高字节]

     例子：
     【Dev】[0x10][00][0x01][00][0x02][0x04][0x00][0x0a][0x01][CrcH][CrcL]
     【Dev】[0x10][00][0x01][00][0x02][CrcH][CrcL]
     */

};


#endif // MASTER_H
