cmake_minimum_required (VERSION 3.7)
project (VideoGame)

include_directories(include)


SET(CMAKE_MODULE_LINKER_FLAGS "-m32 -Wl,--disable-stdcall-fixup ${CMAKE_SHARED_LINKER_FLAGS}")

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

if (NOT DEFINED GLFW_INCLUDE_DIR)
	message(WARNING "GLFW_INCLUDE_DIR is not set! Example: Add -DGLFW_INCLUDE_DIR=C:\\GLFW\\include")
else()
	if (DEFINED CMAKEDEBUG)
	message("GLFW_INCLUDE_DIR is set to ${GLFW_INCLUDE_DIR}")
	endif()
	include_directories(${GLFW_INCLUDE_DIR})
endif()

if (NOT DEFINED GLFW_LIB_DIR)
	message(WARNING "GLFW_LIB_DIR is not set! Example: Add -DGLFW_LIB_DIR=C:\\GLFW\\include\\lib")
else()
	if (DEFINED CMAKEDEBUG)
	message("GLFW_LIB_DIR is set to ${GLFW_LIB_DIR}")
	endif()
	SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${GLFW_LIB_DIR})	
endif()



if (NOT DEFINED GLEW_INCLUDE_DIR)
	message(WARNING "GLEW_INCLUDE_DIR is not set! Example: Add -DGLEW_INCLUDE_DIR=C:\\GLEW\\include")
else()
	if (DEFINED CMAKEDEBUG)
	message("GLEW_INCLUDE_DIR is set to ${GLEW_INCLUDE_DIR}")
	endif()
	include_directories(${GLEW_INCLUDE_DIR})
endif()

if (NOT DEFINED GLEW_LIB_DIR)
	message(WARNING "GLEW_LIB_DIR is not set! Example: Add -DGLEW_LIB_DIR=C:\\GLEW\\include\\lib")
else()
	if (DEFINED CMAKEDEBUG)
	message("GLEW_LIB_DIR is set to ${GLEW_LIB_DIR}")
	endif()
	SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${GLEW_LIB_DIR})	
endif()



if (NOT DEFINED GLM_INCLUDE_DIR)
	message(WARNING "GLM_INCLUDE_DIR is not set! Example: Add -DGLM_INCLUDE_DIR=C:\\GLEW\\include")
else()
	if (DEFINED CMAKEDEBUG)
	message("GLM_INCLUDE_DIR is set to ${GLM_INCLUDE_DIR}")
	endif()
	include_directories(${GLM_INCLUDE_DIR})
endif()



IF("${CMAKE_SYSTEM}" MATCHES "Linux" OR "${CMAKE_SYSTEM}" MATCHES "FreeBSD")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17 -z defs")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -Wall -z defs")
ELSEIF("${CMAKE_SYSTEM}" MATCHES "Windows")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -Wall")
ENDIF()

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_VERSION "0.0.1")

file(GLOB_RECURSE SRC_FILES "src/*.c" "src/*.cc" "src/*.cpp")
file(GLOB_RECURSE INCS_H "include/*.h" "include/*.hpp")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${INCS_H})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

macro(my_find_library)          
    find_library(${ARGV0} ${ARGV1})    
    if (NOT ${ARGV0})
        message(FATAL_ERROR "-- could not find library ${ARGV1}")
    endif(NOT ${ARGV0})
	if (DEFINED CMAKEDEBUG)
	endif()
    message("-- Library ${ARGV1} found as ${${ARGV0}}")   
endmacro()

my_find_library(glew glew32)
my_find_library(GLFW glfw3)
my_find_library(glu glu32)
my_find_library(opengl opengl32)
my_find_library(winsock ws2_32)
my_find_library(winmedia winmm)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
${GLFW}
${glew}
${opengl}
${glu}
${winsock}
${winmedia}
)

#[[
message(STATUS "Generarating ${CMAKE_SOURCE_DIR}/.clang_complete")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
file(WRITE ${CMAKE_SOURCE_DIR}/.clang_complete "")
foreach(dir ${dirs})
  file(APPEND ${CMAKE_SOURCE_DIR}/.clang_complete "-I${dir}\n")
endforeach()
string(REPLACE "'" "" CMAKE_C_FLAGS_SPLIT ${CMAKE_C_FLAGS})
string(REPLACE " " ";" CMAKE_C_FLAGS_SPLIT ${CMAKE_C_FLAGS_SPLIT})
foreach(flag ${CMAKE_C_FLAGS_SPLIT})
  file(APPEND ${CMAKE_SOURCE_DIR}/.clang_complete "${flag}\n")
endforeach()]]
