#include <QTimer>
#include <QLCDNumber>
#include <QChar>
#include <QKeyEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "MainWindow.hpp"
#include "AppContainer.hpp"
#include "LED.hpp"
#include "globals.hpp"


MainWindow::MainWindow(AppContainer& app, QWidget* parent) : QMainWindow(parent), m_app(app)
{
	setup_widget();
	setup_number();
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

void MainWindow::update_bpm()
{
	double bpmVal = m_app.fetch_bpmValue();
	QString str;
	convert_to_string(bpmVal, str);
	m_bpmNumber->display(str);
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
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	Q_UNUSED(event)
	stop_application();
}

void MainWindow::setup_widget()
{
	QPalette Pal = palette();
	Pal.setColor(QPalette::Normal, QPalette::WindowText, Qt::green);
	Pal.setColor(QPalette::Normal, QPalette::Window, Qt::black);
	setPalette(Pal);

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
    m_topLayout->setStretch(0, LAYOUT_STRETCH_TOP_BPM);
    m_topLayout->setStretch(1, LAYOUT_STRETCH_BOT_STAT);

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
	m_bpmNumber->display(0);
	m_bpmNumber->setSegmentStyle(QLCDNumber::Filled);
	m_bpmNumber->setDigitCount(5);
	m_bpmNumber->setAutoFillBackground(true);

	m_rmsNumber = new QLCDNumber(m_widget);
	m_rmsNumber->display(0);
	m_rmsNumber->setSegmentStyle(QLCDNumber::Filled);
	m_rmsNumber->setDigitCount(5);
	m_rmsNumber->setAutoFillBackground(true);
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
}

void MainWindow::setup_led()
{
    m_recLed = new RoundLed(m_widget);
    m_recLed->setPower(false);

    m_detLed = new RoundLed(m_widget);
    m_detLed->setPower(false);
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

void MainWindow::convert_to_string(double number, QString& str)
{
	int hundred = static_cast<int>( number 										 / 100);
	int ten		= static_cast<int>((number - hundred * 100) 					 /  10);
	int one		= static_cast<int>((number - hundred * 100 - ten * 10) 			 /   1);
	int tenth	= static_cast<int>((number - hundred * 100 - ten * 10 - one * 1) *  10);
	str.clear();
	str.append(QString('0' + hundred));
	str.append(QString('0' + ten));
	str.append(QString('0' + one));
	str.append(QString("."));
	str.append(QString('0' + tenth));
}
