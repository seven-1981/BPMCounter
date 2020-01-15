#include "Spectrograph.hpp"
#include <QPainter>
#include <QMouseEvent>
#include <QTimerEvent>
#include <iostream>


Spectrograph::Spectrograph(QWidget* parent) :
	QWidget(parent),
	m_barSelected(-1),
	m_timerId(-1),
	m_minFreq(0.0),
	m_maxFreq(0.0)
{
	setMinimumHeight(100);
}

Spectrograph::~Spectrograph()
{

}

void Spectrograph::set_params(unsigned int num_bars, qreal min_freq, qreal max_freq)
{
	Q_ASSERT(num_bars > 0);
	Q_ASSERT(max_freq > min_freq);
	m_bars.resize(num_bars);
	m_minFreq = min_freq;
	m_maxFreq = max_freq;
	update_bars();
}

void Spectrograph::timer_event(QTimerEvent* event)
{
	Q_ASSERT(event->timerId() == m_timerId);
	Q_UNUSED(event) // suppress warnings in release builds
	killTimer(m_timerId);
	m_timerId = -1;
	m_barSelected = -1;
	update();
}

void Spectrograph::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event)

	std::cout << "paint called." << std::endl;

	QPainter painter(this);
	painter.fillRect(rect(), Qt::black);

	const int numBars = m_bars.count();

	// Highlight region of selected bar
	if (m_barSelected != -1 && numBars > 0)
	{
		QRect regionRect = rect();
		regionRect.setLeft(m_barSelected * rect().width() / numBars);
		regionRect.setWidth(rect().width() / numBars);
		QColor regionColor(202, 202, 64);
		painter.setBrush(Qt::DiagCrossPattern);
		painter.fillRect(regionRect, regionColor);
		painter.setBrush(Qt::NoBrush);
	}

	QColor barColor(51, 204, 102);
	QColor clipColor(255, 255, 0);

	// Draw the outline
	const QColor gridColor = barColor.darker();
	QPen gridPen(gridColor);
	painter.setPen(gridPen);
	painter.drawLine(rect().topLeft(), rect().topRight());
	painter.drawLine(rect().topRight(), rect().bottomRight());
	painter.drawLine(rect().bottomRight(), rect().bottomLeft());
	painter.drawLine(rect().bottomLeft(), rect().topLeft());

	QVector<qreal> dashes;
	dashes << 2 << 2;
	gridPen.setDashPattern(dashes);
	painter.setPen(gridPen);

	// Draw vertical lines between bars
	if (numBars > 0)
	{
		QLine line(rect().topLeft(), rect().bottomLeft());
		for (int i = 1; i < numBars; ++i)
		{
			line.translate(rect().width() / numBars, 0);
			painter.drawLine(line);
		}
	}

	// Draw horizontal lines
	const int numVerticalSections = 10;
	QLine line(rect().topLeft(), rect().topRight());
	for (unsigned int i = 1; i < numVerticalSections; ++i)
	{
		line.translate(0, rect().height() / numVerticalSections);
		painter.drawLine(line);
	}

	barColor = barColor.lighter();
	barColor.setAlphaF(0.75);
	clipColor.setAlphaF(0.75);

	// Draw the bars
	if (numBars > 0)
	{
		// Calculate width of bars and gaps
		const int widgetWidth = rect().width();
		const int barPlusGapWidth = widgetWidth / numBars;
		const int barWidth = 0.8 * barPlusGapWidth;
		const int gapWidth = barPlusGapWidth - barWidth;
		const int paddingWidth = widgetWidth - numBars * (barWidth + gapWidth);
		const int leftPaddingWidth = (paddingWidth + gapWidth) / 2;
		const int barHeight = rect().height() - 2 * gapWidth;

		for (int i = 0; i < numBars; ++i)
		{
			const qreal value = m_bars[i].value;
			Q_ASSERT(value >= 0.0 && value <= 1.0);
			QRect bar = rect();
			bar.setLeft(rect().left() + leftPaddingWidth + (i * (gapWidth + barWidth)));
			bar.setWidth(barWidth);
			bar.setTop(rect().top() + gapWidth + (1.0 - value) * barHeight);
			bar.setBottom(rect().bottom() - gapWidth);

			QColor color = barColor;
			if (m_bars[i].clipped)
				color = clipColor;

			painter.fillRect(bar, color);
		}
	}
}

void Spectrograph::reset()
{
	m_spectrum.reset();
	spectrum_changed(m_spectrum);
}

void Spectrograph::spectrum_changed(const FrequencySpectrum& spectrum)
{
	m_spectrum = spectrum;
	update_bars();
}

int Spectrograph::bar_index(qreal frequency) const
{
	Q_ASSERT(frequency >= m_minFreq && frequency < m_maxFreq);
	const qreal bandWidth = (m_maxFreq - m_minFreq) / m_bars.count();
	const int index = (frequency - m_minFreq) / bandWidth;
	if (index < 0 || index >= m_bars.count())
		Q_ASSERT(false);
	return index;
}

QPair<qreal, qreal> Spectrograph::bar_range(int index) const
{
	Q_ASSERT(index >= 0 && index < m_bars.count());
	const qreal bandWidth = (m_maxFreq - m_minFreq) / m_bars.count();
	return QPair<qreal, qreal>(index * bandWidth, (index + 1) * bandWidth);
}

void Spectrograph::update_bars()
{
	using Iterator = FrequencySpectrum::const_iterator;

	m_bars.fill(Bar());
	for (Iterator it = m_spectrum.begin(); it != m_spectrum.end(); ++it)
	{
		const FrequencySpectrum::Element e = *it;
		if (e.frequency >= m_minFreq && e.frequency < m_maxFreq)
		{
			Bar& bar = m_bars[bar_index(e.frequency)];
			bar.value = qMax(bar.value, e.amplitude);
			bar.clipped |= e.clipped;
		}
	}
	update();
}

void Spectrograph::select_bar(int index)
{
	const QPair<qreal, qreal> frequencyRange = bar_range(index);
	const QString message = QString("%1 - %2 Hz")
							 .arg(frequencyRange.first)
							 .arg(frequencyRange.second);
	emit info_message(message, 2000);

	if (m_timerId != -1)
		killTimer(m_timerId);
	m_timerId = startTimer(2000);

	m_barSelected = index;
	update();
}
