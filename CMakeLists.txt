cmake_minimum_required(VERSION 4.2)

if(UNIX)
    set(CMAKE_TOOLCHAIN_FILE libs/RE-UE4SS/cmake/toolchains/xwin-clang-cl-toolchain.cmake)
endif()

project(SN2CustomCraft)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(libs)
add_subdirectory(SN2CustomCraft)
