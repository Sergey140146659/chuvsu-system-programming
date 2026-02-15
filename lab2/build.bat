@echo off
cl /EHsc /nologo PrintMsg.cpp test.cpp /Fe:test.exe >nul 2>&1
echo All programs built successfully!