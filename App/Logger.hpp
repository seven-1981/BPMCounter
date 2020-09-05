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
	void log(const char* text);

signals:
	void log_signal(const char* text);

private:
	Logger(QObject* parent = nullptr);

	bool m_initDone;
};


#endif
