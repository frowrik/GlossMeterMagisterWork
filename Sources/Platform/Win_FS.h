

#define OS_FILE_NULL     INVALID_HANDLE_VALUE
typedef HANDLE           OS_FILE;


namespace  Core {
namespace  FS {

OS_FILE			    FileOpenRead(PConstWStr NameFile);
OS_FILE			    FileOpenRead(PConstStr NameFile);
OS_FILE			    FileOpenWrite(PConstWStr NameFile);
OS_FILE			    FileOpenWrite(PConstStr NameFile);
OS_FILE			    FileOpenAppend(PConstWStr NameFile);
OS_FILE			    FileOpenAppend(PConstStr NameFile);
bool			    FileisOpen(OS_FILE f);
void			    FileClose(OS_FILE f);
void			    FileSyncWrite(OS_FILE f);
u64				    FileGetSize(OS_FILE f);
u64				    FileGetPos(OS_FILE f);
u64				    FileSeekBegin(OS_FILE f, i64 Offset);
u64				    FileSeekCurr(OS_FILE f, i64 Offset);
u64				    FileSeekEnd(OS_FILE f, i64 Offset);
u64				    FileRead(OS_FILE f, void* Data, u64 Len);
u64				    FileWrite(OS_FILE f, const void * Data, u64 Len);
u64				    FileTimeStamp(OS_FILE f);

bool                FileisExist (PConstWStr NameFile);    
bool                FileisExist (PConstStr NameFile);
bool                FileRenameMove (PConstWStr OldNameFile, PConstWStr NewNameFile);  
bool                FileRenameMove (PConstStr OldNameFile, PConstStr NewNameFile);
bool                FileDelete (PConstWStr NameFile);    
bool                FileDelete (PConstStr NameFile);

bool                DirCreate (PConstWStr Path);
bool                DirCreate (PConstStr Path);
bool                DirDelete (PConstWStr Path);
bool                DirDelete (PConstStr Path);


}
}

