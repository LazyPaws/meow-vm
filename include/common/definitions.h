#pragma once

#include "common/value.h"
#include "memory/meow_object.h"
#include "memory/gc_visitor.h"
#include "pch.h"

namespace meow::common {
    struct ObjBytes : meow::memory::MeowObject {
    private:
        std::vector<uint8_t> data;
    public:
        ObjBytes() : data() {}
        ObjBytes(const std::vector<uint8_t>& bytes): data(bytes) {}

        const std::vector<uint8_t>& get() const {
            return data;
        }
        uint8_t get(size_t index) const {
            return data[index];
        }
        void set(size_t index, uint8_t value) {
            data[index] = value;
        }

        size_t size() const {
            return data.size();
        }
        bool empty() const {
            return data.empty();
        }
        void push(uint8_t value) {
            data.push_back(value);
        }
        void pop() {
            data.pop_back();
        }
        void reserve(size_t capacity) {
            data.reserve(capacity);
        }
        inline constexpr std::vector<uint8_t>::iterator begin() noexcept {
            return data.begin();
        }
        inline constexpr std::vector<uint8_t>::iterator end() noexcept {
            return data.end();
        }

        void trace(meow::memory::GCVisitor&) override {}
    };

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
        inline constexpr std::string::iterator begin() noexcept {
            return data.begin();
        }
        inline constexpr std::string::iterator end() noexcept {
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
        inline constexpr std::vector<meow::common::Value>::iterator begin() noexcept {
            return elements.begin();
        }
        inline constexpr std::vector<meow::common::Value>::iterator end() noexcept {
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