#include "stdafx.h"

namespace  Core {
namespace  FS {

OS_FILE			    FileOpenRead(PConstWStr NameFile)
{
	return CreateFileW(NameFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
}

OS_FILE			    FileOpenWrite(PConstWStr NameFile){
	return CreateFileW(NameFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0); //
}

OS_FILE			    FileOpenAppend(PConstWStr NameFile)
{
	return CreateFileW(NameFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
}


OS_FILE			    FileOpenRead(PConstStr NameFile)
{
	return CreateFileA(NameFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
}

OS_FILE			    FileOpenWrite(PConstStr NameFile){
	return CreateFileA(NameFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0); //
}

OS_FILE			    FileOpenAppend(PConstStr NameFile)
{
	return CreateFileA(NameFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
}

bool			    FileisOpen(OS_FILE f)
{
	if (OS_FILE_NULL != f) return true;
	return false;
}

void			    FileClose(OS_FILE f)
{
	FileSyncWrite(f);
	CloseHandle(f);
}

void			    FileSyncWrite(OS_FILE f)
{
	FlushFileBuffers(f);
}

u64				    FileGetSize(OS_FILE f)
{
	LARGE_INTEGER  size;
	GetFileSizeEx(f, &size);
	return size.QuadPart;
}

u64				    FileGetPos(OS_FILE f)
{
	return FileSeekCurr(f, 0);
}

u64				    FileSeekBegin(OS_FILE f, i64 Offset)
{
	LARGE_INTEGER  ofs, newpos;
    ofs.QuadPart = Offset;
	SetFilePointerEx(f, ofs, &newpos, FILE_BEGIN);
	return ofs.QuadPart;
}

u64				    FileSeekCurr(OS_FILE f, i64 Offset)
{
	LARGE_INTEGER  ofs;			
	LARGE_INTEGER  newpos;
    ofs.QuadPart = Offset;
	SetFilePointerEx(f, ofs, &newpos, FILE_CURRENT);
	return ofs.QuadPart;
}

u64				    FileSeekEnd(OS_FILE f, i64 Offset)
{
	LARGE_INTEGER  ofs;			
	LARGE_INTEGER  newpos;
    ofs.QuadPart = Offset;
	SetFilePointerEx(f, ofs, &newpos, FILE_END);
	return ofs.QuadPart;
}

u64				    FileRead(OS_FILE f, void* Data, u64 Len)
{
	
	if ((!Data) || (Len == 0))						return 0;

    DWORD	bytes;

    u64     ReadBytes = 0;

    while (Len > 0) {

        u64     MinLen  = min (Len, 0x0FFFFFFF);      //  винда не даёт считать больше 2 гигов за раз

        void*   DataOfs = (void*)((u64)Data + ReadBytes);

        if (!ReadFile (f, DataOfs, MinLen, &bytes, NULL)) return ReadBytes;
        
        ReadBytes += bytes;
        Len -= bytes;
    }

	return ReadBytes;
}

u64				    FileWrite(OS_FILE f, const void * Data, u64 Len)
{
	if ((!Data) || (Len == 0))						return 0;

    DWORD	bytes;

    u64     WriteBytes = 0;

    while (Len > 0) {

        u64     MinLen  = min (Len, 0x0FFFFFFF);      //  винда не даёт записать больше 2 гигов за раз

        void*   DataOfs = (void*)((u64)Data + WriteBytes);

        if (!WriteFile (f, DataOfs, MinLen, &bytes, NULL)) return WriteBytes;
        
        WriteBytes += bytes;
        Len -= bytes;
    }

	return WriteBytes;
}

u64				    FileTimeStamp (OS_FILE f)
{
    BY_HANDLE_FILE_INFORMATION info;
    
    GetFileInformationByHandle(f, &info);
    
    return ((u64)info.ftLastWriteTime.dwHighDateTime << 32) | ((u64)info.ftLastWriteTime.dwLowDateTime);
}















bool			    FileisExist(PConstWStr NameFile) 
{
	OS_FILE ff = FileOpenRead(NameFile);
	if (!FileisOpen(ff))  return false;
	FileClose(ff);
	return true;
}

bool			    FileRenameMove(PConstWStr OldNameFile, PConstWStr NewNameFile) 
{
	if (MoveFileW(OldNameFile, NewNameFile)) return true;
	return false;
}

bool			    FileDelete(PConstWStr NameFile) 
{
	if (DeleteFileW(NameFile)) return true;
	return false;
}


bool			    FileisExist(PConstStr NameFile) 
{
	OS_FILE ff = FileOpenRead(NameFile);
	if (!FileisOpen(ff))  return false;
	FileClose(ff);
	return true;
}

bool			    FileRenameMove(PConstStr OldNameFile, PConstStr NewNameFile) 
{
	if (MoveFileA(OldNameFile, NewNameFile)) return true;
	return false;
}

bool			    FileDelete(PConstStr NameFile) 
{
	if (DeleteFileA(NameFile)) return true;
	return false;
}
















bool			    DirCreate(PConstWStr Path) 
{
	if (CreateDirectoryW(Path, NULL)) return true;
	return false;
}

bool			    DirDelete(PConstWStr Path) 
{
	if (RemoveDirectoryW(Path)) return true;
	return false;
}

bool			    DirCreate(PConstStr Path) 
{
	if (CreateDirectoryA(Path, NULL)) return true;
	return false;
}

bool			    DirDelete(PConstStr Path) 
{
	if (RemoveDirectoryA(Path)) return true;
	return false;
}




}
}