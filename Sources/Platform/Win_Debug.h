
#define     DEBUG_INVOKE()      __debugbreak()

#define		DPRINT(fmt, ...)     DebugerPrint(fmt, __VA_ARGS__)
#define		DINFO(fmt, ...)      DebugerPrintER("Info:	(%s)-> " fmt, __FUNCTION__, __VA_ARGS__)
#define		DWARN(fmt, ...)      DebugerPrintER("Warn:	(%s)-> " fmt, __FUNCTION__, __VA_ARGS__)
#define		DERROR(fmt, ...)     DebugerPrintER("Error:	(%s, in %s, line %d)-> " fmt, __FUNCTION__, __FILE__, __LINE__,  __VA_ARGS__)
#define		DFATAL(fmt, ...)    {DebugerPrintER("Fatal:	(%s, in %s, line %d)-> " fmt, __FUNCTION__, __FILE__, __LINE__,  __VA_ARGS__); DEBUG_INVOKE();}
#define     DASSERT( x )         (void)((!!( x )) || ( Assertfunt( __FILE__, __LINE__, #x )))

#define		DWPRINT(fmt, ...)     DebugerPrintW(fmt, __VA_ARGS__)
#define		DWINFO(fmt, ...)      DebugerPrintERW("Info:	(%s)-> " fmt, __FUNCTION__, __VA_ARGS__)
#define		DWWARN(fmt, ...)      DebugerPrintERW("Warn:	(%s)-> " fmt, __FUNCTION__, __VA_ARGS__)
#define		DWERROR(fmt, ...)     DebugerPrintERW("Error:	(%s, in %s, line %d)-> " fmt, __FUNCTION__, __FILE__, __LINE__,  __VA_ARGS__)
#define		DWFATAL(fmt, ...)    {DebugerPrintERW("Fatal:	(%s, in %s, line %d)-> " fmt, __FUNCTION__, __FILE__, __LINE__,  __VA_ARGS__); DEBUG_INVOKE();}
#define     DWASSERT( x )         (void)((!!( x )) || ( AssertfuntW( __FILE__, __LINE__, #x )))


#define		PRINT(fmt, ...)     DebugerPrint(fmt, __VA_ARGS__)
#define		INFO(fmt, ...)      DebugerPrintER("Info:	(%s)-> " fmt, __FUNCTION__, __VA_ARGS__)
#define		WARN(fmt, ...)      DebugerPrintER("Warn:	(%s)-> " fmt, __FUNCTION__, __VA_ARGS__)
#define		ERROR(fmt, ...)     DebugerPrintER("Error:	(%s, in %s, line %d)-> " fmt, __FUNCTION__, __FILE__, __LINE__,  __VA_ARGS__)
#define		FATAL(fmt, ...)    {DebugerPrintER("Fatal:	(%s, in %s, line %d)-> " fmt, __FUNCTION__, __FILE__, __LINE__,  __VA_ARGS__); DEBUG_INVOKE();}
#define     ASSERT( x )         (void)((!!( x )) || ( Assertfunt( __FILE__, __LINE__, #x )))

bool        
AssertfuntW(     wchar_t const *   Name, i32 Line, wchar_t const * EXPR);

bool        
Assertfunt(     char const *   Name, i32 Line, char const * EXPR);


void		    
DebugerPrintW(   wchar_t const *   Format, ...);

void		    
DebugerPrintERW( wchar_t const *   Format, ...);

void		    
DebugerPrint(   char const *   Format, ...);

void		    
DebugerPrintER( char const *   Format, ...);





namespace Core {
namespace Debug {

void        LogMessage (char* Msg);
void        LogMessageW (wchar_t* Msg);
void        LogFlush ();

}

bool        Debug_Init();
void        Debug_Free();

}








void
PlatformWriteLog (char *Message);

