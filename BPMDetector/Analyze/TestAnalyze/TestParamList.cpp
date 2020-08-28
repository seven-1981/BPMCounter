#include <gtest/gtest.h>
#include "ParamList.hpp"
#include "Param.hpp"


class TestParamList : public testing::Test
{
public:
	ParamList list;
	Param* param1;
	Param* param2;
	Param* param3;
	Param* param4;
	Param* param5;

    void SetUp() override
    {
    	param1 = new TypedParam<ParamType::Bool>("My bool", true);
    	param2 = new TypedParam<ParamType::Float>("My float", -3.1f);
    	param3 = new TypedParam<ParamType::Int>("My int", 55);
    	param4 = new TypedParam<ParamType::ConstChar>("My char", "My char param");
    	param5 = new TypedParam<ParamType::String>("My string", "My string param");

    	list.add(param1)
    		.add(param2)
			.add(param3)
			.add(param4)
			.add(param5);
    }

    void TearDown() override
    {
    	delete param1;
    	delete param2;
    	delete param3;
    	delete param4;
    	delete param5;
    }
};


TEST_F(TestParamList, CreatesInstance)
{
	ParamList list;
}

TEST_F(TestParamList, AddsParametersToList)
{
	ParamList list;
	TypedParam<ParamType::Float> param1("My float", -3.1f);
	TypedParam<ParamType::Int> param2("My int", 55);

	list.add(&param1);
	list.add(&param2);

	ASSERT_EQ(list.size(), 2);
}

TEST_F(TestParamList, GetsBoolParamFromList)
{
	bool value = false;
	int success = list.get("My bool", value);

	ASSERT_EQ(success, 0);
	ASSERT_EQ(value, true);
}

TEST_F(TestParamList, GetsFloatParamFromList)
{
	float value = 0.0f;
	int success = list.get("My float", value);

	ASSERT_EQ(success, 0);
	ASSERT_NEAR(value, -3.1f, 0.1);
}

TEST_F(TestParamList, GetsIntParamFromList)
{
	int value = 0;
	int success = list.get("My int", value);

	ASSERT_EQ(success, 0);
	ASSERT_EQ(value, 55);
}

TEST_F(TestParamList, GetsCharParamFromList)
{
	const char* value = "";
	int success = list.get("My char", value);

	ASSERT_EQ(success, 0);
	ASSERT_EQ(value, "My char param");
}

TEST_F(TestParamList, GetsStringParamFromList)
{
	std::string value = "";
	int success = list.get("My string", value);

	ASSERT_EQ(success, 0);
	ASSERT_EQ(value, "My string param");
}

TEST_F(TestParamList, ReturnsMinusOneIfParamNotFound)
{
	int value = 0;
	int success = list.get("My test", value);

	ASSERT_EQ(success, -1);
}
