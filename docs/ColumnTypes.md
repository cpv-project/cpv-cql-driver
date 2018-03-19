This driver supports following column types:
Notice all column types can't hold a null value unless `Nullable<T>` is used.

# Ascii

A sequence of bytes in the ASCII range `[0, 127]`.<br/>
For now there no client side validation (for performance).<br/>

Example:

``` c++
cql::Ascii value("abc");
value->append("de", 2);
std::size_t size = value->size(); // 5
```

# BigInt

A 8 byte two's complement integer.

Example:

``` c++
cql::BigInt value(1);
value += 123;
```

# Blob

Any sequence of bytes.

Example:

``` c++
cql::Blob value("abc");
value->append("de", 2);
std::cout << value->size() << std::endl; // 5
```

# Boolean

A single byte.

Example:

``` c++
cql::Boolean value(true);
if (value) {
	// ...
}
```

# Counter

Same as "bigint", but able to perform increment and decrement.

Example:

``` c++
cql::Counter value(1);
value += 123;
```

# Date

An unsigned integer representing days with epoch centered at 2^31.<br/>
(unix epoch January 1st, 1970).<br/>

Example:

``` c++
cql::Date value(std::chrono::system_clock::now()); // in local timezone
value = cql::Date::create(1970, 1, 15); // in local timezone
value = cql::Date::today(); // in local timezone
```

# Decimal

The decimal format represents an arbitrary-precision number.

Example:

``` c++
cql::Decimal value(2, 123);
std::cout << value << std::endl; // 1.23
```

# Double

A 8 byte floating point number in the IEEE 754 binary64 format.

Example:

``` c++
cql::Double value(0.001);
value += 0.123;
```

# Float

A 4 byte floating point number in the IEEE 754 binary32 format.

Example:

``` c++
cql::Float value(0.001);
value += 0.123;
```

# Inet

A 4 byte or 16 byte sequence denoting an IPv4 or IPv6 address.

Example:

``` c++
cql::Inet value("127.0.0.1");
```

# Int

A 4 byte two's complement integer.

Example:

``` c++
cql::Int value(1);
value += 123;
```

# List<T>

A list store repeatable values.

Example:

``` c++
cql::List<cql::BigInt> value({ 1, 2 });
std::cout << value->size() << std::endl; // 2
```

# Map<TKey, TValue>

A map store key-value pairs.

Example:

``` c++
cql::Map<cql::Text, cql::BigInt> value({ { "abc", 123 } });
value->emplace("asd", 321);
std::cout << value->size() << std::endl; // 2
```

# MemRef

Memory reference, it doesn't have the owership of the target.

Example:

``` c++
cql::MemRef value("abc");
```

# Nullable<T>

Wrapper class that able to represent a null value.</br>
For convenient and performance, getting underlying value will not perform null check,</br>
to check it is null explicitly, use function isNull</br>

Example:

``` c++
cql::Nullable<cql::Int> value;
std::cout << *value << std::endl; // 0
std::cout << value.isNull() << std::endl; // true
value.set(123);
std::cout << *value << std::endl; // 123
std::cout << value.isNull() << std::endl; // false
```

# Set<T>

A set store disinct values.

Example:

``` c++
cql::Set<cql::BigInt> value({ 1, 2 });
std::cout << value->size() << std::endl; // 2
```

# SmallInt

A 2 byte two's complement integer.

Example:

``` c++
cql::SmallInt value(1);
value += 123;
```

# Text

A sequence of bytes of conforming to the UTF-8 specifications.<br/>
For now there no client side validation (for performance).<br/>
Notice: "varchar" is an alias of the "text" type.<br/>

Example:

``` c++
cql::Text value("abc");
value->append("de", 2);
std::cout << value->size() << std::endl; // 5
```

# Time

An 8 byte two's complement long representing nanoseconds since midnignt.<br/>
Valid values are in the range 0 to 86'3999'9999'9999.<br/>

Example:

``` c++
cql::Time value(std::chrono::system_clock::now()); // in local timezone
value = cql::Time::create(1, 2, 3); // in local timezone
value = cql::Time::now(); // in local timezone
```

# Timestamp

An 8 byte two's complement integer representing a millisecond-precision<br/>
offset from the unix epoch (00:00:00, January 1st, 1970).<br/>
Negative values represent a negative offset from the epoch.<br/>

Example:

``` c++
cql::Timestamp value(std::chrono::system_clock::now()); // in utc
value = cql::Timestamp::create(1970, 1, 2, 1, 2, 3); // in local timezone
value = cql::Timestamp::now(); // in utc
value += std::chrono::hours(1);
value += std::chrono::minutes(30);
```

# TimeUUID

A 16 byte sequence representing a version 1 UUID as defined by RFC 4122.

Example:

``` c++
cql::TimeUUID uuid;
std::cout << uuid.empty() << std::endl; // true
uuid = cql::TimeUUID::create();
std::cout << uuid.empty() << std::endl; // false
```

# TinyInt

A 1 byte two's complement integer.

Example:

``` c++
cql::TinyInt value(1);
value += 123;
```

# Tuple<Types...>

A tuple store different types values.

Example:

``` c++
cql::Tuple<cql::Int, cql::Text> value(123, "abc");
std::cout << std::get<0>(*value) << std::endl; // 123
std::cout << std::get<1>(*value) << std::endl; // abc
std::cout << value.get<0>() << std::endl; // 123
std::cout << value.get<1>() << std::endl; // abc
```

# UDT<Fields...>

A user defined type store different types named values.

Example:

``` c++
struct IdField { using Type = cql::Int; static const constexpr char* Name = "id"; };
struct NameField { using Type = cql::Text; static const constexpr char* Name = "name"; };
using UDTType = cql::UDT<IdField, NameField>;
UDTType value(123, "abc");
std::cout << std::get<0>(*value) << std::endl; // 123
std::cout << std::get<1>(*value) << std::endl; // abc
std::cout << value.get<0>() << std::endl; // 123
std::cout << value.get<1>() << std::endl; // abc
```

# UUID

A 16 byte sequence representing any valid UUID as defined by RFC 4122.

Example:

``` c++
cql::UUID uuid;
std::cout << uuid.empty() << std::endl; // true
uuid = "00112233-4455-6677-8899-AABBCCDDEEFF";
std::cout << uuid.empty() << std::endl; // false
uuid = cql::UUID::create(); // create a version 4 (random) uuid
```

# VarInt

A variable length two's complement encoding of a signed integer.<br/>
This class can't store a variable integer that std::int64_t can't hold,<br/>
I should provide a real varint later, but for now this is easier to implement and have better performance.<br/>

Example:

``` c++
cql::VarInt value(1);
value += 123;
```

