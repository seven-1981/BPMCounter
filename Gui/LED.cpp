#include "LED.hpp"


void Led::setOn()
{
	m_onPix.fill(Qt::red);
	setPixmap(m_onPix);
}

void Led::setOff()
{
	m_onPix.fill(Qt::black);
	setPixmap(m_onPix);
}

void Led::setPower(bool power)
{
	if (power == true) setOn();
	else setOff();
}


bool RoundLed::power() const
{
	return m_power;
}

void RoundLed::setPower(bool power)
{
	if (power != m_power)
	{
		m_power = power;
		emit powerChanged();
		update();
	}
}

void RoundLed::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	QPainter ledPainter(this);
	ledPainter.setPen(Qt::black);

	QRect rec = rect();
	rescale(rec);
	if (m_power == true)
		ledPainter.setBrush(Qt::red);
	else
		ledPainter.setBrush(Qt::NoBrush);
	ledPainter.drawEllipse(rec);
}

void RoundLed::rescale(QRect& rec)
{
	int w = rec.width();
	int h = rec.height();
	float alpha = (SCALE_FACTOR - 1.0f) / (2.0f * SCALE_FACTOR);
	int w_scaled = static_cast<int>(w / SCALE_FACTOR);
	int h_scaled = static_cast<int>(h / SCALE_FACTOR);
	rec.setWidth(w_scaled);
	rec.setHeight(h_scaled);
	rec.setX(rec.x() + static_cast<int>(w * alpha));
	rec.setY(rec.y() + static_cast<int>(h * alpha));
}
