cmake_minimum_required(VERSION 3.26)
project(JumpGameTools)

set(CMAKE_CXX_STANDARD 17)

add_executable(JumpGameTools plus.cpp)

target_include_directories(JumpGameTools PRIVATE
        ${PROJECT_SOURCE_DIR}/easyx4mingw/include
)
target_link_libraries(JumpGameTools PRIVATE
        ${PROJECT_SOURCE_DIR}/easyx4mingw/lib64/libeasyx.a
)