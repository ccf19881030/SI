- [implementation details of SI](#implementation-details-of-si)
  - [How specific units are defined](#how-specific-units-are-defined)
  - [Implicit ratio conversion & possible loss of precision](#implicit-ratio-conversion--possible-loss-of-precision)
  - [Conversion to strings](#conversion-to-strings)

# implementation details of SI

## How specific units are defined

All units provided by SI are derived from the template `SI::detail::unit_t`. All units have a base typedef i.e.  `force_t` and further derived typedefs for each ratio i.e. `micro_newton_t`

example:
```cpp
template <typename _type, class _ratio = std::ratio<1>>
using force_t = detail::unit_t<'F', 1, _type, _ratio>;

template <typename _type> using micro_newton_t = force_t<_type, std::micro>;
```

The implementation of the functionality is done purely in the classes of the `detail` namespace.

## Implicit ratio conversion & possible loss of precision

SI handles operations of units of the same ratios as well as when the ratios are different. Operations of between units of the same ratio are overhead-free, else there is additional computation cost to adjust the values to the units. By passing the flag `SI_DISABLE_IMPLICIT_RATIO_CONVERSION` to the compiler implicit ratio conversion is not done and fails with a compiler error. See the [continuous benchmark results](https://si.dominikberner.ch/dev/bench/) for comparing the reference measurements and the implementation in SI.

If the internal type of a unit is an integral type (`int64_t`, `int` etc.) integer rounding applies when assigning to a unit of bigger ratio.

```cpp

const auto almost_a_km = 999_m;
const auto zero_km = 0_km;
const SI::kilo_metre_t km_value = almost_a_km;

assert(almost_a_km == zero_km); // values are equal
```

## Conversion to strings

All units with a special typedef and literals can be passed to (string-) streams using the `<<` operator. Constructing from strings using the `>>` operator. Alternatively using the `SI::to_string()` function can be used to get a string. 

For example:
```cpp

std::stringstream ss;
const auto value = 1_km; // constructs a value of the type SI::kilo_metre_t
ss << value; // ss now contains "1km"
SI::kilo_metre_t<int64_t> from_string{0};
ss >> value_from_string; // the value of from_string is now 1

std::string str = SI::to_string(value);
```

If a value cannot be constructed from a string the failbit of the stream is set. 