#include "AudioCardConfigurator.cpp"
#include "FakeCardConfigProcedure.cpp"
#include <gtest/gtest.h>
#include "ALSACardConfiguration.hpp"


class TestAudioCardConfigurator : public testing::Test
{
public:
    FakeCardConfigProcedure procedure;
    ALSACardConfiguration_t config;
    AudioCardConfigurator configurator;

    void SetUp() override 
    {

    }
};
 
TEST_F(TestAudioCardConfigurator, CreatesInstance)
{
    AudioCardConfigurator configurator;
}

TEST_F(TestAudioCardConfigurator, ChecksForInitializedProcedure)
{
    AudioCardConfigurator configurator_not_initialized;
    
    ASSERT_EQ(configurator_not_initialized.configure(config), Errors_e::AUDIO_MANAGER_NOT_INITIALIZED);
}

TEST_F(TestAudioCardConfigurator, ExecutesProcedure)
{
    procedure.set_state(State_ConfigSuccess);

    configurator.set_procedure(&procedure);

    ASSERT_EQ(configurator.configure(config), Errors_e::NO_ERROR);
}

TEST_F(TestAudioCardConfigurator, ReturnsCorrectErrorIfOpenDeviceFails)
{
    procedure.set_state(State_OpenDeviceFailure);

    configurator.set_procedure(&procedure);

    ASSERT_EQ(configurator.configure(config), Errors_e::AUDIO_MANAGER_CARD_CONFIG_FAILURE);
}

TEST_F(TestAudioCardConfigurator, ReturnsCorrectErrorIfProcedureStepFails)
{
    procedure.set_state(State_AllocationFailure);

    configurator.set_procedure(&procedure);

    ASSERT_EQ(configurator.configure(config), Errors_e::AUDIO_MANAGER_CARD_CONFIG_FAILURE);    
}
