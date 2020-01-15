#ifndef _WAVFILEWRITER_H
#define _WAVFILEWRITER_H

#include "Buffer.hpp"
#include "WavFile.hpp"

#include <string>
#include <fstream>


//Forward declarations
class WavFile;


class WavFileWriter
{
public:
    explicit WavFileWriter() : m_filename() { }
    explicit WavFileWriter(const std::string filename) : m_filename(filename) { }

    void set_name(std::string name) { m_filename = name; }
    std::string get_name() { return m_filename; }

    template <typename T>
    bool write(Buffer<T>& buffer)
    {
        unsigned int size = buffer.get_size();
        if (size == 0)
        {
            return false;
        }
        std::fstream fs(m_filename, std::ios_base::out | std::ios_base::binary);
        if (fs.is_open() == false)
        {
            return false;
        }
        write_wavFile(buffer, fs);
        return true;
    }

private:
    std::string m_filename;

    template <typename T>
    void write_wavFile(Buffer<T>& buffer, std::fstream& fs)
    {
        WavFile wavfile(fs);
        WavHeader_t header;
        wavfile.write_header(header);
        unsigned int size = buffer.get_size();
        for (unsigned int i = 0; i < size; ++i)
        {
            wavfile << buffer[i];
        }
        wavfile.finish();
        fs.close();
    }
};

#endif