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
	std::string str(text);
	if (check_initDone(str) == false)
		return *this;

	emit log_signal_cc(text);
	return *this;
}

Logger& Logger::operator<<(int number)
{
	std::string str = std::to_string(number);
	if (check_initDone(str) == false)
		return *this;

	emit log_signal_i(number);
	return *this;
}

Logger& Logger::operator<<(double number)
{
	std::string str = std::to_string(number);
	if (check_initDone(str) == false)
		return *this;

	emit log_signal_d(number);
	return *this;
}

bool Logger::check_initDone(const std::string& str)
{
	if (m_initDone == false)
	{
		std::cout << "Logger not yet initialized... Message = '" << str << "'" << std::endl;
		return false;
	}
	return true;
}
