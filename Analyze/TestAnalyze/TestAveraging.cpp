#include <gtest/gtest.h>

#include "Averaging.h"


const int TESTSIZE = 10;
const double THRESHOLD1 = 3.0;
const double THRESHOLD2 = 20.0;
const int MAX_BAD_VALUES = 3;


class TestAveraging : public testing::Test
{
public:
	double TESTVALUE1 = 10.0;
	double TESTVALUE2 = 9.0;
	double AVGVALUE1 = 9.9;

	double margin = 0.01;

	double TESTVALUE3 = TESTVALUE1 + THRESHOLD1 + margin;
	double TESTVALUE4 = TESTVALUE1 + THRESHOLD2 + margin;

	void SetUp() override
	{

	}

	void TearDown() override
	{

	}
};


TEST_F(TestAveraging, CreatesInstance)
{
	Averaging<TESTSIZE> averaging(THRESHOLD1, THRESHOLD2, MAX_BAD_VALUES);
	(void)averaging; // prevent unused warning
}

TEST_F(TestAveraging, FirstProcessingReturnsEqualNumber)
{
	Averaging<TESTSIZE> averaging(THRESHOLD1, THRESHOLD2, MAX_BAD_VALUES);

	ASSERT_EQ(averaging.process_value(TESTVALUE1), TESTVALUE1);
}

TEST_F(TestAveraging, ReturnsAverageOfArray)
{
	Averaging<TESTSIZE> averaging(THRESHOLD1, THRESHOLD2, MAX_BAD_VALUES);

	averaging.process_value(TESTVALUE1);

	ASSERT_EQ(averaging.process_value(TESTVALUE2), AVGVALUE1);
}

TEST_F(TestAveraging, ReinitializesArrayIfChangeAboveThreshold1)
{
	Averaging<TESTSIZE> averaging(THRESHOLD1, THRESHOLD2, MAX_BAD_VALUES);

	averaging.process_value(TESTVALUE1);

	ASSERT_EQ(averaging.process_value(TESTVALUE3), TESTVALUE3);
}

TEST_F(TestAveraging, ThrowsMaxBadValues)
{
	Averaging<TESTSIZE> averaging(THRESHOLD1, THRESHOLD2, MAX_BAD_VALUES);

	averaging.process_value(TESTVALUE1);
	for (int i = 0; i < MAX_BAD_VALUES - 1; ++i)
		averaging.process_value(TESTVALUE4);

	ASSERT_EQ(averaging.process_value(TESTVALUE4), TESTVALUE1);
}

TEST_F(TestAveraging, ReinitializesArrayIfMaxBadValuesReached)
{
	Averaging<TESTSIZE> averaging(THRESHOLD1, THRESHOLD2, MAX_BAD_VALUES);

	averaging.process_value(TESTVALUE1);
	for (int i = 0; i < MAX_BAD_VALUES; ++i)
		averaging.process_value(TESTVALUE4);

	ASSERT_EQ(averaging.process_value(TESTVALUE4), TESTVALUE4);
}
