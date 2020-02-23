#ifndef _LED_H
#define _LED_H

#include <QWidget>
#include <QLabel>
#include <QPainter>


class Led : public QLabel
{
	Q_OBJECT

public:
	explicit Led(QWidget* parent = nullptr)	:
		QLabel(parent), m_onPix(48, 48)
	{
		m_onPix.fill(Qt::black);
		setPixmap(m_onPix);
		setScaledContents(true);
	}

	virtual ~Led() = default;

	Led(const Led&) = delete;
	Led& operator=(const Led&) = delete;

public slots:
	void setOn();
	void setOff();
	void setPower(bool val);

private:
	QPixmap m_onPix;
};


class RoundLed : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool power READ power WRITE setPower NOTIFY powerChanged)

public:
    explicit RoundLed(QWidget* parent = nullptr) :
		QWidget(parent), m_power(false) { }

    bool power() const;

    RoundLed(const RoundLed&) = delete;
    RoundLed& operator=(const Led&) = delete;

public slots:
    void setPower(bool power);

signals:
    void powerChanged();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    bool m_power;

    void rescale(QRect& rec);
	const float SCALE_FACTOR = 1.5;
};

#endif
