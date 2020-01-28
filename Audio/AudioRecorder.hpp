#ifndef _AUDIORECORDER_H
#define _AUDIORECORDER_H

#include "IAudioRecorder.hpp"
#include "ALSACardConfiguration.hpp"
#include "ALSACardCallbackConfiguration.hpp"
#include "IRecordingService.hpp"
#include "Buffer.hpp"
#include "globals.hpp"

#include <unordered_map>
#include <atomic>

//Typename simplifications
using GEN_CARD_CONFIG_TYPE = ICardConfiguration_t;
using GEN_CARD_CALLBACK_CONFIG_TYPE = ICardCallbackConfiguration_t;
using CARD_CONFIG_TYPE = ALSACardConfiguration_t;
using CARD_CALLBACK_CONFIG_TYPE = ALSACardCallbackConfiguration_t;
using GEN_REC_SERVICE = IRecordingService;


class AudioRecorder : public IAudioRecorder
{
    using MAP = std::unordered_map<int, Errors_e>;

public:
    explicit AudioRecorder();
    ~AudioRecorder() { }

    //Override interface functions
    void set_service(GEN_REC_SERVICE* service) override;
    Errors_e prepare_recorder(GEN_CARD_CONFIG_TYPE& config) override;

    //Implicit interface functions
    template <typename T>
    Errors_e capture_samples(GEN_CARD_CONFIG_TYPE& config, Buffer<T>& bufferToFill)
    {
        if ((m_service == nullptr) || (m_service->is_handleSet() == false))
        {
            return Errors_e::AUDIO_RECORDER_NOT_INITIALIZED;
        }
        else
        {
            return create_and_fill(config, bufferToFill, m_bufferSize);
        }
    }

    //Asynchronous interface functions
    Errors_e set_callback(GEN_CARD_CALLBACK_CONFIG_TYPE& config) override;
    Errors_e clear_callback(GEN_CARD_CALLBACK_CONFIG_TYPE& config) override;
    Errors_e start_capturing() override;
    Errors_e stop_capturing() override;
    void samples_available(int num_samples) override;
    bool is_samplesAvailable();

    template <typename T>
    Errors_e read_samples(GEN_CARD_CONFIG_TYPE& config, Buffer<T>& bufferToFill)
    {
    	if (is_samplesAvailable() == false)
    	{
    		return Errors_e::AUDIO_RECORDER_NOT_ENOUGH_SAMPLES;
    	}
    	else
    	{
    		return create_and_fill(config, bufferToFill, m_periodSize);
    	}
    }

private:
    int m_bufferSize;
    GEN_REC_SERVICE* m_service;
    MAP m_errorCodes;
    int m_periodSize;
    std::atomic<bool> m_samplesAvailable;


    void set_buffer_size(GEN_CARD_CONFIG_TYPE& config);
    Errors_e convert_errorCode(const int code) const;

    template <typename T>
    Errors_e fill_buffer(T* sourceBuffer, Buffer<T>& bufferToFill, int size)
    {
        int retVal = m_service->pcm_readi(sourceBuffer, size);
        if (retVal != size)
        {
            return convert_errorCode(retVal);
        }
        bufferToFill.init(size);
        for (int i = 0; i < size; ++i)
        {
            bufferToFill[i] = sourceBuffer[i];
        }
        return Errors_e::NO_ERROR;
    }

    template <typename T>
    Errors_e create_and_fill(GEN_CARD_CONFIG_TYPE& config, Buffer<T>& bufferToFill, int size)
    {
        CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
        using SAMPLE_TYPE = decltype(typedConfig.PCM_SAMPLE_TYPE);
        SAMPLE_TYPE buffer[size] = { 0 };
        return fill_buffer(buffer, bufferToFill, size);
    }

    void set_callback_instance(GEN_CARD_CALLBACK_CONFIG_TYPE& config);
};

#endif
