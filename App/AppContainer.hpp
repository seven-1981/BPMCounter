#ifndef _APPCONTAINER_H
#define _APPCONTAINER_H

#include "ALSACardConfiguration.hpp"
#include "AudioController.hpp"
#include "ThreadWrapper.hpp"
#include "Buffer.hpp"

using CARD_CONFIG_TYPE = ALSACardConfiguration_t;
using SAMPLE_TYPE = CARD_CONFIG_TYPE::SAMPLE_TYPE;
using BUFFER_TYPE = Buffer<SAMPLE_TYPE>;

class IAnalyzerTuple;
enum class Errors_e;


class AppContainer
{
public:
	AppContainer();
	~AppContainer();

	Errors_e init();

	Errors_e record_audio();
	double detect_bpm();

	Errors_e fetch_recRetval();
	double fetch_bpmValue();
	double fetch_rmsValue();

	bool get_status();
	void stop_detection();
	void start_detection();

private:
	CARD_CONFIG_TYPE m_config;
	BUFFER_TYPE* m_buffer;
	AudioController m_audioController;
	IAnalyzerTuple* m_analyzerController;
	ThreadWrapper<Errors_e, double, AppContainer> m_wrapper;
};

#endif
