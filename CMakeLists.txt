
# Base Setup
if(UNIX)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/libs/RE-UE4SS/cmake/toolchains/xwin-clang-cl-toolchain.cmake)
    add_compile_options("/clang:-Wno-c++11-narrowing")
    add_compile_options("/clang:-Wno-narrowing")
endif()
add_compile_options("-w")

cmake_minimum_required(VERSION 4.2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET SDF)
project(${TARGET})

add_subdirectory(libs)


# Pre-Build
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/config.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/config.cmake")
endif()

if(NOT SN2_LOCATION)
    set(SN2_LOCATION "$ENV{SN2_LOCATION}")
    if(NOT SN2_LOCATION)
        message(FATAL_ERROR "SN2_LOCATION variable is not set!")
    endif()
endif()
message(VERBOSE "SN2_LOCATION is set to ${SN2_LOCATION}")


# SDF Project
add_library(${TARGET} SHARED
        src/Mod.cpp

        src/registering/Hooks.cpp
        src/registering/RecipeFactory.cpp
        src/registering/ItemTypeFactory.cpp
        src/registering/BuilderActionFactory.cpp
        src/registering/CategoryFactory.cpp
        src/registering/DatabankEntryFactory.cpp
        src/registering/StoryGoalRuleFactory.cpp
        src/registering/BioModFactory.cpp

        src/parsing/IconParser.cpp
        src/parsing/FileTraversal.cpp
        src/parsing/CategoryParser.cpp
        src/parsing/RecipeParser.cpp
        src/parsing/ItemTypeParser.cpp
        src/parsing/BuilderActionParser.cpp
        src/parsing/StoryGoalParser.cpp
        src/parsing/DatabankEntryParser.cpp

        src/util/RegistryHelper.cpp
        # src/util/IconBuilder.cpp
        src/util/Finders.cpp

        libs/CppSDK/SDK/Basic.cpp
        libs/CppSDK/SDK/CoreUObject_functions.cpp
        libs/CppSDK/SDK/Engine_functions.cpp
        libs/CppSDK/SDK/AssetRegistry_functions.cpp
        libs/CppSDK/SDK/UWEAssetRegistry_functions.cpp
        libs/CppSDK/SDK/Subnautica2_functions.cpp
)

target_link_libraries(${TARGET} PUBLIC UE4SS)
target_include_directories(${TARGET} PRIVATE src include ${SDK_INCLUDE} ${HEADER_ONLY_INCLUDE})

target_compile_definitions(${TARGET} PRIVATE DEVELOPMENT)

message(STATUS "${CMAKE_CONFIGURATION_TYPES}")


# Post-Build
add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${TARGET}>
        "${SN2_LOCATION}/Subnautica2/Binaries/Win64/ue4ss/Mods/${TARGET}/dlls/main.dll"
)
