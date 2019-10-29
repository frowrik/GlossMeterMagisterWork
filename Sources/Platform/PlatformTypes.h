
#ifdef OS_WINDOW

typedef signed      char            i8;
typedef unsigned    char            u8;
typedef signed      short           i16;
typedef unsigned    short           u16;
typedef signed      int             i32;
typedef unsigned    int             u32;
typedef signed      long long int   i64;
typedef unsigned    long long int   u64;
typedef float                       f32;
typedef double                      f64;
typedef void*                       ptr;
typedef signed      int             b32;  // my bool  0-false >=1-true

typedef char*                       PStr;
typedef char const*                 PConstStr;

typedef wchar_t*                    PWStr;
typedef wchar_t const*              PConstWStr;

typedef char                        stringPath[260];
typedef char                        string16[16];
typedef char                        string32[32];
typedef char                        string64[64];
typedef char                        string128[128];
typedef char                        string256[256];
typedef char                        string512[512];
typedef char                        string1k[1024];
typedef char                        string2k[2048];
typedef char                        string4k[4096];
typedef char                        string8k[8192];
typedef char                        string16k[16384];
typedef char                        string32k[32768];
typedef char                        string64k[65536];
typedef char                        string128k[131072];
typedef char                        string256k[262144];
typedef char                        string512k[524288];
typedef char                        string1m[1048576];
typedef char                        string2m[2097152];
typedef char                        string4m[4194304];

typedef wchar_t                     wstringPath[260];
typedef wchar_t                     wstring16[16];
typedef wchar_t                     wstring32[32];
typedef wchar_t                     wstring64[64];
typedef wchar_t                     wstring128[128];
typedef wchar_t                     wstring256[256];
typedef wchar_t                     wstring512[512];
typedef wchar_t                     wstring1k[1024];
typedef wchar_t                     wstring2k[2048];
typedef wchar_t                     wstring4k[4096];
typedef wchar_t                     wstring8k[8192];
typedef wchar_t                     wstring16k[16384];
typedef wchar_t                     wstring32k[32768];
typedef wchar_t                     wstring64k[65536];
typedef wchar_t                     wstring128k[131072];
typedef wchar_t                     wstring256k[262144];
typedef wchar_t                     wstring512k[524288];
typedef wchar_t                     wstring1m[1048576];
typedef wchar_t                     wstring2m[2097152];
typedef wchar_t                     wwstring4m[4194304];


#else 
#error "Unknown types platform"
#endif