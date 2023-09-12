#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>

#include "actbigint.h"
#include "spdlog/fmt/bundled/core.h"

#define SPDLOG
#ifdef SPDLOG
#include "spdlog/fmt/bundled/format.h"
#include "spdlog/fmt/fmt.h"
#else

#define format(...) ___

namespace fmt {
static constexpr auto ___ = "";
}  // namespace fmt

#endif

template <size_t N>
class BigUInt {
  static_assert(N > 0, "dont use zero");

 public:
  BigUInt() { parts.fill(0U); }
  BigUInt(uint32_t value) {
    parts.fill(0U);
    parts[0] = value;
  }

  BigUInt& operator++() {
    uint64_t extPart = 1;
    for (size_t i = 0; i < N; ++i) {
      extPart += parts[i];
      parts[i] = extPart & ~uint32_t{};
      extPart >>= 32;
    }
    return *this;
  }
  BigUInt& operator--() {
    uint64_t extPart = 0;
    for (size_t i = 0; i < N; ++i) {
      extPart += ~uint32_t{};
      extPart += parts[i];
      parts[i] = extPart & ~uint32_t{};
      extPart >>= 32;
    }
    return *this;
  }

  BigUInt& operator+=(const BigUInt& other) {
    uint64_t extPart = 0;
    for (size_t i = 0; i < N; ++i) {
      extPart += other.parts[i];
      extPart += parts[i];
      parts[i] = extPart & ~uint32_t{};
      extPart >>= 32;
    }
    return *this;
  }
  BigUInt& operator+=(uint32_t other) {
    uint64_t extPart = 0;
    for (size_t i = 0; i < N; ++i) {
      extPart += other;
      extPart += parts[i];
      parts[i] = extPart & ~uint32_t{};
      extPart >>= 32;
      other = 0;
    }
    return *this;
  }

  BigUInt& operator-=(uint32_t other) {
    uint64_t extPart = 1;
    for (size_t i = 0; i < N; ++i) {
      extPart += ~other;
      extPart += parts[i];
      parts[i] = extPart & ~uint32_t{};
      extPart >>= 32;
      other = 0;
    }
    return *this;
  }

  BigUInt& operator-=(const BigUInt& other) {
    uint64_t extPart = 1;
    for (size_t i = 0; i < N; ++i) {
      extPart += ~other.parts[i];
      extPart += parts[i];
      parts[i] = extPart & ~uint32_t{};
      extPart >>= 32;
    }
    return *this;
  }

  // BigUInt& operator*=(uint32_t other) {
  //   BigUInt temp;
  //   uint64_t extPart;
  //   for (size_t i = 0; i < N - 1; ++i) {
  //     extPart = parts[i];
  //     extPart *= other;
  //     parts[i] = extPart & ~uint32_t{};
  //     std::cout << fmt::format("{:08x}", extPart >> 32) << "\n";
  //     temp.parts[i + 1] = extPart >> 32;
  //   }
  //   std::cout << temp.parts << "\n";
  //   extPart = parts[N - 1] * other;
  //   parts[N - 1] = extPart & ~uint32_t{};
  //   return *this += temp;
  // }

  BigUInt& operator*=(uint32_t other) {
    uint64_t multRes;
    uint64_t extPart = 0;
    for (size_t i = 0; i < N; ++i) {
      multRes = parts[i];
      multRes *= other;
      extPart += multRes & ~uint32_t{};
      parts[i] = extPart & ~uint32_t{};
      extPart >>= 32;
      extPart += multRes >> 32;
    } 
    return *this;
  }

  BigUInt operator++(int) {
    BigUInt res = *this;
    ++*this;
    return res;
  }
  BigUInt operator--(int) {
    BigUInt res = *this;
    --*this;
    return res;
  }

  BigUInt operator+(const BigUInt& other) const {
    BigUInt res = *this;
    return res += other;
  }
  BigUInt operator-(const BigUInt& other) const {
    BigUInt res = *this;
    return res -= other;
  }

  void print(std::ostream& os) const {
    os << "0x";
    for (auto it = parts.rbegin(); it != parts.rend(); ++it)
      os << fmt::format("'{:08x}", *it);
  }

 private:
  std::array<uint32_t, N> parts;

  struct Value32 {};
  BigUInt& operator<<=(Value32) {
    for (size_t i = N; i > 0; --i) parts[i] = parts[i - 1];
    parts[0] = 0U;
  }
};

template <size_t N>
std::ostream& operator<<(std::ostream& os, const BigUInt<N>& bigUInt) {
  bigUInt.print(os);
  return os;
}

long powerFast(size_t num, size_t deg) {
  static constexpr size_t bits = 2;
  long result = 0;
  while (deg) {
    if (auto v = deg & ((1 << bits) - 1)) result += v * num;
    num <<= bits;
    deg >>= bits;
  }
  return result;
}

int main(int argc, char* argv[]) {
  std::cout << "bigint"
            << "\n";
  BigUInt<4> big{~uint32_t{}};
  BigUInt<4> big2{~uint32_t{}};

  big += ~uint32_t{};
  big += big2;

  ActBigInt bb{~uint32_t{}};
  bb += ~uint32_t{};
  bb += ~uint32_t{};
  std::cout << ++ ++ ++big << "\n";
  std::cout << ++ ++ ++bb << "\n";

  uint32_t d = 1;
  std::cout << (big -= d) << "\n";
  std::cout << (bb -= d) << "\n";

  std::cout << --big << "\n";
  std::cout << --bb << "\n";

  big *= 0xf164ffffU;
  bb *= 0xf164ffffU;
  std::cout << big << "\n";
  std::cout << bb << "\n";

  return 0;
}