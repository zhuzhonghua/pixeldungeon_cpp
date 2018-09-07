#pragma once

#include "wndtitledmessage.h"
#include "icon.h"

namespace{
	const char* TXT_TITLE = "ERROR";
}

class WndError :public WndTitledMessage{
private:
	
public:
	WndError(const std::string& message)
		:WndTitledMessage(Icons::get(Icons::WARNING), TXT_TITLE, message)
	{

	}
};