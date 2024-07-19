@echo off
mkdir build
cd build

cl /I"..\include" /O2 /LD /Fe"tsclicker.dll" ..\src\*.cpp /link /LIBPATH:"C:\Qt\Qt5.12.2\5.12.2\msvc2017_64\lib" User32.lib Qt5Widgets.lib

cd ..