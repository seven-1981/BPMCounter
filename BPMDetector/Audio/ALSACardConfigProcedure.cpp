#include "ALSACardConfigProcedure.hpp"
#include "ALSACardConfiguration.hpp"

#include <alsa/asoundlib.h>


int ALSACardConfigProcedure::execute(std::string& hw_id, GEN_CARD_CONFIG_TYPE& config)
{
    CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
    int error = 0;
    if ((error = reopen(hw_id, typedConfig)) < 0)    	return error;
    if ((error = configure(typedConfig)) < 0)         	return error;
	if ((error = prepare_interface(typedConfig)) < 0)	return error;
	typedConfig.INITIALIZED = true;
	return 0;
}

void ALSACardConfigProcedure::finish(GEN_CARD_CONFIG_TYPE& config)
{
	CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
	free_params(typedConfig);
}

int ALSACardConfigProcedure::configure(GEN_CARD_CONFIG_TYPE& config)
{
	CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
    int error = 0;
	if ((error = allocate_params(typedConfig)) < 0)		return error;
	if ((error = init_params(typedConfig)) < 0) 		return error;
	if ((error = set_access_mode(typedConfig)) < 0)   	return error;
	if ((error = set_audio_format(typedConfig)) < 0)  	return error;
	if ((error = set_sample_rate(typedConfig)) < 0)		return error;
	if ((error = set_num_channels(typedConfig)) < 0)	return error;
	if ((error = set_period_size(typedConfig)) < 0)		return error;
	if ((error = apply_params(typedConfig)) < 0)  		return error;
    return 0;
}

int ALSACardConfigProcedure::reopen(std::string& hw_id, GEN_CARD_CONFIG_TYPE& config)
{
	CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
    int error = 0;
	if ((error = close_device(typedConfig)) < 0)			return error;
	if ((error = open_device(hw_id, typedConfig)) < 0) 		return error;
    return 0;
}

int ALSACardConfigProcedure::open_device(std::string& hw_id, CARD_CONFIG_TYPE& config)
{
	//Open PCM device
	return snd_pcm_open(&config.PCM_HANDLE, hw_id.c_str(), config.PCM_CAPTURE_MODE, config.PCM_BLOCKING_MODE);	
}
	
int ALSACardConfigProcedure::allocate_params(CARD_CONFIG_TYPE& config)
{
	//Allocate hardware parameter structure
	return snd_pcm_hw_params_malloc(&config.PCM_HW_PARAMS);
}

int ALSACardConfigProcedure::init_params(CARD_CONFIG_TYPE& config)
{
	//Initialize parameter structure
	return snd_pcm_hw_params_any(config.PCM_HANDLE, config.PCM_HW_PARAMS);
}

int ALSACardConfigProcedure::set_access_mode(CARD_CONFIG_TYPE& config)
{
	//Set access mode
	return snd_pcm_hw_params_set_access(config.PCM_HANDLE, config.PCM_HW_PARAMS, config.PCM_ACCESS_MODE);	
}

int ALSACardConfigProcedure::set_audio_format(CARD_CONFIG_TYPE& config)
{
	//Set audio format
	return snd_pcm_hw_params_set_format(config.PCM_HANDLE, config.PCM_HW_PARAMS, config.PCM_AUDIO_FORMAT);	
}

int ALSACardConfigProcedure::set_sample_rate(CARD_CONFIG_TYPE& config)
{
	//Set sampling rate
	return snd_pcm_hw_params_set_rate_near(config.PCM_HANDLE, config.PCM_HW_PARAMS, &config.PCM_SAMPLE_RATE, 0);
}

int ALSACardConfigProcedure::set_num_channels(CARD_CONFIG_TYPE& config)
{
	//Set number of channels
	return snd_pcm_hw_params_set_channels(config.PCM_HANDLE, config.PCM_HW_PARAMS, config.PCM_NUM_CHANNELS);
}

int ALSACardConfigProcedure::apply_params(CARD_CONFIG_TYPE& config)
{
	//Apply hardware parameters
	return snd_pcm_hw_params(config.PCM_HANDLE, config.PCM_HW_PARAMS);	
}

int ALSACardConfigProcedure::prepare_interface(CARD_CONFIG_TYPE& config)
{
	//Prepare audio interface for usage
	return snd_pcm_prepare(config.PCM_HANDLE);	
}

void ALSACardConfigProcedure::free_params(CARD_CONFIG_TYPE& config)
{
	//Free allocated memory for hw params
	snd_pcm_hw_params_free(config.PCM_HW_PARAMS);
}

int ALSACardConfigProcedure::close_device(CARD_CONFIG_TYPE& config)
{
	//Close opened device
	if (config.INITIALIZED == true)
	{
		config.INITIALIZED = false;
		return snd_pcm_close(config.PCM_HANDLE);
	}
	else
	{
		return 0;
	}
}

int ALSACardConfigProcedure::set_period_size(CARD_CONFIG_TYPE& config)
{
	//Set period size - after period samples available, callback is executed
	return snd_pcm_hw_params_set_period_size_near(config.PCM_HANDLE, config.PCM_HW_PARAMS, &config.PCM_PERIOD_SIZE, &config.PCM_DIRECTION);
}
