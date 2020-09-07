#ifndef _LOG_H
#define _LOG_H

#include <QPlainTextEdit>
#include <QString>
#include <QFile>
#include <QScrollBar>


class Log : public QPlainTextEdit
{
	Q_OBJECT

public:
	explicit Log(QWidget* parent = nullptr);
	~Log();

    void appendMessage(const QString& text);

private:
    QFile m_logFile;

    void appendTimestamp(QString& text);
};

#endif
