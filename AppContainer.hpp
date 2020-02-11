#ifndef _APPCONTAINER_H
#define _APPCONTAINER_H

#include "ALSACardConfiguration.hpp"
#include "Buffer.hpp"
#include "AudioController.hpp"
#include "AnalyzerController.hpp"
#include "globals.hpp"

#include <condition_variable>
#include <mutex>
#include <iostream>


class AppContainer
{
public:
	AppContainer() :
		m_config(),
		m_buffer(nullptr),
		m_audioController(m_config),
		m_analyzerController(nullptr)
	{
		using BUFFER_TYPE = decltype(m_config.PCM_SAMPLE_TYPE);
		m_buffer = new Buffer<BUFFER_TYPE>(m_config.PCM_BUFFER_SIZE);
		m_analyzerController = new AnalyzerController<BUFFER_TYPE>(*m_buffer);
	}

	~AppContainer()
	{
		delete m_buffer;
		delete m_analyzerController;
	}

	Errors_e init()
	{
		Errors_e retVal = Errors_e::NO_ERROR;
		if ((retVal = m_audioController.config_audio(m_config)) != Errors_e::NO_ERROR)
			return retVal;
		return retVal;
	}

	Errors_e record_audio()
	{
		std::lock_guard<std::mutex> lg(m_mtx);
		std::cout << "Recording audio..." << std::endl;
		using BUFFER_TYPE = decltype(m_config.PCM_SAMPLE_TYPE);
		using BT = Buffer<BUFFER_TYPE>;
		BT* typedBuffer = dynamic_cast<BT*>(m_buffer);
		if (typedBuffer == nullptr)
			return Errors_e::APP_CONTAINER_BUFFER_TYPE_ERROR;
		Errors_e retVal = Errors_e::NO_ERROR;
		if ((retVal = m_audioController.synchronous_record(*typedBuffer)) != Errors_e::NO_ERROR)
			return retVal;
		m_cv.notify_one();
		return retVal;
	}

	double detect_bpm(AudioController& controller, BufferBase& buffer)
	{
		std::unique_lock<std::mutex> lck(m_mtx);
		m_cv.wait(lck);
		std::cout << "Detecting..." << std::endl;
		double bpm = m_analyzerController->analyze();
		std::cout << "Value = " << bpm << std::endl;
		return bpm;
	}

private:
	ALSACardConfiguration_t m_config;
	BufferBase* m_buffer;
	AudioController m_audioController;
	IAnalyzer* m_analyzerController;

	std::condition_variable m_cv;
	std::mutex m_mtx;
};

#endif
