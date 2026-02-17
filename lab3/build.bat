@echo off

rem UTF-8 source + UNICODE build
cl /utf-8 /EHsc /nologo /DUNICODE /D_UNICODE PrintMsg.cpp ReportError.cpp test_error.cpp user32.lib /Fe:test_error.exe

cl /utf-8 /EHsc /nologo /DUNICODE /D_UNICODE PrintMsg.cpp ReportError.cpp locker.cpp user32.lib /Fe:locker.exe

cl /utf-8 /EHsc /nologo /DUNICODE /D_UNICODE PrintMsg.cpp ReportError.cpp tester.cpp user32.lib /Fe:tester.exe
