#include "wndtitledmessage.h"
#include "pixeldungeon.h"
#include "highlightedtext.h"
#include "icontitle.h"
#include "pixelscene.h"
#include "mob.h"
#include "buffindicator.h"
#include "charsprite.h"

WndTitledMessage::WndTitledMessage(Image* icon, const std::string& title, const std::string& message)
:WndTitledMessage(new IconTitle(icon, title), message)
{

}

WndTitledMessage::WndTitledMessage(Component* titlebar, const std::string& message)
:Window()
{
	int width = PixelDungeon::landscape() ? WIDTH_L : WIDTH_P;

	titlebar->setRect(0, 0, width, 0);
	add(titlebar);

	HighlightedText* text = new HighlightedText(6);
	text->text(message, width);
	text->setPos(titlebar->left(), titlebar->bottom() + GAP);
	add(text);

	resize(width, (int)text->bottom());
}

WndInfoMob::MobTitle::MobTitle(Mob* mob)
{
	name = PixelScene::createText(mob->name, 9);
	name->hardlight(TITLE_COLOR);
	name->measure();
	add(name);

	image = mob->Sprite();
	add(image);

	health = new HealthBar();
	health->Level((float)mob->HP / mob->HT);
	add(health);

	buffs = new BuffIndicator(mob);
	add(buffs);
}

void WndInfoMob::MobTitle::layout()
{
	image->x = 0;
	image->y = std::max(0, (int)(name->Height() + GAP + health->height() - image->height));

	name->x = image->width + GAP;
	name->y = image->height - health->height() - GAP - name->baseLine();

	float w = _width - image->width - GAP;

	health->setRect(image->width + GAP, image->height - health->height(), w, health->height());

	buffs->setPos(
		name->x + name->Width() + GAP,
		name->y + name->baseLine() - BuffIndicator::SIZE);

	_height = health->bottom();
}

String WndInfoMob::desc(Mob* mob)
{
	std::stringstream builder;
	builder << mob->description();

	builder << "\n\n" << mob->state->status() << ".";

	return builder.str();
}

WndInfoMob::WndInfoMob(Mob* mob)
:WndTitledMessage(new MobTitle(mob), desc(mob))
{

}
