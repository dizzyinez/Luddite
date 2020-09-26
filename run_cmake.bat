@echo off
cls
cmake -H. -Bbuild -G "MinGW Makefiles"  ^
-DGLFW_LIB_DIR="R:\Dev\Libraries\C++\glfw-3.3.2.bin.WIN32\lib-mingw" -DGLFW_INCLUDE_DIR="R:\Dev\Libraries\C++\glfw-3.3.2.bin.WIN32\include" ^
-DGLEW_LIB_DIR="R:\Dev\Libraries\C++\glew-2.1.0\lib" -DGLEW_INCLUDE_DIR="R:\Dev\Libraries\C++\glew-2.1.0\include" ^
-DGLM_INCLUDE_DIR="R:\Dev\Libraries\C++\glm" ^
-DLUA_LIB_DIR="R:\Dev\Libraries\C++\lua-5.4.0"
cd build
compile_pause.bat
cd ..
pause