#include "ctrlhandler.h"
#include "comm.h"

namespace gnss{

	HANDLE *m_handle;

	BOOL WINAPI handler(DWORD ctrlType){
		switch(ctrlType){
			case CTRL_C_EVENT:
			case CTRL_BREAK_EVENT:
				if(!INVALID_HANDLE_VALUE){
					closePort(*m_handle);
				}
				exit(0);
				return TRUE;
			default:
				return FALSE;
		};
	}

	bool activateCtrlHandler(HANDLE *handle){
		m_handle = handle;
		return SetConsoleCtrlHandler(handler, true);
	}

	bool deactivateCtrlHandler(){
		*m_handle = INVALID_HANDLE_VALUE;
		return SetConsoleCtrlHandler(handler, false);
	}
}