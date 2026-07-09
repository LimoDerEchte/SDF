//
// Created by limo on 7/8/26.
//

#pragma once

#include "SDK/GameplayTags_structs.hpp"

class TempFinders {
    static SDK::UObject* FindDefaultObjectInternal(const std::string &package, const std::string &clazz);

public:
    static SDK::UClass* FindClass(const std::string &package, const std::string &clazz);
    static SDK::UFunction* FindFunction(const std::string &package, const std::string &clazz, const std::string &function);

    template<class T>
    static T* FindDefaultObject(const std::string &package, const std::string &clazz) {
        return static_cast<T*>(FindDefaultObjectInternal(package, clazz));
    }

    static SDK::UWorld* TryGetWorld();
    static SDK::UTexture2D* ImportFileAsTexture2D(const std::string &fileName);
    static SDK::FGameplayTag CreateTag(const std::string &name);
};
