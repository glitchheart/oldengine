@echo off

set WIGNORE=-wd4100 -wd4996


WHERE cl
IF %ERRORLEVEL% NEQ 0 call %VCVARSALL% x64

set CommonCompilerFlags=/MD -nologo -W4 %WIGNORE%
set CommonLinkerFlags=kernel32.lib user32.lib gdi32.lib winmm.lib shell32.lib Shlwapi.lib -incremental:no -PDB:game%random%.pdb -EXPORT:BuildGlitch -EXPORT:ReleaseGame

IF NOT EXIST build mkdir build
pushd build

cl %CommonCompilerFlags% ..\build_main.cpp -Febuild /LD /DLL /link %CommonLinkerFlags%

popd

