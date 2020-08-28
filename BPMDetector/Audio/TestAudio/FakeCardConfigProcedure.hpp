#ifndef _FAKECARDCONFIGPROCEDURE_H
#define _FAKECARDCONFIGPROCEDURE_H

#include "ICardConfigProcedure.hpp"

#include <string>


//Forward declarations
struct ICardConfiguration_t;
struct ALSACardConfiguration_t;
//Using for type simplifications
using GEN_CARD_CONFIG_TYPE = ICardConfiguration_t;
using CARD_CONFIG_TYPE = ALSACardConfiguration_t;


//States used in this mock
enum FakeCardConfigProcedureStates_e
{
	State_OpenDeviceFailure,		//Open device returns error
	State_AllocationFailure, 		//HW param allocation error
	State_ParamInitFailure,			//HW param initialization error
	State_SetAccessModeFailure, 	//Set access mode returns error
	State_SetAudioFormatFailure,	//Set audio format returns error
	State_SetSampleRateFailure,		//Set sample rate returns error
	State_SetNumChannelsFailure,	//Set number of channels returns error
	State_ApplyParamsFailure,		//Apply params returns error
	State_PrepareInterfaceFailure,	//Prepare interface returns error
    State_CloseDeviceFailure,       //Close device returns error
	State_SetPeriodSizeFailure,		//Set period size returns error
	State_ConfigSuccess				//Configuration successful
};


class FakeCardConfigProcedure : public ICardConfigProcedure
{
public:
    FakeCardConfigProcedure() : m_state(State_OpenDeviceFailure) { }
    ~FakeCardConfigProcedure() { }
	int execute(std::string& hw_id, GEN_CARD_CONFIG_TYPE& config) override;
	void finish(GEN_CARD_CONFIG_TYPE& config) override;
    int configure(GEN_CARD_CONFIG_TYPE& config);
	int reopen(std::string& hw_id, GEN_CARD_CONFIG_TYPE& config);

	//Set state of mock - defines test case
	void set_state(FakeCardConfigProcedureStates_e state);

private:
	FakeCardConfigProcedureStates_e m_state;

	int open_device(std::string& hw_id, CARD_CONFIG_TYPE& config);
	int allocate_params(CARD_CONFIG_TYPE& config);
	int init_params(CARD_CONFIG_TYPE& config);
	int set_access_mode(CARD_CONFIG_TYPE& config);
	int set_audio_format(CARD_CONFIG_TYPE& config);
	int set_sample_rate(CARD_CONFIG_TYPE& config);
	int set_num_channels(CARD_CONFIG_TYPE& config);
	int apply_params(CARD_CONFIG_TYPE& config);
	int prepare_interface(CARD_CONFIG_TYPE& config);
	void free_params(CARD_CONFIG_TYPE& config);
	int close_device(CARD_CONFIG_TYPE& config);
    int set_period_size(CARD_CONFIG_TYPE& config);
};

#endif



