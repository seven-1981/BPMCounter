#ifndef _FAKERECORDINGSERVICE_H
#define _FAKERECORDINGSERVICE_H

#include "IRecordingService.hpp"


//Forward declarations
struct ICardConfiguration_t;
struct ICardCallbackConfiguration_t;
struct ALSACardConfiguration_t;
struct ALSACardCallbackConfiguration_t;
//Type simplifications
using GEN_CARD_CONFIG_TYPE = ICardConfiguration_t;
using CARD_CONFIG_TYPE = ALSACardConfiguration_t;
using GEN_CARD_CALLBACK_CONFIG_TYPE = ICardCallbackConfiguration_t;
using CARD_CALLBACK_CONFIG_TYPE = ALSACardCallbackConfiguration_t;


//States used in this mock
enum FakeRecordingServiceStates_e
{
	State_CaptureSuccess,
    State_CaptureReturns_EBADFD_RecoverNotPossible,
    State_CaptureReturns_EPIPE_RecoverError,
	State_CaptureReturns_EPIPE_RecoverSuccess,
    State_CaptureReturns_ESTRPIPE_RecoverError,
	State_CaptureReturns_ESTRPIPE_RecoverSuccess,
	State_CaptureLessSamples,
	State_CaptureAsyncSuccess,
	State_CaptureAsyncSuccessWithMemberFunction,
	State_CaptureAsyncFailure
};


//Mock object for testing
class FakeRecordingService : public IRecordingService
{
public:
	FakeRecordingService() : 
	  m_state(FakeRecordingServiceStates_e::State_CaptureReturns_EBADFD_RecoverNotPossible), 
      m_isHandleSet(false), m_callbackConfig(nullptr), m_isCallbackSet(false) { }
	~FakeRecordingService() { }

    //Interface functions
    int set_handle(GEN_CARD_CONFIG_TYPE& config) override;
	bool is_handleSet() override;
    int pcm_readi(void* buffer, int size) override;
    int add_pcm_handler(GEN_CARD_CALLBACK_CONFIG_TYPE& config) override;
    int pcm_start() override;
    int pcm_stop() override;

	//Set state of mock - defines test case
	void set_state(FakeRecordingServiceStates_e state);
	bool is_callbackSet() override;
	
private:
	FakeRecordingServiceStates_e m_state;
	bool m_isHandleSet;
	GEN_CARD_CALLBACK_CONFIG_TYPE* m_callbackConfig;
	bool m_isCallbackSet;

    int pcm_drop() override;
    int pcm_recover(int err, int silent) override;
};

#endif
