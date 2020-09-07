#include <iostream>
#include <string>

#include "Logger.hpp"
#include "MainWindow.hpp"


Logger::Logger(QObject* parent) :
	m_initDone(false) { }

void Logger::init_logger(MainWindow* w)
{
	connect(this, &Logger::log_signal_cc, w, &MainWindow::log_message_cc);
	connect(this, &Logger::log_signal_i, w, &MainWindow::log_message_i);
	connect(this, &Logger::log_signal_d, w, &MainWindow::log_message_d);
	m_initDone = true;
}

Logger& Logger::operator<<(const char* text)
{
	if (m_initDone == false)
	{
		std::string str(text);
		std::cout << "Logger not yet initialized... Msg = " << str << std::endl;
		return *this;
	}

	emit log_signal_cc(text);
	return *this;
}

Logger& Logger::operator<<(int number)
{
	if (m_initDone == false)
	{
		std::string str = std::to_string(number);
		std::cout << "Logger not yet initialized... Msg = " << str << std::endl;
		return *this;
	}

	emit log_signal_i(number);
	return *this;
}

Logger& Logger::operator<<(double number)
{
	if (m_initDone == false)
	{
		std::string str = std::to_string(number);
		std::cout << "Logger not yet initialized... Msg = " << str << std::endl;
		return *this;
	}

	emit log_signal_d(number);
	return *this;
}
