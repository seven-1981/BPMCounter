#include <gtest/gtest.h>
#include "Param.hpp"


class TestParam : public testing::Test
{
public:

    void SetUp() override { }
    void TearDown() override { }
};


TEST_F(TestParam, CreatesInstance)
{
	TypedParam<ParamType::Bool> tpb("My param", false);
}

TEST_F(TestParam, GetNameWorksProperly)
{
	TypedParam<ParamType::Bool> tpb("My param", false);

	ASSERT_EQ(tpb.name(), "My param");
}

TEST_F(TestParam, SetAndGetBool)
{
	TypedParam<ParamType::Bool> tpb("My param", false);

	tpb.set(true);

	ASSERT_EQ(tpb.type(), ParamType::Bool);
	ASSERT_EQ(tpb.get(), true);
}

TEST_F(TestParam, SetAndGetInt)
{
	TypedParam<ParamType::Int> tpi("My param", 0);

	tpi.set(77);

	ASSERT_EQ(tpi.type(), ParamType::Int);
	ASSERT_EQ(tpi.get(), 77);
}

TEST_F(TestParam, SetAndGetUnsignedInt)
{
	TypedParam<ParamType::UInt> tpu("My param", 0U);

	tpu.set(55U);

	ASSERT_EQ(tpu.type(), ParamType::UInt);
	ASSERT_EQ(tpu.get(), 55U);
}

TEST_F(TestParam, SetAndGetFloat)
{
	TypedParam<ParamType::Float> tpf("My param", 0.0f);

	tpf.set(8.7f);

	ASSERT_EQ(tpf.type(), ParamType::Float);
	ASSERT_NEAR(tpf.get(), 8.7f, 0.1);
}

TEST_F(TestParam, SetAndGetDouble)
{
	TypedParam<ParamType::Double> tpd("My param", 0.0L);

	tpd.set(-11.2);

	ASSERT_EQ(tpd.type(), ParamType::Double);
	ASSERT_NEAR(tpd.get(), -11.2, 0.1);
}

TEST_F(TestParam, SetAndGetString)
{
	std::string init = "init";
	std::string test = "test";
	TypedParam<ParamType::String> tps("My param", init);

	tps.set(test);

	ASSERT_EQ(tps.type(), ParamType::String);
	ASSERT_EQ(tps.get(), test);
}

TEST_F(TestParam, SetAndGetConstChar)
{
	TypedParam<ParamType::ConstChar> tpc("My param", "init");

	tpc.set("test");

	ASSERT_EQ(tpc.type(), ParamType::ConstChar);
	ASSERT_EQ(tpc.get(), "test");
}
