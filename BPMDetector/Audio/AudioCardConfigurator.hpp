#ifndef _AUDIOCARDCONFIGURATOR_H
#define _AUDIOCARDCONFIGURATOR_H

#include "ICardConfigurator.hpp"

#include <string>


//Forward declarations
enum class Errors_e;
struct ICardConfiguration_t;
class ICardConfigProcedure;
//Using for type simplifications
using GEN_CARD_CONFIG_TYPE = ICardConfiguration_t;


//Card info configuration with ALSA API
class AudioCardConfigurator : public ICardConfigurator
{
public:
	explicit AudioCardConfigurator() : m_hw_id(), m_procedure(nullptr) { }
	~AudioCardConfigurator() { }

	//Override the base class methods
	void set_hw_id(std::string& hw_id) override;
	void set_procedure(ICardConfigProcedure* procedure) override;
	Errors_e configure(GEN_CARD_CONFIG_TYPE& config) override;
	
private:
	//Hardware identifier used for open PCM device
	std::string m_hw_id;
	//Procedure used for configuration
	ICardConfigProcedure* m_procedure;

	Errors_e start_procedure(GEN_CARD_CONFIG_TYPE& config);
	void finish_procedure(GEN_CARD_CONFIG_TYPE& config);
};

#endif
