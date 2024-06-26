@echo off

pushd
cd %~dp0

if not exist "bin/win32" ( mkdir bin/win32 )

pip install pyinstaller
pyinstaller --onefile cli.py --distpath=bin/win32 --name=cli

del cli.spec

popd