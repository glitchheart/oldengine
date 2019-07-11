#!/bin/bash

chmod 755 osx_project_build.sh
chmod 755 osx_build_build.sh
chmod 755 osx_run_build.sh
chmod 755 osx_install_build.sh
chmod 755 osx_glitch_build.sh
time ./osx_project_build.sh
time ./osx_build_build.sh
time ./osx_run_build.sh
time ./osx_install_build.sh
time ./osx_glitch_build.sh