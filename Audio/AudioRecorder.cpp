#include "AudioRecorder.hpp"

#include "IRecordingService.hpp"
#include "ALSACardConfiguration.hpp"
#include "globals.hpp"

 
AudioRecorder::AudioRecorder() :
  m_bufferSize(0), 
  m_service(nullptr),
  m_periodSize(0),
  m_samplesAvailable(false)
{
    m_errorCodes[0]   = Errors_e::NO_ERROR;
    m_errorCodes[-1]  = Errors_e::AUDIO_RECORDER_CAPTURE_ERROR;
    m_errorCodes[-11] = Errors_e::AUDIO_RECORDER_CAPTURE_EAGAIN;
    m_errorCodes[-32] = Errors_e::AUDIO_RECORDER_CAPTURE_EPIPE;
    m_errorCodes[-77] = Errors_e::AUDIO_RECORDER_CAPTURE_EBADFD;
    m_errorCodes[-86] = Errors_e::AUDIO_RECORDER_CAPTURE_ESTRPIPE;
}

void AudioRecorder::set_service(GEN_REC_SERVICE* service)
{
    m_service = service;
}

Errors_e AudioRecorder::prepare_recorder(GEN_CARD_CONFIG_TYPE& config)
{
    if (m_service != nullptr)
    {
        set_buffer_size(config);
        if (m_service->set_handle(config) < 0)
        {
        	return Errors_e::AUDIO_RECORDER_NOT_INITIALIZED;
        }
        else
        {
        	return Errors_e::NO_ERROR;
        }
    }
    else
    {
        return Errors_e::AUDIO_RECORDER_NOT_INITIALIZED;
    }
}

Errors_e AudioRecorder::set_callback(GEN_CARD_CALLBACK_CONFIG_TYPE& config)
{
	if (m_service != nullptr)
	{
		set_callback_instance(config);
		if (m_service->add_pcm_handler(config) < 0)
		{
			return Errors_e::AUDIO_RECORDER_NOT_INITIALIZED;
		}
		else
		{
			return Errors_e::NO_ERROR;
		}
	}
	else
	{
		return Errors_e::AUDIO_RECORDER_NOT_INITIALIZED;
	}
}

Errors_e AudioRecorder::start_capturing()
{
	if ((m_service == nullptr) || (m_service->is_callbackSet() == false))
	{
		return Errors_e::AUDIO_RECORDER_CALLBACK_ERROR;
	}
	else
	{
		return convert_errorCode(m_service->pcm_start());
	}
}

Errors_e AudioRecorder::stop_capturing()
{
	if ((m_service == nullptr) || (m_service->is_callbackSet() == false))
	{
		return Errors_e::AUDIO_RECORDER_CALLBACK_ERROR;
	}
	else
	{
		return convert_errorCode(m_service->pcm_stop());
	}
}

void AudioRecorder::samples_available(int num_samples)
{
	//Interrupt context
	m_samplesAvailable = false;
	if (num_samples < m_periodSize)
		return;
	m_samplesAvailable = true;
}

bool AudioRecorder::is_samplesAvailable()
{
	return m_samplesAvailable;
}

void AudioRecorder::set_buffer_size(GEN_CARD_CONFIG_TYPE& config)
{
    CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
    m_bufferSize = typedConfig.PCM_BUFFER_SIZE;
    m_periodSize = typedConfig.PCM_PERIOD_SIZE;
}

Errors_e AudioRecorder::convert_errorCode(const int code) const
{
    Errors_e retVal = Errors_e::NO_ERROR;
    if (code > 0)
    {
        retVal = Errors_e::AUDIO_RECORDER_CAPTURE_ERROR;
    }
    else
    {
        auto search = m_errorCodes.find(code);
        if (search != m_errorCodes.end())
        {
            retVal = search->second;
        }
        else
        {
            retVal = Errors_e::AUDIO_RECORDER_CAPTURE_ERROR;
        }
    }
    return retVal;
}

void AudioRecorder::set_callback_instance(GEN_CARD_CALLBACK_CONFIG_TYPE& config)
{
	CARD_CALLBACK_CONFIG_TYPE& typedConfig = dynamic_cast<CARD_CALLBACK_CONFIG_TYPE&>(config);
	if (typedConfig.PCM_ASYNC_PRIVATE_DATA.RECORDER_INSTANCE == nullptr)
	{
		typedConfig.PCM_ASYNC_PRIVATE_DATA.RECORDER_INSTANCE = this;
	}
}

