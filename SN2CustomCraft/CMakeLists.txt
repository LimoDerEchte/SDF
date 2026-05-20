cmake_minimum_required(VERSION 4.2)

set(TARGET SN2CustomCraft)
project(${TARGET})

add_library(${TARGET} SHARED
        "src/Mod.cpp"
        src/registering/Hooks.cpp
        src/registering/Hooks.hpp
)

target_link_libraries(${TARGET} PUBLIC UE4SS)
target_include_directories(${TARGET} PRIVATE . ${SDK_INCLUDE})

message(STATUS "${CMAKE_CONFIGURATION_TYPES}")

add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${TARGET}>
        "C:/Program Files (x86)/Steam/steamapps/common/Subnautica2/Subnautica2/Binaries/Win64/ue4ss/Mods/${TARGET}/dlls/main.dll"
)
