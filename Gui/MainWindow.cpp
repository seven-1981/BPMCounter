#include <qt5/QtWidgets/qpushbutton.h>
#include <qt5/QtCore/QTimer>
#include <QLCDNumber>
#include <QKeyEvent>
#include <QApplication>
#include "MainWindow.hpp"
#include "Spectrograph.hpp"
#include "AudioController.hpp"
#include <iostream>


void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Q)
		QCoreApplication::quit();
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	m_spectrograph = new Spectrograph(this);
	setCentralWidget(m_spectrograph);
	m_spectrograph->set_params(10, 0.0, 100.0);
	FrequencySpectrum* spec = new FrequencySpectrum(10);
	(*spec)[1].amplitude = 0.4;
	(*spec)[1].frequency = 10.0;
	(*spec)[2].amplitude = 0.8;
	(*spec)[2].frequency = 20.0;
	(*spec)[3].amplitude = 0.2;
	(*spec)[3].frequency = 60.0;
	m_spectrograph->spectrum_changed(*spec);
	m_spectrograph->update();

	QPalette Pal = palette();
	Pal.setColor(QPalette::Normal, QPalette::WindowText, Qt::green);
	Pal.setColor(QPalette::Normal, QPalette::Window, Qt::black);
	setPalette(Pal);
	m_number = new QLCDNumber(this);
	m_number->setSegmentStyle(QLCDNumber::Filled);
	m_number->display(11.44);
	m_number->setAutoFillBackground(true);
	//m_number->setStyleSheet("background-color: rgba(0,0,0,0)");
//
//    m_chart->setOrientation( orientationBox->currentIndex() );
//    connect( orientationBox, SIGNAL( currentIndexChanged( int ) ),
//             m_chart, SLOT( setOrientation( int ) ) );
//
//
//    QTimer* updateTimer = new QTimer(this);
//    connect (updateTimer, SIGNAL( timeout() ), this, SLOT( update() ) );
//    updateTimer->start(1000);
}

void MainWindow::update()
{
	std::cout << "update called." << std::endl;

	QVector<double> series;
	for ( int i = 0; i < 15; i++ )
	{
		series += ( 2 + qrand() % 8 );
	}
	//emit m_chart->set_samples(series);
}
