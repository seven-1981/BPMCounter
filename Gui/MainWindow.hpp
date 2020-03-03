#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QString>
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
	void update_bpm();
	void update_rec();

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

    void stop_application();

    void convert_to_string(double number, QString& str, int digits, int precision);
    void set_color(QWidget* widget, Qt::GlobalColor color);
    void set_number(QLCDNumber* lcdnbr, double number, int digits, int precision);

    const int VALUE_UPDATE_INTERVAL_MS = 200;
    const int FONT_SIZE = 48;
    const int LAYOUT_STRETCH_TOP_BPM  = 4;
    const int LAYOUT_STRETCH_BOT_STAT = 1;
    const int LAYOUT_STRETCH_LT_STAT = 1;
    const int LAYOUT_STRETCH_RT_STAT = 1;
    const int LAYOUT_STRETCH_RMS_LT = 1;
    const int LAYOUT_STRETCH_RMS_RT = 4;
    const int LAYOUT_STRETCH_LED_LT = 1;
    const int LAYOUT_STRETCH_LED_RT = 8;
};

#endif
