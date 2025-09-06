#pragma once

#include "common/value.h"
#include "common/pch.h"

namespace meow::runtime {
    struct Chunk {
    private:
        std::vector<uint8_t> code;
        std::vector<meow::common::Value> constantPool;
        size_t ip;
    public:
        void writeByte(uint8_t byte) { 
            code.push_back(byte);
        }

        void writeInt64(int64_t value) {
            for (int i = 0; i < 8; ++i) {
                code.push_back(static_cast<uint8_t>((value >> (i * 8)) & 0xff));
            }
        }

        uint8_t readByte() {
            return code[ip++];
        }

        int64_t readInt64() {
            int64_t value = 0;
            for (int i = 0; i < 8; i++) {
                value |= static_cast<int64_t>(code[ip++]) << (i * 8);
            }
            return value;
        }

        meow::common::Value readConstant(size_t index) const {
            if (index < 0 || index >= constantPool.size()) {
                return meow::common::Value(meow::common::Null{});
            }
        }
    };
}