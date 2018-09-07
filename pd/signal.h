#pragma once

#include <vector>
#include <algorithm>

template<class T>
class Signal{
public:
	class Listener {
	public:
		virtual void onSignal(T* t) = 0;
	};

	Signal() :Signal(false)
	{
	}

	Signal(bool stackMode) 
	{
		this->_stackMode = stackMode;
	}

	void add(Listener* listener)
	{
		std::vector<Listener*>::iterator itr = std::find(_listeners.begin(), _listeners.end(), listener);
		if (itr == _listeners.end())
		{
			if (_stackMode)
			{
				_listeners.insert(_listeners.begin(), listener);
			}
			else 
			{
				_listeners.push_back(listener);
			}
		}
	}

	void remove(Listener* listener) 
	{
		std::vector<Listener*>::iterator itr = std::find(_listeners.begin(), _listeners.end(), listener);
		if (itr != _listeners.end())
		{
			_listeners.erase(itr);
		}
	}

	void removeAll()
	{
		_listeners.clear();
	}

	void replace(Listener* listener) 
	{
		removeAll();
		add(listener);
	}
	int numListeners() 
	{
		return _listeners.size();
	}
	void dispatch(T* t)
	{
		_canceled = false;
		for (int i = 0; i < _listeners.size(); i++) 
		{
			Listener* listener = _listeners[i];

			listener->onSignal(t);
			if (_canceled) 
			{
				return;
			}

		}
	}
	void cancel()
	{
		_canceled = true;
	}
private:
	std::vector<Listener*> _listeners;
	bool _canceled;
	bool _stackMode;
};