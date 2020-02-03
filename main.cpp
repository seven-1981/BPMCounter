#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
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

void recording_thread()
{
	while(1);
}

int main (int argc, char **argv)
{
	clear_screen();
	ALSACardConfiguration_t config;
	AudioController controller(config);
	std::cout << "Config = " << (int)controller.config_audio(config) << std::endl;
	//std::cout << "Starting = " << (int)controller.start_asynchronous_record() << std::endl;
	//std::this_thread::sleep_for(std::chrono::seconds(1));

	Buffer<short signed int> abuffer(config.PCM_BUFFER_SIZE);

	AnalyzerController<short signed int> analyzer(abuffer);
	std::cout << "Record = " << (int)controller.synchronous_record(abuffer) << std::endl;
	double bpm = analyzer.analyze();
	std::cout << "Value = " << bpm << std::endl;

	//write_wavfile(abuffer);
	////while (cycle++ < 1000)
	//{
	//	controller.read_asynchronous_samples(abuffer);
	//}
	//std::cout << "Stopping = " << (int)controller.stop_asynchronous_record() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));

//    QApplication a(argc, argv);
//    MainWindow w;
//    //w.setBaseSize(800, 600);
//    w.showFullScreen();
//    w.show();
//    return a.exec();
	return 0;
}
