#include "Logger.hpp"
#include "MainWindow.hpp"


Logger::Logger() :
	m_mainWindow(nullptr)
{

}

void Logger::init_logger(MainWindow* w)
{
	m_mainWindow = w;
}

void Logger::log(const char* text)
{
	if (m_mainWindow == nullptr) return;
	m_mainWindow->log_message(text);
}
