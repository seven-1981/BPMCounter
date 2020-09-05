#include "Logger.hpp"
#include "MainWindow.hpp"


Logger::Logger(QObject* parent) :
	m_initDone(false) { }

void Logger::init_logger(MainWindow* w)
{
	connect(this, &Logger::log_signal, w, &MainWindow::log_message);
	m_initDone = true;
}

void Logger::log(const char* text)
{
	if (m_initDone == false) return;
	emit log_signal(text);
}
