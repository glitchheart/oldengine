@echo off
call timecmd.bat project_build
call timecmd.bat build_build
call timecmd.bat run_build
call timecmd.bat install_build
call timecmd.bat glitch_build