#include "AppContainer.hpp"
#include "AnalyzerController.hpp"
#include "globals.hpp"
#include "ThreadWrapper.hpp"


AppContainer::AppContainer() :
	m_config(),
	m_buffer(nullptr),
	m_audioController(m_config),
	m_analyzerController(nullptr),
	m_wrapper(this)
{
	m_buffer = new BUFFER_TYPE(m_config.PCM_BUFFER_SIZE);
	m_analyzerController = new AnalyzerController<SAMPLE_TYPE>(*m_buffer);
	m_wrapper.set_producer(&AppContainer::record_audio);
	m_wrapper.set_consumer(&AppContainer::detect_bpm);
}

AppContainer::~AppContainer()
{
	delete m_buffer;
	delete m_analyzerController;
}

Errors_e AppContainer::init()
{
	Errors_e retVal = Errors_e::NO_ERROR;
	if ((retVal = m_audioController.config_audio(m_config)) != Errors_e::NO_ERROR)
	{
		return retVal;
	}

	m_wrapper.start_producer();
	m_wrapper.start_consumer();

	return retVal;
}

Errors_e AppContainer::record_audio()
{
	return record();
}

double AppContainer::detect_bpm()
{
	return detect();
}

double AppContainer::fetch_bpmValue()
{
	return m_wrapper.get_value();
}

void AppContainer::stop_detection()
{
	m_wrapper.stop_production();
}

Errors_e AppContainer::record()
{
	Errors_e retVal = Errors_e::NO_ERROR;
	if ((retVal = m_audioController.synchronous_record(*m_buffer)) != Errors_e::NO_ERROR)
		return retVal;
	return Errors_e::NO_ERROR;
}

double AppContainer::detect()
{
	double bpm = m_analyzerController->analyze();
	return bpm;
}