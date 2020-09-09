#include "Log.hpp"

#include <QTime>


Log::Log(QWidget* parent) :
	QPlainTextEdit(parent),
	m_logFile(LOGFILE_PATH),
	m_crlfFound(false)
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
	handle_crlf(textToLog);

	this->insertPlainText(textToLog); // Adds the message to the widget
	if (is_scrollbarAtBottom() == true)
		this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom

	m_logFile.write(textToLog.toStdString().c_str()); // Logs to file
}

void Log::appendTimestamp(QString& text)
{
	QString actTime = QTime::currentTime().toString("hh:mm:ss,zzz");
	text = "<" + actTime + "> " + text;
}

bool Log::is_crlfAppended(const QString& str)
{
	// 2nd argument is start pos, -1 causes to start at last character.
	// Returns -1 if character not found.
	return (str.indexOf("\n", -1) != -1);
}

void Log::handle_crlf(QString& str)
{
	if (m_crlfFound == true)
	{
		m_crlfFound = false;
		appendTimestamp(str);
	}

	if (is_crlfAppended(str) == true)
		m_crlfFound = true;
}

bool Log::is_scrollbarAtBottom()
{
	QScrollBar *scrollbar = this->verticalScrollBar();
	return (scrollbar->value() >= (scrollbar->maximum() - 4));
}
