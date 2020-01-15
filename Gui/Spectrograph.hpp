#ifndef _SPECTROGRAPH_H
#define _SPECTROGRAPH_H

#include <QWidget>
#include "FrequencySpectrum.hpp"


QT_FORWARD_DECLARE_CLASS(QMouseEvent)


class Spectrograph : public QWidget
{
	Q_OBJECT

public:
	Spectrograph(QWidget* parent = nullptr);
	~Spectrograph();

	void set_params(unsigned int num_bars, qreal min_freq, qreal max_freq);

	void timer_event(QTimerEvent* event);
	void paintEvent(QPaintEvent* event);

signals:
	void info_message(const QString& message, unsigned int interval_ms);

public slots:
	void reset();
	void spectrum_changed(const FrequencySpectrum& spectrum);

private:
	int bar_index(qreal frequency) const;
	QPair<qreal, qreal> bar_range(int bar_index) const;
	void update_bars();
	void select_bar(int index);

	struct Bar
	{
		Bar() :
			value(0.0),
			clipped(false) { }

		qreal value;
		bool clipped;
	};

	QVector<Bar> m_bars;
	int m_barSelected;
	int m_timerId;
	qreal m_minFreq;
	qreal m_maxFreq;
	FrequencySpectrum m_spectrum;
};

#endif
