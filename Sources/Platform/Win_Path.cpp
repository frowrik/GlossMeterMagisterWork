#include "stdafx.h"

#define     PathStr    L"%s\\"

stringPath      CurrectPath;
stringPath      CurrectDataPath;

wstringPath     CurrectPathW;
wstringPath     CurrectDataPathW;

void
FindPaths ()
{
    //
    _wgetcwd(CurrectPathW, 260);
    //
    wstringPath      TempDataPathW;
    Strings::WSPrintf (TempDataPathW, PathStr, CurrectPathW);
    _wfullpath(CurrectDataPathW, TempDataPathW, 260);
    //
    SetCurrentDirectoryW (CurrectDataPathW);
    //
    Strings::WideToAnsi (CurrectPath, CurrectPathW);
    Strings::WideToAnsi (CurrectDataPath, CurrectDataPathW);

}

PConstStr
GetPathData (){
    return CurrectDataPath;
}

PConstWStr
GetPathDataW (){
    return CurrectDataPathW;
}

PConstStr
GetPathCurrent (){
    return CurrectPath;
}

PConstWStr
GetPathCurrentW (){
    return CurrectPathW;
}

