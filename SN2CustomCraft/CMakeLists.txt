cmake_minimum_required(VERSION 4.2)

set(CMAKE_CXX_STANDARD 23)

option(DEVELOPMENT "Enable Development Mode" OFF)

set(TARGET SN2CustomCraft)
project(${TARGET})

add_library(${TARGET} SHARED
        src/Mod.cpp

        src/registering/Hooks.cpp
        src/registering/RecipeFactory.cpp
        src/registering/ItemTypeFactory.cpp
        src/registering/BuilderActionFactory.cpp
        src/registering/CategoryFactory.cpp
        src/registering/DatabankEntryFactory.cpp
        src/registering/StoryGoalRuleFactory.cpp

        src/parsing/IconParser.cpp
        src/parsing/FileTraversal.cpp
        src/parsing/CategoryParser.cpp
        src/parsing/RecipeParser.cpp
        src/parsing/ItemTypeParser.cpp
        src/parsing/BuilderActionParser.cpp
        src/parsing/StoryGoalParser.cpp
        src/parsing/DatabankEntryParser.cpp

        src/util/RegistryHelper.cpp
        #src/util/IconBuilder.cpp
        src/util/Finders.cpp

        ../libs/SN2-SDK/CppSDK/SDK/Basic.cpp
        ../libs/SN2-SDK/CppSDK/SDK/CoreUObject_functions.cpp
        ../libs/SN2-SDK/CppSDK/SDK/Engine_functions.cpp
        ../libs/SN2-SDK/CppSDK/SDK/AssetRegistry_functions.cpp
        ../libs/SN2-SDK/CppSDK/SDK/UWEAssetRegistry_functions.cpp
        ../libs/SN2-SDK/CppSDK/SDK/Subnautica2_functions.cpp
)

target_link_libraries(${TARGET} PUBLIC UE4SS)
target_include_directories(${TARGET} PRIVATE src include ${SDK_INCLUDE} ${HEADER_ONLY_INCLUDE})

if(DEVELOPMENT)
    target_compile_definitions(${TARGET} PRIVATE DEVELOPMENT)
    message(STATUS "Development Mode Is ENABLED")
else()
    message(STATUS "Development Mode Is DISABLED")
endif()
