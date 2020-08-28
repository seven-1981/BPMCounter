#include "WavFileWriter.hpp"
#include "Buffer.hpp"
#include "WavFile.hpp"
#include <gtest/gtest.h>


const short int SOME_SHORT_PCM_DATA[] = { 
    0x001C,     0x0000,     0x0504,     0x0706,     0x0908,     0x0B0A,     0x0D0C,     0x0F0E,
    0x1110,     0x1312,     0x1514,     0x1716,     0x1918,     0x1B1A,     0x1D1C,     0x1F1E };
const unsigned int PCM_DATA_SIZE = 16;
const std::string FILE_NAME = "wavfile.wav";


class TestWavFileWriter : public testing::Test
{
public:
    Buffer<signed short int> buffer;

    void SetUp() override
    {
        buffer.init(PCM_DATA_SIZE);
        for (unsigned int i = 0; i < PCM_DATA_SIZE; ++i)
        {
            buffer[i] = SOME_SHORT_PCM_DATA[i];
        }
    }
};

TEST_F(TestWavFileWriter, CreatesInstance)
{
    WavFileWriter writer;
}
 
TEST_F(TestWavFileWriter, CreatesInstanceFromFile)
{
    WavFileWriter writer(FILE_NAME);
}

TEST_F(TestWavFileWriter, SetsFileName)
{
    WavFileWriter writer;

    writer.set_name(FILE_NAME);

    ASSERT_EQ(writer.get_name(), FILE_NAME);
}

TEST_F(TestWavFileWriter, SLOW_WritesBufferToFile)
{
    WavFileWriter writer(FILE_NAME);

    ASSERT_EQ(writer.write(buffer), true);
}
