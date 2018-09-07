#include "pd/stdafx.h"
#include "pd/pixeldungeon.h"
#include "pd/titlescene.h"

int main(int argc, char** args)
{
	PixelDungeon pd;
	pd.init();

	pd.switchScene(new TitleScene());	
	pd.run();
	return 0;
}
