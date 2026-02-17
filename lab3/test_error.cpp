#include "EvryThng.h"
#include <stdio.h>

int main() {
#ifdef UNICODE
    PrintMsg(GetStdHandle(STD_ERROR_HANDLE), _T("UNICODE=ON\n"));
#else
    PrintMsg(GetStdHandle(STD_ERROR_HANDLE), _T("UNICODE=OFF\n"));
#endif
    HANDLE h = CreateFile(_T("no_such_file_123.txt"),
                          GENERIC_READ,
                          FILE_SHARE_READ,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);

    if (h == INVALID_HANDLE_VALUE) {
        ReportError(_T("WinAPI: не удалось открыть файл"), 0, TRUE);
    } else {
        CloseHandle(h);
    }

    FILE* f = NULL;
    #ifdef UNICODE
        _wfopen_s(&f, L"no_such_file_123.txt", L"rb");
    #else
        fopen_s(&f, "no_such_file_123.txt", "rb");
    #endif

    if (!f) {
        perror("perror: fopen");
    } else {
        fclose(f);
    }

    return 0;
}
