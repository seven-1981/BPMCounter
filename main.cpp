#include <iostream>
#include <vector>
#include <string>
#include <cmath> 
#include <fstream>
#include <thread>
#include <chrono>
#include "AudioController.hpp"
#include "ALSACardConfiguration.hpp"
#include "Buffer.hpp"
#include "WavFile.hpp"
#include "AnalyzerController.hpp"
#include "DSP.hpp"

#include <qt5/QtWidgets/qapplication.h>
#include "MainWindow.hpp"


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

int main (int argc, char **argv)
{
	clear_screen();
	ALSACardConfiguration_t config;
	AudioController controller(config);
	std::cout << "Config = " << (int)controller.config_audio(config) << std::endl;
	//std::cout << "Starting = " << (int)controller.start_asynchronous_record() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));

	//int cycle = 0;
	Buffer<short signed int> abuffer(config.PCM_BUFFER_SIZE);
	AnalyzerController<short signed int> analyzer(abuffer);
	std::cout << "Record = " << (int)controller.synchronous_record(abuffer) << std::endl;
	std::cout << "Value = " << analyzer.analyze() << std::endl;
	Buffer<float> b(88200);
	DSP::convert_buffer(abuffer, b);
	//PLOT::plot(b, "Audio data");
//	while (cycle++ < 1000)
//	{
//		controller.read_asynchronous_samples(abuffer);
//	}
//	std::cout << "Stopping = " << (int)controller.stop_asynchronous_record() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));

//	Buffer<short signed int> buffer;
//	std::cout << "Capture = " << (int)recorder.capture_samples(config, buffer) << std::endl;
//
//	WavHeader_t header;
//	std::fstream fs("/home/pi/eclipse-workspace/BPMDetector/record.wav", std::ios_base::out | std::ios_base::binary);
//	WavFile wavfile(fs);
//	wavfile.write_header(header);
//	for (unsigned int i = 0; i < buffer.get_size(); ++i)
//	{
//		wavfile << buffer[i];
//	}
//	wavfile.finish();
//	fs.close();
	return 0;
//    QApplication a(argc, argv);
//    MainWindow w;
//   // w.setBaseSize(800, 600);
//    w.showFullScreen();
//    w.show();
//    return a.exec();
}
