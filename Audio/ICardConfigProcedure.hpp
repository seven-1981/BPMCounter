#ifndef _ICARDCONFIGPROCEDURE_H
#define _ICARDCONFIGPROCEDURE_H

#include <string>


//Forward declarations
struct ICardConfiguration_t;


class ICardConfigProcedure
{
public:
	virtual ~ICardConfigProcedure() { }
	virtual int execute(std::string& hw_id, ICardConfiguration_t& config) = 0;
	virtual void finish(ICardConfiguration_t& config) = 0;
    virtual int configure(ICardConfiguration_t& config) = 0;
	virtual int reopen(std::string& hw_id, ICardConfiguration_t& config) = 0;
};

#endif
