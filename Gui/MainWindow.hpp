#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QMainWindow>


class Spectrograph;
class AudioController;
QT_FORWARD_DECLARE_CLASS(QStatusBar)
QT_FORWARD_DECLARE_CLASS(QLCDNumber)


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    MainWindow(QWidget* = nullptr);

public Q_SLOTS:
	void update();

private:
	AudioController* m_controller;
    Spectrograph* m_spectrograph;
    QStatusBar* m_statusbar;
    QLCDNumber* m_number;
};

#endif
