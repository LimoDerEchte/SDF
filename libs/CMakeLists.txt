
set(UE4SS_VERSION_CHECK OFF)
add_subdirectory(RE-UE4SS)

add_library(SDK SHARED
        "CppSDK/SDK/ABP_AnemoneCrab_functions.cpp"
)
target_include_directories(SDK PUBLIC CppSDK)
