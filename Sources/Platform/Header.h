// include libs
#include "PlatformDetect.h"
#include "PlatformTypes.h"

#include "Win_Inludes.h"
#include "Win_DisableWarning.h"

#include "Strings.h"

#include "Win_FS.h"
#include "Win_Memory.h"
#include "Win_Time.h"
#include "Win_Path.h"
#include "Win_Debug.h"
#include "Win_Thread.h"



namespace  Core {
    bool        Init();
    void        Free();
}


uint64_t xorshift128plus();
/*
Core::
Core::FS::
Core::Mem::
Core::Time::
Strings::
Core::Debug::
//..

*/