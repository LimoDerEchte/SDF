cmake_minimum_required(VERSION 4.2)

if(UNIX)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/libs/RE-UE4SS/cmake/toolchains/xwin-clang-cl-toolchain.cmake)
    # add_compile_options("-w") # Removes Warnings, Recommended To Keep On Cuz Thousands Of Warnings Come Up From The SDK
    add_compile_options("/clang:-Wno-c++11-narrowing") # Removes Some Enum Issues
    add_compile_options("/clang:-Wno-narrowing") # Same As Above But Above Sometimes Doesn't Work IDK Why
endif()

project(LimoMods)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(libs)
add_subdirectory(SN2CustomCraft)
