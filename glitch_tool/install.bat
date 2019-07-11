@echo off
set path "%cd%\build;%path%;" 2>NUL
setx path "%cd%\build;%path%;" 2>NUL
cd ../ >NUL
set GLITCH_ENGINE %cd% 2>NUL
setx GLITCH_ENGINE %cd% 2>NUL