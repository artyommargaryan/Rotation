cmake_minimum_required(VERSION 3.25)
project(CardRotation)

set(CMAKE_CXX_STANDARD 17)
set(OpenCV_DIR "/home/artyommargaryan/installation/OpenCV-master/installation/OpenCV-master/lib64/cmake/opencv4/")

find_package(OpenCV REQUIRED)

add_executable(CardRotation main.cpp)

target_link_libraries(CardRotation ${OpenCV_LIBS})