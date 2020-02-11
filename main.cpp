#include <iostream>
#include <fstream>
#include <chrono>
#include <future>
#include <condition_variable>
#include <mutex>
#include "AudioController.hpp"
#include "AudioRecorder.hpp"
#include "ALSACardConfiguration.hpp"
#include "Buffer.hpp"
#include "WavFile.hpp"
#include "AnalyzerController.hpp"
#include "DSP.hpp"

#include <qt5/QtWidgets/qapplication.h>
#include "MainWindow.hpp"

#include "FCAsyncLauncher.hpp"

std::mutex mtx;
std::condition_variable cv;


void set_console_location(int col, int row)
{
	//We use special escape sequences to control the cursor
	//Note: this part is probably only Linux compatible
	std::cout << "\033[" << std::to_string(row) << ";" << std::to_string(col) << "H" << std::flush;
}

void clear_screen()
{
	//Clear screen
	std::cout << "\033[2J\033[1;1H";
}

template <typename T>
void write_wavfile(Buffer<T>& buffer)
{
	WavHeader_t header;
	std::fstream fs("/home/pi/eclipse-workspace/BPMDetector/record.wav", std::ios_base::out | std::ios_base::binary);
	WavFile wavfile(fs);
	wavfile.write_header(header);
	for (unsigned int i = 0; i < buffer.get_size(); ++i)
	{
		wavfile << buffer[i];
	}
	wavfile.finish();
	fs.close();
}

template <typename T>
void async_rec(AudioController& controller, ALSACardConfiguration_t& config, Buffer<T>& buffer)
{
	std::cout << "Starting = " << (int)controller.start_asynchronous_record() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	static int cycle = 0;
	while (cycle++ < 1000)
	{
		controller.read_asynchronous_samples(buffer);
	}
	std::cout << "Stopping = " << (int)controller.stop_asynchronous_record() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

template <typename T>
void detect_bpm(AudioController& controller, Buffer<T>& buffer)
{
	std::cout << "Detecting..." << std::endl;
	std::unique_lock<std::mutex> lck(mtx);
	cv.wait(lck);
	AnalyzerController<short signed int> analyzer(buffer);
	std::cout << "Record = " << (int)controller.synchronous_record(buffer) << std::endl;
	double bpm = analyzer.analyze();
	std::cout << "Value = " << bpm << std::endl;
}

int main (int argc, char** argv)
{
	clear_screen();

	ALSACardConfiguration_t config;
	AudioController controller(config);
	std::cout << "Config = " << (int)controller.config_audio(config) << std::endl;

	Buffer<short signed int> buffer(config.PCM_BUFFER_SIZE);
	//detect_bpm(controller, buffer);
	//write_wavfile(buffer);

	auto fut = std::async(std::launch::async, detect_bpm<short signed int>, std::ref(controller), std::ref(buffer));
	std::this_thread::sleep_for(std::chrono::seconds(1));
	{
		std::lock_guard<std::mutex> lg(mtx);
		cv.notify_one();
	}

    QApplication a(argc, argv);
    MainWindow w;
    //w.setBaseSize(800, 600);
    w.showFullScreen();
    w.show();
    return a.exec();
}
