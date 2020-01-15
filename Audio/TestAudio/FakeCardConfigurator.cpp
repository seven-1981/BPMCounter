#include "FakeCardConfigurator.hpp"
#include "ICardConfigProcedure.hpp"
#include "ALSACardConfiguration.hpp"
#include "globals.hpp"


void FakeCardConfigurator::set_hw_id(std::string& hw_id)
{
	m_hw_id = hw_id;
}

void FakeCardConfigurator::set_procedure(ICardConfigProcedure* procedure)
{
	m_procedure = procedure;
}

Errors_e FakeCardConfigurator::configure(GEN_CARD_CONFIG_TYPE& config)
{
	//Simulate results of configuration process
	Errors_e retVal = Errors_e::AUDIO_MANAGER_CARD_CONFIG_FAILURE;
	CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
	switch (m_state)
	{
		case State_ConfigurationFailure:
			retVal = Errors_e::AUDIO_MANAGER_CARD_CONFIG_FAILURE;
			break;

		case State_ConfigurationSuccess:
			//Simulate init flag - this is normally set by config procedure
			typedConfig.INITIALIZED = true;
			retVal = Errors_e::NO_ERROR;
			break;
			
		default:
			retVal =  Errors_e::AUDIO_MANAGER_CARD_CONFIG_FAILURE;
			break;
	}
	return retVal;
}

void FakeCardConfigurator::set_state(FakeCardConfiguratorStates_e state)
{
	m_state = state;
}
