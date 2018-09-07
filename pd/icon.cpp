#include "icon.h"

Image* Icons::get()
{
	return get(this->type);
}

Image* Icons::get(Type type)
{
	Image* icon = new Image(Assets::ICONS);
	switch (type) 
	{
	case SKULL:
		icon->frame(icon->tex->uvRect(0, 0, 8, 8));
		break;
	case BUSY:
		icon->frame(icon->tex->uvRect(8, 0, 16, 8));
		break;
	case COMPASS:
		icon->frame(icon->tex->uvRect(0, 8, 7, 13));
		break;
	case PREFS:
		icon->frame(icon->tex->uvRect(30, 0, 46, 16));
		break;
	case WARNING:
		icon->frame(icon->tex->uvRect(46, 0, 58, 12));
		break;
	case TARGET:
		icon->frame(icon->tex->uvRect(0, 13, 16, 29));
		break;
	case WATA:
		icon->frame(icon->tex->uvRect(30, 16, 45, 26));
		break;
	case WARRIOR:
		icon->frame(icon->tex->uvRect(0, 29, 16, 45));
		break;
	case MAGE:
		icon->frame(icon->tex->uvRect(16, 29, 32, 45));
		break;
	case ROGUE:
		icon->frame(icon->tex->uvRect(32, 29, 48, 45));
		break;
	case HUNTRESS:
		icon->frame(icon->tex->uvRect(48, 29, 64, 45));
		break;
	case CLOSE:
		icon->frame(icon->tex->uvRect(0, 45, 13, 58));
		break;
	case DEPTH:
		icon->frame(icon->tex->uvRect(45, 12, 54, 20));
		break;
	case SLEEP:
		icon->frame(icon->tex->uvRect(13, 45, 22, 53));
		break;
	case ALERT:
		icon->frame(icon->tex->uvRect(22, 45, 30, 53));
		break;
	case SUPPORT:
		icon->frame(icon->tex->uvRect(30, 45, 46, 61));
		break;
	case SUPPORTED:
		icon->frame(icon->tex->uvRect(46, 45, 62, 61));
		break;
	case BACKPACK:
		icon->frame(icon->tex->uvRect(58, 0, 68, 10));
		break;
	case SCROLL_HOLDER:
		icon->frame(icon->tex->uvRect(68, 0, 78, 10));
		break;
	case SEED_POUCH:
		icon->frame(icon->tex->uvRect(78, 0, 88, 10));
		break;
	case WAND_HOLSTER:
		icon->frame(icon->tex->uvRect(88, 0, 98, 10));
		break;
	case KEYRING:
		icon->frame(icon->tex->uvRect(64, 29, 74, 39));
		break;
	case CHECKED:
		icon->frame(icon->tex->uvRect(54, 12, 66, 24));
		break;
	case UNCHECKED:
		icon->frame(icon->tex->uvRect(66, 12, 78, 24));
		break;
	case EXIT:
		icon->frame(icon->tex->uvRect(98, 0, 114, 16));
		break;
	case CHALLENGE_OFF:
		icon->frame(icon->tex->uvRect(78, 16, 102, 40));
		break;
	case CHALLENGE_ON:
		icon->frame(icon->tex->uvRect(102, 16, 126, 40));
		break;
	case RESUME:
		icon->frame(icon->tex->uvRect(114, 0, 126, 11));
		break;
	}
	return icon;
}

Image* Icons::get(HeroClass& cl)
{
	switch ((HeroClass::Type)cl.ordinal()) 
	{
	case HeroClass::Type::E_WARRIOR:
		return get(WARRIOR);
	case HeroClass::Type::E_MAGE:
		return get(MAGE);
	case HeroClass::Type::E_ROGUE:
		return get(ROGUE);
	case HeroClass::Type::E_HUNTRESS:
		return get(HUNTRESS);
	default:
		return NULL;
	}
}