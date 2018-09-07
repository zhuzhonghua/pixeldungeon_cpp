#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <string>

typedef glm::vec2 PointF;
typedef glm::ivec2 Point;
typedef glm::mat4 Matrix;
typedef glm::vec4 RectF;
typedef glm::ivec4 Rect;

typedef unsigned char byte;
typedef unsigned short Uint16;

#define INT_MAX_VALUE 0x3FFFFFFF

class Assets{
public:
	static const char* ARCS_BG ;
	static const char* ARCS_FG ;
	static const char* DASHBOARD ;

	static const char* BANNERS ;
	static const char* BADGES ;
	static const char* LOCKED ;
	static const char* AMULET ;

	static const char* CHROME ;
	static const char* ICONS ;
	static const char* STATUS ;
	static const char* HP_BAR ;
	static const char* XP_BAR ;
	static const char* TOOLBAR ;
	static const char* SHADOW ;

	static const char* WARRIOR ;
	static const char* MAGE ;
	static const char* ROGUE ;
	static const char* HUNTRESS ;
	static const char* AVATARS ;
	static const char* PET ;

	static const char* SURFACE ;

	static const char* FIREBALL ;
	static const char* SPECKS ;
	static const char* EFFECTS ;

	static const char* RAT ;
	static const char* GNOLL ;
	static const char* CRAB ;
	static const char* GOO ;
	static const char* SWARM ;
	static const char* SKELETON ;
	static const char* SHAMAN ;
	static const char* THIEF ;
	static const char* TENGU ;
	static const char* SHEEP ;
	static const char* KEEPER ;
	static const char* BAT ;
	static const char* BRUTE ;
	static const char* SPINNER ;
	static const char* DM300 ;
	static const char* WRAITH ;
	static const char* ELEMENTAL ;
	static const char* MONK ;
	static const char* WARLOCK ;
	static const char* GOLEM ;
	static const char* UNDEAD ;
	static const char* KING ;
	static const char* STATUE ;
	static const char* PIRANHA ;
	static const char* EYE ;
	static const char* SUCCUBUS ;
	static const char* SCORPIO ;
	static const char* ROTTING ;
	static const char* BURNING ;
	static const char* YOG ;
	static const char* LARVA ;
	static const char* GHOST ;
	static const char* MAKER ;
	static const char* TROLL ;
	static const char* IMP ;
	static const char* RATKING ;
	static const char* BEE ;
	static const char* MIMIC ;

	static const char* ITEMS ;
	static const char* PLANTS ;

	static const char* TILES_SEWERS ;
	static const char* TILES_PRISON ;
	static const char* TILES_CAVES ;
	static const char* TILES_CITY ;
	static const char* TILES_HALLS ;

	static const char* WATER_SEWERS ;
	static const char* WATER_PRISON ;
	static const char* WATER_CAVES ;
	static const char* WATER_CITY ;
	static const char* WATER_HALLS ;

	static const char* BUFFS_SMALL ;
	static const char* BUFFS_LARGE ;
	static const char* SPELL_ICONS ;

	static const char* FONTS1X ;
	static const char* FONTS15X ;
	static const char* FONTS2X ;
	static const char* FONTS25X ;
	static const char* FONTS3X ;

	static const char* THEME ;
	static const char* TUNE ;
	static const char* HAPPY ;

	static const char* SND_CLICK ;
	static const char* SND_BADGE ;
	static const char* SND_GOLD ;

	static const char* SND_OPEN ;
	static const char* SND_UNLOCK ;
	static const char* SND_ITEM ;
	static const char* SND_DEWDROP ;
	static const char* SND_HIT ;
	static const char* SND_MISS ;
	static const char* SND_STEP ;
	static const char* SND_WATER ;
	static const char* SND_DESCEND ;
	static const char* SND_EAT ;
	static const char* SND_READ ;
	static const char* SND_LULLABY ;
	static const char* SND_DRINK ;
	static const char* SND_SHATTER ;
	static const char* SND_ZAP ;
	static const char* SND_LIGHTNING ;
	static const char* SND_LEVELUP ;
	static const char* SND_DEATH ;
	static const char* SND_CHALLENGE ;
	static const char* SND_CURSED ;
	static const char* SND_TRAP ;
	static const char* SND_EVOKE ;
	static const char* SND_TOMB ;
	static const char* SND_ALERT ;
	static const char* SND_MELD ;
	static const char* SND_BOSS ;
	static const char* SND_BLAST ;
	static const char* SND_PLANT ;
	static const char* SND_RAY ;
	static const char* SND_BEACON ;
	static const char* SND_TELEPORT ;
	static const char* SND_CHARMS ;
	static const char* SND_MASTERY ;
	static const char* SND_PUFF ;
	static const char* SND_ROCKS ;
	static const char* SND_BURNING ;
	static const char* SND_FALLING ;
	static const char* SND_GHOST ;
	static const char* SND_SECRET ;
	static const char* SND_BONES ;
	static const char* SND_BEE ;
	static const char* SND_DEGRADE ;
	static const char* SND_MIMIC ;
};


class Challenges 
{
public:
	static const int NO_FOOD = 1;
	static const int NO_ARMOR = 2;
	static const int NO_HEALING = 4;
	static const int NO_HERBALISM = 8;
	static const int SWARM_INTELLIGENCE = 16;
	static const int DARKNESS = 32;
	static const int NO_SCROLLS = 64;

	static const int NAME_LEN = 7;
	static const char* NAMES[];

	static const int MASKS[];
};
