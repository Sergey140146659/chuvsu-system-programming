#include "EvryThng.h"
#include <vector>

#pragma comment(lib, "user32.lib")

static BOOL PrintOemW(HANDLE hOut, LPCWSTR s) {
    DWORD wlen = (DWORD)lstrlenW(s);
    std::vector<char> oem((size_t)wlen + 1);
    CharToOemBuffW(s, oem.data(), wlen);
    oem[wlen] = '\0';
    DWORD written = 0;
    return WriteFile(hOut, oem.data(), (DWORD)lstrlenA(oem.data()), &written, NULL);
}

VOID ReportError(LPCTSTR UserMessage, DWORD ExitCode, BOOL PrintErrorMsg) {
    DWORD LastErr = GetLastError();
    HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);

    PrintOemW(hStdErr, UserMessage);
    PrintOemW(hStdErr, L"\n");

    if (PrintErrorMsg) {
        LPTSTR lpvSysMsg = NULL;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            LastErr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpvSysMsg,
            0,
            NULL
        );
        PrintOemW(hStdErr, lpvSysMsg);
        PrintOemW(hStdErr, L"\n");
        LocalFree(lpvSysMsg);
    }
    if (ExitCode > 0) {
        ExitProcess(ExitCode);
    }
}
