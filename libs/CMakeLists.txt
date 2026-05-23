
add_subdirectory(RE-UE4SS)

file(GLOB_RECURSE SDK_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/CppSDK/SDK/*.cpp
)

add_library(SDK SHARED
        ${SDK_SOURCES}
)
target_include_directories(SDK PUBLIC CppSDK)
target_compile_options(SDK PRIVATE /Zi)
target_link_options(SDK PRIVATE /DEBUG)

set(HEADER_ONLY_INCLUDE
        "${CMAKE_CURRENT_SOURCE_DIR}/tomlplusplus/include"
PARENT_SCOPE)

set(SDK_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/CppSDK" PARENT_SCOPE)
set(SDK_COMPILE
        "${CMAKE_CURRENT_SOURCE_DIR}/CppSDK/SDK/Basic.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/CppSDK/SDK/CoreUObject_functions.cpp"
PARENT_SCOPE)
