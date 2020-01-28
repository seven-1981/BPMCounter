#ifndef _IAUDIORECORDER_H
#define _IAUDIORECORDER_H


//Forward declarations
enum class Errors_e;
struct ICardConfiguration_t;
struct ICardCallbackConfiguration_t;
class IRecordingService;
template <typename T>
class Buffer;


//Interface class for audio recording
class IAudioRecorder
{
public:
    virtual ~IAudioRecorder() { }

    virtual void set_service(IRecordingService* service) = 0;
    //Synchronous interface functions
    virtual Errors_e prepare_recorder(ICardConfiguration_t& config) = 0;
    //Implicit interface - not virtual
    template <typename T>
    Errors_e capture_samples(ICardConfiguration_t& config, Buffer<T>& buffer);

    //Asynchronous interface functions
    virtual Errors_e set_callback(ICardCallbackConfiguration_t& config) = 0;
    virtual Errors_e clear_callback(ICardCallbackConfiguration_t& config) = 0;
    virtual Errors_e start_capturing() = 0;
    virtual Errors_e stop_capturing() = 0;
    virtual void samples_available(int num_samples) = 0;
    //Implicit interface - not virtual
    template <typename T>
    Errors_e read_samples(ICardConfiguration_t& config, Buffer<T>& buffer);
};

#endif
