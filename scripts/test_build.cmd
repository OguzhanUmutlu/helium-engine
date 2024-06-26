@echo off

python3 ./make_release.py
python3 ../releases/latest-dev/win32/helium.py ../example -g
..\example\bin\win32\example.exe