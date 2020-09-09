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
	bool m_crlfFound;

    void appendTimestamp(QString& text);

	bool is_crlfAppended(const QString& str);
	void handle_crlf(QString& str);

	bool is_scrollbarAtBottom();
};

#endif
