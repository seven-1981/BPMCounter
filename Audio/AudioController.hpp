#ifndef _AUDIOCONTROLLER_H
#define _AUDIOCONTROLLER_H

#include "globals.hpp"
#include "AudioCardLister.hpp"
#include "ALSACardInfoGetter.hpp"
#include "AudioCardConfigurator.hpp"
#include "ALSACardConfigProcedure.hpp"
#include "AudioCardManager.hpp"
#include "AudioRecorder.hpp"
#include "ALSARecordingService.hpp"
#include "ALSACardConfiguration.hpp"
#include "ALSACardCallbackConfiguration.hpp"


class AudioController
{
public:
	AudioController(ALSACardConfiguration_t& config);
	~AudioController();

	Errors_e config_audio(ALSACardConfiguration_t& config);

	template <typename T>
	Errors_e synchronous_record(Buffer<T>& buffer)
	{
		//Buffer must not be initialized here, as this is done in recorder
		unsigned int size = buffer.get_size();
		if (size != 0 && size != m_config.PCM_BUFFER_SIZE)
		{
			return Errors_e::AUDIO_CONTROLLER_BUFFER_ERROR;
		}
		else
		{
			return m_recorder.capture_samples(m_config, buffer);
		}
	}

	Errors_e start_asynchronous_record();
	Errors_e stop_asynchronous_record();

	template <typename T>
	Errors_e read_asynchronous_samples(Buffer<T>& buffer)
	{
		//Buffer must not be initialized here, as this is done in recorder
		unsigned int size = buffer.get_size();
		if (size != 0 && size != m_config.PCM_PERIOD_SIZE)
		{
			return Errors_e::AUDIO_CONTROLLER_BUFFER_ERROR;
		}
		else
		{
			return m_recorder.read_samples(m_config, buffer);
		}
	}

private:
	AudioCardLister m_lister;
	ALSACardInfoGetter m_infoGetter;
	AudioCardConfigurator m_configurator;
	ALSACardConfigProcedure m_procedure;
	AudioCardManager m_manager;
	AudioRecorder m_recorder;
	ALSARecordingService m_service;

	ALSACardConfiguration_t& m_config; // Set from outside, therefore a reference
	ALSACardCallbackConfiguration_t m_callbackConfig;

	void init_audio();
};

#endif
