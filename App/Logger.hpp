#ifndef _LOGGER_H
#define _LOGGER_H

#include <QObject>
#include <QVariant>

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
	void log_signal(QVariant message);

private:
	Logger(QObject* parent = nullptr);

	bool check_initDone(const QString& str);
	Logger& do_log(QString& str);

	bool m_initDone;
};


#endif
