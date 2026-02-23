#include "EvryThng.h"
#include <tchar.h>

int _tmain(int argc, LPTSTR argv[]) {
    BOOL failIfExists = FALSE;

    DWORD first = Options(argc, argv, _T("i"), &failIfExists, NULL);

    if ((int)first + 2 > argc) {
        PrintMsg(GetStdHandle(STD_ERROR_HANDLE), _T("Usage: atou [-i] FileIn FileOut\n"));
        return 1;
    }

    LPCTSTR fIn  = argv[first];
    LPCTSTR fOut = argv[first + 1];

    DWORD inFlags = FILE_FLAG_SEQUENTIAL_SCAN;
    DWORD outFlags = 0;

    BOOL ok = Asc2Un(fIn, fOut, failIfExists, inFlags, outFlags);

    if (!ok) {
        ReportError(_T("atou: error"), 0, TRUE);
        return 2;
    }

    return 0;
}