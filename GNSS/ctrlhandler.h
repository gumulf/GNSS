#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace gnss{
	class ShutdownSequence{
	
	private:
		std::atomic<bool> m_activated;
		std::mutex m_sq_mx;
		std::condition_variable m_sq_cv;

	public:
		ShutdownSequence();
		void activate();
		bool isActivated();

		void waitForActivation();

		ShutdownSequence &operator=(const ShutdownSequence &other);

	};

	bool activateCtrlHandler(ShutdownSequence &sq);
	bool deactivateCtrlHandler();


}