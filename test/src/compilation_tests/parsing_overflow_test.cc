#include <SI/detail/number_parser.h>

/// helper literal

namespace {
template <char... _Digits> constexpr int64_t operator""_literal_op() {
  return SI::detail::parsing::Number<_Digits...>::value;
}

int overflow_test() {
  constexpr auto v = 0xFFFFFFFFFFFFFFFFF_literal_op;
  /// needed to avoid unused variable warning
  if (v > 0) {
    return 1;
  }
  return 0;
}
} // namespace