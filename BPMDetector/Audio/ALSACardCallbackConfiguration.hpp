#ifndef _ALSACARDCALLBACKCONFIGURATION_H
#define _ALSACARDCALLBACKCONFIGURATION_H

#include "ICardCallbackConfiguration.hpp"
#include "IAudioRecorder.hpp"

#include <alsa/asoundlib.h>

using ALSA_CALLBACK_RETURN = void;
using ALSA_CALLBACK_PARAM = snd_async_handler_t*;
using ALSA_CALLBACK_TYPE = void(*)(snd_async_handler_t*);


//Struct for passing private data
struct ALSACardCallbackPrivate_t
{
	IAudioRecorder*	RECORDER_INSTANCE = nullptr;
};

//Struct for ALSA card configuration parameters
struct ALSACardCallbackConfiguration_t : public ICardCallbackConfiguration_t
{
	ALSA_CALLBACK_PARAM 		PCM_ASYNC_HANDLER 		= nullptr;
	ALSA_CALLBACK_TYPE			PCM_ASYNC_CALLBACK		= nullptr;
	ALSACardCallbackPrivate_t	PCM_ASYNC_PRIVATE_DATA;
};

#endif
