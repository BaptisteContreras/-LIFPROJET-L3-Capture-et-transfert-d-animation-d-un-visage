cmake_minimum_required(VERSION 3.6)
# Replace VisionTest with your project name
project(AM1)
#set(OpenCV_DIR /home/baptiste/Documents/opencv-3.4.5/build)
find_package(OpenCV REQUIRED)
find_package(Qt5Widgets REQUIRED)
# find_package(dlib REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(GLOB SOURCES *.h *.cpp train/*.h train/*.cpp structs/*.h)
# This is to compile main.cpp into an executable named vision_test
add_executable(AM1 main.cpp ${SOURCES})
target_link_libraries(AM1 ${OpenCV_LIBS})
target_link_libraries(AM1 opencv_face)
target_link_libraries(AM1 Qt5::Widgets Qt5::Core)

# target_link_libraries(AM1 dlib::dlib)

