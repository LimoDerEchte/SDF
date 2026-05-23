cmake_minimum_required(VERSION 4.2)

set(CMAKE_CXX_STANDARD 23)

set(TARGET SN2CustomCraft)
project(${TARGET})

add_library(${TARGET} SHARED
        src/Mod.cpp
        src/registering/Hooks.cpp
        src/registering/RecipeFactory.cpp
        src/registering/CategoryFactory.cpp
        src/util/Log.hpp

        ../libs/CppSDK/SDK/Basic.cpp
        ../libs/CppSDK/SDK/CoreUObject_functions.cpp
        ../libs/CppSDK/SDK/Engine_functions.cpp
)

target_link_libraries(${TARGET} PUBLIC UE4SS)
target_include_directories(${TARGET} PRIVATE src ${SDK_INCLUDE} ${HEADER_ONLY_INCLUDE})

message(STATUS "${CMAKE_CONFIGURATION_TYPES}")

add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${TARGET}>
        "C:/Program Files (x86)/Steam/steamapps/common/Subnautica2/Subnautica2/Binaries/Win64/ue4ss/Mods/${TARGET}/dlls/main.dll"
)
