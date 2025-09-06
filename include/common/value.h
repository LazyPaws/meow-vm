#pragma once

#include "common/pch.h"

namespace meow::common {
    // Objects
    struct ObjBytes;
    struct ObjString;
    struct ObjArray;
    struct ObjHash;

    struct ObjModule;
    struct ObjFunctionProto;

    // Value types
    using Null = std::monostate;
    using Int = int64_t;
    using Float = double;
    using Bool = bool;

    using Bytes = ObjBytes*;
    using String = ObjString*;
    using Array = ObjArray*;
    using Object = ObjHash*;

    using Module = ObjModule*;
    using Proto = ObjFunctionProto*;

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

    template<class... Ts> 
    struct overloaded : Ts... { 
        using Ts::operator()...; 
    };

    template<class... Ts> 
    overloaded(Ts...) -> overloaded<Ts...>;

    struct Value: BaseValue {
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