#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <numeric>
#include <variant>
#include <cstdint>

// ROX Runtime
using num32 = int32_t;
using num = int64_t;
using rox_float = double;
using rox_char = char;
using rox_bool = bool;
struct None { bool operator==(const None&) const { return true; } };
const None none = {};
template<typename T>
struct rox_result {
    T value;
    num32 err;
};
template<typename T>
bool isOk(rox_result<T> r) {
    return r.err == 0;
}
template<typename T>
T getValue(rox_result<T> r) {
    if (r.err != 0) {
        std::cerr << "Called getValue on runtime error result!" << std::endl;
        exit(r.err);
    }
    return r.value;
}
void print_loop(num32 n) {
    for (int i = 0; i < n; ++i) {
        std::cout << "Hello, World!" << std::endl;
    }
}
// Result constructors
template<typename T>
rox_result<T> ok(T value) { return {value, 0}; }
template<typename T>
rox_result<T> error(num32 code) { return {T{}, code}; }
const double pi = 3.141592653589793;
const double e  = 2.718281828459045;
class RoxString {
public:
    std::string val;
    RoxString(const char* s) : val(s) {}
    RoxString(std::string s) : val(std::move(s)) {}
    RoxString() = default;

    num size() const { return (num)val.size(); }
    bool operator==(const RoxString& other) const { return val == other.val; }
    bool operator!=(const RoxString& other) const { return val != other.val; }
};

std::ostream& operator<<(std::ostream& os, const RoxString& s) {
    return os << s.val;
}

RoxString rox_str(const char* s) {
    return RoxString(s);
}

// I/O
std::ostream& operator<<(std::ostream& os, const std::vector<char>& s) {
    for (char c : s) os << c;
    return os;
}

template<typename... Args>
None print(const Args&... args) {
    ((std::cout << args), ...);
    return none;
}


// List access
template<typename T>
rox_result<T> rox_at(const std::vector<T>& xs, num i) {
    if (i < 0 || i >= (num)xs.size()) return error<T>(1); // index_out_of_range
    return ok(xs[i]);
}

// String access
rox_result<char> rox_at(const RoxString& s, num i) {
    if (i < 0 || i >= s.size()) return error<char>(1);
    return ok(s.val[i]);
}

// String to List
std::vector<char> rox_to_list(const RoxString& s) {
    return std::vector<char>(s.val.begin(), s.val.end());
}

// Division
template<typename T>
rox_result<T> rox_div(T a, T b) {
    if (b == 0) return error<T>(3); // division_by_zero
    return ok(a / b);
}

// Modulo
template<typename T>
rox_result<T> rox_mod(T a, T b) {
    if (b == 0) return error<T>(3); // division_by_zero
    return ok(a % b);
}

num32 num32_abs(num32 x) { return std::abs(x); }
num32 num32_min(num32 x, num32 y) { return std::min(x, y); }
num32 num32_max(num32 x, num32 y) { return std::max(x, y); }
rox_result<num32> num32_pow(num32 base, num32 exp) {
    if (exp < 0) return error<num32>(10); // invalid_argument
    num32 res = 1;
    for (int i = 0; i < exp; ++i) res *= base;
    return ok(res);
}

num num_abs(num x) { return std::abs(x); }
num num_min(num x, num y) { return std::min(x, y); }
num num_max(num x, num y) { return std::max(x, y); }
rox_result<num> num_pow(num base, num exp) {
    if (exp < 0) return error<num>(10); // invalid_argument
    num res = 1;
    for (int i = 0; i < exp; ++i) res *= base;
    return ok(res);
}

double float_abs(double x) { return std::abs(x); }
double float_min(double x, double y) { return std::min(x, y); }
double float_max(double x, double y) { return std::max(x, y); }
double float_pow(double x, double y) { return std::pow(x, y); }
rox_result<double> float_sqrt(double x) {
    if (x < 0) return error<double>(10); // invalid_argument
    return ok(std::sqrt(x));
}
double float_sin(double x) { return std::sin(x); }
double float_cos(double x) { return std::cos(x); }
double float_tan(double x) { return std::tan(x); }
rox_result<double> float_log(double x) {
    if (x <= 0) return error<double>(10); // invalid_argument
    return ok(std::log(x));
}
double float_exp(double x) { return std::exp(x); }
double float_floor(double x) { return std::floor(x); }
double float_ceil(double x) { return std::ceil(x); }
std::vector<char> numToString(num n) {
    std::string s = std::to_string(n);
    return std::vector<char>(s.begin(), s.end());
}

std::vector<char> charToString(char c) {
    return {c};
}

std::string any_to_string(num n) { return std::to_string(n); }
std::string any_to_string(int n) { return std::to_string(n); }
std::string any_to_string(double d) { return std::to_string(d); }
std::string any_to_string(char c) { return std::string(1, c); }
std::string any_to_string(bool b) { return b ? "true" : "false"; }
std::string any_to_string(const RoxString& s) { return s.val; }

template <typename T>
std::vector<char> listToString(const std::vector<T>& list) {
    std::string s = "[";
    for (size_t i = 0; i < list.size(); ++i) {
        if (i > 0) s += ", ";
        s += any_to_string(list[i]);
    }
    s += "]";
    return std::vector<char>(s.begin(), s.end());
}


// End Runtime

int main() {
  RoxString s = rox_str("Hello");
  if ((s.size() != ((num)5)))   {
    print(rox_str("Size Fail\n"));
    return 0;
  }
  rox_result<char> r = rox_at(s, ((num)1));
  if ((!isOk(r)))   {
    print(rox_str("At Fail 1\n"));
    return 0;
  }
  if ((getValue(r) != 'e'))   {
    print(rox_str("At Fail 2\n"));
    return 0;
  }
  if ((s == rox_str("Hello")))   {
  }
  else   {
    print(rox_str("Eq Fail\n"));
    return 0;
  }
  print(s);
  print(rox_str("\n"));
  print(rox_str("Variadic"), rox_str(" "), rox_str("Print"), rox_str(" "), rox_str("Works\n"));
  print(rox_str("Passed\n"));
  return 0;
}
