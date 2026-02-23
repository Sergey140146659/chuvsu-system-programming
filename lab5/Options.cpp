#include "EvryThng.h"
#include <stdarg.h>

DWORD Options(int argc, LPTSTR argv[], LPCTSTR OptStr, ...) {
    va_list pFlagList;
    LPBOOL pFlag;
    int iFlag = 0, iArg;

    va_start(pFlagList, OptStr);

    while ((pFlag = va_arg(pFlagList, LPBOOL)) != NULL && iFlag < (int)_tcslen(OptStr)) {
        *pFlag = FALSE;

        for (iArg = 1; !(*pFlag) && iArg < argc && argv[iArg][0] == _T('-'); iArg++) {
            LPTSTR opt = argv[iArg] + 1;
            if (_tcschr(opt, OptStr[iFlag]) != NULL) {
                *pFlag = TRUE;
            }
        }

        iFlag++;
    }

    va_end(pFlagList);

    for (iArg = 1; iArg < argc && argv[iArg][0] == _T('-'); iArg++) {
    }

    return (DWORD)iArg;
}