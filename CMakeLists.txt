cmake_minimum_required (VERSION 3.10)
project (Luddite VERSION 0.1)

configure_file(include/Config.h.in ${CMAKE_SOURCE_DIR}/include/Config.h)

include_directories(include)

SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SET(CMAKE_MODULE_LINKER_FLAGS "-m32 -Wl,--disable-stdcall-fixup ${CMAKE_SHARED_LINKER_FLAGS}")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES "src/*.c" "src/*.cc" "src/*.cpp")
file(GLOB_RECURSE INCS_H "include/*.h" "include/*.hpp")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${INCS_H})
#set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

include_directories(
"R:/Dev/Libraries/C++/glfw-3.3.2.bin.WIN32/include"
"R:/Dev/Libraries/C++/glew-2.1.0/include"
"R:/Dev/Libraries/C++/glm"
"R:/Dev/Libraries/C++/lua-5.4.1/include"
"R:/Dev/Libraries/C++/freetype-2.10.0/include"
"C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/x86_64-w64-mingw32/include"
"R:/Dev/Libraries/C++/enet-1.3.16/include"
"R:/Dev/Libraries/C++/openal-soft/include"
"R:/Dev/Libraries/C++/dr_libs/include"
"R:/Dev/Libraries/C++/stb"
"R:/Dev/Libraries/C++/poisson-disk-sampling/thinks/poisson_disk_sampling"
"R:/Dev/Libraries/C++/delaunator-cpp/include"
"R:/Dev/Libraries/C++/jc_voronoi/include"
)

macro(find_library_wrapper)          
    find_library(${ARGV0} ${ARGV1})    
    if (NOT ${ARGV0})
        message(FATAL_ERROR "-- couldn't find library ${ARGV1}")
    endif(NOT ${ARGV0})
	if (DEFINED CMAKEDEBUG)
	endif()
    message("-- Library ${ARGV1} found as ${${ARGV0}}")   
endmacro()

SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} 
"R:/Dev/Libraries/C++/glfw-3.3.2/Lib"
"R:/Dev/Libraries/C++/glew-2.1.0/lib"
"R:/Dev/Libraries/C++/lua-5.4.1/lib"
"R:/Dev/Libraries/C++/freetype-2.10.0/lib"
"C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/x86_64-w64-mingw32/lib"
"R:/Dev/Libraries/C++/enet-1.3.16/Lib"
"R:/Dev/Libraries/C++/openal-soft/build/Debug"
)

find_library_wrapper(glew libglew32.dll)
find_library_wrapper(GLFW glfw3)
find_library_wrapper(glu glu32)
find_library_wrapper(opengl opengl32)
find_library_wrapper(lua liblua)
find_library_wrapper(freetype libfreetype)
find_library_wrapper(enet libenet.a)
find_library_wrapper(winsock ws2_32)
find_library_wrapper(winmedia winmm)
find_library_wrapper(OpenAL OpenAL32)


target_link_libraries(${PROJECT_NAME}
${GLFW}
${glew}
${opengl}
${glu}
${lua}
${freetype}
${enet}
${winsock}
${winmedia}
${OpenAL}
)