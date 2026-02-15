#undef UNICODE  
#ifdef UNICODE
  #define _UNICODE
#else
  #undef _UNICODE
#endif

// Общие заголовочные файлы
#include <windows.h>
#include <tchar.h>	
#include <stdio.h>
#include <io.h>
#include <stdlib.h>

BOOL PrintStrings (HANDLE hOut, ...);
BOOL PrintMsg (HANDLE hOut, LPCTSTR pMsg);
BOOL ConsolePrompt (LPCTSTR pPromptMsg, 
LPTSTR pResponse, DWORD MaxTchar, BOOL Echo);
