// SPDX-License-Identifier: MIT
/**
 * @file value.h
 * @author lazypaws
 * @brief Defines all values in MeowScript
 * @copyright Copyright(c) 2025 LazyPaws
 */

#pragma once

#include "common/pch.h"

namespace meow::common {
    // Forward declarations for heap-allocated objects
    struct ObjBytes;
    struct ObjString;
    struct ObjArray;
    struct ObjHash;
    struct ObjModule;
    struct ObjProto;

    /**
     * @name Primitive value types
     */
    using Null = std::monostate;    // Null type
    using Int = int64_t;            // Integer type
    using Float = double;           // Floating-point type
    using Bool = bool;              // Boolean type

    /**
     * @name Object references
     * @note All managed by GC
     */
    using Bytes = ObjBytes*;
    using String = ObjString*;
    using Array = ObjArray*;
    using Object = ObjHash*;
    using Module = ObjModule*;
    using Proto = ObjProto*;

    /**
     * @brief Union for all supported types
     */
    using BaseValue = std::variant<
        Null,
        Int,
        Float,
        Bool,
        Bytes,
        String,
        Array,
        Object,
        Module,
        Proto
    >;

    /**
     * @brief Helper for std::visit with mutiple lambdas
     * @tparam Ts A parameter pack of lambda types
     */
    template <typename... Ts>
    struct overloaded : Ts... {
        using Ts::operator()...;
    };
    template <typename... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    /**
     * @struct Value
     * @brief Main definition of Value in MeowScript
     * @details Inherits from BaseValue, provides multiple ultilities
     */
    struct Value : BaseValue {
        /**
         * @brief Default constructor for Value
         * @details Initializes Value with null value
         */
        Value() : BaseValue(Null{}) {}

        /**
         * @brief Constructs an Value from an existing C++ value
         * @details Initializes the object by forwarding reference
         * @param[in] t The value to copy from
         * @tparam T The type of the value to initialize with
         */
        template <typename T>
        Value(T&& t) : BaseValue(std::forward<T>(t)) {}

        /**
         * @brief Gets the value of a specific type
         * @tparam T The type of the value
         * @return The read-only held value
         */
        template <typename T>
        const T& get() const {
            return std::get<T>(*this);
        }

        /**
         * @brief Gets the value of a specific type
         * @tparam T The type of the value
         * @return The mutable held value
         */
        template <typename T>
        T& get() {
            return std::get<T>(*this);
        }

        /**
         * @brief Get a pointer to the value if the type matches
         * @tparam T The type of the value
         * @return The read-only pointer to heild value, or nullptr if the type doesn't match
         */
        template <typename T>
        const T* get_if() const {
            return std::get_if<T>(this);
        }

        /**
         * @brief Get a pointer to the value if the type matches
         * @tparam T The type of the value
         * @return The mutable pointer to heild value, or nullptr if the type doesn't match
         */
        template <typename T>
        T* get_if() {
            return std::get_if<T>(this);
        }


        /**
         * @brief Checks if the Value holds a specific type
         * @tparam T The type of the value
         * @return 'true' if the type matchs, 'false' otherwise
         */
        template <typename T>
        bool is() const {
            return std::holds_alternative<T>(*this);
        }

        /**
         * @brief Visits the held value with a set of lambdas
         * @param[in] ts A parameter pack of callable object
         * @return The return value of matched lambda
         */
        template <typename... Ts>
        decltype(auto) visit(Ts&&... ts) {
            return std::visit(overloaded{std::forward<Ts>(ts)...}, *this);
        }

        /**
         * @brief Visits the held value with a set of lambdas
         * @param[in] ts A parameter pack of callable object
         * @return The return value of matched lambda but it's read-only
         */
        template <typename... Ts>
        decltype(auto) visit(Ts&&... ts) const {
            return std::visit(overloaded{std::forward<Ts>(ts)...}, *this);
        }

        /**
         * @brief Casts value to int64_t
         * @return Casted integer value. What do you want more?
         */
        int64_t asInt() const;

        /**
         * @brief Casts value to double
         * @return Casted floating-point value
         */
        double asFloat() const;

        /**
         * @brief Casts value to bool
         * @return Casted boolean value
         */
        bool asBool() const;

        /**
         * @brief Casts value to std::string
         * @return Casted string value
         */
        std::string asString() const;
    };
}