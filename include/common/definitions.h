/**
 * @file definitions.h
 * @author lazypaws
 * @brief Defines main values in MeowScript
 * @note Object values managed by Garbage Collector
 * @copyright Copyright(c) 2025 LazyPaws. All rights reserved. No copying or use for personal or commercial purposes is permitted!
 */

#pragma once

#include "common/value.h"
#include "memory/meow_object.h"
#include "memory/gc_visitor.h"
#include "pch.h"

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
        ObjBytes() : data() {}
        ObjBytes(const std::vector<uint8_t>& bytes): data(bytes) {}

        /**
         * @brief Gets the constant reference to byte array
         * @return A const reference to the array of bytes (read-only)
         */
        const std::vector<uint8_t>& get() const {
            return data;
        }
        /**
         * @brief Gets the value at specified index
         * @param[in] index The index of element to retrieve
         * @return The read-only value at specified index
         * @warning No bound checking
         */
        uint8_t get(size_t index) const {
            return data[index];
        }
        
        /**
         * @brief Sets the value at specified index
         * @param[in] index The index of element to set
         * @param[in] value The new value to assign to the element at index
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

        /** @brief Checks if the byte array is empty */
        bool empty() const {
            return data.empty();
        }

        /** 
         * @brief Appends a byte to the end of the array 
         * @param[in] value The new value to append to the end of the array
         */ 
        void push(uint8_t value) {
            data.push_back(value);
        }

        /** @brief Pops a byte at the end of array */
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
         * @see meow::memory::MeowObject::trace
         */
        void trace([[maybe_unused]] meow::memory::GCVisitor& visitor) override {}
    };

    // NOTE: Not comment all below yet

    struct ObjString : meow::memory::MeowObject {
    private:
        std::string data;
    public:
        ObjString() : data() {}
        ObjString(const std::string& str) : data(str) {}
        const std::string& get() const {
            return data;
        }
        char get(size_t index) const {
            return data[index];
        }

        size_t size() const {
            return data.size();
        }
        bool empty() const {
            return data.empty();
        }
        inline constexpr std::string::iterator begin() const noexcept {
            return data.begin();
        }
        inline constexpr std::string::iterator end() const noexcept {
            return data.end();
        }

        void trace(meow::memory::GCVisitor&) override {}
    };

    struct ObjArray : meow::memory::MeowObject {
    private:
        std::vector<meow::common::Value> elements;
    public:
        ObjArray(): elements() {}
        ObjArray(const std::vector<meow::common::Value>& vector) : elements(vector) {}

        const std::vector<meow::common::Value>& get() const {
            return elements;
        }
        const meow::common::Value& get(size_t index) const {
            return elements[index];
        }
        void set(size_t index, const meow::common::Value& value) {
            elements[index] = value;
        }

        size_t size() const {
            return elements.size();
        }
        bool empty() const {
            return elements.empty();
        }
        void push(const meow::common::Value& value) {
            elements.push_back(value);
        }
        void pop() {
            elements.pop_back();
        }
        void reserve(size_t capacity) {
            elements.reserve(capacity);
        }
        inline constexpr std::vector<meow::common::Value>::iterator begin() const noexcept {
            return elements.begin();
        }
        inline constexpr std::vector<meow::common::Value>::iterator end() const noexcept {
            return elements.end();
        }

        void trace(meow::memory::GCVisitor& visitor) override {
            for (auto& element : elements) {
                visitor.visitValue(element);
            }
        }
    };

    struct ObjHash : meow::memory::MeowObject {
    private:
        std::unordered_map<std::string, meow::common::Value> methods;
    public:
        ObjHash(): methods() {}
        ObjHash(const std::unordered_map<std::string, meow::common::Value>& pairs) : methods(pairs) {}
        ObjHash(const std::map<std::string, meow::common::Value>& pairs) {
            for (const auto& pair : pairs) {
                methods[pair.first] = pair.second;
            }
        }
        const std::unordered_map<std::string, meow::common::Value>& get() const {
            return methods;
        }

        template <typename T>
        meow::common::Value get(const T& key) const {
            auto it = methods.find(key);
            if (it != methods.end()) {
                return it->second;
            }
            return meow::common::Value(Null{});
        }
        meow::common::Value get(String key) const {
            auto it = methods.find(key->get());
            if (it != methods.end()) {
                return it->second;
            }
            return meow::common::Value(Null{});
        }

        template <typename T>
        void set(const T& key, const meow::common::Value& value) {
            methods[key] = value;
        }
        void set(const String& key, const meow::common::Value& value) {
            methods[key->get()] = value;
        }

        size_t size() const {
            return methods.size();
        }
        bool empty() const {
            return methods.empty();
        }
        inline constexpr std::unordered_map<std::string, meow::common::Value>::const_iterator begin() const noexcept {
            return methods.begin();
        }
        inline constexpr std::unordered_map<std::string, meow::common::Value>::const_iterator end() const noexcept {
            return methods.end();
        }

        void trace(meow::memory::GCVisitor& visitor) override {
            for (auto& pair : methods) {
                visitor.visitValue(pair.second);
            }
        }
    };

    
}