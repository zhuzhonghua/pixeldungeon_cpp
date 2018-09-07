#pragma once

#include "pixelscene.h"
#include "redbutton.h"
#include "heroclass.h"
#include "heroclass.h"
#include "emitter.h"

class StartScene :public PixelScene{
public:
	class GameButton :public RedButton{
	private:
		static const int SECONDARY_COLOR_N = 0xCACFC2;
		static const int SECONDARY_COLOR_H = 0xFFFF88;

		BitmapText* _secondary;		
	protected:
		virtual void createChildren();
		virtual void layout();
	public:
		GameButton(const std::string& primary);
		void secondary(const std::string& text, bool highlighted);
	};
	class ClassShield :public Button{
	private:
		static const float MIN_BRIGHTNESS;

		static const int BASIC_NORMAL = 0x444444;
		static const int BASIC_HIGHLIGHTED = 0xCACFC2;

		static const int MASTERY_NORMAL = 0x666644;
		static const int MASTERY_HIGHLIGHTED = 0xFFFF88;

		static const int WIDTH = 24;
		static const int HEIGHT = 28;
		static const int SCALE = 2;

		HeroClass cl;

		Image* avatar;
		BitmapText* name;
		Emitter* emitter;

		float brightness;

		int normal;
		int highlighted;

		void updateBrightness();		
	public:
		static StartScene* startScene;

		ClassShield(HeroClass cl);

		virtual void update();

		void highlight(bool value);
	protected:
		virtual void createChildren();
		virtual void layout();
		virtual void onTouchDown();
	};
	class ChallengeButton:public Button{
	public:
		Image* image;
	public:
		StartScene* scene;
		ChallengeButton(StartScene* sce);
	protected:
		virtual void createChildren();
		virtual void layout();
		virtual void onClick();
		virtual void onTouchDown();
	};

	static HeroClass curClass;
public:
	static const int BUTTON_HEIGHT = 24.0f;
	static const int GAP = 2;

	static const char* TXT_LOAD;
	static const char* TXT_NEW;

	static const char* TXT_ERASE;
	static const char* TXT_DPTH_LVL;

	static const char* TXT_REALLY;
	static const char* TXT_WARNING;
	static const char* TXT_YES;
	static const char* TXT_NO;

	static const char* TXT_UNLOCK;

	static const char* TXT_WIN_THE_GAME;

	static const int WIDTH_P = 116;
	static const int HEIGHT_P = 220;

	static const int WIDTH_L = 224;
	static const int HEIGHT_L = 124;
private:
	float buttonX;
	float buttonY;

	GameButton* btnLoad;
	GameButton* btnNewGame;

	bool huntressUnlocked;
	Group* unlock;

	
	static std::map<HeroClass, ClassShield*> shields;

	void updateClass(HeroClass cl);

public:
	virtual void init();
	virtual void destroy();

	void startNewGame();

	StartScene();
	~StartScene();

	virtual void onBackPressed();
};