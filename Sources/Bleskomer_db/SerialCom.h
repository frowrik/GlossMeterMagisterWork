#ifndef SERIALCLASS_H_INCLUDED
#define SERIALCLASS_H_INCLUDED

#define ARDUINO_WAIT_TIME 500

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <objbase.h>
#include <initguid.h>
#include <Setupapi.h>
#pragma comment (lib, "Setupapi.lib")
class Serial {
private:
    HANDLE hSerial;
    bool connected;
    COMSTAT status;
    DWORD errors;
public:
    void Open(PConstStr portName);
    void Close();
    // ���������� -1 ��� ������
    int ReadData(char *buffer, unsigned int nbChar);
    // ���������� true ��� �����
    bool WriteData(char *buffer, unsigned int nbChar);
    // ���������� ������ ����������
    bool IsConnected();
};

bool EnumerateCOMPorts ();

extern u32 CountComports;
extern string64 Comports[64];
extern PConstStr Comports2[64];
#endif // SERIALCLASS_H_INCLUDED