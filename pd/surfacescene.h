#pragma once

#include "pixelscene.h"
#include "heroclass.h"
#include "movieclip.h"

class SurfaceScene :public PixelScene{
private:
	class GrassPatch :public Image{
	public:
		static const int WIDTH = 16;
		static const int HEIGHT = 14;
	private:
		float tx;
		float ty;

		double a;
		double angle;

		boolean forward;
	public:
		GrassPatch(float tx, float ty, boolean forward);
		virtual void update();
	protected:
		virtual void updateMatrix();
	};

	class Avatar :public Image{
	private:
		static const int WIDTH = 24;
		static const int HEIGHT = 28;

	public:
		Avatar(HeroClass cl);
	};

	class Pet :public MovieClip,public MovieClip::Listener{
	private:
		Animation* idle;
		Animation* jump;

	public:
		Pet();
		void Jump() {
			play(jump);
		}
		virtual void onComplete(Animation* anim) {
			if (anim == jump) {
				play(idle);
			}
		}
	};
private:
	static const int FRAME_WIDTH = 88;
	static const int FRAME_HEIGHT = 125;

	static const int FRAME_MARGIN_TOP = 9;
	static const int FRAME_MARGIN_X = 4;

	static const int BUTTON_HEIGHT = 20;

	static const int SKY_WIDTH = 80;
	static const int SKY_HEIGHT = 112;

	static const int NSTARS = 100;
	static const int NCLOUDS = 5;

	Camera* viewport;

private:
	class Sky :public Visual{
	private:
		static const int _day[];
		static const int _night[];
		static std::vector<int> day;
		static std::vector<int> night;

		SmartTexture* texture;
		FloatBuffer* verticesBuffer;

	public:
		Sky(boolean dayTime);
		virtual void draw();
	};

	class Cloud :public Image{
	private:
		static int lastIndex;

	public:
		Cloud(float y, boolean dayTime);
		virtual void update();
	};
public:
	virtual void init();
};