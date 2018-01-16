#include <CqlDriver/Common/Utility/EnumUtils.hpp>
#include <TestUtility/GTestUtils.hpp>

namespace cql {
	enum class MyTestEnum {
		None = 0,
		A = 1,
		B = 2,
		C = 4
	};
}

TEST(TestEnumUtils, operators) {
	{
		auto value = cql::MyTestEnum::A | cql::MyTestEnum::B;
		ASSERT_EQ(static_cast<std::size_t>(value), 3);
	}
	{
		auto value = cql::MyTestEnum::A & (cql::MyTestEnum::A | cql::MyTestEnum::B);
		ASSERT_EQ(static_cast<std::size_t>(value), 1);
	}
	{
		auto value = ~cql::MyTestEnum::A & (cql::MyTestEnum::A | cql::MyTestEnum::B);
		ASSERT_EQ(static_cast<std::size_t>(value), 2);
	}
	{
		auto value = cql::MyTestEnum::A;
		ASSERT_TRUE(enumTrue(value));
	}
	{
		auto value = cql::MyTestEnum::None;
		ASSERT_TRUE(enumFalse(value));
	}
	{
		auto value = cql::MyTestEnum::A | cql::MyTestEnum::B;
		ASSERT_EQ(enumValue(value), 3);
	}
}

