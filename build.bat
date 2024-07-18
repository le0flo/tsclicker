mkdir build
cd build
cl /I..\include /O2 /LD /Fetsclicker.dll ..\src\*.cpp User32.lib
cd ..