#include <iostream>
#include <string>

#include "Logger.hpp"
#include "MainWindow.hpp"

#include <QString>


Logger::Logger(QObject* parent) :
	m_initDone(false) { }

void Logger::init_logger(MainWindow* w)
{
	connect(this, &Logger::log_signal, w, &MainWindow::log_message);
	m_initDone = true;
}

Logger& Logger::operator<<(const char* text)
{
	QString str(text);

	return do_log(str);
}

Logger& Logger::operator<<(int number)
{
	QString str;
	str.setNum(number);

	return do_log(str);
}

Logger& Logger::operator<<(double number)
{
	QString str;
	str.setNum(number);

	return do_log(str);
}

bool Logger::check_initDone(const QString& str)
{
	if (m_initDone == false)
	{
		std::cout << "Logger not yet initialized... Message = '" << str.toStdString() << "'" << std::endl;
		return false;
	}
	return true;
}

Logger& Logger::do_log(QString& str)
{
	if (check_initDone(str) == false)
		return *this;

	QVariant msg(str);
	emit log_signal(msg);
	return *this;
}
