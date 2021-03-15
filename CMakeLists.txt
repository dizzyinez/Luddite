cmake_minimum_required (VERSION 3.10)
project (Luddite VERSION 0.200227)

configure_file(include/Config.h.in ${CMAKE_SOURCE_DIR}/include/Config.h)

include_directories(include)

SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SET(CMAKE_MODULE_LINKER_FLAGS "-m32 -Wl,--disable-stdcall-fixup ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES "src/*.c" "src/*.cc" "src/*.cpp")
file(GLOB_RECURSE INCS_H "include/*.h" "include/*.hpp")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${INCS_H})
#set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

macro(find_library_wrapper)          
    find_library(${ARGV0} ${ARGV1})    
    if (NOT ${ARGV0})
        message(FATAL_ERROR "-- couldn't find library ${ARGV1}")
    endif(NOT ${ARGV0})
    message("-- Library ${ARGV1} found as ${${ARGV0}}")   
endmacro()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    
include_directories(
    "R:/Dev/Libraries/C++/glfw-3.3.2.bin.WIN32/include"
    "R:/Dev/Libraries/C++/glew-2.1.0/include"
    "R:/Dev/Libraries/C++/glm"
    "R:/Dev/Libraries/C++/lua-5.4.1/include"
    "R:/Dev/Libraries/C++/freetype-2.10.0/include"
    "C:/msys64/mingw64/include"
    "C:/msys64/mingw64/x86_64-w64-mingw32/include"
    "R:/Dev/Libraries/C++/enet-1.3.16/include"
    "R:/Dev/Libraries/C++/openal-soft/include"
    "R:/Dev/Libraries/C++/dr_libs/include"
    "R:/Dev/Libraries/C++/stb"
    "R:/Dev/Libraries/C++/poisson-disk-sampling/"
    "R:/Dev/Libraries/C++/delaunator-cpp/include"
    "R:/Dev/Libraries/C++/jc_voronoi/include"
    "R:/Dev/Libraries/C++/soloud20200207/include"
    "R:/Dev/Libraries/C++/SDL2-2.0.10/include/SDL2"
    )
    
    SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} 
    "R:/Dev/Libraries/C++/glfw-3.3.2/Lib"
    "R:/Dev/Libraries/C++/glew-2.1.0/lib"
    "R:/Dev/Libraries/C++/lua-5.4.1/lib"
    "R:/Dev/Libraries/C++/freetype-2.10.0/lib"
    "C:/msys64/mingw64/lib"
    "C:/msys64/mingw64/x86_64-w64-mingw32/lib"
    "R:/Dev/Libraries/C++/enet-1.3.16/Lib"
    )
    
    find_library_wrapper(glew libglew32.dll)
    find_library_wrapper(GLFW glfw3)
    find_library_wrapper(glu libglu32)
    find_library_wrapper(opengl opengl32)
    find_library_wrapper(lua liblua)
    find_library_wrapper(freetype libfreetype)
    find_library_wrapper(enet libenet.a)
    find_library_wrapper(winsock ws2_32)
    # find_library_wrapper(winmedia winmm)
    
    
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
    )
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -W -lX11 -lXrandr -lXi -lXxf86vm -ldl -lm -lpthread -pthread")
    
include_directories(
    "/usr/include/"
    "/usr/include/freetype2/"
    "/usr/include/soloud/include/"
    )
    
    SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} 
    "/usr/lib64"
    "/usr/local/lib"
    "/usr/lib/x86_64-linux-gnu"
    )
    
    find_library_wrapper(glew libGLEW.so)
    find_library_wrapper(GLFW libglfw3.a)
    find_library_wrapper(opengl libOpenGL.so)
    find_library_wrapper(glu libGLU.so)
    find_library_wrapper(lua liblua.a)
    find_library_wrapper(freetype libfreetype.a)
    find_library_wrapper(enet libenet.a)
    find_library_wrapper(x11 X11)
    find_library_wrapper(png libpng.a)
    find_library_wrapper(zlib libz.so)
    find_library_wrapper(brotlicommon libbrotlicommon.so)
    find_library_wrapper(brotlidec libbrotlidec.so)
    find_library_wrapper(brotlienc libbrotlienc.so)
    
    
    target_link_libraries(${PROJECT_NAME}
    ${GLFW}
    ${glew}
    ${opengl}
    ${glu}
    ${lua}
    ${freetype}
    ${enet}
    ${x11}
    ${png}
    ${zlib}
    ${brotlicommon}
    ${brotlidec}
    ${CMAKE_DL_LIBS}
    )
endif()