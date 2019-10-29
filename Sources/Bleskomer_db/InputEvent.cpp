

struct InputEvent   {
    f32         Diffuse;
    f32         Reflected;
};

Serial          ComPort;
i32             ComPort_Number;
InputEvent      ComPort_Events[1000];
i32             ComPort_Events_Number;

void            Port_Open (){
    ComPort_Events_Number = 0;
    ComPort_Number = 0;
    EnumerateCOMPorts ();
    ComPort.Open (Comports2[ComPort_Number]);
}

void            Port_Close (){
    ComPort.Close ();
}

void            Port_ReOpen (){
    ComPort.Close ();
    ComPort.Open (Comports2[ComPort_Number]);
}

bool            Port_isOpen (){
    if (ComPort.IsConnected ()) return true;
    ComPort.Open (Comports2[ComPort_Number]);
    return false;
}

u32             Command;
u32             Value;
u32             order = 0;

bool            Port_ReadEvent (){
    int res; 
    switch (order) {
        case 0: { 
            res = ComPort.ReadData ((char*)&Command, sizeof (Command)); 
            if (res>=0) order =1; 
            break;  
        }
        case 1: { 
            res = ComPort.ReadData ((char*)&Value, sizeof (Value)); 
            if (res>=0) order = 2; 
            break;  
        }
        case 2: { 
            if ((Command == 5)) {
                f32 Diffuse = *((f32*)&Value);
                ComPort_Events[ComPort_Events_Number].Diffuse = Diffuse;
            }

            if ((Command == 6)) {
                f32 Specular = *((f32*)&Value);
                ComPort_Events[ComPort_Events_Number].Reflected = Specular;
                ComPort_Events_Number++;
            }

            order = 0; 
            return true;
        }
    }
    return false;
}

void            Port_FindArduino (){

}