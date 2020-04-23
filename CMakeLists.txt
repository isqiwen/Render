cmake_minimum_required(VERSION 3.8)
project(Render VERSION 1.0.0 LANGUAGES CXX)

find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)

# static library
add_executable(Render src/main.cpp)
# target_sources(Render PRIVATE src/main.cpp)  # add your cpp files here
target_compile_features(Render PUBLIC cxx_std_17)
target_link_libraries(Render PRIVATE imgpp opencv_core opencv_highgui opencv_imgproc)

