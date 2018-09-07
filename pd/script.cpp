#include "stdafx.h"

#include "script.h"

std::map<std::string, Script*> Script::_all;
Script* Script::_curScript;
std::string Script::_curScriptClass;

Script* Script::get(const std::string& c)
{
	if (c.compare(_curScriptClass) != 0)
	{
		std::map<std::string, Script*>::iterator itr = _all.find(c);
		if (itr == _all.end())
		{
			return NULL;
		}
		else
		{
			if (_curScript != NULL)
			{
				_curScript->unuse();
			}

			_curScript = itr->second;
			_curScriptClass = c;
			_curScript->use();			
		}
	}

	return _curScript;
}