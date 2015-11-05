@ECHO OFF

REM Assumes that you've already ran the vsvars32.bat
REM needed to set up the VC++ build environment...

MKDIR build
PUSHD build

cl.exe /Zi /EHs ^
       /I .. ^
       /I ..\tests ^
       /I ..\submodules\catch\include ^
       ..\tests\testmain.cpp ^
       ..\tests\scoped_ptr_tests.cpp ^
       ..\tests\shared_ptr_tests.cpp ^
       /MDd /D _DEBUG ^
       /Od ^
       /link /out:test.exe
POPD
