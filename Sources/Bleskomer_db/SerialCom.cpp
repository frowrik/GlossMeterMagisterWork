#include "stdafx.h"

void Serial::Open(PConstStr portName)
{
    // пока ещё не подключились
    this->connected = false;

    // пытаемся подключиться к порту посредством CreateFile
    this->hSerial = CreateFile(portName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    // проверяем – было ли соединение успешным
    if(this->hSerial==INVALID_HANDLE_VALUE)
    {
        //если не было – выводим ошибку
        if(GetLastError()==ERROR_FILE_NOT_FOUND){

            // печатаем ошибку
            printf("ERROR: Handle was not attached. Reason: %s not available.\n", portName);

        }
        else
        {
            printf("ERROR!!!");
        }
    }
    else
    {
        // подключились – теперь устанавливаем параметры
        DCB dcbSerialParams = {0};

        // считываем текущие параметры
        if (!GetCommState(this->hSerial, &dcbSerialParams))
        {
            // если невозможно – говорим о неудаче
            printf("failed to get current serial parameters!");
        }
        else
        {
            // устанавливаем параметры для соединения с Arduino
            dcbSerialParams.BaudRate=CBR_9600;
            dcbSerialParams.ByteSize=8;
            dcbSerialParams.StopBits=ONESTOPBIT;
            dcbSerialParams.Parity=NOPARITY;

             // применяем параметры
             if(!SetCommState(hSerial, &dcbSerialParams))
             {
                printf("ALERT: Could not set Serial Port parameters");
             }
             else
             {
                 // отлично! Мы успешно подлючились :)
                 this->connected = true;
                 //ждём пока Arduino перезагрузится
                 Sleep(ARDUINO_WAIT_TIME);
             }
        }
    }

}

void Serial::Close()
{
    // проверяем статус подключение
    if(this->connected)
    {
        // отключаемся
        this->connected = false;
        // закрываем дескриптор порта
        CloseHandle(this->hSerial);
    }
}

int Serial::ReadData(char *buffer, unsigned int nbChar)
{
    // число считываемых байт
    DWORD bytesRead;
    // число байт, которое мы действительно хотим считать
    unsigned int toRead;

    // используем ClearCommError для получения информации о статусе последовательного порта
    ClearCommError(this->hSerial, &this->errors, &this->status);

    // проверяем – есть ли информация для считывания
    if(this->status.cbInQue>0)
    {
        
        // если количество данных меньше требуемого – считываем
        // столько, сколько есть
        if(this->status.cbInQue>=nbChar)
        {
            toRead = nbChar;
        }
        else
        {
            return -1;
        }

        // Пытаемся считать нужное количество данных и 
        //возвращаем число считанных байт при удачном завершении.
        if(ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0)
        {
            return bytesRead;
        }

    }

    // ничего не считали или где-то была ошибка – возвращаем -1
    return -1;

}


bool Serial::WriteData(char *buffer, unsigned int nbChar)
{
    DWORD bytesSend;

    // пытаемся записать значение буфера buffer в COM-порт
    if(!WriteFile(this->hSerial, (void *)buffer, nbChar, &bytesSend, 0))
    {
        // если не получилось – получаем код ошибки и возвращаем false
        ClearCommError(this->hSerial, &this->errors, &this->status);

        return false;
    }
    else
        return true;
}

bool Serial::IsConnected()
{
    // просто возвращаем статус соединения :)
    return this->connected;
}


u32 CountComports = 0;
string64 Comports[64];
PConstStr Comports2[64];
bool EnumerateCOMPorts() {
static GUID GUID_DEVINTERFACE_COMPORT =     {0x86E0D1E0L, 0x8089, 0x11D0, 0x9C, 0xE4, 0x08, 0x00, 0x3E, 0x30, 0x1F, 0x73};

    HDEVINFO hDevInfo = SetupDiGetClassDevs((LPGUID) &GUID_DEVINTERFACE_COMPORT, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hDevInfo == INVALID_HANDLE_VALUE) {
        return false;
    }
    BOOL bMoreItems = TRUE;
    int iIndex = 0;
    SP_DEVINFO_DATA devInfoData;
    while (bMoreItems) {
        devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        bMoreItems = SetupDiEnumDeviceInfo(hDevInfo, iIndex, &devInfoData);

        if (bMoreItems) {
            HKEY hDeviceKey = SetupDiOpenDevRegKey(
                hDevInfo,
                &devInfoData,
                DICS_FLAG_GLOBAL,
                0,
                DIREG_DEV,
                KEY_QUERY_VALUE);

            u8 PortName[256];
            if (hDeviceKey != INVALID_HANDLE_VALUE) {
                DWORD dwSize = sizeof(PortName);
                DWORD dwType;

                memset(PortName, 0x00, sizeof(TCHAR) * 256);

                if ((RegQueryValueEx(hDeviceKey, "PortName", NULL, &dwType, PortName, &dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ)) {
                    //process port name here
                    if (strlen((PConstStr)PortName) > 3) {
                        int iPort;
                        if (sscanf ((PConstStr)PortName, "COM%d", &iPort) == 1) {
                            PRINT ("'%s'  COM%d\n", (PConstStr)PortName, iPort);
                            /* // \\\\.\\ */
                            Strings::SPrintf (Comports[CountComports],"%s", (PConstStr)PortName); 
                            Comports2[CountComports] = Comports[CountComports];

                            CountComports++;
                        }
                            
                    }
                }
                RegCloseKey(hDeviceKey);
            }
        }
        ++iIndex;
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    return true;
}