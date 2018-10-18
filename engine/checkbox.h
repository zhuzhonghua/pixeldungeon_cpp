#pragma once

#include "button.h"

class CheckBox :public Button{
protected:
	bool checked;
public:
	bool isChecked() { return checked; }
	void check(bool value) { 
		if (checked != value){
			checked = value;
			updateState();
		}
	}
protected:
	void updateState(){}
	void onClick(){ check(!checked); onChange(); }
	void onChange(){}
};