project(IIG3D-BaseTP)

############################################
# Configure CMake and GCC flags
cmake_minimum_required(VERSION 2.8.9) # Minimal version compatible QT5
CMAKE_POLICY(SET CMP0043 NEW) # This will silence the Cmake Warning "Policy CMP0043 is not set"

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

find_package(Qt5 COMPONENTS OpenGL REQUIRED)
if (Qt5OpenGL_FOUND)
    message(STATUS "Found Qt ${Qt5OpenGL_VERSION}")
else (Qt5OpenGL_FOUND)
    message(EROR "Unable to find QTOpenGL library")
endif(Qt5OpenGL_FOUND)
find_package(OpenGL REQUIRED) # define OPENGL_LIBRARIES

#OpenGL deprecated on MacosX : remove compilation warning
add_definitions(-DGL_SILENCE_DEPRECATION)

################################################################################
# Define project private sources and headers
#
# the variable "folder_source" contains all .cpp files of this project
FILE(GLOB_RECURSE
    folder_source
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

FILE(GLOB_RECURSE
    folder_header
    ${CMAKE_SOURCE_DIR}/src/*.h

)

FILE(GLOB_RECURSE
    folder_ui
    ${CMAKE_SOURCE_DIR}/src/*.ui
)

include_directories(
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_SOURCE_DIR}/glm
)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})
SOURCE_GROUP("Shader Files" FILES ${folder_shader})

################################################################################
# Configure QT

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set( CMAKE_INCLUDE_CURRENT_DIR ON )


################################################################################
# Build target application

add_executable(${PROJECT_NAME}
               ${folder_source}
               ${folder_header}
               ${folder_ui}
               )

set(EXT_LIBS Qt5::OpenGL ${OPENGL_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${EXT_LIBS} )
