#ifndef _LOGGER_H
#define _LOGGER_H

class MainWindow;


class Logger
{
public:
	static Logger& logger()
	{
		static Logger logger;
		return logger;
	}

	void init_logger(MainWindow* w);

	void log(const char* text);

private:
	Logger();

	MainWindow* m_mainWindow;
};


#endif
