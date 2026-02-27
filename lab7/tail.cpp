#include <windows.h>
#include <tchar.h>

static BOOL IsConsoleHandle(HANDLE h) {
    DWORD mode;
    return GetConsoleMode(h, &mode);
}

static void CopyFromPosToEnd(HANDLE hFile, LARGE_INTEGER pos, HANDLE hOut) {
    const DWORD BUF = 65536;
    char buf[BUF];
    DWORD rd, wr;

    SetFilePointerEx(hFile, pos, NULL, FILE_BEGIN);

    BOOL outIsConsole = IsConsoleHandle(hOut);

    while (ReadFile(hFile, buf, BUF, &rd, NULL) && rd > 0) {
        if (outIsConsole) {
            WriteConsoleA(hOut, buf, rd, &wr, NULL);
        } else {
            WriteFile(hOut, buf, rd, &wr, NULL);
        }
    }
}

int _tmain(int argc, TCHAR* argv[]) {
    HANDLE hFile = CreateFile(argv[1],
                              GENERIC_READ,
                              FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);

    LARGE_INTEGER size;
    GetFileSizeEx(hFile, &size);

    LARGE_INTEGER cur;
    cur.QuadPart = size.QuadPart;

    int lfCount = 0;
    char ch;
    DWORD rd;

    while (cur.QuadPart > 0) {
        cur.QuadPart -= 1;
        SetFilePointerEx(hFile, cur, NULL, FILE_BEGIN);
        ReadFile(hFile, &ch, 1, &rd, NULL);

        if (ch == '\n') {
            lfCount++;
            if (lfCount == 11) {
                cur.QuadPart += 1;
                break;
            }
        }
    }

    LARGE_INTEGER startPos;
    if (lfCount < 10) {
        startPos.QuadPart = 0;
    } else {
        startPos = cur;
    }

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CopyFromPosToEnd(hFile, startPos, hOut);

    CloseHandle(hFile);
    return 0;
}