#include <QTimer>
#include <QLCDNumber>
#include <QChar>
#include <QKeyEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <cmath>
#include <iostream>
#include "MainWindow.hpp"
#include "AppContainer.hpp"
#include "LED.hpp"
#include "Log.hpp"
#include "globals.hpp"


MainWindow::MainWindow(AppContainer& app, QWidget* parent) : QMainWindow(parent), m_app(app)
{
	setup_widget();
	setup_number();
	setup_log();
	setup_label();
	setup_led();
	setup_layout();
	setup_updater(VALUE_UPDATE_INTERVAL_MS);
}

MainWindow::~MainWindow()
{
	//Qt automatically deletes childs
	delete m_widget;
	delete m_timer;
}

void MainWindow::log_message_cc(const char* message)
{
	QString msg(message);
	m_log->appendMessage(msg);
}

void MainWindow::log_message_i(int number)
{
	QString msg;
	msg.setNum(number);
	m_log->appendMessage(msg);
}

void MainWindow::log_message_d(double number)
{
	QString msg;
	msg.setNum(number);
	m_log->appendMessage(msg);
}

void MainWindow::update_bpm()
{
	double rmsVal = m_app.fetch_rmsValue();
	set_number(m_rmsNumber, fabs(rmsVal), 5, 0);
	if (rmsVal < 0)
	{
		set_color(m_rmsNumber, Qt::red);
		return;
	}
	set_color(m_rmsNumber, Qt::green);
	double bpmVal = m_app.fetch_bpmValue();
	set_number(m_bpmNumber, bpmVal, 4, 1);
}

void MainWindow::update_rec()
{
	Errors_e recRetval = m_app.fetch_recRetval();
	if (recRetval != Errors_e::NO_ERROR)
	{
		stop_application();
	}

	m_recLed->setPower(!m_app.get_status());
	m_detLed->setPower(m_app.get_status());
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Q)
	{
		stop_application();
	}

	if (event->key() == Qt::Key_S)
	{
		m_app.stop_detection();
	}

	if (event->key() == Qt::Key_C)
	{
		m_app.start_detection();
	}
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	Q_UNUSED(event)
	stop_application();
}

void MainWindow::setup_widget()
{
	set_color(this, Qt::green);
	m_widget = new QWidget;
	setCentralWidget(m_widget);
}

void MainWindow::setup_layout()
{
    m_topLayout = new QVBoxLayout(m_widget);

    m_l1BotLayout = new QHBoxLayout;
    m_l2LeftLayout = new QVBoxLayout;
    m_l2RightLayout = new QHBoxLayout;
    m_l3LeftTopLayout = new QHBoxLayout;
    m_l3LeftBotLayout = new QHBoxLayout;

    m_topLayout->addWidget(m_bpmNumber);
    m_topLayout->addLayout(m_l1BotLayout);
    m_topLayout->addWidget(m_log);
    m_topLayout->setStretch(0, LAYOUT_STRETCH_TOP_BPM);
    m_topLayout->setStretch(1, LAYOUT_STRETCH_CNT_STAT);
    m_topLayout->setStretch(2, LAYOUT_STRETCH_BOT_LOG);

    m_l1BotLayout->addLayout(m_l2LeftLayout);
    m_l1BotLayout->addLayout(m_l2RightLayout);
    m_l1BotLayout->setStretch(0, LAYOUT_STRETCH_LT_STAT);
    m_l1BotLayout->setStretch(1, LAYOUT_STRETCH_RT_STAT);

    m_l2RightLayout->addWidget(m_rmsLabel);
    m_l2RightLayout->addWidget(m_rmsNumber);
    m_l2RightLayout->setStretch(0, LAYOUT_STRETCH_RMS_LT);
    m_l2RightLayout->setStretch(1, LAYOUT_STRETCH_RMS_RT);

    m_l2LeftLayout->addLayout(m_l3LeftTopLayout);
    m_l2LeftLayout->addLayout(m_l3LeftBotLayout);

    m_l3LeftTopLayout->addWidget(m_recLed);
    m_l3LeftTopLayout->addWidget(m_recLabel);
    m_l3LeftTopLayout->setStretch(0, LAYOUT_STRETCH_LED_LT);
    m_l3LeftTopLayout->setStretch(1, LAYOUT_STRETCH_LED_RT);
    m_l3LeftBotLayout->addWidget(m_detLed);
    m_l3LeftBotLayout->addWidget(m_detLabel);
    m_l3LeftBotLayout->setStretch(0, LAYOUT_STRETCH_LED_LT);
    m_l3LeftBotLayout->setStretch(1, LAYOUT_STRETCH_LED_RT);

    m_widget->setLayout(m_topLayout);
}

void MainWindow::setup_number()
{
	m_bpmNumber = new QLCDNumber(m_widget);
	m_bpmNumber->setSegmentStyle(QLCDNumber::Filled);
	m_bpmNumber->setDigitCount(5);
	m_bpmNumber->setAutoFillBackground(true);

	m_rmsNumber = new QLCDNumber(m_widget);
	m_rmsNumber->setSegmentStyle(QLCDNumber::Filled);
	m_rmsNumber->setDigitCount(5);
	m_rmsNumber->setAutoFillBackground(true);

	set_number(m_bpmNumber, 0, 4, 1);
	set_number(m_rmsNumber, 0, 5, 0);
}

void MainWindow::setup_log()
{
    m_log = new Log(m_widget);
}

void MainWindow::setup_label()
{
	m_recLabel = new QLabel(" Recording");
	m_detLabel = new QLabel(" Detecting");
	m_rmsLabel = new QLabel("RMS");

	QFont font = m_recLabel->font();
	font.setPointSize(FONT_SIZE);
	font.setBold(true);
	m_recLabel->setFont(font);
	m_detLabel->setFont(font);
	m_rmsLabel->setFont(font);

	set_color(m_recLabel, Qt::white);
	m_recLabel->setFrameStyle(QFrame::Box | QFrame::Plain);
	set_color(m_detLabel, Qt::white);
	m_detLabel->setFrameStyle(QFrame::Box | QFrame::Plain);
	set_color(m_rmsLabel, Qt::white);
	m_rmsLabel->setFrameStyle(QFrame::Box | QFrame::Plain);
}

void MainWindow::setup_led()
{
    m_recLed = new RoundLed(m_widget);
    m_recLed->setPower(false);
    m_recLed->setFrameStyle(QFrame::Box | QFrame::Plain);

    m_detLed = new RoundLed(m_widget);
    m_detLed->setPower(false);
    m_detLed->setFrameStyle(QFrame::Box | QFrame::Plain);
}

void MainWindow::setup_updater(int interval_ms)
{
    m_timer = new QTimer(this);
    connect (m_timer, SIGNAL( timeout() ), this, SLOT( update_bpm() ) );
    connect (m_timer, SIGNAL( timeout() ), this, SLOT( update_rec() ) );
    m_timer->start(interval_ms);
}

void MainWindow::stop_application()
{
	m_app.stop_detection();
	QCoreApplication::quit();
}

void MainWindow::convert_to_string(double number, QString& str, int digits, int precision)
{
	int digit[digits] = { 0 };
	double sum = 0;
	int signfDigits = digits - precision - 1;
	double ex = pow(10, -signfDigits);
	str.clear();
	for (int i = 0; i < digits; ++i)
	{
		digit[i] = static_cast<int>((number - sum) * ex);
		sum += digit[i] / ex;
		ex *= 10;
		str.append(QString('0' + digit[i]));
		if ((i == signfDigits) && (precision != 0))
			str.append(QString("."));
	}
}

void MainWindow::set_color(QWidget* widget, Qt::GlobalColor color)
{
	QPalette Pal = palette();
	Pal.setColor(QPalette::Normal, QPalette::WindowText, color);
	Pal.setColor(QPalette::Normal, QPalette::Window, Qt::black);
	widget->setPalette(Pal);
}

void MainWindow::set_number(QLCDNumber* lcdnbr, double number, int digits, int precision)
{
	QString str;
	convert_to_string(number, str, digits, precision);
	lcdnbr->display(str);
}
