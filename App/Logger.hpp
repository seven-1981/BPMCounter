#ifndef _LOGGER_H
#define _LOGGER_H

#include <QObject>

class MainWindow;


class Logger : public QObject
{
	Q_OBJECT

public:
	static Logger& logger()
	{
		static Logger logger;
		return logger;
	}

	void init_logger(MainWindow* w);

	Logger& operator<<(const char* text);
	Logger& operator<<(int number);
	Logger& operator<<(double number);

signals:
	void log_signal_cc(const char* text);
	void log_signal_i(int number);
	void log_signal_d(double number);

private:
	Logger(QObject* parent = nullptr);

	bool m_initDone;
};


#endif
