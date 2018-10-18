#include "surfacescene.h"
#include "archs.h"
#include "quad.h"
#include "dungeon.h"
#include "noosascript.h"
#include "game.h"
#include "redbutton.h"
#include "titlescene.h"
#include "hero.h"

void SurfaceScene::init()
{
	PixelScene::init();

	//Music.INSTANCE.play(Assets.HAPPY, true);
	//Music.INSTANCE.volume(1f);

	uiCamera->visible = false;

	int w = Camera::mainCamera->width;
	int h = Camera::mainCamera->height;

	Archs* archs = new Archs();
	archs->reversed = true;
	archs->setSize(w, h);
	add(archs);

	float vx = align((w - SKY_WIDTH) / 2);
	float vy = align((h - SKY_HEIGHT - BUTTON_HEIGHT) / 2);

	Point s = Camera::mainCamera->cameraToScreen(vx, vy);
	viewport = new Camera(s.x, s.y, SKY_WIDTH, SKY_HEIGHT, defaultZoom);
	Camera::add(viewport);

	Group* window = new Group();
	window->cameraf = viewport;
	add(window);

	boolean dayTime = !Dungeon::nightMode;

	Sky* sky = new Sky(dayTime);
	GameMath::PointFSet(&sky->scale, SKY_WIDTH, SKY_HEIGHT);
	//sky->scale.set(SKY_WIDTH, SKY_HEIGHT);
	window->add(sky);

	if (!dayTime) {
		for (int i = 0; i < NSTARS; i++) {
			float size = Random::Float();
			ColorBlock* star = new ColorBlock(size, size, 0xFFFFFFFF);
			star->x = Random::Float(SKY_WIDTH) - size / 2;
			star->y = Random::Float(SKY_HEIGHT) - size / 2;
			star->am = size * (1 - star->y / SKY_HEIGHT);
			window->add(star);
		}
	}

	float range = SKY_HEIGHT * 2 / 3;
	for (int i = 0; i < NCLOUDS; i++) {
		Cloud* cloud = new Cloud((NCLOUDS - 1 - i) * (range / NCLOUDS) + Random::Float(range / NCLOUDS), dayTime);
		window->add(cloud);
	}

	int nPatches = (int)(sky->getWidth() / GrassPatch::WIDTH + 1);

	for (int i = 0; i < nPatches * 4; i++) {
		GrassPatch* patch = new GrassPatch((i - 0.75f) * GrassPatch::WIDTH / 4, SKY_HEIGHT + 1, dayTime);
		patch->brightness(dayTime ? 0.7f : 0.4f);
		window->add(patch);
	}

	Avatar* a = new Avatar(Dungeon::hero->heroClass);
	a->x = PixelScene::align((SKY_WIDTH - a->width) / 2);
	a->y = SKY_HEIGHT - a->height;
	window->add(a);

	Pet* pet = new Pet();
	pet->rm = pet->gm = pet->bm = 1.2f;
	pet->x = SKY_WIDTH / 2 + 2;
	pet->y = SKY_HEIGHT - pet->height;
	window->add(pet);

	class TouchAreaNew :public TouchArea{
	private:
		Pet* pet;
	public:
		TouchAreaNew(Pet* p, Visual* target) :pet(p), TouchArea(target){}
	protected:
		virtual void onClick(TouchScreen::Touch* touch){
			pet->Jump();
		}
	};
	window->add(new TouchAreaNew(pet, sky));

	for (int i = 0; i < nPatches; i++) {
		GrassPatch* patch = new GrassPatch((i - 0.5f) * GrassPatch::WIDTH, SKY_HEIGHT, dayTime);
		patch->brightness(dayTime ? 1.0f : 0.8f);
		window->add(patch);
	}

	Image* frame = new Image(Assets::SURFACE);
	frame->frame(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
	frame->x = vx - FRAME_MARGIN_X;
	frame->y = vy - FRAME_MARGIN_TOP;
	add(frame);

	if (dayTime) {
		a->brightness(1.2f);
		pet->brightness(1.2f);
	}
	else {
		frame->hardlight(0xDDEEFF);
	}

	class RedButtonNew :public RedButton{
	public:
		RedButtonNew(const std::string& t) :RedButton(t){}
	protected:
		virtual void onClick(){
			Game::switchScene(new TitleScene);
		}
	};
	RedButton* gameOver = new RedButtonNew("Game Over");
	gameOver->setSize(SKY_WIDTH - FRAME_MARGIN_X * 2, BUTTON_HEIGHT);
	gameOver->setPos(frame->x + FRAME_MARGIN_X * 2, frame->y + frame->height + 4);
	add(gameOver);

	Badges::validateHappyEnd();

	fadeIn();
}

const int SurfaceScene::Sky::_day[] = { 0xFF4488FF, 0xFFCCEEFF };

const int SurfaceScene::Sky::_night[] = { 0xFF001155, 0xFF335980 };
std::vector<int> SurfaceScene::Sky::day = std::vector<int>(_day, _day+sizeof(_day)/sizeof(int));
std::vector<int> SurfaceScene::Sky::night = std::vector<int>(_night, _night + sizeof(_night) / sizeof(int));

SurfaceScene::Sky::Sky(boolean dayTime)
:Visual(0, 0, 1, 1)
{
	texture = new Gradient(dayTime ? day : night);

	float vertices[16];
	verticesBuffer = Quad::create();

	vertices[2] = 0.25f;
	vertices[6] = 0.25f;
	vertices[10] = 0.75f;
	vertices[14] = 0.75f;

	vertices[3] = 0;
	vertices[7] = 1;
	vertices[11] = 1;
	vertices[15] = 0;


	vertices[0] = 0;
	vertices[1] = 0;

	vertices[4] = 1;
	vertices[5] = 0;

	vertices[8] = 1;
	vertices[9] = 1;

	vertices[12] = 0;
	vertices[13] = 1;

	verticesBuffer->position(0);
	verticesBuffer->put(std::vector<float>(vertices, vertices+sizeof(vertices) / sizeof(float)));
}

void SurfaceScene::Sky::draw()
{
	Visual::draw();

	NoosaScript* script = NoosaScript::get();

	texture->bind();

	script->camera(camera());

	script->uModel.valueM4(_mat);
	script->lighting(
		rm, gm, bm, am,
		ra, ga, ba, aa);

	script->drawQuad(verticesBuffer->buffer());
}

int SurfaceScene::Cloud::lastIndex = -1;

SurfaceScene::Cloud::Cloud(float y, boolean dayTime)
:Image(Assets::SURFACE)
{
	int index;
	do {
		index = Random::Int(3);
	} while (index == lastIndex);

	switch (index) {
	case 0:
		frame(88, 0, 49, 20);
		break;
	case 1:
		frame(88, 20, 49, 22);
		break;
	case 2:
		frame(88, 42, 50, 18);
		break;
	}

	lastIndex = index;

	this->y = y;

	GameMath::PointFSet(&scale, 1 - y / SKY_HEIGHT);
//	scale.set(1 - y / SKY_HEIGHT);
	x = Random::Float(SKY_WIDTH + getWidth()) - getWidth();
	speed.x = scale.x * (dayTime ? +8 : -8);

	if (dayTime) {
		tint(0xCCEEFF, 1 - scale.y);
	}
	else {
		rm = gm = bm = +3.0f;
		ra = ga = ba = -2.1f;
	}
}

void SurfaceScene::Cloud::update()
{
	Image::update();
	if (speed.x > 0 && x > SKY_WIDTH) {
		x = -getWidth();
	}
	else if (speed.x < 0 && x < -getWidth()) {
		x = SKY_WIDTH;
	}
}

SurfaceScene::GrassPatch::GrassPatch(float tx, float ty, boolean forward)
:Image(Assets::SURFACE)
{
	a = Random::Float(5);
	
	frame(88 + Random::Int(4) * WIDTH, 60, WIDTH, HEIGHT);

	this->tx = tx;
	this->ty = ty;

	this->forward = forward;
}

void SurfaceScene::GrassPatch::update()
{
	Image::update();
	a += Random::Float(Game::elapsed * 5);
	angle = (2 + std::cos(a)) * (forward ? +0.2 : -0.2);

	scale.y = (float)std::cos(angle);

	x = tx + (float)std::tan(angle) * width;
	y = ty - scale.y * height;
}

void SurfaceScene::GrassPatch::updateMatrix()
{
	Image::updateMatrix();
	GameMath::MATRIXSkewX(_mat, (float)(angle / GameMath::G2RAD));
}

SurfaceScene::Avatar::Avatar(HeroClass cl)
:Image(Assets::AVATARS)
{
	TextureFilm tf(tex, WIDTH, HEIGHT);
	frame(tf.get(cl.ordinal()));
}

SurfaceScene::Pet::Pet()
: MovieClip(Assets::PET)
{
	TextureFilm frames(tex, 16, 16);

	idle = new Animation(2, true);
	int arr1[] = { 0, 0, 0, 0, 0, 0, 1 };
	idle->Frames(&frames, std::vector<int>(arr1, arr1+sizeof(arr1)/sizeof(int)));

	jump = new Animation(10, false);
	int arr2[] = { 2, 3, 4, 5, 6 };
	jump->Frames(&frames, std::vector<int>(arr2, arr2 + sizeof(arr2) / sizeof(int)));

	listener = this;

	play(idle);
}
