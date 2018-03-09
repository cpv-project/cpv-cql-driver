#include <CqlDriver/Common/Utility/EnumUtils.hpp>
#include <CqlDriver/Common/Utility/StringUtils.hpp>
#include <TestUtility/GTestUtils.hpp>

namespace cql {
	enum class MyTestEnum {
		None = 0,
		A = 1,
		B = 2,
		C = 4
	};

	template <>
	struct EnumDescriptions<MyTestEnum> {
		static const std::vector<std::pair<MyTestEnum, const char*>>& get() {
			static std::vector<std::pair<MyTestEnum, const char*>> staticNames({
				{ MyTestEnum::None, "None" },
				{ MyTestEnum::A, "A" },
				{ MyTestEnum::B, "B" },
				{ MyTestEnum::C, "C" }
			});
			return staticNames;
		}
	};
}

TEST(TestEnumUtils, operators) {
	{
		auto value = cql::MyTestEnum::A | cql::MyTestEnum::B;
		ASSERT_EQ(static_cast<std::size_t>(value), 3);
	}
	{
		auto value = cql::MyTestEnum::A;
		value |= cql::MyTestEnum::B;
		ASSERT_EQ(static_cast<std::size_t>(value), 3);
	}
	{
		auto value = cql::MyTestEnum::A & (cql::MyTestEnum::A | cql::MyTestEnum::B);
		ASSERT_EQ(static_cast<std::size_t>(value), 1);
	}
	{
		auto value = cql::MyTestEnum::A | cql::MyTestEnum::B;
		value &= cql::MyTestEnum::A;
		ASSERT_EQ(static_cast<std::size_t>(value), 1);
	}
	{
		auto value = ~cql::MyTestEnum::A & (cql::MyTestEnum::A | cql::MyTestEnum::B);
		ASSERT_EQ(static_cast<std::size_t>(value), 2);
	}
	{
		auto value = cql::MyTestEnum::A | cql::MyTestEnum::B;
		value &= cql::MyTestEnum::B;
		ASSERT_EQ(static_cast<std::size_t>(value), 2);
	}
	{
		auto value = ~cql::MyTestEnum::A;
		ASSERT_EQ(static_cast<std::size_t>(value), ~1);
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
	{
		ASSERT_EQ(cql::joinString("", cql::MyTestEnum::None), "None");
		ASSERT_EQ(cql::joinString("", cql::MyTestEnum::A | cql::MyTestEnum::B), "A|B");
	}
}

