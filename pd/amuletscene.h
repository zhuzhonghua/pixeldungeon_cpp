#include "pixelscene.h"

class AmuletScene :public PixelScene{
private:
	static const String TXT_EXIT;
	static const String TXT_STAY;

	static const int WIDTH = 120;
	static const int BTN_HEIGHT = 18;
	static const float SMALL_GAP;
	static const float LARGE_GAP;

	static const String TXT;

	Image* amulet;

	float timer;
public:
	AmuletScene();
	static boolean noText;

	virtual void init();

	virtual void onBackPressed();

	virtual void update();
};