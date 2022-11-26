/*
 * Author  : SunXin
 * Modify  : 2022-11-26
 * Version : 1.0.0.0
 * Content :
 *   1.First initialize.
 */

#pragma once

# include <cstdint> // std::uintXX_t
# include <utility> // std::move
# include <cstdio> // snprintf
# include <string>
# include <fstream>
# include <climits>
# include <algorithm>
# include <sstream>
# include <streambuf>
# include <type_traits>

//------------------------------------------------------------------------------
// https://stackoverflow.com/a/36079786
# define DECLARE_HAS_MEMBER(__trait_name__, __member_name__)                         \
    template <typename __boost_has_member_T__>                                       \
    struct __trait_name__ {                                                          \
        private:                                                                     \
        using check_type = ::std::remove_const_t<__boost_has_member_T__>;            \
        struct no_type {char x[2];};                                                 \
        using  yes_type = char;                                                      \
        struct  base { void __member_name__() {}};                                   \
        struct mixin : public base, public check_type {};                            \
        template <void (base::*)()> struct aux {};                                   \
        template <typename U> static no_type  test(aux<&U::__member_name__>*);       \
        template <typename U> static yes_type test(...);                             \
        public:                                                                      \
        static constexpr bool value = (sizeof(yes_type) == sizeof(test<mixin>(0)));  \
    }

// https://stackoverflow.com/a/8597498
# define DECLARE_HAS_NESTED(Name, Member)                                          \
    template<class T>                                                              \
    struct has_nested_##Name {                                                    \
        typedef char yes;                                                          \
        typedef yes(&no)[2];                                                       \
        template<class U> static yes test(decltype(U::Member)*);                   \
        template<class U> static no  test(...);                                    \
        static bool const value = sizeof(test<T>(0)) == sizeof(yes);               \
    };

# define DECLARE_KEY_TESTER(Key)                                                                    \
    DECLARE_HAS_NESTED(Key, Key)                                                                    \
    struct KeyTester_##Key {                                                                       \
        template <typename T>                                                                       \
        static int Get(typename std::enable_if<has_nested_##Key<ImGuiKey>::value, T>::type*) {    \
            return ImGui::GetKeyIndex(T::Key);                                                      \
        }                                                                                           \
        template <typename T>                                                                       \
        static int Get(typename std::enable_if<!has_nested_##Key<ImGuiKey>::value, T>::type*) {    \
            return -1;                                                                              \
        }                                                                                           \
    }

