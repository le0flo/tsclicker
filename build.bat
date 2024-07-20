@echo off

mkdir build
cd build
cls

cl ^
/I"..\include" ^
/I"C:\Qt\Qt5.12.2\5.12.2\msvc2017_64\include" ^
/I"C:\Qt\Qt5.12.2\5.12.2\msvc2017_64\include\QtCore" ^
/I"C:\Qt\Qt5.12.2\5.12.2\msvc2017_64\include\QtWidgets" ^
/I"C:\Qt\Qt5.12.2\5.12.2\msvc2017_64\include\QtMultimedia" ^
/O2 /LD /EHsc /Fe"tsclicker.dll" ..\src\*.cpp ^
/link /LIBPATH:"C:\Qt\Qt5.12.2\5.12.2\msvc2017_64\lib" User32.lib Qt5Core.lib Qt5Widgets.lib Qt5Multimedia.lib

cd ..