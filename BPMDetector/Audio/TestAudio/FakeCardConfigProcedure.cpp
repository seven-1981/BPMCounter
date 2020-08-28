#include "FakeCardConfigProcedure.hpp"


const int RETURN_NO_ERR = 0;
const int RETURN_ANY_ERR = -1;


void FakeCardConfigProcedure::set_state(FakeCardConfigProcedureStates_e state)
{
	m_state = state;
}

int FakeCardConfigProcedure::execute(std::string& hw_id, GEN_CARD_CONFIG_TYPE& config)
{
    CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
    int error = 0;
    if ((error = reopen(hw_id, typedConfig)) < 0)    	return error;
    if ((error = configure(typedConfig)) < 0)         	return error;
	if ((error = prepare_interface(typedConfig)) < 0)	return error;
	typedConfig.INITIALIZED = true;
	return 0;
}

void FakeCardConfigProcedure::finish(GEN_CARD_CONFIG_TYPE& config)
{
	CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
	free_params(typedConfig);
}

int FakeCardConfigProcedure::configure(GEN_CARD_CONFIG_TYPE& config)
{
	CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
    int error = 0;
	if ((error = allocate_params(typedConfig)) < 0)		return error;
	if ((error = init_params(typedConfig)) < 0) 		return error;
	if ((error = set_access_mode(typedConfig)) < 0)   	return error;
	if ((error = set_audio_format(typedConfig)) < 0)  	return error;
	if ((error = set_sample_rate(typedConfig)) < 0)		return error;
	if ((error = set_num_channels(typedConfig)) < 0)	return error;
	if ((error = set_period_size(typedConfig)) < 0)		return error;
	if ((error = apply_params(typedConfig)) < 0)  		return error;
    return 0;
}

int FakeCardConfigProcedure::reopen(std::string& hw_id, GEN_CARD_CONFIG_TYPE& config)
{
	CARD_CONFIG_TYPE& typedConfig = static_cast<CARD_CONFIG_TYPE&>(config);
    int error = 0;
	if ((error = close_device(typedConfig)) < 0)			return error;
	if ((error = open_device(hw_id, typedConfig)) < 0) 		return error;
    return 0;
}

int FakeCardConfigProcedure::open_device(std::string& hw_id, CARD_CONFIG_TYPE& config)
{
    if (m_state == State_OpenDeviceFailure) return RETURN_ANY_ERR;
    return RETURN_NO_ERR;
}

int FakeCardConfigProcedure::allocate_params(CARD_CONFIG_TYPE& config)
{
    if (m_state == State_AllocationFailure) return RETURN_ANY_ERR;
    return RETURN_NO_ERR;
}

int FakeCardConfigProcedure::init_params(CARD_CONFIG_TYPE& config)
{
    if (m_state == State_ParamInitFailure) return RETURN_ANY_ERR;
    return RETURN_NO_ERR;
}

int FakeCardConfigProcedure::set_access_mode(CARD_CONFIG_TYPE& config)
{
    if (m_state == State_SetAccessModeFailure) return RETURN_ANY_ERR;
    return RETURN_NO_ERR;
}

int FakeCardConfigProcedure::set_audio_format(CARD_CONFIG_TYPE& config)
{
    if (m_state == State_SetAudioFormatFailure) return RETURN_ANY_ERR;
    return RETURN_NO_ERR;
}

int FakeCardConfigProcedure::set_sample_rate(CARD_CONFIG_TYPE& config)
{
    if (m_state == State_SetSampleRateFailure) return RETURN_ANY_ERR;
    return RETURN_NO_ERR;
}

int FakeCardConfigProcedure::set_num_channels(CARD_CONFIG_TYPE& config)
{
    if (m_state == State_SetNumChannelsFailure) return RETURN_ANY_ERR;
    return RETURN_NO_ERR;
}

int FakeCardConfigProcedure::apply_params(CARD_CONFIG_TYPE& config)
{
    if (m_state == State_ApplyParamsFailure) return RETURN_ANY_ERR;
    return RETURN_NO_ERR;
}

int FakeCardConfigProcedure::prepare_interface(CARD_CONFIG_TYPE& config)
{
    if (m_state == State_PrepareInterfaceFailure) return RETURN_ANY_ERR;
    return RETURN_NO_ERR;
}

void FakeCardConfigProcedure::free_params(CARD_CONFIG_TYPE& config)
{

}

int FakeCardConfigProcedure::close_device(CARD_CONFIG_TYPE& config)
{
    if (m_state == State_CloseDeviceFailure) return RETURN_ANY_ERR;
    return RETURN_NO_ERR;
}

int FakeCardConfigProcedure::set_period_size(CARD_CONFIG_TYPE& config)
{
	if (m_state == State_CloseDeviceFailure) return RETURN_ANY_ERR;
    return RETURN_NO_ERR;
}
