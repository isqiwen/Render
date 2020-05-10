cmake_minimum_required(VERSION 3.8)
project(Render VERSION 1.0.0 LANGUAGES CXX)

find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)
find_package(nlohmann_json REQUIRED)

FILE(GLOB CodeSources src/*.cpp include/*.h)

# static library
add_executable(Render ${CodeSources})
target_compile_features(Render PUBLIC cxx_std_17)
target_link_libraries(Render PRIVATE imgpp opencv_core opencv_highgui opencv_imgproc PRIVATE nlohmann_json::nlohmann_json)

