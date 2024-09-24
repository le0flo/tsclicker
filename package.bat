@echo off

mkdir package
del /F /Q package\*
cd package

copy ..\package.ini .
mkdir plugins
mkdir plugins\tsclicker
copy ..\build\tsclicker.dll .\plugins
copy ..\test\* .\plugins\tsclicker

tar -a -c -f package.zip plugins package.ini
move package.zip tsclicker.ts3_plugin

cd ..
