@echo off

mkdir package
del /F /Q package\*
cd package
cls

copy ..\package.ini .
mkdir plugins
mkdir plugins\tsclicker
copy ..\build\tsclicker.dll .\plugins
copy ..\test_clicks.tsc .\plugins\tsclicker

cd ..