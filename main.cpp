#include <iostream>
#include <fstream>
#include <chrono>
#include <future>

#include "Buffer.hpp"
#include "WavFile.hpp"
#include "AppContainer.hpp"

#include <qt5/QtWidgets/qapplication.h>
#include "MainWindow.hpp"
#include "Log.hpp"


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

int main (int argc, char** argv)
{
	//clear_screen();

	AppContainer app;
    QApplication a(argc, argv);
    MainWindow w(app);
    Logger::logger().init_logger(&w);

	Errors_e retVal = Errors_e::NO_ERROR;
	if ((retVal = app.init()) != Errors_e::NO_ERROR)
	{
		std::cout << "Configuration error. Code = " << (int)retVal << "." << std::endl;
		exit((int)retVal);
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));

    //w.setBaseSize(800, 600);
    w.showFullScreen();
    w.show();
    return a.exec();
}
