#include "stdafx.h"



bool        
Assertfunt (char const *   Name, i32 Line, char const * EXPR)
{
    DPRINT("Assert: (in %s, line %d)->'%s'\n", Name, Line, EXPR);
	DEBUG_INVOKE();
	return true;
}

bool        
AssertfuntW (wchar_t const *   Name, i32 Line, wchar_t const * EXPR)
{
    DWPRINT(L"Assert: (in %s, line %d)->'%s'\n", Name, Line, EXPR);
	DEBUG_INVOKE();
	return true;
}

void		    
DebugerPrint (char const *   Format, ...)
{
    string4k  Buffer;

	va_list			mark;
	va_start(mark, Format);

	int Len = Strings::SCountPrintf(Format, mark) + 1;
	if (Len > 4093) Len = 4093;
	
	Strings::SPrintf(Buffer, Len, Format, mark);

	Buffer[Len] = 0;
	Core::Debug::LogMessage (Buffer);
	
	va_end(mark);
}

void		    
DebugerPrintER (char const *   Format, ...)
{
    string4k  Buffer;

	va_list			mark;
	va_start(mark, Format);

	int Len = Strings::SCountPrintf(Format, mark) + 1;
	if (Len > 4093) Len = 4093;
	
	Strings::SPrintf(Buffer, Len, Format, mark);

	Buffer[Len] = 0;
	Core::Debug::LogMessage (Buffer);

	va_end(mark);
}


void		    
DebugerPrintW (wchar_t const *   Format, ...)
{
    wstring4k  Buffer;

	va_list			mark;
	va_start(mark, Format);

	int Len = Strings::WSCountPrintf(Format, mark) + 1;
	if (Len > 4093) Len = 4093;
	
	Strings::WSPrintf(Buffer, Len, Format, mark);

	Buffer[Len] = 0;
	
	Core::Debug::LogMessageW (Buffer);
	
	va_end(mark);
}

void		    
DebugerPrintERW (wchar_t const *   Format, ...)
{
    wstring4k  Buffer;

	va_list			mark;
	va_start(mark, Format);

	int Len = Strings::WSCountPrintf(Format, mark) + 1;
	if (Len > 4093) Len = 4093;
	
	Strings::WSPrintf(Buffer, Len, Format, mark);

	Buffer[Len] = 0;
	
	Core::Debug::LogMessageW (Buffer);

	va_end(mark);
}







namespace Core {


#define LogBufferLimit		(1024 - 4)

stringPath      NameLog;
OS_FILE         LogFile;
string1k        LogBuffer;
u32             LogBufferPos;

void		    AppendBuffer (PConstStr Message, u64 Len);

bool        IsStudioOut = false;

bool        Debug_Init(){
    PConstStr     Name = "Log.txt";
    if (Name)   My_sprintf(NameLog, "%s\\%s", GetPathCurrent (), Name);
    else        My_sprintf(NameLog, "%s\\%s", GetPathCurrent (), "Gamelog.txt");

	LogFile = Core::FS::FileOpenWrite(NameLog);
	if (!Core::FS::FileisOpen (LogFile)) return false; 

	LogBufferPos	= 0;
	LogBuffer[0]	= 0;
     IsStudioOut = false;
    return true;
}

void        Debug_Free(){
    Debug::LogFlush();
    Core::FS::FileClose (LogFile);
}

void		AppendBuffer (PConstStr Message, u64 Len){
    if (Len > LogBufferLimit) { // Big Message
        // save old
        Debug::LogFlush();
        // write new
		Core::FS::FileWrite(LogFile, (LPVOID)Message, Len);
		return ;
	}

    if ((LogBufferPos + Len) > LogBufferLimit) Debug::LogFlush(); // в буфере мало место сохраним
    
	Strings::Append_N(LogBuffer, Message, Len);
	LogBufferPos += Len;
}

void		Message (PConstStr Message){
    // Modyfi \n -> \r\n Txt files
	u64 MessageLen		        = strlen(Message);
	PConstStr	Control		    = "\r\n";
	PConstStr	ParseStr        	= Message;
	PConstStr	ParseStrEnd	    = ParseStr + (MessageLen - 1);
	PConstStr	End;

	while (ParseStr <= ParseStrEnd) {
		while (ParseStr <= ParseStrEnd) {
			if (ParseStr[0] == 0) return ;
			if (ParseStr[0] != '\n') break; // start message
			if (ParseStr[0] == '\n') AppendBuffer(Control,	2);
			ParseStr++;
		}
		
		End	= ParseStr;

		while (End <= ParseStrEnd) {
			if (End[0] == 0) return ;
			if (End[0] == '\n') break; // start message
			End++;
		}

		u32	len		= End - ParseStr;

		if (len > 0) AppendBuffer(ParseStr, len);

		ParseStr	= End;
	}
}

namespace Debug {


void        LogMessage(char* Msg){
    if (IsStudioOut) OutputDebugStringA(Msg);
    //..                                                 
    Message (Msg);
}

void        LogMessageW(wchar_t* Msg){
    ///if (IsStudioOut) OutputDebugStringW(Msg);
    //..
    // traslate to ansi/ not used wideshar
    string4k MsgTemp;
    Strings::WideToAnsi (MsgTemp, Msg);
    LogMessage(MsgTemp);
}


void        LogFlush(){
    if (LogBufferPos > 0) Core::FS::FileWrite(LogFile, &LogBuffer[0], LogBufferPos);
    LogBuffer[0]	= 0;
    LogBufferPos	= 0;
}

}

}



