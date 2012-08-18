#ifndef Corrade_Containers_EnumSet_h
#define Corrade_Containers_EnumSet_h
/*
    Copyright © 2007, 2008, 2009, 2010, 2011, 2012
              Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Corrade.

    Corrade is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Corrade is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Class Corrade::Containers::EnumSet
 */

namespace Corrade { namespace Containers {

/**
@brief Set of enum values
@tparam T Enum type
@tparam U Underlying type of the enum

Provides strongly-typed set-like functionality for strongly typed enums, such
as binary OR and AND operations. The only requirement for enum type is that
all the values must be binary exclusive.

Desired usage is via typedef'ing and then calling SET_OPERATORS() macro with
the resulting type as parameter to have all the operators implemented.
@code
enum class State: unsigned char {
    Ready = 1 << 0,
    Waiting = 1 << 1,
    Done = 1 << 2
};

typedef EnumSet<State, unsigned char> States;
SET_OPERATORS(States)
@endcode
*/
template<class T, class U> class EnumSet {
    public:
        typedef T Type;             /**< @brief Enum type */
        typedef U UnderlyingType;   /**< @brief Underlying type of the enum */

        /** @brief Create empty set */
        inline constexpr EnumSet(): value() {}

        /** @brief Create set from one value */
        inline constexpr EnumSet(T value): value(static_cast<UnderlyingType>(value)) {}

        /** @brief Union of two sets */
        inline constexpr EnumSet<T, U> operator|(EnumSet<T, U> other) const {
            return EnumSet<T, U>(value | other.value);
        }

        /** @brief Union two sets and assign */
        inline EnumSet<T, U>& operator|=(EnumSet<T, U> other) {
            value |= other.value;
            return *this;
        }

        /** @brief Intersection of two sets */
        inline constexpr EnumSet<T, U> operator&(EnumSet<T, U> other) const {
            return EnumSet<T, U>(value & other.value);
        }

        /** @brief Intersect two sets and assign */
        inline EnumSet<T, U>& operator&=(EnumSet<T, U> other) {
            value &= other.value;
            return *this;
        }

        /** @brief EnumSet complement */
        inline constexpr EnumSet<T, U> operator~() const {
            return EnumSet<T, U>(~value);
        }

        /** @brief Value as boolean */
        inline constexpr explicit operator bool() const {
            return value == 0;
        }

        /** @brief Value in underlying type */
        inline constexpr explicit operator UnderlyingType() const {
            return value;
        }

    private:
        inline constexpr explicit EnumSet(UnderlyingType type): value(type) {}

        UnderlyingType value;
};

/** @hideinitializer
@brief Define out-of-class operators for given EnumSet implementation
*/
#define SET_OPERATORS(class)                                                \
    inline constexpr class operator|(class::Type a, class b) {              \
        return b | a;                                                       \
    }                                                                       \
    inline constexpr class operator&(class::Type a, class b) {              \
        return b & a;                                                       \
    }

}}

#endif