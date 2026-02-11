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
None print(const std::vector<char>& s) {
    for (char c : s) std::cout << c;
    return none;
}

None print(const RoxString& s) {
    std::cout << s.val;
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
std::vector<char> numToString(num n) {
    std::string s = std::to_string(n);
    return std::vector<char>(s.begin(), s.end());
}

std::vector<char> charToString(char c) {
    return {c};
}

std::string any_to_string(num n) { return std::to_string(n); }
std::string any_to_string(int n) { return std::to_string(n); }
std::string any_to_string(char c) { return std::string(1, c); }
std::string any_to_string(bool b) { return b ? "true" : "false"; }

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

bool contains(std::vector<char> items, char c) {
  num n = items.size();
  for (auto i = ((num)0); i < n; i += ((num)1))   {
    rox_result<char> r = rox_at(items, i);
    if (isOk(r))     {
      char val = getValue(r);
      if ((val == c))       {
        return true;
      }
    }
  }
  return false;
}
num length_of_longest_substring(std::vector<char> items) {
  num n = items.size();
  num max_len = ((num)0);
  num left = ((num)0);
  for (auto k = ((num)0); k < n; k += ((num)1))   {
    rox_result<char> r_k = rox_at(items, k);
    if ((!isOk(r_k)))     {
      return ((num)0);
    }
    char c = getValue(r_k);
    bool found = false;
    num found_index = ((num)0);
    for (auto j = left; j < k; j += ((num)1))     {
      rox_result<char> r_j = rox_at(items, j);
      if (isOk(r_j))       {
        char char_j = getValue(r_j);
        if ((char_j == c))         {
          (found = true);
          (found_index = j);
        }
      }
    }
    if (found)     {
      if ((found_index >= left))       {
        (left = (found_index + ((num)1)));
      }
    }
    num current_len = ((k - left) + ((num)1));
    if ((current_len > max_len))     {
      (max_len = current_len);
    }
  }
  return max_len;
}
int main() {
  std::vector<char> s1 = std::vector<char>{'a', 'b', 'c', 'a', 'b', 'c', 'b', 'b'};
  if ((length_of_longest_substring(s1) == ((num)3)))   {
    std::vector<char> s2 = std::vector<char>{'b', 'b', 'b', 'b', 'b'};
    if ((length_of_longest_substring(s2) == ((num)1)))     {
      std::vector<char> s3 = std::vector<char>{'p', 'w', 'w', 'k', 'e', 'w'};
      if ((length_of_longest_substring(s3) == ((num)3)))       {
        print(rox_to_list(rox_str("Longest Substring: Passed\n")));
        return 0;
      }
    }
  }
  print(rox_to_list(rox_str("Longest Substring: Failed\n")));
  return 0;
}
