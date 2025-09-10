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
     */
    template<class... Ts> 
    struct overloaded : Ts... { 
        using Ts::operator()...; 
    };
    template<class... Ts> 
    overloaded(Ts...) -> overloaded<Ts...>;

    /**
     * @struct Value
     * @brief Main definition of Value in MeowScript
     * @details Inherits from BaseValue, provides multiple ultilities
     */
    struct Value: BaseValue {
        /**
         * @brief Default constructor for Value
         * @details Initializes Value with null value
         */
        Value() : BaseValue(Null{}) {}

        template <typename T>
        Value(T&& t) : BaseValue(std::forward<T>(t)) {}

        template <typename T>
        const T& get() const {
            return std::get<T>(*this);
        }

        template <typename T>
        T& get() {
            return std::get<T>(*this);
        }

        template <typename T>
        const T* get_if() const {
            return std::get_if<T>(this);
        }

        template <typename T>
        T* get_if() {
            return std::get_if<T>(this);
        }

        template <typename T>
        bool is() const {
            return std::holds_alternative<T>(*this);
        }

        template <typename... Ts>
        decltype(auto) visit(Ts&&... ts) {
            return std::visit(overloaded{std::forward<Ts>(ts)...}, *this);
        }

        template <typename... Ts>
        decltype(auto) visit(Ts&&... ts) const {
            return std::visit(overloaded{std::forward<Ts>(ts)...}, *this);
        }

        int64_t asInt() const;
        double asFloat() const;
        bool asBool() const;
        std::string asString() const;

        // ... for all types    
    };
}