//
// Created by limo on 7/8/26.
//

#pragma once

#include "FProperty.hpp"
#include "UObject.hpp"

#define ClassBase(Package, Name) \
public: \
    static UClass* StaticClass() { \
        const auto found = UObjectGlobals::StaticFindObject(nullptr, nullptr, L"/Script/"#Package"."#Name); \
        return reinterpret_cast<UClass*>(found); \
    } \
    static U##Name* DefaultObject() { \
        const auto found = UObjectGlobals::StaticFindObject(nullptr, nullptr, L"/Script/"#Package".Default__"#Name); \
        return reinterpret_cast<U##Name*>(found); \
    } \
    static UFunction* StaticFunction(const std::wstring &name) { \
        const auto found = UObjectGlobals::StaticFindObject(nullptr, nullptr, L"/Script/"#Package"."#Name":" + name); \
        return reinterpret_cast<UFunction*>(found); \
    } \
    \
    bool Is##Name() { \
        /* TODO: Type and Null Check */ \
    } \

#define ClassProperty(Name, T) \
    T* Get##Name() { \
        return GetPropertyByName(L#Name)->ContainerPtrToValuePtr<T>(this); \
    } \
    \
    void Set##Name(T value) { \
        *Get##Name() = value; \
    }

#define ClassFunctionRet0(Name, T) \
    struct Name##_Params { \
        T ReturnValue; \
    }; \
    \
    T Invoke##Name() { \
        Name##_Params params {}; \
        ProcessEvent(GetFunctionByName(L#Name), &params); \
        return params.ReturnValue; \
    }


#define ClassFunctionRet1(Name, T, T1, N1) \
    struct Name##_Params { \
        T1 n1; \
        T ReturnValue; \
    }; \
    \
    T Invoke##Name(T1 N1) { \
        Name##_Params params { \
            .n1 = N1, \
        }; \
        ProcessEvent(GetFunctionByName(L#Name), &params); \
        return params.ReturnValue; \
    }


#define ClassFunctionRet2(Name, T, T1, N1, T2, N2) \
    struct Name##_Params { \
        T1 n1; \
        T2 n2; \
        T ReturnValue; \
    }; \
    \
    T Invoke##Name(T1 N1, T2 N2) { \
        Name##_Params params { \
            .n1 = N1, \
            .n2 = N2, \
        }; \
        ProcessEvent(GetFunctionByName(L#Name), &params); \
        return params.ReturnValue; \
    }
