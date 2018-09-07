#pragma once

#include "redbutton.h"
class CheckBoxRed :public RedButton{
private:
	bool checked;

protected:
	virtual void layout();
	virtual void onClick();
public:
	CheckBoxRed(const std::string& label);
	bool Checked() { return checked; }
	void Checked(bool value);
};