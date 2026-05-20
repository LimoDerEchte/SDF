
add_subdirectory(RE-UE4SS)

add_library(SDK SHARED
        "CppSDK/SDK/Basic.cpp"
        "CppSDK/SDK/CoreUObject_functions.cpp"
        "CppSDK/SDK/WBP_RecipeCategory_functions.cpp"
)
target_include_directories(SDK PUBLIC CppSDK)

set(SDK_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/CppSDK" PARENT_SCOPE)
set(SDK_COMPILE
        "${CMAKE_CURRENT_SOURCE_DIR}/CppSDK/SDK/Basic.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/CppSDK/SDK/CoreUObject_functions.cpp"
PARENT_SCOPE)
