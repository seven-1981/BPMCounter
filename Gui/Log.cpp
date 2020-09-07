#include "Log.hpp"

#include <QTime>


Log::Log(QWidget* parent) :
	QPlainTextEdit(parent),
	m_logFile(LOGFILE_PATH)
{
	QPlainTextEdit::setReadOnly(true);

	QPalette p = palette();
	p.setColor(QPalette::Base, Qt::black);
	p.setColor(QPalette::Text, Qt::green);
	setPalette(p);

	QFont font("Monospace");
	font.setPointSize(12);
	QPlainTextEdit::setFont(font);

	m_logFile.open(QFile::Append);
}

Log::~Log()
{
	m_logFile.close();
}

void Log::appendMessage(const QString& text)
{
	QString textToLog = text;
	appendTimestamp(textToLog);
	this->appendPlainText(textToLog); // Adds the message to the widget
	this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom
	m_logFile.write(textToLog.toStdString().c_str()); // Logs to file
}

void Log::appendTimestamp(QString& text)
{
	QString actTime = QTime::currentTime().toString("hh:mm:ss,zzz");
	text = "<" + actTime + "> " + text;
}
