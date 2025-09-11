// SPDX-License-Identifier: MIT
/**
 * @file definitions.h
 * @author lazypaws
 * @brief Defines main values in MeowScript
 * @note Object values managed by Garbage Collector
 * @copyright Copyright(c) 2025 LazyPaws
 */

#pragma once

#include "common/value.h"
#include "memory/meow_object.h"
#include "memory/gc_visitor.h"
#include "pch.h"

namespace meow::runtime {
    struct Chunk;
}

namespace meow::common {
    // Defines base objects

    /**
     * @struct ObjBytes
     * @brief Represents an array of bytes in MeowScript
     * @details A wrapper around an \c std::vector<uint8_t> to represent a byte array
     */
    struct ObjBytes : meow::memory::MeowObject {
    private:
        std::vector<uint8_t> data;
    public:
        /**
         * @brief The default constructor for ObjBytes
         * @details Initializes an empty byte array
         */
        ObjBytes() : data() {}

        /**
         * @brief Constructs an ObjBytes from an existing array of bytes
         * @details Initializes the object by copying the data from provided byte array
         * @param[in] bytes The array of bytes to copy from
         */
        ObjBytes(const std::vector<uint8_t>& bytes): data(bytes) {}

        /**
         * @brief Gets the constant reference to byte array
         * @return The read-only array of bytes
         */
        const std::vector<uint8_t>& get() const {
            return data;
        }
        /**
         * @brief Gets the byte at specified index
         * @param[in] index The index of byte to retrieve
         * @return The read-only byte at specified index
         * @warning No bound checking
         */
        uint8_t get(size_t index) const {
            return data[index];
        }
        
        /**
         * @brief Sets the byte at specified index
         * @param[in] index The index of byte to set
         * @param[in] value The new byte to assign to the byte at index
         * @warning No bound checking
         */
        void set(size_t index, uint8_t value) {
            data[index] = value;
        }

        /** 
         * @brief Gets the number of byte in the array
         * @return Size of array of bytes
         */
        size_t size() const {
            return data.size();
        }

        /** 
         * @brief Checks if the byte array is empty
         * @return 'true' if the array is empty, 'false' otherwise
         */
        bool empty() const {
            return data.empty();
        }

        /** 
         * @brief Appends a byte to the end of the array 
         * @param[in] value The new byte to append to the end of the array
         */ 
        void push(uint8_t value) {
            data.push_back(value);
        }

        /** @brief Removes the last byte from the array */
        void pop() {
            data.pop_back();
        }

        /** 
         * @brief Reserves more capacity for the array 
         * @param[in] capacity The new capacity to reserve for the array
         */
        void reserve(size_t capacity) {
            data.reserve(capacity);
        }

        /**
         * @brief Gets an iterator to browse the array
         * @return An iterator to the beginning of the array
         */
        inline constexpr std::vector<uint8_t>::iterator begin() noexcept {
            return data.begin();
        }

        /**
         * @brief Gets an iterator to browse the array
         * @return An iterator to the end of the array
         */
        inline constexpr std::vector<uint8_t>::iterator end() noexcept {
            return data.end();
        }

        /**
         * @brief Trace all reachable MeowObjects for Garbage Collector
         * @details This method is one of the most important part of the Garbage Collector mechanism. It's called by GC to mark all of reachable MeowObjects
         * @param[in,out] visitor The Visitor that performs the tracing
         * @note This parameter is unused because ObjBytes holds no traceable objects
         * @see meow::memory::MeowObject::trace
         */
        void trace([[maybe_unused]] meow::memory::GCVisitor& visitor) override {}
    };

    /**
     * @struct ObjString
     * @brief Represents a string in MeowScript
     * @details A wrapper around an \c std::string to represent string
     */
    struct ObjString : meow::memory::MeowObject {
    private:
        std::string data;
    public:
        /**
         * @brief The default constructor for  ObjString
         * @details Initializes an empty string
         */
        ObjString() : data() {}

        /**
         * @brief Constructs an ObjString from an existing string
         * @details Initializes the object by copying the data from provided string
         * @param[in] str The string to copy from
         */
        ObjString(const std::string& str) : data(str) {}

        /**
         * @brief Gets the constant reference to string
         * @return The read-only string
         */
        const std::string& get() const {
            return data;
        }

        /**
         * @brief Gets the character at specified index
         * @param[in] index The index of character to retrieve
         * @return The read-only character at specified index
         */
        char get(size_t index) const {
            return data[index];
        }

        /** 
         * @brief Gets the number of character in the string
         * @return Size of string
         */
        size_t size() const {
            return data.size();
        }

        /** 
         * @brief Checks if the byte string is empty
         * @return 'true' if the string is empty, 'false' otherwise
         */
        bool empty() const {
            return data.empty();
        }

        /**
         * @brief Gets an iterator to browse the string
         * @return An iterator to the beginning of the string
         */
        inline constexpr std::string::iterator begin() noexcept {
            return data.begin();
        }

        /**
         * @brief Gets an iterator to browse the string
         * @return An iterator to the end of the string
         */
        inline constexpr std::string::iterator end() noexcept {
            return data.end();
        }

        /**
         * @brief Trace all reachable MeowObjects for Garbage Collector
         * @details This method is one of the most important part of the Garbage Collector mechanism. It's called by GC to mark all of reachable MeowObjects
         * @param[in,out] visitor The Visitor that performs the tracing
         * @note This parameter is unused because ObjString holds no traceable objects
         * @see meow::memory::MeowObject::trace
         */
        void trace([[maybe_unused]] meow::memory::GCVisitor& visitor) override {}
    };

    /**
     * @struct ObjArray
     * @brief Represents an array in MeowScript
     * @details A wrapper around an \c std::vector<Value> to represent an array
     */
    struct ObjArray : meow::memory::MeowObject {
    private:
        std::vector<Value> elements;
    public:

        /**
         * @brief The default constructor for ObjArray
         * @details Initializes an empty array
         */
        ObjArray(): elements() {}

        /**
         * @brief Constructs an ObjArray from an existing array
         * @details Initializes the object by copying the data from provided array
         * @param[in] vector The array to copy from
         */
        ObjArray(const std::vector<Value>& vector) : elements(vector) {}

        /**
         * @brief Gets the constant reference to array
         * @return The read-only array
         */
        const std::vector<Value>& get() const {
            return elements;
        }

        /**
         * @brief Gets the value at specified index
         * @param[in] index The index of value to retrieve
         * @return The read-only value at specified index
         * @warning No bound checking
         */
        const Value& get(size_t index) const {
            return elements[index];
        }

        /**
         * @brief Sets the value at specified index
         * @param[in] index The index of value to set
         * @param[in] value The new value to assign to the value at index
         * @warning No bound checking
         */
        void set(size_t index, const Value& value) {
            elements[index] = value;
        }

        /**
         * @brief Gets the number of value in the array
         * @return Size of array
         */
        size_t size() const {
            return elements.size();
        }

        /**
         * @brief Checks if the array is empty
         * @return 'true' if the array is empty, 'false' otherwise
         */
        bool empty() const {
            return elements.empty();
        }

        /** 
         * @brief Appends a value to the end of the array 
         * @param[in] value The new value to append to the end of the array
         */ 
        void push(const Value& value) {
            elements.push_back(value);
        }

        /** @brief Removes the last element from the array */
        void pop() {
            elements.pop_back();
        }

        /** 
         * @brief Reserves more capacity for the array 
         * @param[in] capacity The new capacity to reserve for the array
         */
        void reserve(size_t capacity) {
            elements.reserve(capacity);
        }

        /**
         * @brief Gets an iterator to browse the array
         * @return An iterator to the beginning of the array
         */
        inline constexpr std::vector<Value>::iterator begin() noexcept {
            return elements.begin();
        }

        /**
         * @brief Gets an iterator to browse the array
         * @return An iterator to the end of the array
         */
        inline constexpr std::vector<Value>::iterator end() noexcept {
            return elements.end();
        }

        /**
         * @brief Trace all reachable MeowObjects for Garbage Collector
         * @details This method is one of the most important part of the Garbage Collector mechanism. It's called by GC to mark all of reachable MeowObjects
         * @param[in,out] visitor The Visitor that performs the tracing
         * @see meow::memory::MeowObject::trace
         */
        void trace(meow::memory::GCVisitor& visitor) override {
            for (auto& element : elements) {
                visitor.visitValue(element);
            }
        }
    };

    /**
     * @struct ObjHash
     * @brief Represents an object in MeowScript
     * @details A wrapper around an \c std::unordered_map<Value> to represent an object
     */
    struct ObjHash : meow::memory::MeowObject {
    private:
        std::unordered_map<std::string, Value> methods;
    public:

        /**
         * @brief The default constructor for ObjHash
         * @details Initializes an empty object
         */
        ObjHash(): methods() {}

        /**
         * @brief Constructs an ObjHash from an existing hash map
         * @details Initializes the object by copying the data from provided hash map
         * @param[in] pairs The hash map to copy from
         */
        ObjHash(const std::unordered_map<std::string, Value>& pairs) : methods(pairs) {}

        /**
         * @brief Constructs an ObjHash from an existing map
         * @details Initializes the object by copying the data from provided map
         * @param[in] pairs The map to copy from
         */
        ObjHash(const std::map<std::string, Value>& pairs) {
            for (const auto& pair : pairs) {
                methods[pair.first] = pair.second;
            }
        }

        /**
         * @brief Gets the constant reference to object
         * @return The read-only object
         */
        const std::unordered_map<std::string, Value>& get() const {
            return methods;
        }

        /**
         * @brief Gets the value at specified normal string key
         * @param[in] key The normal string key of value to retrieve
         * @tparam T the type of key
         * @return The read-only value at specified normal string key
         * @warning No bound checking
         */
        template <typename T>
        const Value& get(const T& key) const {
            return methods.at(key);
        }

        /**
         * @brief Gets the value at specified String object key
         * @param[in] key The String object key of value to retrieve
         * @return The read-only value at specified key
         * @warning No bound checking
         */
        const Value& get(const String& key) const {
            return methods.at(key->get());
        }

        /**
         * @brief Sets the value at specified normal string key
         * @param[in] key The normal string key of value to set
         * @param[in] value The new value to assign to the value at normal string key
         * @tparam T the type of key
         */
        template <typename T>
        void set(const T& key, const Value& value) {
            methods[key] = value;
        }

        /**
         * @brief Sets the value at specified String object key
         * @param[in] key The String object key of value to set
         * @param[in] value The new value to assign to the value at String object key
         */
        void set(const String& key, const Value& value) {
            methods[key->get()] = value;
        }

        /**
         * @brief Gets the size of object
         * @return Size of object
         */        
        size_t size() const {
            return methods.size();
        }

        /**
         * @brief Checks if the object is empty
         * @return 'true' if the object is empty, 'false' otherwise
         */
        bool empty() const {
            return methods.empty();
        }

        /**
         * @brief Checks if the object has that normal string key
         * @param[in] key The normal string key want to check if
         * @tparam T the type of key
         * @return 'true' if the key exists, 'false' otherwise
         */
        template <typename T>
        bool has(const T& key) const {
            return methods.find(key) != methods.end();
        }

        /**
         * @brief Checks if the object has that String object key
         * @param[in] key The String object key want to check if
         * @return 'true' if the object is empty, 'false' otherwise
         */
        bool has(const String& key) const {
            return methods.find(key->get()) != methods.end();
        }

        /**
         * @brief Gets an iterator to browse the object
         * @return An iterator to the beginning of the object
         */
        inline constexpr std::unordered_map<std::string, Value>::const_iterator begin() const noexcept {
            return methods.begin();
        }

        /**
         * @brief Gets an iterator to browse the object
         * @return An iterator to the end of the object
         */
        inline constexpr std::unordered_map<std::string, Value>::const_iterator end() const noexcept {
            return methods.end();
        }

        /**
         * @brief Trace all reachable MeowObjects for Garbage Collector
         * @details This method is one of the most important part of the Garbage Collector mechanism. It's called by GC to mark all of reachable MeowObjects
         * @param[in,out] visitor The Visitor that performs the tracing
         * @see meow::memory::MeowObject::trace
         */
        void trace(meow::memory::GCVisitor& visitor) override {
            for (auto& pair : methods) {
                visitor.visitValue(pair.second);
            }
        }
    };   

    struct UpvalueDesc {
        bool isLocal = true;
        size_t index;
    };

    /**
     * @struct ObjProto
     * @brief Represents function proto in MeowScript
     */
    struct ObjProto : meow::memory::MeowObject {
        size_t registers;
        size_t upvalues;

        meow::runtime::Chunk* chunk;
        void trace(meow::memory::GCVisitor& visitor) override;
    };

    struct ObjClosure : meow::memory::MeowObject {

    };

    struct CallFrame {

    };
}