#include "Log.hpp"


Log::Log(QWidget* parent) :
	QPlainTextEdit(parent)
{
	QPlainTextEdit::setReadOnly(true);
	//QPlainTextEdit::setAutoFillBackground(true);

	QPalette p = palette();
	p.setColor(QPalette::Base, Qt::black);
	p.setColor(QPalette::Text, Qt::green);
	setPalette(p);

	QFont font = QPlainTextEdit::font();
	font.setPointSize(10);
	QPlainTextEdit::setFont(font);
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
