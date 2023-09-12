

#include <array>
#include <cstdint>
#include <iostream>
#include <ostream>

#include "spdlog/fmt/bundled/format.h"

using uint128_t = fmt::detail::uint128_t;

class ActBigInt {
 public:
  ActBigInt() : val() {}
  ActBigInt(uint128_t val) : val(val) {}

  ActBigInt& operator++() {
    ++val;
    return *this;
  }
  ActBigInt operator++(int) {
    ActBigInt res = *this;
    return ++res;
  }
  ActBigInt& operator--() {
    --val;
    return *this;
  }
  ActBigInt operator--(int) {
    ActBigInt res = *this;
    return --res;
  }
  ActBigInt operator+(ActBigInt other) {
    ActBigInt res = *this;
    return res += other;
  }
  ActBigInt& operator+=(ActBigInt other) {
    val += other.val;
    return *this;
  }
  ActBigInt operator-(ActBigInt other) {
    ActBigInt res = *this;
    return res -= other;
  }
  ActBigInt& operator-=(ActBigInt other) {
    val -= other.val;
    return *this;
  }

  ActBigInt& operator*=(ActBigInt other) {
    val *= other.val;
    return *this;
  }
  ActBigInt operator*(ActBigInt other) {
    ActBigInt res = *this;
    return res *= other;
  }

  void print(std::ostream& os) const {
    os << "0x";
    std::array<uint32_t, 4> parts;
    *reinterpret_cast<uint128_t*>(parts.data()) = val;
    for (auto it = parts.rbegin(); it != parts.rend(); ++it)
      os << fmt::format("'{:08x}", *it);
  }

 private:
  uint128_t val;
};

inline std::ostream& operator<<(std::ostream& os, ActBigInt abi) {
  abi.print(os);
  return os;
}

inline std::ostream& operator<<(std::ostream& os,
                                std::array<uint32_t, 4> parts) {
  for (auto it = parts.rbegin(); it != parts.rend(); ++it)
    os << fmt::format("'{:08x}", *it);
  return os;
}
