#include "Log.hpp"


Log::Log(QWidget* parent) :
	QPlainTextEdit(parent)
{
	QPlainTextEdit::setReadOnly(true);
}

Log::~Log()
{

}

void Log::appendMessage(const QString& text)
{
	this->appendPlainText(text); // Adds the message to the widget
	this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom
	m_logFile.write(text.toStdString().c_str()); // Logs to file
}
