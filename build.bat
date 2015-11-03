@ECHO OFF

REM Assumes that you've already ran the vsvars32.bat
REM needed to set up the VC++ build environment...

MKDIR build
PUSHD build

cl.exe /Zi /EHs /I .. ^
       ..\tests\main.cpp ^
       /MDd /D _DEBUG ^
       /O2 ^
       /link /out:test.exe
POPD
