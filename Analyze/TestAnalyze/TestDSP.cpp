#include "Buffer.hpp"
#include "DSP.hpp"
#include <gtest/gtest.h>
#include <cmath>

using TEST_TYPE = short signed int;
using CHAR_TYPE = signed char;
using FLOAT_TYPE = DSP::FLOAT_TYPE;


#define ASSERT_BUFFER_EQ(a, b) for (SIZE_TYPE i = 0; i < a.get_size(); ++i) \
EXPECT_EQ(a[i], b[i]);


class TestDSP : public testing::Test
{
public:
    Buffer<TEST_TYPE> buffer;
    const unsigned int TEST_LENGHT = 16;

    Buffer<CHAR_TYPE> buffer_char;
    Buffer<CHAR_TYPE> buffer_maxed;
    Buffer<CHAR_TYPE> combined_buffer;
    Buffer<CHAR_TYPE> buffer_gain;
    Buffer<float> buffer_convert;
    Buffer<CHAR_TYPE> buffer_rect;
    const unsigned int CHAR_LENGHT = 5;

    void SetUp() override
    {
    	buffer = { -32768,-30629,-2085,-1755,0,1810,1812,1936,2030,2059,10161,17845,26020,30555,30705,32767 };
    	buffer_char = { -105, 55, 0, 104, -4 };
    	buffer_maxed = { -128, 67, 0, 126, -4 };
    	buffer_gain = { 52, -27, 0, -52, 2 };
    	combined_buffer = { -105, 52, 55, -27, 0, 0, 104, -52, -4, 2 };
    	buffer_convert = { 52.0, -27.0, 0.0, -52.0, 2.0 };
    	buffer_rect = { 52, 27, 0, 52, 2 };
    }

};

TEST_F(TestDSP, Swap)
{
	int a = -77;
	int b = 66;

	DSP::SWAP(a, b);

	ASSERT_EQ(a, 66);
	ASSERT_EQ(b, -77);
}
 
TEST_F(TestDSP, GetMaxValue)
{
	TEST_TYPE max_val = DSP::get_max_value(buffer);

	ASSERT_EQ(max_val, 0x7fff);
}

TEST_F(TestDSP, GetMinValue)
{
	TEST_TYPE min_val = DSP::get_min_value(buffer);

	ASSERT_EQ(min_val, -0x8000);
}

TEST_F(TestDSP, FindMaxIndex)
{
	TEST_TYPE max_index = DSP::get_max_index(buffer);

	ASSERT_EQ(max_index, 15);
}

TEST_F(TestDSP, Maximize)
{
	DSP::maximize(buffer_char);

	ASSERT_BUFFER_EQ(buffer_char, buffer_maxed);
}

TEST_F(TestDSP, Gain)
{
	DSP::gain(buffer_char, -0.5);

	ASSERT_BUFFER_EQ(buffer_char, buffer_gain);
}

TEST_F(TestDSP, Mean)
{
	FLOAT_TYPE mean_value = DSP::get_mean_value(buffer);

	ASSERT_FLOAT_EQ(mean_value, 5653.9375);
}

TEST_F(TestDSP, Variance)
{
	FLOAT_TYPE var_value = DSP::get_variance_value(buffer);

	ASSERT_FLOAT_EQ(var_value, 3.7168749e8);
}

TEST_F(TestDSP, Combine)
{
	Buffer<CHAR_TYPE> combined(2 * CHAR_LENGHT);

	DSP::combine_buffers(buffer_char, buffer_gain, combined);

	ASSERT_BUFFER_EQ(combined, combined_buffer);
}

TEST_F(TestDSP, Separate)
{
	Buffer<CHAR_TYPE> combined(2 * CHAR_LENGHT);
	DSP::combine_buffers(buffer_char, buffer_gain, combined);
	Buffer<CHAR_TYPE> separated1(CHAR_LENGHT);
	Buffer<CHAR_TYPE> separated2(CHAR_LENGHT);

	DSP::separate_buffers(combined, separated1, separated2);

	ASSERT_BUFFER_EQ(separated1, buffer_char);
	ASSERT_BUFFER_EQ(separated2, buffer_gain);
}

TEST_F(TestDSP, Convert)
{
	Buffer<float> convert(CHAR_LENGHT);

	DSP::convert_buffer(buffer_gain, convert);

	ASSERT_BUFFER_EQ(convert, buffer_convert);
}

TEST_F(TestDSP, NormalizePos)
{
	CHAR_TYPE value = 33;
	DSP::normalize(buffer_gain, value);

	CHAR_TYPE max_value = DSP::get_max_value(buffer_gain);

	ASSERT_EQ(max_value, value);
}

TEST_F(TestDSP, NormalizeNeg)
{
	TEST_TYPE value = 444;
	DSP::normalize(buffer, value);

	TEST_TYPE min_value = DSP::get_min_value(buffer);

	ASSERT_EQ(min_value, -value);
}

TEST_F(TestDSP, Rectify)
{
	DSP::rectify(buffer_gain);

	ASSERT_BUFFER_EQ(buffer_gain, buffer_rect);
}

TEST_F(TestDSP, RMS)
{
	SIZE_TYPE size = 500;
	FLOAT_TYPE amplitude = 10.0;
	Buffer<FLOAT_TYPE> sinbuffer(size);

	for (SIZE_TYPE i = 0 ; i < size; ++i)
		sinbuffer[i] = amplitude * sin(i / 10.0);

	FLOAT_TYPE rms_value = DSP::get_rms_value(sinbuffer);

	ASSERT_NEAR(rms_value, amplitude / sqrt(2), 0.1f);
}

TEST_F(TestDSP, Downsample)
{
	Buffer<CHAR_TYPE> buffer = { 1, 2, 3, 4, 5, 6 };
	Buffer<CHAR_TYPE> buffer_downsampled = { 1, 3, 5 };
	Buffer<CHAR_TYPE> result(3);

	DSP::downsample_buffer(buffer, result, 2);

	ASSERT_BUFFER_EQ(buffer_downsampled, result);
}
