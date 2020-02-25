#ifndef _ALSARECORDINGSERVICE_H
#define _ALSARECORDINGSERVICE_H

#include "IRecordingService.hpp"
#include "ALSACardCallbackConfiguration.hpp"

#include <alsa/asoundlib.h>


//Forward declaration
struct ALSACardConfiguration_t;
struct ALSACardCallbackConfiguration_t;
//Typename simplifications
using CARD_CONFIG_TYPE = ALSACardConfiguration_t;
using GEN_CARD_CONFIG_TYPE = ICardConfiguration_t;
using CARD_CALLBACK_CONFIG_TYPE = ALSACardCallbackConfiguration_t;
using GEN_CARD_CALLBACK_CONFIG_TYPE = ICardCallbackConfiguration_t;


class ALSARecordingService : public IRecordingService
{
public:
    ALSARecordingService() : 
      m_pcmHandle(nullptr), 
      m_isHandleSet(false),
	  m_isCallbackSet(false) { }
    ~ALSARecordingService() { }

    //Interface functions
    int set_handle(GEN_CARD_CONFIG_TYPE& config) override;
    bool is_handleSet() override;
    int pcm_readi(void* buffer, int size) override;

    int add_pcm_handler(GEN_CARD_CALLBACK_CONFIG_TYPE& config) override;
    int del_pcm_handler(GEN_CARD_CALLBACK_CONFIG_TYPE& config) override;
    int pcm_start() override;
    int pcm_stop() override;
    bool is_callbackSet() override;

private:
    snd_pcm_t* m_pcmHandle;
    bool m_isHandleSet;
	bool m_isCallbackSet;

    int pcm_drop() override;
    int pcm_recover(int err, int silent) override;
    int pcm_prepare() override;

    static void callback_wrapper(ALSA_CALLBACK_PARAM handler);
};

#endif
