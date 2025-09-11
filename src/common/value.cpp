// SPDX-License-Identifier: MIT
/**
 * @file value.cpp
 * @author lazypaws
 * @brief Implementation of MeowScript values
 * @copyright Copyright(c) 2025 LazyPaws
 */

#include "common/value.h"
#include "common/definitions.h"

using namespace meow::common;

int64_t Value::asInt() const {
    using i64_limits = std::numeric_limits<int64_t>;
    return visit(
        [](Null) -> int64_t { return 0; },
        [](Int i) -> int64_t { return i; },
        [](Float f) -> int64_t { 
            if (std::isinf(f)) {
                return (f > 0) ? i64_limits::max() : i64_limits::min();
            }
            if (std::isnan(f)) return 0;
            return static_cast<int64_t>(f);
        },
        [](Bool b) -> int64_t { return b ? 1 : 0; },
        [](const String& s) -> int64_t {
            // By using std::string_view, it's make everything safe and faster
            std::string_view str = s->get();

            // By using unsigned long long - the largest primitive integer type
            // We can avoid many issues about overflow
            using ull = unsigned long long;

            // Trim whitespace in the first and end of the string
            // We can't do many way to trim whitespace instead
            // Like using trim() function or using erase and find_if,...
            size_t left = 0;
            while (left < str.size() && std::isspace(static_cast<unsigned char>(str[left]))) ++left;
            size_t right = str.size();
            while (right > left && std::isspace(static_cast<unsigned char>(str[right - 1]))) --right;
            if (left >= right) return 0;

            // str.remove_prefix(left);
            // str.remove_suffix(str.size() - right);
            str = str.substr(left, right - left);

            bool negative = false;
            size_t pos = 0;
            if (str[0] == '-') {
                negative = true;
                str.remove_prefix(1);
            } else if (str[0] == '+') {
                str.remove_prefix(1);
            }

            int base = 10;

            if (str.size() >= 2) {
                std::string_view prefix = str.substr(0, 2);

                // We can handle binary right there
                if (prefix == "0b" || prefix == "0B") {
                    ull accumulator = 0;
                    const ull limit = static_cast<ull>(i64_limits::max()); // The maximum value Integer can reach

                    str.remove_prefix(2);
                    for (char c : str) {
                        if (c == '0' || c == '1') {
                            int d = c - '0';

                            // Here is our algorithm
                            // accumulator_new = old_accumulator * 2 + d
                            // And new_accumulator must be less than or equal to limit
                            // new_accumulator <= limits
                            // We can't do that right on the accumulator, it's can overflow
                            // But we can operate on limits by transform our formula
                            // old_accumulator * 2 + d <= limits
                            // -> old_accumulator <= (limits - d) / 2
                            // And now, we can know if the value is overflow and handle it

                            if (accumulator > (limit - d) / 2) {
                                return negative ? i64_limits::min() : i64_limits::max();
                            }

                            // This expression equals to accumulator * 2 + d
                            // But we can use this to make a bit faster
                            accumulator = (accumulator << 1) | static_cast<ull>(d);
                        } else break;
                    }

                    int64_t result = static_cast<int64_t>(accumulator);
                    return negative ? -result : result;
                } else if (prefix == "0x" || prefix == "0X") {
                    base = 16;
                    str.remove_prefix(2);
                } else if (prefix == "0o" || prefix == "0O") {
                    base = 8;
                    str.remove_prefix(2);
                }
            }

            // Handles Integer casting
            // Using strtoll is a bit faster than using stoll
            // We still can use stoll if we need C++ casting style
            // However it's just a choice between many different choices

            errno = 0;
            char* endptr = nullptr;
            const std::string token(str.begin(), str.end());
            long long val = std::strtoll(token.c_str(), &endptr, base);
            if (endptr == token.c_str()) return 0;
            if (errno == ERANGE) {
                return (val > 0) ? i64_limits::max() : i64_limits::min();
            }
            if (val > static_cast<long long>(i64_limits::max())) {
                return i64_limits::max();
            }
            if (val < static_cast<long long>(i64_limits::min())) {
                return i64_limits::min();
            }

            return static_cast<int64_t>(val);
        },
        [](auto&&) -> int64_t { return 0; }
    );
}

double Value::asFloat() const {
    return visit(
        [](Null) -> double { return 0.0; },
        [](Int i) -> double { return static_cast<double>(i); },
        [](Float f) -> double { return f; },
        [](Bool b) -> double { return b ? 1.0 : 0.0; },
        [](const String& s) -> double {
            std::string str = s->get();

            for (auto &c : str) {
                c = static_cast<char>(std::tolower((unsigned char)c));
            }

            if (str == "nan") {
                return std::numeric_limits<double>::quiet_NaN();
            }
            if (str == "infinity" || str == "+infinity" || str == "inf" || str == "+inf") {
                return std::numeric_limits<double>::infinity();
            }
            if (str == "-infinity" || str == "-inf") {
                return -std::numeric_limits<double>::infinity();
            }

            const char* cs = str.c_str();
            errno = 0;
            char* endptr = nullptr;
            double val = std::strtod(cs, &endptr);

            if (cs == endptr) return 0.0;

            // Checks range
            if (errno == ERANGE) {
                return (val > 0) ? std::numeric_limits<double>::infinity() : -std::numeric_limits<double>::infinity();
            }
            return static_cast<double>(val);
        },
        [](auto&&) -> double { return 0.0; }
    );
}

bool Value::asBool() const {
    return visit(
        [](Null) -> bool { return 0; },
        [](Int i) -> bool { return i != 0; },
        [](Float f) -> bool {
            return f != 0.0 && !std::isnan(f);
        },
        [](Bool b) -> bool { return b; },
        [](const Bytes& b) -> bool { !b->empty(); },
        [](const String& s) -> bool { !s->empty(); },
        [](const Array& a) -> bool { return !a->empty(); },
        [](const Object& o) -> bool { return !o->empty(); },
        [](auto&&) -> bool { return true; }
    );
}

std::string Value::asString() const {
    return visit(
        [](Null) -> std::string { return "null"; },
        [](Int i) -> std::string { return std::to_string(i); },
        [](Float f) -> std::string {
            if (std::isnan(f)) return "NaN";
            if (std::isinf(f)) return (f > 0) ? "Infinity" : "-Infinity";

            if (f == 0.0 && std::signbit(f)) return "-0";
            std::ostringstream os;
            os << std::fixed << std::setprecision(15) << f;

            // Removes extra '0' character
            std::string str = os.str();
            auto pos = str.find('.');

            // This is impossible for floating-point string value
            // How it can be? Maybe C++ casted failed
            if (pos == std::string::npos) return str;
            size_t end = str.size();

            // Search from the end of string makes a bit faster
            while (end > pos + 1 && str[end - 1] == '0') --end;
            if (end == pos + 1) --pos;
            return str.substr(0, end);
        },
        [](Bool b) -> std::string { return b ? "true" : "false"; },
        [](String b) -> std::string { return b->get(); },
        [](const Array& a) -> std::string {
            std::string out = "[";
            for (size_t i = 0; i < a->size(); ++i) {
                if (i > 0) out += ", ";
                out += a->get(i).asString();
            }
            out += "]";
            return out;
        },
        [](const Object& o) -> std::string {
            std::string out = "{";
            bool first = true;
            for (auto it = o->begin(); it != o->end(); ++it) {
                if (!first) out += ", ";
                out += it->first + ": " + (it->second).asString();
                first = false;
            }
            out += "}";
            return out;
        }
    );
}