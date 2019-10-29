
namespace Strings {

////////////////////////////////////////////////
// sprint

inline	int 
SCountPrintf(const char * format, va_list pargs) 
{
	int retval;
	va_list argcopy;
	va_copy(argcopy, pargs);
	retval = vsnprintf(NULL, 0, format, argcopy);
	va_end(argcopy);
	return retval;
}

inline	int 
SPrintf(char * const  Desc, unsigned int Desc_Size, char const * Format, va_list args) 
{
	return vsnprintf(Desc, Desc_Size, Format, args);
}


inline	int	
SPrintf(char* Buffer, unsigned int Buffer_Size, char const *  Format, ...) 
{
	va_list			mark;
	va_start(mark, Format);
	int retval = SPrintf(Buffer, Buffer_Size, Format, mark);
	va_end(mark);
	return retval;
}

template <int Size>
inline	int	
SPrintf(char(&Buffer)[Size], char const *  Format, ...) 
{
	va_list			mark;
	va_start(mark, Format);
	int retval = SPrintf(&Buffer[0], Size, Format, mark);
	va_end(mark);
	return retval;
}

inline	int 
WSCountPrintf(const wchar_t * format, va_list pargs) 
{
	int retval;
	va_list argcopy;
	va_copy(argcopy, pargs);
	retval = _vsnwprintf(NULL, 0, format, argcopy);
	va_end(argcopy);
	return retval;
}

inline	int 
WSPrintf(wchar_t * const  Desc, unsigned int Desc_Size, wchar_t const * Format, va_list args) 
{
	return _vsnwprintf(Desc, Desc_Size, Format, args);
}


inline	int	
WSPrintf(wchar_t* Buffer, unsigned int Buffer_Size, wchar_t const *  Format, ...) 
{
	va_list			mark;
	va_start(mark, Format);
	int retval = WSPrintf(Buffer, Buffer_Size, Format, mark);
	va_end(mark);
	return retval;
}

template <int Size>
inline	int	
WSPrintf(wchar_t(&Buffer)[Size], wchar_t const *  Format, ...) 
{
	va_list			mark;
	va_start(mark, Format);
	int retval = WSPrintf(&Buffer[0], Size, Format, mark);
	va_end(mark);
	return retval;
}

////////////////////////////////////////////////
// Converting

template <u32 Size>
inline void 
WideToAnsi(char (&Buffer)[Size], wchar_t* StrIn) 
{
    int res_len = WideCharToMultiByte(CP_ACP, 0, StrIn, -1, NULL, 0, NULL, NULL);

    if (res_len == 0){   Buffer[0] = 0; return ;  }
    if (res_len > Size) res_len = Size;

    res_len = WideCharToMultiByte(CP_ACP, 0, StrIn, -1, &Buffer[0], res_len, NULL, NULL);
  ///  DASSERT (res_len != 0); // else error string convert
}

template <u32 Size>
inline void 
AnsiToWide(wchar_t (&Buffer)[Size], char* StrIn) 
{
    int res_len = MultiByteToWideChar(CP_ACP, 0, StrIn, -1, NULL, 0);
    
    if (res_len == 0){   Buffer[0] = 0; return ;  }
    if (res_len > Size) res_len = Size;

    res_len = MultiByteToWideChar(CP_ACP, 0, StrIn, -1, &Buffer[0], res_len);
    DASSERT (res_len != 0); // else error string convert
}

template <u32 Size>
inline void 
WideToUTF8(char (&Buffer)[Size], wchar_t* StrIn) 
{
    int res_len = WideCharToMultiByte(CP_UTF8, 0, StrIn, -1, NULL, 0, NULL, NULL);

    if (res_len <= 0){   Buffer[0] = 0; return ;  }
    if (res_len > Size) res_len = Size;

    res_len = WideCharToMultiByte(CP_UTF8, 0, StrIn, -1, &Buffer[0], res_len, NULL, NULL);
    DASSERT (res_len != 0); // else error string convert
}

template <u32 Size>
inline void 
UTF8ToWide(wchar_t (&Buffer)[Size], char* StrIn) 
{
    int res_len = MultiByteToWideChar(CP_UTF8, 0, StrIn, -1, NULL, 0);

    if (res_len <= 0){   Buffer[0] = 0; return ;  }
    if (res_len > Size) res_len = Size;

    res_len = MultiByteToWideChar(CP_UTF8, 0, StrIn, -1, &Buffer[0], res_len);
    DASSERT (res_len != 0); // else error string convert
}

}


#define My_sprintf      Strings::SPrintf

#ifdef OS_LINUX
#define stricmp		strcasecmp
#define strnicmp	strncasecmp
#endif

namespace Strings {
	////
	inline	u64	Len(PConstStr Str)	{		
		return strlen(Str);					
	}
	////
	inline	void	CorrectDirLunix(PStr Str) {
		u64	 DestLen = Len(Str);
		for (u64 i = 0; i < DestLen; i++) {
			if (Str[i] == '\\') Str[i] = '/';
		}
	}
	inline	void	CorrectDirWin(PStr Str) {
		u64	 DestLen = Len(Str);
		for (u64 i = 0; i < DestLen; i++) {
			if (Str[i] == '/') Str[i] = '\\';
		}
	}
	////
	inline	void	Copy_N(PStr Dest, u32 Dest_Size, PConstStr Source, u32 Source_Size) {
		///DASSERT((Dest_Size-1) > Source_Size);
		strncpy(Dest, Source, Source_Size);
		Dest[Source_Size] = '\0';
	}

	inline	void	Copy(PStr Dest, u32 Dest_Size, PConstStr Source) {
		Copy_N(Dest, Dest_Size, Source, u32(Len(Source)));
	}
	////
	inline	void	Append_N(PStr Dest, u32 Dest_Size, PConstStr Source, u32 Source_Size) {
		u32		DestLen		= u32(Len(Dest));
		///DASSERT((Dest_Size-DestLen-1) > Source_Size);
		Copy_N(Dest + DestLen, Dest_Size - DestLen, Source, Source_Size);
	}

	inline	void	Append(PStr Dest, u32 Dest_Size, PConstStr Source) {
		Append_N(Dest, Dest_Size, Source, u32(Len(Source)));
	}
	////
	inline	int		Cmp(PConstStr L, PConstStr R) {
		return		strcmp(L, R);
	}

	inline	int		CmpLen(PConstStr  L, PConstStr R, u32 R_Len) {
		return		strncmp(L, R, R_Len);
	}

	inline	int		CmpI(PConstStr  L, PConstStr R) { 										// без учета регистра
		return		stricmp(L, R);
	}

	inline	int		CmpLenI(PConstStr  L, PConstStr R, u32 R_Len) { 						// без учета регистра
		return		strnicmp(L, R, R_Len);
	}
	// Find Pos
	inline	PConstStr	FindPtrChar(PConstStr Str, int ch) { 								// first pos char in str
		return strchr(Str, ch);
	}

	inline	u32	FindLenStr_NotChars(PConstStr Str, PConstStr contolChars) { 				// len str not have contol Chars
		size_t	pos = strcspn(Str, contolChars);											// x64/x32
		if (pos > 0xFFFFFFFF) { pos = 0xFFFFFFFF; }

		return (u32)pos;
	}

	inline	u32	FindLenStr_Chars(PConstStr Str, PConstStr contolChars) {					// len str have contol Chars
		size_t	pos = strspn(Str, contolChars);											// x64/x32
		if (pos > 0xFFFFFFFF) { pos = 0xFFFFFFFF; }

		return (u32)pos;
	}
	// convert str to 
	inline	f32		ToFloat(PConstStr Str) {
		return	f32(atof(Str));
	}

	inline	int		ToInt(PConstStr Str) {
		return	atoi(Str);
	}

	inline	long	ToLong(PConstStr Str) {
		return	atol(Str);
	}
	// zero end chars
	inline	void		DelEndChar(PStr Str, int ch) {
		u32 len = u32(Len(Str));
		for (len; ((len > 0) && (Str[len - 1] == ch)); len--) {
			Str[len - 1] = 0; // zero end
		}
	}

	
	// stringbuffers
	template <u32 Size>
	inline	u64	Len(char(&Buffer)[Size]) { return strnlen(&Buffer[0], Size); }

	template <u32 Size>
	inline	void	Copy(char(&Buffer)[Size], PConstStr Source) { Copy((&Buffer)[0], Size, Source); }

	template <u32 Size>
	inline	void	Copy_N(char(&Buffer)[Size], PConstStr Source, u32 Source_Size) { Copy_N((&Buffer)[0], Size, Source, Source_Size); }

	template <u32 Size>
	inline	void	Append(char(&Buffer)[Size], PConstStr Source) { Append((&Buffer)[0], Size, Source); }

	template <u32 Size>
	inline	void	Append_N(char(&Buffer)[Size], PConstStr Source, u32 Source_Size) { Append_N((&Buffer)[0], Size, Source, Source_Size); }


    inline	 bool      SpecialCmpLen(PConstStr  L, PConstStr R, u32 R_Len){
        u64 L_Len = Strings::Len(L);
        if (L_Len != R_Len)     return false;
        for (u64 i = 0; i < R_Len; i++) if (L[i] !=  R[i])     return false;
        return true;
    }
}
