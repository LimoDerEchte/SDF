//
// Created by Limo on 28/05/2026.
//

#pragma once

#include <vector>
#include <type_traits>

#include "UnrealDef.hpp"

class BufferBuilder {
    std::vector<uint8_t> buffer;

public:
    BufferBuilder() = default;

    explicit BufferBuilder(const size_t reserveSize) {
        buffer.reserve(reserveSize);
    }

    template<typename T>
    void add(T value) {
        static_assert(std::is_integral_v<T>, "Integral type required");

        for (size_t i = 0; i < sizeof(T); i++) {
            buffer.push_back(static_cast<uint8_t>(value >> (i * 8)));
        }
    }

    std::vector<uint8_t> build() {
        return buffer;
    }

    [[nodiscard]] UC::TArray<UC::uint8> buildTArray() const {
        RC::Unreal::TArray<uint8_t> array{};
        array.ResizeTo(static_cast<int32_t>(buffer.size()));

        for (const auto& value : buffer) {
            array.Add(value);
        }
        return *reinterpret_cast<UC::TArray<UC::uint8>*>(&array);
    }
};
