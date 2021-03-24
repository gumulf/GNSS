#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace gnss{

	bool activateCtrlHandler(HANDLE *handle);

}