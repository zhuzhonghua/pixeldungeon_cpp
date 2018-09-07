#include "scrollpane.h"
#include "camera.h"
#include "pixelscene.h"
#include "colorblock.h"

const float ScrollPane::THUMB_ALPHA = 0.5f;

void ScrollPane::createChildren()
{
	controller = new TouchController(this);
	add(controller);

	thumb = new ColorBlock(1, 1, THUMB_COLOR);
	thumb->am = THUMB_ALPHA;
	add(thumb);
}

void ScrollPane::layout()
{
	content->setPos(0, 0);
	controller->x = _x;
	controller->y = _y;
	controller->width = _width;
	controller->height = _height;

	Point p = camera()->cameraToScreen(_x, _y);
	Camera* cs = content->cameraf;
	cs->x = p.x;
	cs->y = p.y;
	cs->resize((int)_width, (int)_height);

	thumb->visible = _height < content->height();
	if (thumb->visible) {
		GameMath::PointFSet(&thumb->scale, 2, _height * _height / content->height());
		//thumb->scale.set(2, _height * _height / content->height());
		thumb->x = right() - thumb->width();
		thumb->y = _y;
	}
}

ScrollPane::ScrollPane(Component* content)
{
	init();

	this->content = content;
	addToBack(content);

	_width = content->width();
	_height = content->height();

	content->cameraf = new Camera(0, 0, 1, 1, PixelScene::defaultZoom);
	Camera::add(content->cameraf);
}

void ScrollPane::destroy()
{
	Component::destroy();
	Camera::remove(content->cameraf);

	delete content->cameraf;
}

void ScrollPane::scrollTo(float x, float y)
{
	GameMath::PointFSet(&content->cameraf->scroll, x, y);
	//content->cameraf->scroll.set(x, y);
}

void ScrollPane::TouchController::onClick(TouchScreen::Touch* touch)
{
	if (dragging) {

		dragging = false;
		sp->thumb->am = THUMB_ALPHA;

	}
	else {

		PointF p = sp->content->cameraf->screenToCamera((int)touch->current.x, (int)touch->current.y);
		sp->onClick(p.x, p.y);

	}
}

void ScrollPane::TouchController::onDrag(TouchScreen::Touch* t)
{
	if (dragging) {

		Camera* c = sp->content->cameraf;

		PointF tmp = GameMath::PointFDiff(lastPos, t->current);
		GameMath::PointFInvScale(&tmp, c->zoomf);
		GameMath::PointFOffSet(&c->scroll, &tmp);

		//c->scroll.offset(PointF.diff(lastPos, t.current).invScale(c->zoom));
		if (c->scroll.x + width > sp->content->width()) {
			c->scroll.x = sp->content->width() - width;
		}
		if (c->scroll.x < 0) {
			c->scroll.x = 0;
		}
		if (c->scroll.y + height > sp->content->height()) {
			c->scroll.y = sp->content->height() - height;
		}
		if (c->scroll.y < 0) {
			c->scroll.y = 0;
		}

		sp->thumb->y = y + height * c->scroll.y / sp->content->height();

		GameMath::PointFSet(&lastPos, t->current);
		//lastPos.set(t->current);

	}
	else if (GameMath::PointFDistance(t->current, t->start) > dragThreshold) {

		dragging = true;
		GameMath::PointFSet(&lastPos, t->current);
		//lastPos.set(t.current);
		sp->thumb->am = 1;

	}
}

ScrollPane::TouchController::TouchController(ScrollPane* s)
:TouchArea(0, 0, 0, 0),
sp(s)
{
	dragging = false;

	dragThreshold = PixelScene::defaultZoom * 8;
}
