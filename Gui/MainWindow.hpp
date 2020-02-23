#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QMainWindow>

class AppContainer;
class RoundLed;
QT_FORWARD_DECLARE_CLASS(QLCDNumber)
QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(QKeyEvent)
QT_FORWARD_DECLARE_CLASS(QCloseEvent)
QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)
QT_FORWARD_DECLARE_CLASS(QLabel)


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    MainWindow(AppContainer& app, QWidget* = nullptr);
    ~MainWindow();

public Q_SLOTS:
	void update();

private:
    AppContainer& m_app;
    QWidget* m_widget;

    QVBoxLayout* m_topLayout;
    QHBoxLayout* m_l1BotLayout;

    QVBoxLayout* m_l2LeftLayout;
    QHBoxLayout* m_l2RightLayout;

    QHBoxLayout* m_l3LeftTopLayout;
    QHBoxLayout* m_l3LeftBotLayout;

    QLCDNumber* m_bpmNumber;
    QLCDNumber* m_rmsNumber;

    QLabel* m_recLabel;
    QLabel* m_detLabel;
    QLabel* m_rmsLabel;

    RoundLed* m_recLed;
    RoundLed* m_detLed;

    QTimer* m_timer;

    void keyPressEvent(QKeyEvent* event);
    void closeEvent(QCloseEvent* event);

    void setup_widget();
    void setup_layout();
    void setup_number();
    void setup_label();
    void setup_led();
    void setup_updater(int interval_ms);
};

#endif
