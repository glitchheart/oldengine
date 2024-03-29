@echo off

set WIGNORE=-wd4100 -wd4996


WHERE cl
IF %ERRORLEVEL% NEQ 0 call %VCVARSALL% x64

set CommonCompilerFlags=/MD -nologo -W4 %WIGNORE%
set CommonLinkerFlags=kernel32.lib user32.lib gdi32.lib winmm.lib shell32.lib Shlwapi.lib

IF NOT EXIST build mkdir build
pushd build

cl %CommonCompilerFlags% ..\main.cpp -Feglitch /link %CommonLinkerFlags%

popd

