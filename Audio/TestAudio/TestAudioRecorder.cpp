#include "FakeRecordingService.cpp"
#include "AudioRecorder.cpp"
#include "ALSACardConfiguration.hpp"
#include "ALSACardCallbackConfiguration.hpp"
#include "Buffer.hpp"
#include <gtest/gtest.h>


bool samples_available = false;

ALSA_CALLBACK_RETURN callback_test(ALSA_CALLBACK_PARAM param)
{
	//For testing, just call samples_available
	samples_available = true;
}


class TestAudioRecorder : public testing::Test
{
public:
    using SAMPLE_TYPE = short signed int;

    AudioRecorder recorder;
    AudioCardLister lister;
    FakeCardInfoGetter infoGetter;
    FakeCardConfigurator configurator;
    AudioCardManager manager;
    ALSACardConfiguration_t config;
    FakeRecordingService service;
    Buffer<SAMPLE_TYPE> buffer;
    ALSACardCallbackConfiguration_t callback_config;

    void SetUp() override
    {
        infoGetter.set_state(State_GetSuccess);
	    lister.init(&infoGetter);
	    manager.init_lister(&lister);
        configurator.set_state(State_ConfigurationSuccess);
	    manager.init_configurator(&configurator);
        manager.select_and_configure(config);
        recorder.set_service(&service);
        buffer.init(config.PCM_BUFFER_SIZE);
        callback_config.PCM_ASYNC_CALLBACK = callback_test;
        callback_config.PCM_ASYNC_PRIVATE_DATA.RECORDER_INSTANCE = &recorder;
    }

    void TearDown() override
    {
    	samples_available = false;
    }
};
 
TEST_F(TestAudioRecorder, CreatesInstance)
{
    AudioRecorder recorder;
}

TEST_F(TestAudioRecorder, ChecksForInitializationOfRecordingService)
{
    AudioRecorder recorderNotInitialized;
    Errors_e returnValue = recorderNotInitialized.prepare_recorder(config);

    ASSERT_EQ(returnValue, Errors_e::AUDIO_RECORDER_NOT_INITIALIZED);    
}

TEST_F(TestAudioRecorder, ChecksForInitializationOfPcmHandle)
{
    ALSACardConfiguration_t empty_config;

    Errors_e returnValue = recorder.prepare_recorder(empty_config);

    ASSERT_EQ(returnValue, Errors_e::AUDIO_RECORDER_NOT_INITIALIZED); 
}

TEST_F(TestAudioRecorder, PreparesBuffer)
{
    Errors_e returnValue = recorder.prepare_recorder(config);

    ASSERT_EQ(returnValue, Errors_e::NO_ERROR);
}

TEST_F(TestAudioRecorder, ReturnsErrorCodeIfCaptureCalledWithoutPreparation)
{
    Errors_e returnValue = recorder.capture_samples(config, buffer);

    ASSERT_EQ(returnValue, Errors_e::AUDIO_RECORDER_NOT_INITIALIZED);
}

TEST_F(TestAudioRecorder, CapturesSamplesInSynchronousMode)
{
    service.set_state(State_CaptureSuccess);
    IGNORE recorder.prepare_recorder(config);

    ASSERT_EQ(recorder.capture_samples(config, buffer), Errors_e::NO_ERROR);
}

TEST_F(TestAudioRecorder, CapturesLessSamplesThanBufferSize)
{
    service.set_state(State_CaptureLessSamples);
    IGNORE recorder.prepare_recorder(config);

    ASSERT_EQ(recorder.capture_samples(config, buffer), Errors_e::AUDIO_RECORDER_CAPTURE_ERROR);
}

TEST_F(TestAudioRecorder, ReturnsErrorCodeIfCaptureFailsWithEBADFD)
{
    service.set_state(State_CaptureReturns_EBADFD_RecoverNotPossible);
    IGNORE recorder.prepare_recorder(config);

    ASSERT_EQ(recorder.capture_samples(config, buffer), Errors_e::AUDIO_RECORDER_CAPTURE_EBADFD);
}

TEST_F(TestAudioRecorder, ReturnsErrorCodeIfCaptureFailsWithEPIPERecoverError)
{
    service.set_state(State_CaptureReturns_EPIPE_RecoverError);
    IGNORE recorder.prepare_recorder(config);

    ASSERT_EQ(recorder.capture_samples(config, buffer), Errors_e::AUDIO_RECORDER_CAPTURE_EPIPE);
}

TEST_F(TestAudioRecorder, ReturnsErrorCodeIfCaptureFailsWithEPIPERecoverSuccess)
{
    service.set_state(State_CaptureReturns_EPIPE_RecoverSuccess);
    IGNORE recorder.prepare_recorder(config);

    ASSERT_EQ(recorder.capture_samples(config, buffer), Errors_e::NO_ERROR);
}

TEST_F(TestAudioRecorder, ReturnsErrorCodeIfCaptureFailsWithESTRPIPERecoverError)
{
    service.set_state(State_CaptureReturns_ESTRPIPE_RecoverError);
    IGNORE recorder.prepare_recorder(config);

    ASSERT_EQ(recorder.capture_samples(config, buffer), Errors_e::AUDIO_RECORDER_CAPTURE_ESTRPIPE);
}

TEST_F(TestAudioRecorder, ReturnsErrorCodeIfCaptureFailsWithESTRPIPERecoverSuccess)
{
    service.set_state(State_CaptureReturns_ESTRPIPE_RecoverSuccess);
    IGNORE recorder.prepare_recorder(config);

    ASSERT_EQ(recorder.capture_samples(config, buffer), Errors_e::NO_ERROR);
}

TEST_F(TestAudioRecorder, PerformsCheckIfCallbackIsSet)
{
	service.set_state(State_CaptureAsyncSuccess);
	IGNORE recorder.prepare_recorder(config);

	ASSERT_EQ(recorder.start_capturing(), Errors_e::AUDIO_RECORDER_CALLBACK_ERROR);
}

TEST_F(TestAudioRecorder, CallsCallbackFunctionInAsyncMode)
{
	service.set_state(State_CaptureAsyncSuccess);
	IGNORE recorder.prepare_recorder(config);
	recorder.set_callback(callback_config);

	recorder.start_capturing();

	ASSERT_EQ(samples_available, true);
}

TEST_F(TestAudioRecorder, ReturnsErrorIfStartCaptureFailsInAsyncMode)
{
	service.set_state(State_CaptureAsyncFailure);
	IGNORE recorder.prepare_recorder(config);
	recorder.set_callback(callback_config);

	ASSERT_EQ(recorder.start_capturing(), Errors_e::AUDIO_RECORDER_CAPTURE_ERROR);
}

TEST_F(TestAudioRecorder, CallsMemberCallbackFunctionInAsyncMode)
{
	service.set_state(State_CaptureAsyncSuccessWithMemberFunction);
	IGNORE recorder.prepare_recorder(config);
	recorder.set_callback(callback_config);

	recorder.start_capturing();

	ASSERT_EQ(recorder.is_samplesAvailable(), true);
}

TEST_F(TestAudioRecorder, SetCallbackReturnsErrorIfNotConfigured)
{
	service.set_state(State_CaptureAsyncSuccessWithMemberFunction);

	ASSERT_EQ(recorder.set_callback(callback_config), Errors_e::AUDIO_RECORDER_NOT_INITIALIZED);
}

TEST_F(TestAudioRecorder, ClearsCallbackConfig)
{
	IGNORE recorder.prepare_recorder(config);
	recorder.set_callback(callback_config);

	ASSERT_EQ(recorder.clear_callback(callback_config), Errors_e::NO_ERROR);
}
