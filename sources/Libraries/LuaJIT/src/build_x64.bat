@echo off
cd /d "%~dp0"
call "C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64 || exit /b 1
cd /d "%~dp0"
set PATH=%CD%;%PATH%
call "%~dp0msvcbuild.bat"
exit /b %errorlevel%
