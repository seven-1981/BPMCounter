#include "FakeRecordingService.hpp"

#include "ALSACardConfiguration.hpp"
#include "ALSACardCallbackConfiguration.hpp"


const int RETURN_EBADFD = -77;  //Values from ALSA library
const int RETURN_EPIPE = -32;
const int RETURN_ESTRPIPE = -86;
const int RETURN_EAGAIN = -11;
const int RETURN_NO_ERROR = 0;
const int RETURN_ANY_ERROR = -1;

const int RECOVER_SILENT = 1;

const int MORE_THAN_PERIOD_SIZE = 257;


int FakeRecordingService::set_handle(GEN_CARD_CONFIG_TYPE& config)
{
    CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
    if (typedConfig.INITIALIZED == true)
    {
        m_isHandleSet = true;
        return RETURN_NO_ERROR;
    }
    else
    {
        return RETURN_ANY_ERROR;
    }
}

bool FakeRecordingService::is_handleSet()
{
    return m_isHandleSet;
}

int FakeRecordingService::pcm_readi(void* buffer, int size)
{
    switch (m_state)
    {
        case State_CaptureSuccess:
            return size;
            break;

        case State_CaptureReturns_EBADFD_RecoverNotPossible:
            IGNORE pcm_drop();
            return pcm_recover(RETURN_EBADFD, RECOVER_SILENT);
            break;

        case State_CaptureReturns_EPIPE_RecoverError:
        case State_CaptureReturns_EPIPE_RecoverSuccess:
            IGNORE pcm_drop();
            return pcm_recover(RETURN_EPIPE, RECOVER_SILENT);
            break;

        case State_CaptureReturns_ESTRPIPE_RecoverError:
        case State_CaptureReturns_ESTRPIPE_RecoverSuccess:
            IGNORE pcm_drop();
            return pcm_recover(RETURN_ESTRPIPE, RECOVER_SILENT);
            break;

        case State_CaptureLessSamples:
            return size - 1;
            break;
        
        default:
            return RETURN_ANY_ERROR;
            break;
    }
}

int FakeRecordingService::pcm_drop()
{
    //Drop return value is ignored, we set it to error 
    //to remind us when that behaviour changes
    return RETURN_ANY_ERROR;
}

int FakeRecordingService::pcm_recover(int err, int silent)
{
    switch (m_state)
    {
        case State_CaptureSuccess:
            //pcm_recover is not called upon success
            return RETURN_ANY_ERROR;
            break;

        case State_CaptureReturns_EBADFD_RecoverNotPossible:
            return RETURN_EBADFD;
            break;

        case State_CaptureReturns_EPIPE_RecoverError:
            return RETURN_EPIPE;
            break;

        case State_CaptureReturns_EPIPE_RecoverSuccess:
            return RETURN_NO_ERROR;
            break;

        case State_CaptureReturns_ESTRPIPE_RecoverError:
            return RETURN_ESTRPIPE;
            break;

        case State_CaptureReturns_ESTRPIPE_RecoverSuccess:
            return RETURN_NO_ERROR;
            break;

        default:
            return RETURN_ANY_ERROR;
            break;        
    }
}

int FakeRecordingService::add_pcm_handler(GEN_CARD_CALLBACK_CONFIG_TYPE& config)
{
	if (is_handleSet() == true)
	{
		m_callbackConfig = &dynamic_cast<CARD_CALLBACK_CONFIG_TYPE&>(config);
		m_isCallbackSet = true;
		return RETURN_NO_ERROR;
	}
	else
	{
		return RETURN_ANY_ERROR;
	}
}

int FakeRecordingService::pcm_start()
{
	if (m_isCallbackSet == false)
		return RETURN_ANY_ERROR;

	switch (m_state)
	{
	    case State_CaptureAsyncSuccess:
	    {
	    	CARD_CALLBACK_CONFIG_TYPE* typedConfig = dynamic_cast<CARD_CALLBACK_CONFIG_TYPE*>(m_callbackConfig);
	    	(typedConfig->PCM_ASYNC_CALLBACK)(typedConfig->PCM_ASYNC_HANDLER);
	        return RETURN_NO_ERROR;
	        break;
	    }

	   case State_CaptureAsyncSuccessWithMemberFunction:
	   {
		   CARD_CALLBACK_CONFIG_TYPE* typedConfig = dynamic_cast<CARD_CALLBACK_CONFIG_TYPE*>(m_callbackConfig);
		   typedConfig->PCM_ASYNC_PRIVATE_DATA.RECORDER_INSTANCE->samples_available(MORE_THAN_PERIOD_SIZE);
	       return RETURN_NO_ERROR;
	       break;
	   }

	   case State_CaptureAsyncFailure:
	        return RETURN_ANY_ERROR;
	        break;

	   default:
		   	return RETURN_ANY_ERROR;
		   	break;
	}
}

int FakeRecordingService::pcm_stop()
{
	switch (m_state)
	{
	    case State_CaptureAsyncSuccess:
	        return RETURN_NO_ERROR;
	        break;

	   case State_CaptureAsyncFailure:
	        return RETURN_ANY_ERROR;
	        break;

	   default:
		   	return RETURN_ANY_ERROR;
		   	break;
	}
}

void FakeRecordingService::set_state(FakeRecordingServiceStates_e state)
{
	m_state = state;
}

bool FakeRecordingService::is_callbackSet()
{
	return m_isCallbackSet;
}
