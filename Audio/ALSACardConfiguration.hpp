#ifndef _ALSACARDCONFIGURATION_H
#define _ALSACARDCONFIGURATION_H

#include "ICardConfiguration.hpp"

#include <alsa/asoundlib.h>


//Struct for ALSA card configuration parameters
struct ALSACardConfiguration_t : public ICardConfiguration_t
{
	snd_pcm_stream_t 	PCM_CAPTURE_MODE 	= SND_PCM_STREAM_CAPTURE;
	int 				PCM_BLOCKING_MODE 	= 0;
	snd_pcm_access_t 	PCM_ACCESS_MODE 	= SND_PCM_ACCESS_RW_INTERLEAVED;
	snd_pcm_format_t 	PCM_AUDIO_FORMAT 	= SND_PCM_FORMAT_S16_LE;
	unsigned int		PCM_SAMPLE_RATE		= 44100;
	unsigned int		PCM_NUM_CHANNELS	= 1;

	snd_pcm_hw_params_t* 	PCM_HW_PARAMS 	= nullptr;
	snd_pcm_t* 				PCM_HANDLE		= nullptr;
	bool					INITIALIZED		= false;

	int 				PCM_BUFFER_SIZE 	= 88200;
	short signed int	PCM_SAMPLE_TYPE		= 0;

	snd_pcm_uframes_t	PCM_PERIOD_SIZE		= 256;
	int					PCM_DIRECTION		= 0;
};

#endif
