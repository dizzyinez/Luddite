@echo off
cls
cmake -H. -Bbuild -G "MinGW Makefiles" -DPROJECT_NAME="Luddite"
cd build
compile.bat
cd ..