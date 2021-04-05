#include "ctrlhandler.h"
#include "comm.h"

namespace gnss{

	ShutdownSequence *m_sq{};
	
	BOOL WINAPI handler(DWORD ctrlType){
		switch(ctrlType){
			case CTRL_C_EVENT:
			case CTRL_BREAK_EVENT:
				(*m_sq).activate();
				Sleep(5000);
				exit(0); // Just shut everything down // TODO remove from here?
				return TRUE;
			default:
				return FALSE;
		};
	}

	bool activateCtrlHandler(ShutdownSequence &sq){

		if(SetConsoleCtrlHandler(handler, true)){
			m_sq = &sq;
			return true;
		}
		else{
			return false;
		}
	}

	bool deactivateCtrlHandler(){
		if(SetConsoleCtrlHandler(handler, false)){
			ShutdownSequence sq;
			m_sq = &sq;
			return true;
		}
		else{
			return false;
		}
	}

	ShutdownSequence::ShutdownSequence(){
		std::atomic_init(&m_activated, false);
	}


	void ShutdownSequence::activate(){
		m_activated.store(true);
		m_sq_cv.notify_all();
	}

	bool ShutdownSequence::isActivated(){
		return m_activated.load();
	}

	void ShutdownSequence::waitForActivation(){
		std::unique_lock<std::mutex> sq_lock(m_sq_mx);
		m_sq_cv.wait(sq_lock, [this] (){return this->isActivated(); });
	}


	ShutdownSequence &ShutdownSequence::operator=(const ShutdownSequence &other){
		if(this == &other)
			return *this;
		this->m_activated = &other.m_activated;
		return *this;
	}

}