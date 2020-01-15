#include "ALSARecordingService.hpp"

#include "ALSACardConfiguration.hpp"
#include "globals.hpp"


//From ALSA interface, 0 = message, 1 = silent
const int RECOVER_SILENT = 1;
const int NO_ERROR = 0;
const int ANY_ERROR = -1;


int ALSARecordingService::set_handle(GEN_CARD_CONFIG_TYPE& config)
{
    CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
    if (typedConfig.INITIALIZED == true)
    {
        m_isHandleSet = true;
        m_pcmHandle = typedConfig.PCM_HANDLE;
        return NO_ERROR;
    }
    else
    {
        return ANY_ERROR;
    }
}

bool ALSARecordingService::is_handleSet()
{
    return m_isHandleSet;
}

int ALSARecordingService::pcm_readi(void* buffer, int size)
{
    int retVal = snd_pcm_readi(m_pcmHandle, buffer, size);
    if (retVal < 0)
    {
        IGNORE pcm_drop();
        return pcm_recover(retVal, RECOVER_SILENT);
    }
    else
    {
        return retVal;
    }
}

int ALSARecordingService::add_pcm_handler(GEN_CARD_CALLBACK_CONFIG_TYPE& config)
{
	if (is_handleSet() == true)
	{
		CARD_CALLBACK_CONFIG_TYPE& typedConfig = dynamic_cast<CARD_CALLBACK_CONFIG_TYPE&>(config);
		ALSA_CALLBACK_PARAM ahandler = typedConfig.PCM_ASYNC_HANDLER;
		ALSA_CALLBACK_TYPE callback = typedConfig.PCM_ASYNC_CALLBACK;
		if (callback == nullptr)
		{
			callback = callback_wrapper;
		}
		IAudioRecorder* instance = typedConfig.PCM_ASYNC_PRIVATE_DATA.RECORDER_INSTANCE;
		if (snd_async_add_pcm_handler(&ahandler, m_pcmHandle, callback, instance) < 0)
		{
			return ANY_ERROR;
		}
		else
		{
			m_isCallbackSet = true;
			return NO_ERROR;
		}
	}
	else
	{
		return ANY_ERROR;
	}
}

bool ALSARecordingService::is_callbackSet()
{
	return m_isCallbackSet;
}

int ALSARecordingService::pcm_start()
{
	if (is_callbackSet() == true)
	{
		if (snd_pcm_start(m_pcmHandle) == 0)
		{
			return NO_ERROR;
		}
		else
		{
			return ANY_ERROR;
		}
	}
	else
	{
		return ANY_ERROR;
	}
}

int ALSARecordingService::pcm_stop()
{
	if (is_callbackSet() == true)
	{
		if (snd_pcm_drop(m_pcmHandle) == 0)
		{
			return NO_ERROR;
		}
		else
		{
			return ANY_ERROR;
		}
	}
	else
	{
		return ANY_ERROR;
	}
}

int ALSARecordingService::pcm_drop()
{
    return snd_pcm_drop(m_pcmHandle);
}

int ALSARecordingService::pcm_recover(int err, int silent)
{
    return snd_pcm_recover(m_pcmHandle, err, silent);

}

void ALSARecordingService::callback_wrapper(snd_async_handler_t* ahandler)
{
	snd_pcm_t* handle = snd_async_handler_get_pcm(ahandler);
	snd_pcm_sframes_t num_samples = snd_pcm_avail_update(handle);
	IAudioRecorder* instance = static_cast<IAudioRecorder*>(snd_async_handler_get_callback_private(ahandler));
	instance->samples_available(num_samples);
}
