#pragma once

#include "component.h"
#include "simplebutton.h"

class NinePatch;
class BitmapText;

class Toast :public Component{
private:
	class SimpleButtonNew :public SimpleButton{
	public:
		Toast* t;
		SimpleButtonNew(Toast* tp, Image* img) :SimpleButton(img),t(tp){}
	protected:
		virtual void onClick() { t->onClose(); };
	};
	static const int MARGIN_HOR = 2;
	static const int MARGIN_VER = 2;

protected:
	NinePatch* bg;
	SimpleButton* close;
	BitmapText* text;

	virtual void createChildren();
	virtual void layout();

	virtual void onClose() {};
public:
	Toast(const std::string& text);
	void Text(const std::string& txt);
};