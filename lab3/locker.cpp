#include "EvryThng.h"

int _tmain(int argc, TCHAR* argv[]){
    LPCTSTR path = argv[1];
    LPCTSTR mode = argv[2];
    DWORD share = 0;
    if (_tcscmp(mode, _T("0")) == 0) {
        share = 0;
    } else if (_tcscmp(mode, _T("R")) == 0) {
        share = FILE_SHARE_READ;
    } else if (_tcscmp(mode, _T("RW")) == 0) {
         share = FILE_SHARE_READ | FILE_SHARE_WRITE;
    } else {
        PrintMsg(GetStdHandle(STD_ERROR_HANDLE), _T("share must be 0, R, or RW\n"));
        return 2;
    }

    HANDLE h = CreateFile(path, GENERIC_READ | GENERIC_WRITE, share, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (h == INVALID_HANDLE_VALUE) {
        ReportError(_T("locker: CreateFile failed"), 0, TRUE);
        return 3;
    }

    PrintMsg(GetStdHandle(STD_ERROR_HANDLE), _T("locker: file is opened and held. Press Enter to release...\n"));

    TCHAR buf[8];
    ConsolePrompt(_T(""), buf, 8, TRUE);

    CloseHandle(h);
    PrintMsg(GetStdHandle(STD_ERROR_HANDLE), _T("locker: released\n"));
    return 0;
}
