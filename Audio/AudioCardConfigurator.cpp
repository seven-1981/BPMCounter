#include "AudioCardConfigurator.hpp"
#include "ICardConfigProcedure.hpp"
#include "globals.hpp"


void AudioCardConfigurator::set_hw_id(std::string& hw_id)
{
	m_hw_id = hw_id;
}

void AudioCardConfigurator::set_procedure(ICardConfigProcedure* procedure)
{
	m_procedure = procedure;
}

Errors_e AudioCardConfigurator::configure(GEN_CARD_CONFIG_TYPE& config)
{
	if (m_procedure != nullptr)
	{
		return start_procedure(config);
	}
	else
	{
		return Errors_e::AUDIO_MANAGER_NOT_INITIALIZED;
	}
}

Errors_e AudioCardConfigurator::start_procedure(GEN_CARD_CONFIG_TYPE& config)
{
	if (m_procedure->execute(m_hw_id, config) < 0)
	{
		return Errors_e::AUDIO_MANAGER_CARD_CONFIG_FAILURE;
	}
	else
	{
		return Errors_e::NO_ERROR;
	}

}

void AudioCardConfigurator::finish_procedure(GEN_CARD_CONFIG_TYPE& config)
{
	m_procedure->finish(config);
}

