# QUnitConversion

## Overview

`QUnitConversion` is a simple lightweight library providing tools for runtime
unit conversion. Originally built for Qt, it now works with any string type — Qt
dependency is optional. The library is tested with `std::string` and `QString`.

`QUnitConversion` stores units as strings grouped by "family" (for example
length or temperature). Each family has its own base unit, conversion inside a
family is performed by converting through base unit providing conversion from
any unit to any other unit in a family.

Note that each unit should have a unique name, as long as conversion is unit
name-based.

`QUnitConversion` supports aliases for unit names, see aliases example below.

## Requirements

C++20 or later. Qt is not required unless you want to use `QString` as the
string type.

## Integration

The recommended way to integrate is via CMake `FetchContent`

```cmake
set(QUNITCONV_BUILD_TESTS OFF)
FetchContent_Declare(
    QUnitConversion
    GIT_REPOSITORY https://github.com/race-engineering-center/QUnitConversion.git
    GIT_TAG e42603f
)
FetchContent_MakeAvailable(QUnitConversion)

target_link_libraries(your_target PRIVATE QUnitConversion)
```

## Documentation

Documentation is available
[here](https://race-engineering-center.github.io/QUnitConversion/index.html)

## Examples

### Basic usage:

```cpp
QUnitConvertor<QString> convertor;

// fill the convertor instance with rules
convertor.addConversionRule(QUnitConversionRule<QString>("length", "m", "km", 0.001, 0));
convertor.addConversionRule(QUnitConversionRule<QString>("length", "m", "cm", 100, 0));

// you can convert a single value
double m = convertor.convert(50, "km", "m");   // returns value of a 50 km converted to meters

// or get a linear function that holds conversion from one unit to another
// to apply this conversion to many numbers without finding a conversion each time 
QLinearFunction convertFunction = convertor.convert("km", "m");
std::vector<double> meters;
// meters is filled here...
std::vector<double> kilometers;
for (double m: meters)
    kilometers.push_back(convertFunction.y(m));
```

### Aliases:

`QUnitConversion` supports aliases for units with possible conversion on the
fly, so you km/h, kmph and kmh will be converted to m/s properly.

```cpp
QUnitConvertor<std::string> convertor;
convertor.addConversionRule(QUnitConversionRule<std::string>("speed", "m/s", "km/h", 3.6, 0));

// register alternative names for "km/h"
convertor.addAlias("km/h", "kmph");
convertor.addAlias("km/h", "kmh");

// all three refer to the same unit
double a = convertor.convert(10, "m/s", "km/h");   // 36
double b = convertor.convert(10, "m/s", "kmph");   // 36
double c = convertor.convert(10, "m/s", "kmh");    // 36
```

## License

`QUnitConversion` is distributed under MIT license

Copyright Dmitriy Linev 2020-2026
