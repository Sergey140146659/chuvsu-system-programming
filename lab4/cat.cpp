#include "EvryThng.h"

static BOOL IsConsoleHandle(HANDLE h) {
    DWORD mode = 0;
    return GetConsoleMode(h, &mode);
}

static VOID CatFile(HANDLE hInFile, HANDLE hOutFile) {
    const DWORD BUF_SIZE = 1024;
    char buf[BUF_SIZE];
    DWORD bytesRead = 0, bytesWritten = 0;

    BOOL outIsConsole = IsConsoleHandle(hOutFile);

    while (ReadFile(hInFile, buf, BUF_SIZE, &bytesRead, NULL) && bytesRead > 0) {
        if (outIsConsole) {
            WriteConsoleA(hOutFile, buf, bytesRead, &bytesWritten, NULL);
        } else {
            WriteFile(hOutFile, buf, bytesRead, &bytesWritten, NULL);
        }
    }
}

int _tmain(int argc, LPTSTR argv[]) {
    BOOL suppress = FALSE;

    DWORD firstFile = Options(argc, argv, _T("s"), &suppress, NULL);

    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if ((int)firstFile >= argc) {
        CatFile(hStdIn, hStdOut);
        return 0;
    }

    for (int i = (int)firstFile; i < argc; i++) {
        HANDLE hIn = CreateFile(argv[i],
                                GENERIC_READ,
                                FILE_SHARE_READ,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

        if (hIn == INVALID_HANDLE_VALUE) {
            if (!suppress) {
                ReportError(_T("cat: не удалось открыть файл"), 0, TRUE);
            }
        } else {
            CatFile(hIn, hStdOut);
            CloseHandle(hIn);
        }
    }

    return 0;
}