char* BuildBat = "timecmd actual_build";
char* BuildSh = "#!/bin/bash \n\n time ./actual_build.sh";
char* BuildShOSX = "#!/bin/bash \n\n time ./osx_actual_build.sh";
char* RunBat = "@echo off\n pushd build\n start cmd /k main\n popd\n";
char* RunSh = "#!/bin/bash \n\n pushd build\n gnome-terminal -x sh -c \"padsp ./main ; exec /bin/bash -i\"\n popd\n";
char* RunShOSX = "#!/bin/bash \n\n pushd build\n gnome-terminal -x sh -c \"padsp ./main ; exec /bin/bash -i\"\n popd\n";
char* ConfigFile = "title untitled\n\
version v0.1\n\
screen_width 1600\n\
screen_height 900\n\
fullscreen 0\n\
muted 0\n\
sfx_volume 0.90\n\
music_volume 0.90\n\
contrast 1.22\n\
brightness 0.1\n\
zoom 20";

char* ActualBuildBat = "@echo off \n\
set WERROR=\n\
set DEBUG=-DGLITCH_DEBUG=1\n\
set GLM= \n\
set PRP= \n\
set WIGNORED=-wd4201 -wd4100 -wd4189 -wd4505 -wd4530 -wd4577 -wd4996 -wd4456 -wd4706 -wd4390 -wd4127 \n\
\n\
WHERE cl \n\
IF %ERRORLEVEL% NEQ 0 call %VCVARSALL% x64 \n\
\n\
set CommonCompilerFlags=/MD -nologo -Od -Oi- -W4 -Gm- -EHsc -FC -Z7 %PRP% %WIGNORED% %DEBUG% %GLM% /I%GLITCH_ENGINE%\\libs /I%GLITCH_ENGINE%\\libs\\glfw\\include /I %GLITCH_ENGINE%\\libs\\freetype\\include /I %GLITCH_ENGINE%\\libs\\soft_oal\\include /I%GLITCH_ENGINE%\\libs\\glad\\include /I%GLITCH_ENGINE%\\src \n\
\n\
IF NOT EXIST build mkdir build \n\
pushd build \n\
\n\
REM 64-bit build \n\
del *.pdb > NUL 2> NUL \n\
\n\
echo Compilation started on: %time% \n\
cl %CommonCompilerFlags% ..\\src\\game.cpp -LD /DLL /link -incremental:no -PDB:game%random%.pdb -EXPORT:Update  \n\
echo Compilation finished on: %time% \n\
popd";

char* ActualBuildSh = "#!/bin/bash\n\n\
WIGNORE='-Wno-nested-anon-types -Wno-variadic-macros -Wno-old-style-cast -Wno-sign-conversion -Wno-reserved-id-macro -Wno-disabled-macro-expansion -Wno-cast-align -Wno-gnu-anonymous-struct -Wno-unused-value -Wno-c++98-compat -Wno-padded -Wno-implicit-fallthrough -Wno-double-promotion -Wno-c++98-compat-pedantic -Wno-conversion -Wno-newline-eof -Wno-missing-prototypes -Wno-float-equal -Wno-unused-parameter -Wno-documentation -Wno-documentation-unknown-command -Wno-undef -Wno-gnu-zero-variadic-macro-arguments -Wno-writable-strings -Wno-missing-variable-declarations -Wno-global-constructors -Wno-missing-noreturn -Wno-format-security -Wno-char-subscripts'\n\n\
WIGNORETEMP='-Wno-shadow -Wno-deprecated -Wno-switch-enum -Wno-switch '\n\
W='-Wno-null-dereference'\n\n\
\
CommonCompilerFlags=\"-I $GLITCH_ENGINE/libs -I $GLITCH_ENGINE/libs/glfw/include -I  $GLITCH_ENGINE/src\"\n\
\n\
pushd build\n\n\
\
clang --std=c++14 -Weverything $WIGNORE $WIGNORETEMP -DGLITCH_DEBUG=1 $CommonCompilerFlags -O0 ../src/game.cpp -fPIC -shared -o libgame.so -ldl -lpng -lz -lm\n\n\
popd";

char* ActualBuildShOSX = "#!/bin/bash\n\n\
\
WIGNORE='-Wno-nested-anon-types -Wno-variadic-macros -Wno-old-style-cast -Wno-sign-conversion -Wno-cast-align -Wno-gnu-anonymous-struct  -Wno-c++98-compat -Wno-padded -Wno-double-promotion -Wno-c++98-compat-pedantic -Wno-missing-prototypes -Wno-float-equal -Wno-unused-parameter -Wno-documentation -Wno-documentation-unknown-command -Wno-gnu-zero-variadic-macro-arguments -Wno-writable-strings -Wno-global-constructors -Wno-switch-enum -Wno-format-security -Wno-char-subscripts -Wno-unused-function'\n\n\
\
CommonCompilerFlags='-isystem $GLITCH_ENGINE/libs -isystem $GLITCH_ENGINE/libs/glfw/include -isystem  $GLITCH_ENGINE/src -I ../level_editor/src'\n\n\
pushd build\n\n\
\
clang -dynamiclib --std=c++14 -Weverything $WIGNORE $WIGNORETEMP -DGLITCH_DEBUG=1 $CommonCompilerFlags -O0 ../src/game.cpp -o libgame.dylib -ldl -lpng -lz -lm\n\n\
\
popd";

char* TimeCmd = "@echo off \n\
@setlocal \n\
\n\
set start=%time% \n\
\n\
:: runs your command \n\
cmd /c %* \n\
\n\
set end=%time% \n\
set options=\"tokens=1-4 delims=:.,\" \n\
for /f %options% %%a in (\"%start%\") do set start_h=%%a&set /a start_m=100%%b %% 100&set /a start_s=100%%c %% 100&set /a start_ms=100%%d %% 100 \n\
for /f %options% %%a in (\"%end%\") do set end_h=%%a&set /a end_m=100%%b %% 100&set /a end_s=100%%c %% 100&set /a end_ms=100%%d %% 100 \n\
\n\
set /a hours=%end_h%-%start_h% \n\
set /a mins=%end_m%-%start_m% \n\
set /a secs=%end_s%-%start_s% \n\
set /a ms=%end_ms%-%start_ms% \n\
if %ms% lss 0 set /a secs = %secs% - 1 & set /a ms = 100%ms% \n\
if %secs% lss 0 set /a mins = %mins% - 1 & set /a secs = 60%secs% \n\
if %mins% lss 0 set /a hours = %hours% - 1 & set /a mins = 60%mins% \n\
if %hours% lss 0 set /a hours = 24%hours% \n\
if 1%ms% lss 100 set ms=0%ms% \n\
\n\
:: mission accomplished \n\
set /a totalsecs = %hours%*3600 + %mins%*60 + %secs% \n\
echo command took %hours%:%mins%:%secs%.%ms% (%totalsecs%.%ms%s total)";

#define GAME_HEADER_INCLUDES 5
#define GAME_STATE_FIELDS 2

char* GameHeaderIncludes[GAME_HEADER_INCLUDES] = {
    "\"platform.h\"",
    "\"shared.h\"",
    "\"rendering.h\"",
    "\"sound.h\"",
    "\"keycontroller.h\"",
};

char* GameStateFields[GAME_STATE_FIELDS] = 
{
    "memory_arena TotalArena;",
    "b32 IsInitialized;",
};

#define GAME_CPP_INCLUDES 5

char* GameCppIncludes[GAME_CPP_INCLUDES] = 
{
    "\"game.h\"",
    "\"gmap.cpp\"",
    "\"rendering.cpp\"",
    "\"keycontroller.cpp\"",
    "\"sound.cpp\""
};

char* InitUpdate = 
"extern \"C\" UPDATE(Update) \n\
{ \n\
\tPlatform = GameMemory->PlatformAPI;\n\
\n\
\tgame_state* GameState = GameMemory->GameState;\n\
\n\
\tif(!GameState)\n\
\t{\n\
\tGameState = GameMemory->GameState = BootstrapPushStruct(game_state, TotalArena);\n\
\tGameState->IsInitialized = false;\n\
\t}\n\
\n\
\tAssert(GameState);\n\
\n\
\tif(!GameState->IsInitialized || !GameMemory->IsInitialized)\n\
\t{\n\
\t// Write your initialization code here \n\
\tGameState->IsInitialized = true;\n\
\tGameMemory->IsInitialized = true;\n\
\t}\n\
}";

