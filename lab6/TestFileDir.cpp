#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <locale.h>

static void PrintHelp() {
    _tprintf(
        _T("TestFileDir - управление файлами и каталогами (WinAPI)\r\n")
        _T("Использование:\r\n")
        _T("  TestFileDir Имя1 Имя2\r\n")
        _T("  TestFileDir /?\r\n\r\n")
        _T("Назначение:\r\n")
        _T("  Демонстрация основных операций с файлами/каталогами: создание, удаление,\r\n")
        _T("  копирование, переименование в текущем каталоге.\r\n\r\n")
        _T("Автор: Щербаков СВ\r\n")
    );
}

static void PrintLastError(LPCTSTR userMsg) {
    DWORD e = GetLastError();
    LPTSTR sysMsg = nullptr;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, e, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&sysMsg, 0, NULL
    );
    _tprintf(_T("%s\r\n"), userMsg);
    if (sysMsg) {
        _tprintf(_T("%s\r\n"), sysMsg);
        LocalFree(sysMsg);
    } else {
        _tprintf(_T("GetLastError=%lu\r\n"), e);
    }
}

static bool Exists(LPCTSTR path) {
    DWORD a = GetFileAttributes(path);
    return a != INVALID_FILE_ATTRIBUTES;
}

static bool IsDir(LPCTSTR path) {
    DWORD a = GetFileAttributes(path);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
}

static bool IsFile(LPCTSTR path) {
    DWORD a = GetFileAttributes(path);
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}

static bool AskYesNo(LPCTSTR question) {
    _tprintf(_T("%s (y/n): "), question);
    TCHAR buf[16] = {0};
    _fgetts(buf, 16, stdin);
    return (buf[0] == _T('y') || buf[0] == _T('Y'));
}

static bool CreateEmptyFile(LPCTSTR name) {
    HANDLE h = CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        return false;
    }
    CloseHandle(h);
    return true;
}

static bool CopyDirRecursive(LPCTSTR srcDir, LPCTSTR dstDir);

static bool CopyAny(LPCTSTR src, LPCTSTR dst) {
    if (!Exists(src)) {
        SetLastError(ERROR_FILE_NOT_FOUND);
        return false;
    }
    if (IsFile(src)) {
        return CopyFile(src, dst, FALSE) ? true : false;
    }
    return CopyDirRecursive(src, dst);
}

static bool EnsureDirExists(LPCTSTR path) {
    if (Exists(path)) {
        return IsDir(path);
    }
    return CreateDirectory(path, NULL) ? true : false;
}

static bool CopyDirRecursive(LPCTSTR srcDir, LPCTSTR dstDir) {
    if (!EnsureDirExists(dstDir)) return false;

    TCHAR pattern[MAX_PATH];
    _stprintf_s(pattern, _T("%s\\*"), srcDir);

    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(pattern, &fd);
    if (hFind == INVALID_HANDLE_VALUE) return false;

    bool ok = true;

    do {
        if (_tcscmp(fd.cFileName, _T(".")) == 0 || _tcscmp(fd.cFileName, _T("..")) == 0) {
            continue;
        }

        TCHAR srcPath[MAX_PATH], dstPath[MAX_PATH];
        _stprintf_s(srcPath, _T("%s\\%s"), srcDir, fd.cFileName);
        _stprintf_s(dstPath, _T("%s\\%s"), dstDir, fd.cFileName);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (!CopyDirRecursive(srcPath, dstPath)) { ok = false; break; }
        } else {
            if (!CopyFile(srcPath, dstPath, FALSE)) { ok = false; break; }
        }
    } while (FindNextFile(hFind, &fd));

    FindClose(hFind);
    return ok;
}

static bool DeleteDirRecursive(LPCTSTR dirPath) {
    if (!IsDir(dirPath)) {
        SetLastError(ERROR_PATH_NOT_FOUND);
        return false;
    }

    TCHAR pattern[MAX_PATH];
    _stprintf_s(pattern, _T("%s\\*"), dirPath);

    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(pattern, &fd);
    if (hFind == INVALID_HANDLE_VALUE) return false;

    bool ok = true;

    do {
        if (_tcscmp(fd.cFileName, _T(".")) == 0 || _tcscmp(fd.cFileName, _T("..")) == 0) {
            continue;
        }

        TCHAR child[MAX_PATH];
        _stprintf_s(child, _T("%s\\%s"), dirPath, fd.cFileName);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (!DeleteDirRecursive(child)) { ok = false; break; }
        } else {
            if (!DeleteFile(child)) { ok = false; break; }
        }
    } while (FindNextFile(hFind, &fd));

    FindClose(hFind);
    if (!ok) return false;
    return RemoveDirectory(dirPath) ? true : false;
}

static void PrintMenu(LPCTSTR name1, LPCTSTR name2) {
    _tprintf(_T("\r\n=== TestFileDir ===\r\n"));
    _tprintf(_T("Имя1: %s\r\n"), name1);
    _tprintf(_T("Имя2: %s\r\n\r\n"), name2);

    _tprintf(_T("1) Создать пустой файл Имя1\r\n"));
    _tprintf(_T("2) Создать каталог Имя1\r\n"));
    _tprintf(_T("3) Удалить файл Имя1\r\n"));
    _tprintf(_T("4) Удалить каталог Имя1 (рекурсивно)\r\n"));
    _tprintf(_T("5) Копировать Имя1 -> Имя2 (файл/каталог)\r\n"));
    _tprintf(_T("6) Переименовать/переместить Имя1 -> Имя2 (с подтверждением)\r\n"));
    _tprintf(_T("0) Выход\r\n"));
    _tprintf(_T("\r\nВыбор: "));
}

int _tmain(int argc, TCHAR* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "");
    if (argc >= 2 && (_tcscmp(argv[1], _T("/?")) == 0 || _tcscmp(argv[1], _T("-?")) == 0)) {
        PrintHelp();
        return 0;
    }

    if (argc < 3) {
        PrintHelp();
        return 1;
    }

    LPCTSTR name1 = argv[1];
    LPCTSTR name2 = argv[2];

    while (true) {
        PrintMenu(name1, name2);

        TCHAR buf[32] = {0};
        _fgetts(buf, 32, stdin);
        int choice = _ttoi(buf);

        SetLastError(0);

        if (choice == 0) {
            return 0;
        } else if (choice == 1) {
            if (!CreateEmptyFile(name1)) {
                PrintLastError(_T("Ошибка: не удалось создать файл."));
            } else {
                _tprintf(_T("OK: файл создан.\r\n"));
            }
        } else if (choice == 2) {
            if (!CreateDirectory(name1, NULL)) {
                PrintLastError(_T("Ошибка: не удалось создать каталог."));
            } else {
                _tprintf(_T("OK: каталог создан.\r\n"));
            }
        } else if (choice == 3) {
            if (!IsFile(name1)) {
                _tprintf(_T("Ошибка: файл не найден.\r\n"));
            } else if (!DeleteFile(name1)) {
                PrintLastError(_T("Ошибка: не удалось удалить файл."));
            } else {
                _tprintf(_T("OK: файл удалён.\r\n"));
            }
        } else if (choice == 4) {
            if (!IsDir(name1)) {
                _tprintf(_T("Ошибка: каталог не найден.\r\n"));
            } else if (!DeleteDirRecursive(name1)) {
                PrintLastError(_T("Ошибка: не удалось удалить каталог."));
            } else {
                _tprintf(_T("OK: каталог удалён.\r\n"));
            }
        } else if (choice == 5) {
            if (!CopyAny(name1, name2)) {
                if (!Exists(name1)) _tprintf(_T("Ошибка: источник не найден.\r\n"));
                else PrintLastError(_T("Ошибка: не удалось выполнить копирование."));
            } else {
                _tprintf(_T("OK: копирование выполнено.\r\n"));
            }
        } else if (choice == 6) {
            if (!Exists(name1)) {
                _tprintf(_T("Ошибка: источник не найден.\r\n"));
            } else {
                if (AskYesNo(_T("Подтвердите переименование/перемещение"))) {
                    if (!MoveFile(name1, name2)) {
                        PrintLastError(_T("Ошибка: не удалось переименовать/переместить."));
                    } else {
                        _tprintf(_T("OK: переименование/перемещение выполнено.\r\n"));
                    }
                } else {
                    _tprintf(_T("Отменено.\r\n"));
                }
            }
        } else {
            _tprintf(_T("Неизвестный пункт меню.\r\n"));
        }
    }
}