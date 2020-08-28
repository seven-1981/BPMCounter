#ifndef _ALSACardConfigProcedure_H
#define _ALSACardConfigProcedure_H

#include "ICardConfigProcedure.hpp"

#include <string>


//Forward declarations
struct ICardConfiguration_t;
struct ALSACardConfiguration_t;
//Using for type simplifications
using GEN_CARD_CONFIG_TYPE = ICardConfiguration_t;
using CARD_CONFIG_TYPE = ALSACardConfiguration_t;


class ALSACardConfigProcedure : public ICardConfigProcedure
{
public:
    ~ALSACardConfigProcedure() { }
	int execute(std::string& hw_id, GEN_CARD_CONFIG_TYPE& config) override;
	void finish(GEN_CARD_CONFIG_TYPE& config) override;
    int configure(GEN_CARD_CONFIG_TYPE& config);
	int reopen(std::string& hw_id, GEN_CARD_CONFIG_TYPE& config);

private:
	//Methods for ALSA configuration commands
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
