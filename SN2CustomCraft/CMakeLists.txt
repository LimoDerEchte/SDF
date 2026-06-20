cmake_minimum_required(VERSION 4.2)

set(CMAKE_CXX_STANDARD 23)

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

        src/util/RegistryHelper.cpp
        #src/util/IconBuilder.cpp
        src/util/Finders.cpp

        ../libs/CppSDK/SDK/Basic.cpp
        ../libs/CppSDK/SDK/CoreUObject_functions.cpp
        ../libs/CppSDK/SDK/Engine_functions.cpp
        ../libs/CppSDK/SDK/AssetRegistry_functions.cpp
        ../libs/CppSDK/SDK/UWEAssetRegistry_functions.cpp
        ../libs/CppSDK/SDK/Subnautica2_functions.cpp
)

target_link_libraries(${TARGET} PUBLIC UE4SS)
target_include_directories(${TARGET} PRIVATE src include ${SDK_INCLUDE} ${HEADER_ONLY_INCLUDE})

target_compile_definitions(${TARGET} PRIVATE DEVELOPMENT)

message(STATUS "${CMAKE_CONFIGURATION_TYPES}")

add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${TARGET}>
        "C:/Program Files (x86)/Steam/steamapps/common/Subnautica2/Subnautica2/Binaries/Win64/ue4ss/Mods/${TARGET}/dlls/main.dll"
)
