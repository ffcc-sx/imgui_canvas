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

//------------------------------------------------------------------------------
// https://stackoverflow.com/a/36079786
# define DECLARE_HAS_MEMBER(__trait_name__, __member_name__)                         \
    template <typename __boost_has_member_T__>                                       \
    class __trait_name__ {                                                           \
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

