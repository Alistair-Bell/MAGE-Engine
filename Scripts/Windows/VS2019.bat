@echo off
pushd %~dp0\..\..\
call Externals\Windows\premake5.exe vs2019
popd
PAUSE