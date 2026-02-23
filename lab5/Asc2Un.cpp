#include "EvryThng.h"


BOOL Asc2Un(LPCTSTR fIn, LPCTSTR fOut, BOOL bFailIfExists, DWORD inFlags, DWORD outFlags) {
    HANDLE hIn = INVALID_HANDLE_VALUE, hOut = INVALID_HANDLE_VALUE;
    DWORD fdwOut = bFailIfExists ? CREATE_NEW : CREATE_ALWAYS;

    const DWORD BUF_SIZE = 1024;

    CHAR  aBuffer[BUF_SIZE];
    WCHAR uBuffer[BUF_SIZE];

    DWORD nIn = 0, nOut = 0;

    hIn = CreateFile(fIn,
                     GENERIC_READ,
                     FILE_SHARE_READ,
                     NULL,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL | inFlags,
                     NULL);

    if (hIn == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    hOut = CreateFile(fOut,
                      GENERIC_WRITE,
                      0,
                      NULL,
                      fdwOut,
                      FILE_ATTRIBUTE_NORMAL | outFlags,
                      NULL);

    if (hOut == INVALID_HANDLE_VALUE) {
        CloseHandle(hIn);
        return FALSE;
    }

    BOOL writeOk = TRUE;

    while (ReadFile(hIn, aBuffer, BUF_SIZE, &nIn, NULL) && nIn > 0 && writeOk) {
        for (DWORD i = 0; i < nIn; i++) {
            uBuffer[i] = (WCHAR)(unsigned char)aBuffer[i];
        }
        writeOk = WriteFile(hOut, uBuffer, 2 * nIn, &nOut, NULL);
    }

    CloseHandle(hIn);
    CloseHandle(hOut);
    return writeOk;
}