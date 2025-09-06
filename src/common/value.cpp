#include "common/value.h"
#include "common/definitions.h"

using namespace meow::common;

int64_t Value::asInt() const {
    return visit(
        [](Null) -> int64_t { return 0; },
        [](Int i) -> int64_t { return i; },
        [](Float f) -> int64_t { 
            if (std::isinf(f)) {
                return (f > 0) ? std::numeric_limits<int64_t>::max() : std::numeric_limits<int64_t>::min();
            }
            if (std::isnan(f)) return 0;
            return static_cast<int64_t>(f);
        },
        [](Bool b) -> int64_t { return b ? 1 : 0; },
        [](const String& s) -> int64_t { 
            const std::string sfull = s->get();

            size_t left = 0;
            while (left < sfull.size() && std::isspace(static_cast<unsigned char>(sfull[left]))) ++left;
            size_t right = sfull.size();
            while (right > left && std::isspace(static_cast<unsigned char>(sfull[right - 1]))) --right;
            if (left >= right) return 0;

            std::string token = sfull.substr(left, right - left);

            bool negative = false;
            size_t pos = 0;
            if (token[pos] == '+' || token[pos] == '-') {
                negative = (token[pos] == '-');
                ++pos;
                if (pos >= token.size()) return 0;
            }

            if (token.size() - pos >= 2 && token[pos] == '0' && (token[pos+1] == 'b' || token[pos+1] == 'B')) {

                unsigned long long accumulator = 0;
                const unsigned long long limit = static_cast<unsigned long long>(std::numeric_limits<Int>::max());
                for (size_t i = pos + 2; i < token.size(); ++i) {
                    char c = token[i];
                    if (c == '0' || c == '1') {
                        int d = c - '0';
                        if (accumulator > (limit - d) / 2) {
                            return negative ? std::numeric_limits<int64_t>::min() : std::numeric_limits<int64_t>::max();
                        }
                        accumulator = (accumulator << 1) | static_cast<unsigned long long>(d);
                    } else break;
                }
                int64_t result = static_cast<int64_t>(accumulator);
                return negative ? -result : result;
            }

            int base = 10;
            if (token.size() - pos >= 2 && token[pos] == '0' && (token[pos+1] == 'x' || token[pos+1] == 'X')) {
                base = 16;
            } else if (token.size() - pos >= 2 && token[pos] == '0' && (token[pos+1] == 'o' || token[pos+1] == 'O')) {
                base = 8;
            } else if (token.size() - pos >= 2 && token[pos] == '0' && std::isdigit(static_cast<unsigned char>(token[pos+1]))) {
                base = 8;
            }

            errno = 0;
            char* endptr = nullptr;
            const std::string tokstd(token.begin(), token.end());
            long long val = std::strtoll(tokstd.c_str(), &endptr, base);
            if (endptr == tokstd.c_str()) return 0;
            if (errno == ERANGE) {
                return (val > 0) ? std::numeric_limits<int64_t>::max() : std::numeric_limits<int64_t>::min();
            }

            if (val > static_cast<long long>(std::numeric_limits<int64_t>::max())) {
                return std::numeric_limits<int64_t>::max();
            }
            if (val < static_cast<long long>(std::numeric_limits<int64_t>::min())) {
                return std::numeric_limits<int64_t>::min();
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
        [](const String& s) -> bool { !s->empty(); },
        [](const Array& a) -> bool { return !a->empty(); },
        [](const Object& o) -> bool { return !o->empty(); },
        [](auto&&) -> bool { return true; } // Maybe I want it's false. How fun!
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

            std::string str = os.str();
            auto pos = str.find('.');
            if (pos == std::string::npos) return str;
            size_t end = str.size();
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

// String Value::toString() const {
//     return visit(
//         // ObjString* needs to managed by Memory Manager and GC
//         // But toString() don't need to add more parameter for GC or VM*
//         // I think whenever we use toString, just make a new Obj
//         // And at call place, we can't make GC to claim it
//         [](Null) -> String { return makeString("null"); },
//         [](Int i) -> String { return makeString(std::to_string(i)); },
//         [](Float f) -> String { 
//             // Instead of using makeString() everywhere, we can do this
//             auto processor = [f]() -> std::string {
//                 if (std::isnan(f)) return "NaN";
//                 if (std::isinf(f)) return (f > 0) ? "Infinity" : "-Infinity";

//                 if (f == 0.0 && std::signbit(f)) return "-0";
//                 std::ostringstream ss;
//                 ss << std::fixed << std::setprecision(15) << f;

//                 std::string str = ss.str();
//                 auto pos = str.find('.');
//                 if (pos == std::string::npos) return str;
//                 size_t end = str.size();
//                 while (end > pos + 1 && str[end - 1] == '0') --end;
//                 if (end == pos + 1) --pos;
//                 return str.substr(0, end);
//             };

//             return makeString(processor());
//         },
//         [](Bool b) -> String { return makeString(b ? "true" : "false"); },
//         [](const String& s) -> String { return makeString(s->get()); },
//         [](const Array& a) -> String {
//             std::string out = "[";
//             for (size_t i = 0; i < a->size(); ++i) { // I don't think I want any contibutors operate throw a->get()
//                 if (i > 0) out += ", ";
//                 out += (a->get(i)).toString()->get(); 
//                 // Suddenly I realize... toString() will make a new Obj but..
//                 // This logic is not a mistake (maybe) but, where will objs go when we use toString() but no GC receive it?
//                 // It is something like this: .toString()->get(), .toString() make new object, but it's memory leak
//                 // Well, I think, I need to change something
//                 // toString() returns std::string?
//                 // or toString() wants one more parameter for MemoryManager*
//             }
//             out += "]";
//             return makeString(out);
//         },
//         [](const Object& o) {
//             // Later, I need to change something
//         },
//         [](auto&&) -> String { return makeString("unknown"); }
//     );
// }

