//
// Created by sunny on 2022-11-27.
//

#pragma once


#include "internal_common.h"

namespace imgui_details {
    template<typename T, typename Tag>
    struct SafeType {
        SafeType(T t) : m_Value(std::move(t)) { }
        SafeType(const SafeType &) = default;
        template<typename T2, typename Tag2>
        SafeType(const SafeType<
                typename std::enable_if<!std::is_same<T, T2>::value, T2>::type,
                typename std::enable_if<!std::is_same<Tag, Tag2>::value, Tag2>::type
                > &) = delete;
        SafeType &operator=(const SafeType &) = default;
        explicit operator T() const { return Get(); }
        T Get() const { return m_Value; }
    private:
        T m_Value;
    };

    template<typename Tag>
    struct SafePointerType : SafeType<uintptr_t, Tag> {
        static const Tag Invalid;
        using SafeType<uintptr_t, Tag>::SafeType;
        SafePointerType() : SafePointerType(Invalid) {}
        template<typename T = void>
        explicit SafePointerType(T *ptr): SafePointerType(reinterpret_cast<uintptr_t>(ptr)) {}
        template<typename T = void>
        T *AsPointer() const { return reinterpret_cast<T *>(this->Get()); }
        explicit operator bool() const { return *this != Invalid; }
    };

    template<typename Tag>
    const Tag SafePointerType<Tag>::Invalid = {0};

    template<typename Tag>
    inline bool operator==(const SafePointerType<Tag> &lhs, const SafePointerType<Tag> &rhs) {
        return lhs.Get() == rhs.Get();
    }
    template<typename Tag>
    inline bool operator!=(const SafePointerType<Tag> &lhs, const SafePointerType<Tag> &rhs) {
        return lhs.Get() != rhs.Get();
    }
}
