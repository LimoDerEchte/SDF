cmake_minimum_required(VERSION 4.2)

set(TARGET SN2CustomCraft)
project(${TARGET})

add_library(${TARGET} SHARED
        "src/Mod.cpp"
)
target_include_directories(${TARGET} PRIVATE .)

target_link_libraries(${TARGET} PRIVATE UE4SS SDK)

message(STATUS "${CMAKE_CONFIGURATION_TYPES}")
