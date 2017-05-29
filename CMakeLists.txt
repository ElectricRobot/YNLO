cmake_minimuM_required(VERSION 2.8)
project(YNLO)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -march=native -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native -pthread")

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
if(COMPILER_SUPPORTS_CXX17)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
   add_definitions(-DCOMPILEDWITHC17)
   message(STATUS "Using flag -std=c++17.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
endif()

find_package(OpenCV 3.2 REQUIRED)

include_directories(
${PROJECT_SOURCE_DIR}
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/include/util
${PROJECT_SOURCE_DIR}/include/socket
${PROJECT_SOURCE_DIR}/include/container
${PROJECT_SOURCE_DIR}/include/cv
${PROJECT_SOURCE_DIR}/include/slam
${OpenCV_INCLUDE_DIRS}
)

# Build shared library
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_library(${PROJECT_NAME} SHARED
src/util/Except.cpp
src/util/Pipeline.cpp
src/socket/UdpSocket.cpp
src/util/util.cpp
src/util/yaml.cpp
src/cv/ORBExtractor.cpp
src/slam/eurocloader.cpp
src/slam/kittiloader.cpp
src/util/Trace.cpp
)

target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
)

# Build examples
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(pipeline
examples/pipeline/main.cpp)
target_link_libraries(pipeline ${PROJECT_NAME})
