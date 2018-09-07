#include "wndoptions.h"
#include "bitmaptextmultiline.h"
#include "redbutton.h"
#include "pixelscene.h"

namespace{
	class NewRedButton :public RedButton{
	public:
		WndOptions* wnd;
		int index;
		NewRedButton(WndOptions* wnd, int index, const std::string& title) 
			:wnd(NULL)
			,index(0)
			,RedButton(title)
		{
			this->wnd = wnd;
			this->index = index;
		}

	protected:
		virtual void onClick()
		{
			wnd->hide();
			wnd->onSelect(index);

			delete wnd;
		}
	};
}
WndOptions::WndOptions(const std::string& title, const std::string& message, const std::vector<std::string>& options)
{
	BitmapTextMultiline* tfTitle = PixelScene::createMultiline(title, 9);
	tfTitle->hardlight(TITLE_COLOR);
	tfTitle->x = tfTitle->y = MARGIN;
	tfTitle->maxWidth = WIDTH - MARGIN * 2;
	tfTitle->measure();
	add(tfTitle);

	BitmapTextMultiline* tfMesage = PixelScene::createMultiline(message, 8);
	tfMesage->maxWidth = WIDTH - MARGIN * 2;
	tfMesage->measure();
	tfMesage->x = MARGIN;
	tfMesage->y = tfTitle->y + tfTitle->Height() + MARGIN;
	add(tfMesage);

	float pos = tfMesage->y + tfMesage->Height() + MARGIN;

	for (int i = 0; i < options.size(); i++) 
	{
		int index = i;
		RedButton* btn = new NewRedButton(this, index, options[i]);
		btn->setRect(MARGIN, pos, WIDTH - MARGIN * 2, BUTTON_HEIGHT);
		add(btn);

		pos += BUTTON_HEIGHT + MARGIN;
	}

	resize(WIDTH, (int)pos);
}