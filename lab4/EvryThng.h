// заголовочные файлы
#include <windows.h>
#include <tchar.h>	
#include <stdio.h>
#include <io.h>
#include <stdlib.h>

BOOL PrintStrings(HANDLE hOut, ...);
BOOL PrintMsg(HANDLE hOut, LPCTSTR pMsg);
BOOL ConsolePrompt(LPCTSTR pPromptMsg, LPTSTR pResponse, DWORD MaxTchar, BOOL Echo);
VOID ReportError(LPCTSTR UserMessage, DWORD ExitCode, BOOL PrintErrorMsg);
DWORD Options(int argc, LPTSTR argv[], LPCTSTR OptStr, ...);