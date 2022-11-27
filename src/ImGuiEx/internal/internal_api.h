//
// Created by sunny on 2022-11-27.
//

#pragma once

#include "internal_common.h"
#include "object_config.h"

namespace ImGuiEx::api {
    //==================== Template defines for SafeType ====================//

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

    //<========== End Template defines for SafeType.

    struct NodeId final : SafePointerType<NodeId> {
        using SafePointerType::SafePointerType;
    };

    struct LinkId final : SafePointerType<LinkId> {
        using SafePointerType::SafePointerType;
    };

    struct PinId final : SafePointerType<PinId> {
        using SafePointerType::SafePointerType;
    };

}

