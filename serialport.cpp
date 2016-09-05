
#include <vcl.h>
#pragma hdrstop

#include "serialport.h"
// HANDLE comHandle = NULL;  //���ڵľ��
//DCB dcb;                  //�豸���ƿ�
//---------------------------------------------------------------------------
#pragma package(smart_init)
__fastcall SerialPort::SerialPort()
{
    cout<<"�����๦�ܲ��Ի���Win API"<<endl;
     comHandle=NULL;
}
__fastcall SerialPort::~SerialPort ()
{
   CloseCOM();
   cout<<"���ڹر�"<<endl;

}
__fastcall SerialPort::SerialPort(AnsiString comm)
{
    //OpenCOM (comm);
    Open(comm);
   // cout<<"�����๦�ܲ��Ի���Win API"<<endl;
    //
}
//�򿪴���
void SerialPort:: OpenCOM(AnsiString comm) {
        //CloseCOM();
    comHandle = CreateFile(
                comm.c_str(),                                   //ָ��Ҫ�򿪵Ĵ���
                GENERIC_READ | GENERIC_WRITE,                   //��д����ģʽ
                0,                                              //����ģʽ ���ڴ��ڲ��ܹ������Ϊ0
                NULL,                                           //���ð�ȫ�����Խṹ ȱʡΪNULL
                OPEN_EXISTING,                                  //�򿪴��ڱ�����������
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,   //�ص�������ʽ�ֳ��첽������ʽ ��д�ļ�����ָ��һ��OVERLAPPED�ṹ
                NULL                                            //���ڴ��ڱ���ΪNULL
        );
    //ConfigCOM(9600);
     GetCommState(comHandle,&dcb);                     //��֪Ŀǰ���ж˿ڵ�״̬
  dcb.BaudRate = CBR_9600;
  dcb.ByteSize = 8;
  dcb.Parity = NOPARITY;
  dcb.StopBits = ONESTOPBIT;
  if(!SetCommState(comHandle,&dcb)) //����ͨ�Ŷ˿ڵ�״̬
  {
        MessageBox(0,"ͨ�Ŷ˿����ô���!!!","Set ERROR",MB_OK);
        CloseHandle(comHandle);
        return;
  }
    if (comHandle == INVALID_HANDLE_VALUE) {
        //MainForm->ShowMsg("��" + comm + "ʧ��", clRed);
        cout<<"��"<<comm.c_str ()<<"ʧ��"<<endl;
    }
    else
    {
         //   MainForm->ShowMsg("��" + comm + "�ɹ�", clGreen);
         cout<<"��"<<comm.c_str ()<<"�ɹ�"<<endl;
    }
}

void SerialPort::Open(AnsiString ComNo)
{
  DCB dcb;                          //ת����ָ������Char
  comHandle=CreateFile(ComNo.c_str(),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,0);
  if(comHandle==INVALID_HANDLE_VALUE)
  {
        MessageBox(0,"��ͨ�Ŷ˿ڴ���!!","Comm Error",MB_OK);   //������ж˿�δ��
        return;
  }
  GetCommState(comHandle,&dcb);                     //��֪Ŀǰ���ж˿ڵ�״̬
  dcb.BaudRate = CBR_9600;
  dcb.ByteSize = 8;
  dcb.Parity = NOPARITY;
  dcb.StopBits = ONESTOPBIT;
  if(!SetCommState(comHandle,&dcb)) //����ͨ�Ŷ˿ڵ�״̬
  {
        MessageBox(0,"ͨ�Ŷ˿����ô���!!!","Set ERROR",MB_OK);
        CloseHandle(comHandle);
        return;
  }
}

//�رմ���
void SerialPort:: CloseCOM() {
    if (comHandle != NULL) {
        CloseHandle(comHandle);
                comHandle = NULL;
    }
}

//���ô���
void SerialPort:: ConfigCOM(unsigned long rate) {
        SetupComm(comHandle,1024,1024);    //������������������Ĵ�С��Ϊ1024���ֽ�
        COMMTIMEOUTS timeOuts;//�涨��д�����ĳ�ʱ
        //�趨����ʱ
        timeOuts.ReadIntervalTimeout = 1000;
        timeOuts.ReadTotalTimeoutMultiplier = 500;
        timeOuts.ReadTotalTimeoutConstant = 500;
        //�趨д��ʱ
        timeOuts.WriteTotalTimeoutMultiplier = 500;
        timeOuts.WriteTotalTimeoutConstant = 2000;
        SetCommTimeouts(comHandle,&timeOuts);
        //��ȡ���ڵĳ�ʼ����
        GetCommState(comHandle,&dcb);
        //�޸Ĵ��ڵ����ò�����
        dcb.BaudRate = rate;            //������
        dcb.ByteSize = 8;               //ָ�����ڵ�ǰʹ�õ�����λ(4-8)
        dcb.fParity = false;            //�Ƿ�������żУ�� ��Ϊtrue��ʱ��Parity����
        dcb.Parity = NOPARITY;          //У�鷽��(EVENPARITY,MARKPARITY,NOPARITY,ODDPARITY)
        dcb.StopBits = TWOSTOPBITS;     //ֹͣλ(ONESTOPBIT,ONE5STOPBITS,TWOSTOPBITS)
       // SetCommState(comHandle,&dcb);
        if(!SetCommState(comHandle,&dcb)) //����ͨ�Ŷ˿ڵ�״̬
         {
             //  MessageBox(0,"ͨ�Ŷ˿����ô���!!!","Set ERROR",MB_OK);
               cout<<"ͨ�Ŷ˿����ô���!!!"<<endl;
               CloseHandle(comHandle);
               return;
         }
        //��д����֮ǰ������ջ�����
        PurgeComm(comHandle,PURGE_TXCLEAR | PURGE_RXCLEAR);
}
//�첽д��������
unsigned long SerialPort:: WriteCOM(char buffer[])
{
       // char buffer[3] = {0x16,0x54,0x0D};
        DWORD writeBytes = 1024;
        DWORD error;
        COMSTAT comStat;
        OVERLAPPED lapped;
        memset(&lapped,0,sizeof(OVERLAPPED));
        lapped.Offset = 0;
        lapped.OffsetHigh = 0;
        //lapped.hEvent�¼�����Ϊ���ź�״̬
        lapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
        //���������Ϣ
        ClearCommError(comHandle,&error,&comStat);
        bool result = WriteFile(comHandle,buffer,writeBytes,&writeBytes,&lapped);
        if(!result)
        {
                //��ʾ�������ڽ���дȡ����
                if(GetLastError() == ERROR_IO_PENDING)
                {
                        //�ȴ�2s �ö��������
                        WaitForSingleObject(lapped.hEvent,2000);
                        return writeBytes;
                }
                return 0;
        }
        return writeBytes;
}
//---------------------------------------------------------------------------
//�첽���������� ����ʵ�ʶ������ֽ���
unsigned long SerialPort:: ReadCOM(char buffer[])
{
        DWORD readBytes = 1024;
        DWORD error;
        COMSTAT comStat;
        OVERLAPPED lapped;
        memset(&lapped,0,sizeof(OVERLAPPED));
        //lapped.hEvent�¼�����Ϊ���ź�״̬
        lapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
        //���������Ϣ
        ClearCommError(comHandle,&error,&comStat);
        readBytes=min(readBytes,(DWORD)comStat.cbInQue);
        bool result = ReadFile(
                comHandle,
                buffer,
                readBytes,
                &readBytes,
                &lapped
        );
        if(!result)
        {
                //��ʾ�������ڽ��ж�ȡ����
                if(GetLastError() == ERROR_IO_PENDING)
                {
                        //�ȴ�2s �ö��������
                        WaitForSingleObject(lapped.hEvent,2000);
                        //�����ڶ�������ɺ� lapped.hEvent�¼����Ϊ���ź�״̬
                        PurgeComm(comHandle,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
                        return readBytes;
                }
                return 0;
        }
        PurgeComm(comHandle,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
        return readBytes;
}

bool SerialPort:: Send(unsigned char * SendData,unsigned long t_size)                                   //��������
{
  bool SendFlag = false;
  unsigned long lrc;
  if(comHandle==0) return false;                            //���Handleֵ
  SendFlag = WriteFile(comHandle,SendData,t_size,&lrc,NULL);      //�ͳ�����
  return SendFlag;
}
//---------------------------------------------------------------------------
void SerialPort::Receiv(unsigned char * Data)                      //��������
{
  char inbuff[32]={0};
  DWORD nBytesRead,dwError;
  COMSTAT cs;
  ClearCommError(comHandle,&dwError,&cs);
  if(cs.cbInQue>sizeof(inbuff))                //ȡ��״̬
  {                                             //�����Ƿ������׼���Ļ�����
        PurgeComm(comHandle,PURGE_RXCLEAR);     //���ͨ�Ŷ˿�����
        return;
  }
  ReadFile(comHandle,inbuff,cs.cbInQue,&nBytesRead,NULL);     //���ն˿ڵ�����
  memcpy(Data,inbuff,32);
}

