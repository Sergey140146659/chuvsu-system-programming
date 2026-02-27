#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

static BOOL IsConsoleHandle(HANDLE h) {
    DWORD mode;
    return GetConsoleMode(h, &mode);
}

static void OutWrite(HANDLE hOut, const char* data, DWORD len, BOOL outIsConsole) {
    DWORD wr;
    if (outIsConsole) {
        WriteConsoleA(hOut, data, len, &wr, NULL);
    } else {
        WriteFile(hOut, data, len, &wr, NULL);
    }
}

int _tmain(int argc, TCHAR* argv[]) {
    int start = _ttoi(argv[2]);
    int count = _ttoi(argv[3]);

    HANDLE hIn = CreateFile(argv[1],
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    BOOL outIsConsole = IsConsoleHandle(hOut);

    const DWORD BUF = 65536;
    char buf[BUF];
    DWORD rd;

    int line = 1;
    int printed = 0;

    while (ReadFile(hIn, buf, BUF, &rd, NULL) && rd > 0 && printed < count) {
        DWORD i = 0;
        while (i < rd && printed < count) {
            DWORD chunkStart = i;
            while (i < rd && buf[i] != '\n') i++;
            DWORD chunkLen = i - chunkStart;
            BOOL hasLF = (i < rd && buf[i] == '\n');
            if (line >= start && printed < count) {
                OutWrite(hOut, buf + chunkStart, chunkLen, outIsConsole);
                if (hasLF) {
                    OutWrite(hOut, "\n", 1, outIsConsole);
                }
                printed++;
            }
            if (hasLF) {
                i++;
                line++;
            } else {
                break;
            }
        }
    }

    CloseHandle(hIn);
    return 0;
}