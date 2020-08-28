#ifndef _IRECORDINGSERVICE_H
#define _IRECORDINGSERVICE_H


//Forward declarations
struct ICardConfiguration_t;
struct ICardCallbackConfiguration_t;


//Interface class for audio recording service
class IRecordingService
{
public:
    virtual ~IRecordingService() { }

    //Interface functions
    virtual int set_handle(ICardConfiguration_t& config) = 0; //replace this with int -> error converter
    virtual bool is_handleSet() = 0;
    virtual int pcm_readi(void* buffer, int size) = 0;
    virtual int pcm_drop() = 0;
    virtual int pcm_recover(int err, int silent) = 0;
    virtual int pcm_prepare() = 0;
    virtual int add_pcm_handler(ICardCallbackConfiguration_t& config) = 0;
    virtual int del_pcm_handler(ICardCallbackConfiguration_t& config) = 0;
    virtual int pcm_start() = 0;
    virtual int pcm_stop() = 0;
    virtual bool is_callbackSet() = 0;
};

#endif
