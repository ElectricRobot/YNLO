cmake_minimuM_required(VERSION 2.8)
project(YNLO)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native ")

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
${PROJECT_SOURCE_DIR}/YNLO
${PROJECT_SOURCE_DIR}/YNLO/cv
${PROJECT_SOURCE_DIR}/YNLO/slam
${OpenCV_INCLUDE_DIRS}
)

# Build shared library
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_library(${PROJECT_NAME} SHARED
YNLO/Except.cpp
YNLO/Pipeline.cpp
YNLO/UdpSocket.cpp
YNLO/util.cpp
YNLO/yaml.cpp
YNLO/cv/ORBExtractor.cpp
YNLO/slam/eurocloader.cpp
YNLO/slam/kittiloader.cpp
)

target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
)

# Build examples
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(odometry
examples/odometry/main.cpp)
target_link_libraries(odometry ${PROJECT_NAME})
