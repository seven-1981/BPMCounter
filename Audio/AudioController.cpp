#include "AudioController.hpp"


AudioController::AudioController(ALSACardConfiguration_t& config) :
  m_lister(),
  m_infoGetter(),
  m_configurator(),
  m_procedure(),
  m_manager(),
  m_recorder(),
  m_service(),
  m_config(config),
  m_callbackConfig()
{
	init_audio();
}

AudioController::~AudioController()
{

}

Errors_e AudioController::config_audio(ALSACardConfiguration_t& config)
{
	Errors_e retVal = Errors_e::NO_ERROR;
	if ((retVal = m_manager.select_and_configure(config)) != Errors_e::NO_ERROR)
		return retVal;
	if ((retVal = m_recorder.prepare_recorder(config)) != Errors_e::NO_ERROR)
		return retVal;
	//if ((retVal = m_recorder.set_callback(m_callbackConfig)) != Errors_e::NO_ERROR)
	//	return retVal;
	m_config = config;
	return retVal;
}

Errors_e AudioController::start_asynchronous_record()
{
	return m_recorder.start_capturing();
}

Errors_e AudioController::stop_asynchronous_record()
{
	return m_recorder.stop_capturing();
}

void AudioController::init_audio()
{
	m_lister.init(&m_infoGetter);
	m_configurator.set_procedure(&m_procedure);
	m_manager.init_lister(&m_lister);
	m_manager.init_configurator(&m_configurator);
	m_recorder.set_service(&m_service);
}
