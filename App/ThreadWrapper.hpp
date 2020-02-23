#ifndef _THREADWRAPPER_H
#define _THREADWRAPPER_H

#include <condition_variable>
#include <mutex>
#include <future>
#include <atomic>

template <typename C, typename T>
using Func = T(C::*)();


template <typename RP, typename RC, typename CL>
class ThreadWrapper
{
public:
	ThreadWrapper(CL* inst)	:
		m_productionComplete(false),
		m_funcProd(nullptr),
		m_funcCons(nullptr),
		m_instance(inst),
		m_retVal(0),
		m_continueProd(true),
		m_continueCons(true),
		m_finishedProd(false),
		m_finishedCons(false) { }
	~ThreadWrapper() { }

	void set_producer(Func<CL, RP> func) { m_funcProd = func; }
	void set_consumer(Func<CL, RC> func) { m_funcCons = func; }

	void start_producer()
	{
		m_continueProd.store(true);
		m_futureProduce = std::async(std::launch::async, &ThreadWrapper::producer_loop, this);
	}

	void start_consumer()
	{
		m_continueCons.store(true);
		m_futureConsume = std::async(std::launch::async, &ThreadWrapper::consumer_loop, this);
	}

	RP producer_loop()
	{
		RP retVal { };
		while (m_continueProd.load() == true)
		{
			std::unique_lock<std::mutex> lck(m_mtx);
			m_produce.wait(lck, [this] { return !m_productionComplete; } );
			retVal = (m_instance->*m_funcProd)();
			m_productionComplete = true;
			lck.unlock();
			m_consume.notify_one();
		}
		m_finishedProd.store(true);
		return retVal;
	}

	RC consumer_loop()
	{
		RC retVal { };
		while (m_continueCons.load() == true)
		{
			std::unique_lock<std::mutex> lck(m_mtx);
			m_consume.wait(lck, [this] { return m_productionComplete; } );
			retVal = (m_instance->*m_funcCons)();
			m_retVal.store(retVal);
			m_productionComplete = false;
			lck.unlock();
			m_produce.notify_one();
		}
		m_finishedCons.store(true);
		return retVal;
	}

	RC get_value()
	{
		return m_retVal.load();
	}

	void stop_production()
	{
		m_continueProd.store(false);
		m_continueCons.store(false);
		while (m_finishedProd.load() == false) { /*Wait for prod. to exit*/ }
		while (m_finishedCons.load() == false) { /*Wait for cons. to exit*/ }
		m_finishedProd.store(false);
		m_finishedCons.store(false);
		m_productionComplete = false;
	}

private:
	std::condition_variable m_produce;
	std::condition_variable m_consume;
	std::mutex m_mtx;
	bool m_productionComplete;
	std::future<RP> m_futureProduce;
	std::future<RC> m_futureConsume;
	Func<CL, RP> m_funcProd;
	Func<CL, RC> m_funcCons;
	CL* m_instance;
	std::atomic<RC> m_retVal;
	std::atomic<bool> m_continueProd;
	std::atomic<bool> m_continueCons;
	std::atomic<bool> m_finishedProd;
	std::atomic<bool> m_finishedCons;
};

#endif