#include "stdafx.h"
#pragma execution_character_set("utf-8")

#include "InputEvent.cpp"

IMGUITAB        tabMain;

void	        TabDraw_PortSelected (){
    if (ImGui::ListBox ("Comports\n", &ComPort_Number, Comports2, CountComports, 4)) {
        Port_ReOpen ();
    }
}



u32         SaveData_Number = 0;
InputEvent  SaveData[1000];
string128   SaveDataTime[1000];
bool        SaveDataComplex[1000];

void	        TabDraw_OdinIsled (){
    if (ComPort_Events_Number > 0){
        SaveData[SaveData_Number].Diffuse = ComPort_Events[0].Diffuse;
        SaveData[SaveData_Number].Reflected = ComPort_Events[0].Reflected;
        SaveDataComplex[SaveData_Number]= false;

        string64	    timer;
        string64	    date;
	    _strtime_s(timer, 9);
	    _strdate_s(date, 9);
	    Strings::SPrintf(SaveDataTime[SaveData_Number], "Time:%s Data: %s \n", timer, date);

        SaveData_Number++;

        ComPort_Events_Number = 0;
    }

    
    if (ImGui::Button ("Clear")) SaveData_Number = 0;
    ImGui::Text ("");
    ImGui::Text ("");
    if (ImGui::Button ("Export txt")) {
        OS_FILE ff = Core::FS::FileOpenWrite("EXPORT.TXT");
        if (Core::FS::FileisOpen (ff)) {
            
            string1k Buffer;

            Strings::SPrintf (Buffer, "№\tТип\tDiffuse\tReflected\tOut\tДата\r\n");
            Core::FS::FileWrite(ff, Buffer, Strings::Len(Buffer));
            
            for (i32 i = 0; i < SaveData_Number ; i++) {
                Strings::SPrintf (Buffer, "%d\t%s\t%.2f\t%.2f\t%.2f\t%s\r\n", 
                    i+1, 
                    SaveDataComplex[i] ? "Комплексное" : "Одиночное", 
                    SaveData[i].Diffuse, SaveData[i].Reflected,
                    SaveData[i].Reflected - 0.17*SaveData[i].Diffuse,
                    SaveDataTime[i]
                    );

                Core::FS::FileWrite(ff, Buffer, Strings::Len(Buffer));
            }
            Core::FS::FileClose(ff);
        }   
    }

    // draw event list
    ImGui::BeginChild ("Eventlist", ImVec2(0,0), true);
    for (i32 i = 0; i < SaveData_Number ; i++) {
        ImGui::BeginGroup ();
        ImGui::Text ("%d", i+1);
        ImGui::SameLine();
        if (SaveDataComplex[i]) ImGui::Text ("Комплексное"); else ImGui::Text ("Одиночное");
        ImGui::SameLine();
        ImGui::Text("Diffuse  %.2f", SaveData[i].Diffuse);
        ImGui::SameLine();
        ImGui::Text("Reflected %.2f", SaveData[i].Reflected);
        ImGui::SameLine();
        ImGui::Text("Out %.2f", SaveData[i].Reflected - 0.17*SaveData[i].Diffuse);
        ImGui::SameLine();
        ImGui::Text("%s", SaveDataTime[i]);
        ImGui::EndGroup ();
    }
    ImGui::EndChild ();
   
}

u32         ReadNumber = 0;
InputEvent  ReadData[10];

void	        TabDraw_Complex (){
    ImGui::Text ("По ГОСТ 896-86 предусматривающий использование фотоэлектрического блескомера ФБ-2,");
    ImGui::Text ("измерения производятся в 15 точках для дифузного и 5 точках отражённого света.");
    ImGui::Text ("Для данного прибора ребуются 15 измерений.");
    ImGui::Text ("");

    if (ImGui::Button ("Restart")) ReadNumber = 0;

    ImGui::PushStyleColor (ImGuiCol_Text, ImVec4(0,1,0,1));
    ImGui::Text ("Осталось измерений %d\n", 10-ReadNumber);
    ImGui::PopStyleColor ();

    if (ComPort_Events_Number > 0){
        ReadData[ReadNumber] = ComPort_Events[0];
        ReadNumber++;
        ComPort_Events_Number = 0;
    }
    
    if (ReadNumber == 15)  {
        f32 SumD = 0;
        f32 SumR = 0;
        for (i32 i = 0; i < ReadNumber ; i++) {
            SumD += ReadData[i].Diffuse;
            SumR += ReadData[i].Reflected;
        }
        SumD /= ReadNumber;
        SumR /= ReadNumber;
        //
        SaveData[SaveData_Number].Diffuse = SumD;
        SaveData[SaveData_Number].Reflected = SumR;
        SaveDataComplex[SaveData_Number]= true;

        string64	    timer;
        string64	    date;
	    _strtime_s(timer, 9);
	    _strdate_s(date, 9);
	    Strings::SPrintf(SaveDataTime[SaveData_Number], "Time:%s Data: %s \n", timer, date);

        SaveData_Number++;
        //

        ReadNumber = 0;
    }
    
    // draw event list
    ImGui::BeginChild ("Eventlist", ImVec2(0,0), true);
    for (i32 i = 0; i < ReadNumber ; i++) {
        ImGui::Text ("%d\n", i+1);
        ImGui::Text("Diffuse  %.2f", ReadData[i].Diffuse);
        ImGui::Text("Reflected %.2f", ReadData[i].Reflected);
    }
    ImGui::EndChild ();
    
}






void        Main_Init (){
    Port_FindArduino ();
    Port_Open ();

    tabMain.Init ();
    tabMain.AddTab ("Настройки порта", &TabDraw_PortSelected);
    tabMain.AddTab ("Одиночные измерений и результаты измерений", &TabDraw_OdinIsled);
    tabMain.AddTab ("Комплексное измерение", &TabDraw_Complex);
}

void        Main_Free (){
    OS_FILE ff = Core::FS::FileOpenWrite("EXPORT_Close.TXT");
    if (Core::FS::FileisOpen (ff)) {
            
        string1k Buffer;

        Strings::SPrintf (Buffer, "№\tТип\tDiffuse\tReflected\tOut\tДата\r\n");
        Core::FS::FileWrite(ff, Buffer, Strings::Len(Buffer));
            
        for (i32 i = 0; i < SaveData_Number ; i++) {
            Strings::SPrintf (Buffer, "%d\t%s\t%.2f\t%.2f\t%.2f\t%s\r\n", 
                i+1, 
                SaveDataComplex[i] ? "Комплексное" : "Одиночное", 
                SaveData[i].Diffuse, SaveData[i].Reflected,
                SaveData[i].Reflected - 0.17*SaveData[i].Diffuse,
                SaveDataTime[i]
                );

            Core::FS::FileWrite(ff, Buffer, Strings::Len(Buffer));
        }
        Core::FS::FileClose(ff);
    }   

    Port_Close ();
}

void        Main_Frame (){
    ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::Begin("SMEFJHJGJ", NULL,  ImGuiWindowFlags_AlwaysAutoResize |  ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar); 
    
    if (Port_isOpen  ()) {
        Port_ReadEvent ();
        
        tabMain.DrawAuto(ImGui::GetIO().DisplaySize.x);
    } else {
        // select port
        TabDraw_PortSelected ();

        ImGui::PushStyleColor (ImGuiCol_Text, ImVec4(1,0,0,1));
        if (CountComports > 0) {
            ImGui::Text ("Порт не открыт. попробуйте открыть другой порт или подождать откоется ли этот.\n");
        } else {
            ImGui::Text ("Порты не найдены. перезапустите программу.\n");
        }
        ImGui::PopStyleColor (); 
    }

    // еслит сконетились то переключаемся на измерения базовые
    if (tabMain.selectedTab == 0 && Port_isOpen()) tabMain.selectedTab = 1;
    
	ImGui::End();
}


int CALLBACK 
WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int CmdShow)
{   
    if (!Core::Init()) return 0;

    if (!Core::FS::FileisExist("Roboto-Medium.ttf")) {
		MessageBoxW(0, L"Не найден файл Roboto-Medium.ttf", L"Error", 0);
		Core::Free();
		return 0;
	}

    maihftghgfhgfhn();
    Main_Free ();

    Core::Free();
    return  0;
}

void        Main_Init_imgui ();

bool First = true;
void	    MyFrame (){
    if (First) { 
        First = false; 
        Main_Init_imgui ();
        Main_Init (); 
    }
    Main_Frame ();
}



void        Main_Init_imgui () {
    ImFont* ff =  ImGui::GetFont();
	ff->AddRemapChar(0xBC, 0x0458);  // #CYRILLIC SMALL LETTER JE
	ff->AddRemapChar(0xBD, 0x0405);  // #CYRILLIC CAPITAL LETTER DZE
	ff->AddRemapChar(0xBE, 0x0455);  // #CYRILLIC SMALL LETTER DZE
	ff->AddRemapChar(0xBF, 0x0457);  // #CYRILLIC SMALL LETTER YI
	ff->AddRemapChar(0xC0, 0x0410);  // #CYRILLIC CAPITAL LETTER A
	ff->AddRemapChar(0xC1, 0x0411);  // #CYRILLIC CAPITAL LETTER BE
	ff->AddRemapChar(0xC2, 0x0412);  // #CYRILLIC CAPITAL LETTER VE
	ff->AddRemapChar(0xC3, 0x0413);  // #CYRILLIC CAPITAL LETTER GHE
	ff->AddRemapChar(0xC4, 0x0414);  // #CYRILLIC CAPITAL LETTER DE
	ff->AddRemapChar(0xC5, 0x0415);  // #CYRILLIC CAPITAL LETTER IE
	ff->AddRemapChar(0xC6, 0x0416);  // #CYRILLIC CAPITAL LETTER ZHE
	ff->AddRemapChar(0xC7, 0x0417);  // #CYRILLIC CAPITAL LETTER ZE
	ff->AddRemapChar(0xC8, 0x0418);  // #CYRILLIC CAPITAL LETTER I
	ff->AddRemapChar(0xC9, 0x0419);  // #CYRILLIC CAPITAL LETTER SHORT I
	ff->AddRemapChar(0xCA, 0x041A);  // #CYRILLIC CAPITAL LETTER KA
	ff->AddRemapChar(0xCB, 0x041B);  // #CYRILLIC CAPITAL LETTER EL
	ff->AddRemapChar(0xCC, 0x041C);  // #CYRILLIC CAPITAL LETTER EM
	ff->AddRemapChar(0xCD, 0x041D);  // #CYRILLIC CAPITAL LETTER EN
	ff->AddRemapChar(0xCE, 0x041E);  // #CYRILLIC CAPITAL LETTER O
	ff->AddRemapChar(0xCF, 0x041F);  // #CYRILLIC CAPITAL LETTER PE
	ff->AddRemapChar(0xD0, 0x0420);  // #CYRILLIC CAPITAL LETTER ER
	ff->AddRemapChar(0xD1, 0x0421);  // #CYRILLIC CAPITAL LETTER ES
	ff->AddRemapChar(0xD2, 0x0422);  // #CYRILLIC CAPITAL LETTER TE
	ff->AddRemapChar(0xD3, 0x0423);  // #CYRILLIC CAPITAL LETTER U
	ff->AddRemapChar(0xD4, 0x0424);  // #CYRILLIC CAPITAL LETTER EF
	ff->AddRemapChar(0xD5, 0x0425);  // #CYRILLIC CAPITAL LETTER HA
	ff->AddRemapChar(0xD6, 0x0426);  // #CYRILLIC CAPITAL LETTER TSE
	ff->AddRemapChar(0xD7, 0x0427);  // #CYRILLIC CAPITAL LETTER CHE
	ff->AddRemapChar(0xD8, 0x0428);  // #CYRILLIC CAPITAL LETTER SHA
	ff->AddRemapChar(0xD9, 0x0429);  // #CYRILLIC CAPITAL LETTER SHCHA
	ff->AddRemapChar(0xDA, 0x042A);  // #CYRILLIC CAPITAL LETTER HARD SIGN
	ff->AddRemapChar(0xDB, 0x042B);  // #CYRILLIC CAPITAL LETTER YERU
	ff->AddRemapChar(0xDC, 0x042C);  // #CYRILLIC CAPITAL LETTER SOFT SIGN
	ff->AddRemapChar(0xDD, 0x042D);  // #CYRILLIC CAPITAL LETTER E
	ff->AddRemapChar(0xDE, 0x042E);  // #CYRILLIC CAPITAL LETTER YU
	ff->AddRemapChar(0xDF, 0x042F);  // #CYRILLIC CAPITAL LETTER YA
	ff->AddRemapChar(0xE0, 0x0430);  // #CYRILLIC SMALL LETTER A
	ff->AddRemapChar(0xE1, 0x0431);  // #CYRILLIC SMALL LETTER BE
	ff->AddRemapChar(0xE2, 0x0432);  // #CYRILLIC SMALL LETTER VE
	ff->AddRemapChar(0xE3, 0x0433);  // #CYRILLIC SMALL LETTER GHE
	ff->AddRemapChar(0xE4, 0x0434);  // #CYRILLIC SMALL LETTER DE
	ff->AddRemapChar(0xE5, 0x0435);  // #CYRILLIC SMALL LETTER IE
	ff->AddRemapChar(0xE6, 0x0436);  // #CYRILLIC SMALL LETTER ZHE
	ff->AddRemapChar(0xE7, 0x0437);  // #CYRILLIC SMALL LETTER ZE
	ff->AddRemapChar(0xE8, 0x0438);  // #CYRILLIC SMALL LETTER I
	ff->AddRemapChar(0xE9, 0x0439);  // #CYRILLIC SMALL LETTER SHORT I
	ff->AddRemapChar(0xEA, 0x043A);  // #CYRILLIC SMALL LETTER KA
	ff->AddRemapChar(0xEB, 0x043B);  // #CYRILLIC SMALL LETTER EL
	ff->AddRemapChar(0xEC, 0x043C);  // #CYRILLIC SMALL LETTER EM
	ff->AddRemapChar(0xED, 0x043D);  // #CYRILLIC SMALL LETTER EN
	ff->AddRemapChar(0xEE, 0x043E);  // #CYRILLIC SMALL LETTER O
	ff->AddRemapChar(0xEF, 0x043F);  // #CYRILLIC SMALL LETTER PE
	ff->AddRemapChar(0xF0, 0x0440);  // #CYRILLIC SMALL LETTER ER
	ff->AddRemapChar(0xF1, 0x0441);  // #CYRILLIC SMALL LETTER ES
	ff->AddRemapChar(0xF2, 0x0442);  // #CYRILLIC SMALL LETTER TE
	ff->AddRemapChar(0xF3, 0x0443);  // #CYRILLIC SMALL LETTER U
	ff->AddRemapChar(0xF4, 0x0444);  // #CYRILLIC SMALL LETTER EF
	ff->AddRemapChar(0xF5, 0x0445);  // #CYRILLIC SMALL LETTER HA
	ff->AddRemapChar(0xF6, 0x0446);  // #CYRILLIC SMALL LETTER TSE
	ff->AddRemapChar(0xF7, 0x0447);  // #CYRILLIC SMALL LETTER CHE
	ff->AddRemapChar(0xF8, 0x0448);  // #CYRILLIC SMALL LETTER SHA
	ff->AddRemapChar(0xF9, 0x0449);  // #CYRILLIC SMALL LETTER SHCHA
	ff->AddRemapChar(0xFA, 0x044A);  // #CYRILLIC SMALL LETTER HARD SIGN
	ff->AddRemapChar(0xFB, 0x044B);  // #CYRILLIC SMALL LETTER YERU
	ff->AddRemapChar(0xFC, 0x044C);  // #CYRILLIC SMALL LETTER SOFT SIGN
	ff->AddRemapChar(0xFD, 0x044D);  // #CYRILLIC SMALL LETTER E
	ff->AddRemapChar(0xFE, 0x044E);  // #CYRILLIC SMALL LETTER YU
	ff->AddRemapChar(0xFF, 0x044F);  // #CYRILLIC SMALL LETTER YA
    //
}