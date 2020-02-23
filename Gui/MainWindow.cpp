#include <QTimer>
#include <QLCDNumber>
#include <QString>
#include <QChar>
#include <QKeyEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "MainWindow.hpp"
#include "AppContainer.hpp"
#include "LED.hpp"


MainWindow::MainWindow(AppContainer& app, QWidget* parent) : QMainWindow(parent), m_app(app)
{
	setup_widget();
	setup_number();
	setup_label();
	setup_led();
	setup_layout();
	setup_updater(200);
}

MainWindow::~MainWindow()
{
	//Qt automatically deletes childs
	delete m_widget;
	delete m_timer;
}

void MainWindow::update()
{
	double bpm = m_app.fetch_bpmValue();
	int i = static_cast<int>(bpm);
	m_bpmNumber->display(QString("%1").arg(i, 3, 10, QChar('0')) + "."
			+ QString("%1").arg(qRound((bpm - i) * 10), 1, 10, QChar('0')));
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Q)
	{
		m_app.stop_detection();
		QCoreApplication::quit();
	}
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	m_app.stop_detection();
	QCoreApplication::quit();
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
    m_topLayout->setStretch(0, 4);
    m_topLayout->setStretch(1, 1);

    m_l1BotLayout->addLayout(m_l2LeftLayout);
    m_l1BotLayout->addLayout(m_l2RightLayout);
    m_l1BotLayout->setStretch(0, 1);
    m_l1BotLayout->setStretch(1, 1);

    m_l2RightLayout->addWidget(m_rmsLabel);
    m_l2RightLayout->addWidget(m_rmsNumber);
    m_l2RightLayout->setStretch(0, 1);
    m_l2RightLayout->setStretch(1, 4);

    m_l2LeftLayout->addLayout(m_l3LeftTopLayout);
    m_l2LeftLayout->addLayout(m_l3LeftBotLayout);

    m_l3LeftTopLayout->addWidget(m_recLed);
    m_l3LeftTopLayout->addWidget(m_recLabel);
    m_l3LeftTopLayout->setStretch(0, 1);
    m_l3LeftTopLayout->setStretch(1, 8);
    m_l3LeftBotLayout->addWidget(m_detLed);
    m_l3LeftBotLayout->addWidget(m_detLabel);
    m_l3LeftBotLayout->setStretch(0, 1);
    m_l3LeftBotLayout->setStretch(1, 8);

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
	font.setPointSize(48);
	font.setBold(true);
	m_recLabel->setFont(font);
	m_detLabel->setFont(font);
	m_rmsLabel->setFont(font);
}

void MainWindow::setup_led()
{
    m_recLed = new RoundLed(m_widget);
    m_recLed->setPower(true);

    m_detLed = new RoundLed(m_widget);
    m_detLed->setPower(true);
}

void MainWindow::setup_updater(int interval_ms)
{
    m_timer = new QTimer(this);
    connect (m_timer, SIGNAL( timeout() ), this, SLOT( update() ) );
    m_timer->start(interval_ms);
}
