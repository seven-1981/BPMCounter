#ifndef _FAKECARDCONFIGURATOR_H
#define _FAKECARDCONFIGURATOR_H

#include "ICardConfigurator.hpp"


//Forward declarations
struct ICardConfiguration_t;
struct ALSACardConfiguration_t;
class ICardConfigProcedure;
//Using for type simplifications
using GEN_CARD_CONFIG_TYPE = ICardConfiguration_t;
using CARD_CONFIG_TYPE = ALSACardConfiguration_t;


//States used in this mock
enum FakeCardConfiguratorStates_e
{
	State_ConfigurationFailure,
	State_ConfigurationSuccess
};


//Mock object for testing
class FakeCardConfigurator : public ICardConfigurator
{
public:
	FakeCardConfigurator() : m_state(State_ConfigurationFailure), m_hw_id(), m_procedure(nullptr) { }
	~FakeCardConfigurator() { }
	
	//Set state of mock - defines test case
	void set_state(FakeCardConfiguratorStates_e state);
	
	//Override the base class methods
	void set_hw_id(std::string& hw_id) override;
	void set_procedure(ICardConfigProcedure* procedure) override;
	Errors_e configure(GEN_CARD_CONFIG_TYPE& config) override;
	
private:
	FakeCardConfiguratorStates_e m_state;
	std::string m_hw_id;
	ICardConfigProcedure* m_procedure;
};

#endif
