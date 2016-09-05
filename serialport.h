#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <windows.h>
#include <iostream>
using namespace std;
class SerialPort
{
public:
    __fastcall SerialPort();
    __fastcall SerialPort(AnsiString comm);
   __fastcall ~SerialPort();
    void OpenCOM(AnsiString comm);
    void CloseCOM();
    unsigned long WriteCOM(char buffer[]);
    unsigned long ReadCOM(char buffer[]);
    void ConfigCOM(unsigned long rate);
     void Open(AnsiString ComNo);
    bool Send(unsigned char * SendData,unsigned long t_size);
    void Receiv(unsigned char * Data);                      //��������
private:	// User declarations
     HANDLE comHandle;  //���ڵľ��
     DCB dcb;                  //�豸���ƿ�
};

#endif // SERIALPORT_H
