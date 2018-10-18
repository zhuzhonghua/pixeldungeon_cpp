#include "weapon.h"
#include "util.h"
#include "speck.h"
#include "charsprite.h"
#include "level.h"
#include "dungeon.h"
#include "simpleresource.h"
#include "belongings.h"
#include "gamescene.h"
#include "scroll.h"
#include "glog.h"
#include "heap.h"
#include "burning.h"
#include "flameparticle.h"
#include "buffpoison.h"
#include "lightning.h"
#include "lightningtrap.h"
#include "bag.h"
#include "wndoptions.h"
#include "missilesprite.h"
#include "blob.h"
#include "char.h"
#include "hero.h"

const String Weapon::TXT_IDENTIFY = BPT::getText("lang.weapon_identity");
//"You are now familiar enough with your %s to identify it. It is %s.";
const String Weapon::TXT_INCOMPATIBLE = BPT::getText("lang.weapon_incompatible");
//"Interaction of different types of magic has negated the enchantment on this weapon!";

 const String Weapon::TXT_TO_STRING = "%s :%d";
 const String Weapon::TXT_BROKEN = "broken %s :%d";

 const String Weapon::UNFAMILIRIARITY = "unfamiliarity";
 const String Weapon::ENCHANTMENT = "enchantment";
 const String Weapon::IMBUE = "imbue";

 Weapon::Weapon()
 {
	 hitsToKnow = HITS_TO_KNOW;

	 STR = 10;
	 ACU = 1;
	 DLY = 1.0f;

	 imbue = Imbue::NONE;

	 enchantment = NULL;
 }

void Weapon::proc(Char* attacker, Char* defender, int damage)
{
	if (enchantment != NULL) {
		enchantment->proc(this, attacker, defender, damage);
	}
	
	if (!levelKnown) {
		if (--hitsToKnow <= 0) {
			levelKnown = true;
			GLog::i(TXT_IDENTIFY.c_str(), Name(), toString());
			Badges::validateItemLevelAquired(this);
		}
	}

	use();
}

 void Weapon::storeInBundle(Bundle* bundle)
 {
	 KindOfWeapon::storeInBundle(bundle);
	 bundle->put(UNFAMILIRIARITY, hitsToKnow);
	 bundle->put(ENCHANTMENT, enchantment);
	 bundle->put(IMBUE, imbue);
 }

 void Weapon::restoreFromBundle(Bundle* bundle)
 {
	 KindOfWeapon::restoreFromBundle(bundle);
	 if ((hitsToKnow = bundle->getInt(UNFAMILIRIARITY)) == 0) {
		 hitsToKnow = HITS_TO_KNOW;
	 }
	 enchantment = (Enchantment*)bundle->get(ENCHANTMENT);
	 //imbue = bundle.getEnum(IMBUE, Imbue.class);
 }

float Weapon::acuracyFactor(Hero* hero)
{
	int encumbrance = STR - hero->sTR();
	
	if (dynamic_cast<MissileWeapon*>(this)) {
		 switch (hero->heroClass.type()) {
		 case HeroClass::E_WARRIOR:
			 encumbrance += 3;
			 break;
		 case HeroClass::E_HUNTRESS:
			 encumbrance -= 2;
			 break;
		 default:
			 break;
		 }
	}
	
	return
		 (encumbrance > 0 ? (float)(ACU / std::pow(1.5, encumbrance)) : ACU) *
		 (imbue == Imbue::ACCURACY ? 1.5f : 1.0f);
	return 0;
}

float Weapon::speedFactor(Hero* hero)
{
	int encumrance = STR - hero->sTR();
	if (dynamic_cast<MissileWeapon*>(this) && hero->heroClass.type() == HeroClass::E_HUNTRESS) {
		 encumrance -= 2;
	}
	
	return
		 (encumrance > 0 ? (float)(DLY * std::pow(1.2, encumrance)) : DLY) *
		 (imbue == Imbue::SPEED ? 0.6f : 1.0f);
}

int Weapon::damageRoll(Hero* hero)
{
	int damage = KindOfWeapon::damageRoll(hero);

	if ((hero->rangedWeapon != NULL) == (hero->heroClass.type() == HeroClass::E_HUNTRESS)) {
		int exStr = hero->sTR() - STR;
		if (exStr > 0) {
			damage += Random::IntRange(0, exStr);
		}
	}

	return damage;
}

Item* Weapon::upgrade(boolean enchant)
{
	if (enchantment != NULL) {
		if (!enchant && Random::Int(Level()) > 0) {
			GLog::w(TXT_INCOMPATIBLE.c_str());
			this->enchant(NULL);
		}
	}
	else {
		if (enchant) {
			this->enchant();
		}
	}

	return KindOfWeapon::upgrade();
}

 String Weapon::toString()
 {
	 return levelKnown ? 
		 GameMath::format(isBroken() ? 
						TXT_BROKEN.c_str() 
						: TXT_TO_STRING.c_str(), KindOfWeapon::toString().c_str(), STR) 
		 : KindOfWeapon::toString();
 }

 String Weapon::Name()
 {
	 return enchantment == NULL ? KindOfWeapon::Name() : enchantment->name(KindOfWeapon::Name());
 }

 Item* Weapon::random()
 {
	 if (Random::Float() < 0.4) {
		 int n = 1;
		 if (Random::Int(3) == 0) {
			 n++;
			 if (Random::Int(3) == 0) {
				 n++;
			 }
		 }
		 if (Random::Int(2) == 0) {
			 upgrade(n);
		 }
		 else {
			 degrade(n);
			 cursed = true;
		 }
	 }
	 return this;
 }

 Weapon* Weapon::enchant(Enchantment* ench)
 {
	 enchantment = ench;
	 return this;
 }

Weapon* Weapon::enchant()
{	 
	std::string oldEnchantment;
	if (enchantment != NULL)
	{
		oldEnchantment = enchantment->getClassName();
	}
	 
	Enchantment* ench = Enchantment::random();
	while (ench->getClassName() == oldEnchantment) {
		ench = Enchantment::random();
	}

	return enchant(ench);
}

 ItemSprite::Glowing* Weapon::glowing()
 {
	 return enchantment != NULL ? enchantment->glowing() : NULL;
 }

 std::string Weapon::Enchantment::_enchants[] ={
	 "EnchantmentFire", "EnchantmentPoison", "Death", "EnchantmentParalysis", "Leech",
	 "EnchantmentSlow", "Shock", "Instability", "Horror", "Luck",
	 "Tempering" };

 std::vector<std::string> Weapon::Enchantment::enchants(_enchants, _enchants+sizeof(_enchants)/sizeof(std::string));
 float Weapon::Enchantment::_chances[] = { 10, 10, 1, 2, 1, 2, 6, 3, 2, 2, 3 };

 std::vector<float> Weapon::Enchantment::chances(_chances, _chances+sizeof(_chances)/sizeof(float));

 std::map<std::string, Weapon::Enchantment::CreateEnchant> Weapon::Enchantment::clsToInst;

 ItemSprite::Glowing* Weapon::Enchantment::glowing()
 {
	 return ItemSprite::Glowing::WHITE;
 }

Weapon::Enchantment* Weapon::Enchantment::random()
{
	 int idx = Random::chances(chances);
	 std::string clsName = enchants[idx];
	 
	 if (clsToInst.size() <= 0) init();

	 return clsToInst[clsName]();
}

void Weapon::Enchantment::init()
{
	 clsToInst.insert(std::make_pair("EnchantmentFire", EnchantmentFire::Create));
	 clsToInst.insert(std::make_pair("EnchantmentPoison", EnchantmentPoison::Create));
	 clsToInst.insert(std::make_pair("Death",Death::Create));
	 clsToInst.insert(std::make_pair("EnchantmentParalysis",EnchantmentParalysis::Create));
	 clsToInst.insert(std::make_pair("Leech",Leech::Create));
	 clsToInst.insert(std::make_pair("EnchantmentSlow",EnchantmentSlow::Create));
	 clsToInst.insert(std::make_pair("Shock",Shock::Create));
	 clsToInst.insert(std::make_pair("Instability",Instability::Create));
	 clsToInst.insert(std::make_pair("Horror",Horror::Create));
	 clsToInst.insert(std::make_pair("Luck",Luck::Create));
	 clsToInst.insert(std::make_pair("Tempering",Tempering::Create));
}

const String EnchantmentFire::TXT_BLAZING = "blazing %s";
ItemSprite::Glowing* EnchantmentFire::ORANGE = new ItemSprite::Glowing(0xFF4400);

boolean EnchantmentFire::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
{
	 // lvl 0 - 33%
	 // lvl 1 - 50%
	 // lvl 2 - 60%
	int level = std::max(0, weapon->effectiveLevel());
	
	if (Random::Int(level + 3) >= 2) {	
		if (Random::Int(2) == 0) {
			((Burning*)Buff::affect(defender, "Burning"))->reignite(defender);
		}
		defender->damage(Random::Int(1, level + 2), this->getClassName());
	
		defender->sprite->emitter()->burst(FlameParticle::FACTORY, level + 1);
	
		return true;	
	}
	else {	
		return false;
	}
}

 String EnchantmentFire::name(const String& weaponName)
 {
	 return GameMath::format(TXT_BLAZING.c_str(), weaponName);
 }

 const String EnchantmentPoison::TXT_VENOMOUS = "venomous %s";
 ItemSprite::Glowing* EnchantmentPoison::PURPLE = new ItemSprite::Glowing(0x4400AA);

boolean EnchantmentPoison::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
{
	 // lvl 0 - 33%
	 // lvl 1 - 50%
	 // lvl 2 - 60%
	int level = std::max(0, weapon->effectiveLevel());
	
	if (Random::Int(level + 3) >= 2) {		
		BuffPoison* buf = (BuffPoison*)Buff::affect(defender, "BuffPoison");
		buf->set(BuffPoison::durationFactor(defender) * (level + 1));
	
		return true;
	}
	else {
		return false;
	}
}

 String EnchantmentPoison::name(const String& weaponName)
 {
	 return GameMath::format(TXT_VENOMOUS.c_str(), weaponName);
 }

const String Death::TXT_GRIM = "grim %s";
ItemSprite::Glowing* Death::BLACK = new ItemSprite::Glowing(0x000000);

boolean Death::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
{
	 // lvl 0 - 8%
	 // lvl 1 ~ 9%
	 // lvl 2 ~ 10%
	int level = std::max(0, weapon->effectiveLevel());

	if (Random::Int(level + 100) >= 92) {
		defender->damage(defender->HP, this->getClassName());
		defender->sprite->emitter()->burst(ShadowParticle::up(), 5);

		if (!defender->isAlive() && dynamic_cast<Hero*>(attacker)) {
			Badges::validateGrimWeapon();
		}

		return true;

	}
	else {
		return false;
	}
}

 String Death::name(const String& weaponName)
 {
	 return GameMath::format(TXT_GRIM.c_str(), weaponName);
 }

 const String EnchantmentParalysis::TXT_STUNNING = "stunning %s";
 ItemSprite::Glowing* EnchantmentParalysis::YELLOW = new ItemSprite::Glowing(0xCCAA44);

 boolean EnchantmentParalysis::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 // lvl 0 - 13%
	 // lvl 1 - 22%
	 // lvl 2 - 30%
	 int level = std::max(0, weapon->effectiveLevel());

	 if (Random::Int(level + 8) >= 7) {

		 Buff::prolong(defender, "BuffParalysis", Random::Float(1, 1.5f + level));

		 return true;
	 }
	 else {
		 return false;
	 }
 }

 String EnchantmentParalysis::name(const String& weaponName)
 {
	 return GameMath::format(TXT_STUNNING.c_str(), weaponName);
 }

 const String Leech::TXT_VAMPIRIC = "vampiric %s";
 ItemSprite::Glowing* Leech::RED = new ItemSprite::Glowing(0x660022);

 boolean Leech::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 int level = std::max(0, weapon->effectiveLevel());

	 // lvl 0 - 33%
	 // lvl 1 - 43%
	 // lvl 2 - 50%
	 int maxValue = damage * (level + 2) / (level + 6);
	 int effValue = std::min(Random::IntRange(0, maxValue), attacker->HT - attacker->HP);

	 if (effValue > 0) {

		 attacker->HP += effValue;
		 attacker->sprite->emitter()->start(Speck::factory(Speck::HEALING), 0.4f, 1);
		 attacker->sprite->showStatus(CharSprite::POSITIVE, GameMath::format("%d", effValue));

		 return true;

	 }
	 else {
		 return false;
	 }
 }

 String Leech::name(const String& weaponName)
 {
	 return GameMath::format(TXT_VAMPIRIC.c_str(), weaponName);
 }

 const String EnchantmentSlow::TXT_CHILLING = "chilling %s";
 ItemSprite::Glowing* EnchantmentSlow::BLUE = new ItemSprite::Glowing(0x0044FF);

 boolean EnchantmentSlow::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 // lvl 0 - 25%
	 // lvl 1 - 40%
	 // lvl 2 - 50%
	 int level = std::max(0, weapon->effectiveLevel());

	 if (Random::Int(level + 4) >= 3) {

		 Buff::prolong(defender, "BuffSlow", Random::Float(1, 1.5f + level));

		 return true;
	 }
	 else {
		 return false;
	 }
 }

 String EnchantmentSlow::name(const String& weaponName)
 {
	 return GameMath::format(TXT_CHILLING.c_str(), weaponName);
 }

 const String Shock::TXT_SHOCKING = "shocking %s";

 boolean Shock::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 // lvl 0 - 25%
	 // lvl 1 - 40%
	 // lvl 2 - 50%
	 int level = std::max(0, weapon->effectiveLevel());

	 if (Random::Int(level + 4) >= 3) {

		 points[0] = attacker->pos;
		 nPoints = 1;

		 affected.clear();
		 affected.push_back(attacker);

		 hit(defender, Random::Int(1, damage / 2));

		 attacker->sprite->parent->add(new Lightning(std::vector<int>(points, points+sizeof(points)/sizeof(int)), nPoints, NULL));

		 return true;

	 }
	 else {

		 return false;

	 }
 }

 String Shock::name(const String& weaponName)
 {
	 return GameMath::format(TXT_SHOCKING.c_str(), weaponName);
 }

 void Shock::hit(Char* ch, int damage)
 {
	 if (damage < 1) {
		 return;
	 }

	 affected.push_back(ch);
	 ch->damage(Level::water[ch->pos] && !ch->flying ? (int)(damage * 2) : damage, "LIGHTNING");

	 ch->sprite->centerEmitter()->burst(SparkParticle::factory(), 3);
	 ch->sprite->flash();

	 points[nPoints++] = ch->pos;

	 std::set<Char*> ns;
	 for (int i = 0; i < 8; i++) {
		 Char* n = Actor::findChar(ch->pos + Level::NEIGHBOURS8[i]);
		 if (n != NULL && !affected.contains(n)) {
			 ns.insert(n);
		 }
	 }

	 if (ns.size() > 0) {
		 hit(RandomT<Char*>::element(ns), Random::Int(damage / 2, damage));
	 }
 }

 const String Instability::TXT_UNSTABLE = "unstable %s";

 boolean Instability::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 return random()->proc(weapon, attacker, defender, damage);
 }

 String Instability::name(const String& weaponName)
 {
	 return GameMath::format(TXT_UNSTABLE.c_str(), weaponName);
 }


 const String Horror::TXT_ELDRITCH = "eldritch %s";
 ItemSprite::Glowing* Horror::GREY = new ItemSprite::Glowing(0x222222);

 boolean Horror::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 // lvl 0 - 20%
	 // lvl 1 - 33%
	 // lvl 2 - 43%
	 int level = std::max(0, weapon->effectiveLevel());

	 if (Random::Int(level + 5) >= 4) {

		 if (defender == Dungeon::hero) {
			 Buff::affect(defender, "Vertigo", Vertigo::duration(defender));
		 }
		 else {
			 ((Terror*)Buff::affect(defender, "Terror", Terror::DURATION))->object = attacker->Id();
		 }

		 return true;
	 }
	 else {
		 return false;
	 }
 }

 String Horror::name(const String& weaponName)
 {
	 return GameMath::format(TXT_ELDRITCH.c_str(), weaponName);
 }

 const String Luck::TXT_LUCKY = "lucky %s";
 ItemSprite::Glowing* Luck::GREEN = new ItemSprite::Glowing(0x00FF00);

 boolean Luck::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 int level = std::max(0, weapon->effectiveLevel());

	 int dmg = damage;
	 for (int i = 1; i <= level + 1; i++) {
		 dmg = std::max(dmg, attacker->damageRoll() - i);
	 }

	 if (dmg > damage) {
		 defender->damage(dmg - damage, this->getClassName());
		 return true;
	 }
	 else {
		 return false;
	 }
 }

 String Luck::name(const String& weaponName)
 {
	 return GameMath::format(TXT_LUCKY.c_str(), weaponName);
 }

 const String Tempering::TXT_TEMPERED = "tempered %s";
 ItemSprite::Glowing* Tempering::GRAY = new ItemSprite::Glowing(0xCC8888);

 boolean Tempering::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 weapon->polish();
	 return true;
 }

 String Tempering::name(const String& weaponName)
 {
	 return GameMath::format(TXT_TEMPERED.c_str(), weaponName);
 }

 MeleeWeapon::MeleeWeapon(int tier, float acu, float dly)
 {
	 this->tier = tier;

	 ACU = acu;
	 DLY = dly;

	 STR = typicalSTR();
 }

String MeleeWeapon::info()
{
	String p = "\n\n";
	
	std::stringstream info;
	info << desc();
	
	int lvl = visiblyUpgraded();
	String quality = lvl != 0 ?
							 (lvl > 0 ?
							 (isBroken() ? BPT::getText("lang.broken") : BPT::getText("lang.upgraded")) :
							 BPT::getText("lang.degraded")) :
							 "";
	info<< p;
	info << GameMath::format(BPT::getText("lang.this_is_tier_melee_weapon").c_str(), name.c_str(), quality, tier);
	
	if (levelKnown) {
		int min = this->min();
		int max = this->max();
		info << GameMath::format(BPT::getText("lang.its_average_damage").c_str(), (min + (max - min) / 2));		
	}
	else {
		int min = min0();
		int max = max0();

		info << GameMath::format(BPT::getText("lang.its_typical_damage").c_str(), (min + (max - min) / 2), typicalSTR());

		if (typicalSTR() > Dungeon::hero->sTR()) {
			info << BPT::getText("lang.too_heavy_for_you");
		}
	}
	
	if (DLY != 1.0f) {
		std::stringstream ss2;
		ss2 << (DLY < 1.0f ? BPT::getText("lang.fast") : BPT::getText("lang.slow"));
		
		if (ACU != 1.0f) {
			if ((ACU > 1.0f) == (DLY < 1.0f)) {
				ss2 << BPT::getText("lang.and");
			}
			else {
				ss2 << BPT::getText("lang.but");
			}
			ss2 << (ACU > 1.0f ? BPT::getText("lang.accurate") : BPT::getText("lang.inaccurate"));
		}
		info << GameMath::format(BPT::getText("lang.this_is_a_rather_weapon").c_str(), ss2.str().c_str());
	}
	else if (ACU != 1.0f) {
		info << GameMath::format(BPT::getText("lang.this_is_a_rather_weapon").c_str(), (ACU > 1.0f ? BPT::getText("lang.accurate") : BPT::getText("lang.inaccurate")));
	}

	switch (imbue) {
	case SPEED:
		info << BPT::getText("lang.balanced_faster");
		break;
	case ACCURACY:
		info << BPT::getText("lang.balanced_accurate");
		break;
	case NONE:
		break;
	}
	
	if (enchantment != NULL) {
		info << BPT::getText("lang.its_enchanted");
	}
	
	if (levelKnown && Dungeon::hero->belongings->backpack->items.contains(this)) {
		if (STR > Dungeon::hero->sTR()) {
			info << p;
			info << GameMath::format(BPT::getText("lang.inadequate_strength_accuracy_speed").c_str(), name.c_str());
		}
		if (STR < Dungeon::hero->sTR()) {
			info << p;
			info << GameMath::format(BPT::getText("lang.excess_strength_damage_increased").c_str(), name.c_str());
		}
	}
	
	if (isEquipped(Dungeon::hero)) {
		info << p;
		info << GameMath::format(BPT::getText("lang.hold_ready").c_str(), name.c_str());
		if (cursed)
			info << BPT::getText("lang.cursed_not_let_go").c_str();
		else
			info << ".";
	}
	else {
		if (cursedKnown && cursed) {
			info << p;
			info << GameMath::format(BPT::getText("lang.feel_malevolent_lurking").c_str(), name.c_str());
		}
	}
	
	return info.str();
}

 int MeleeWeapon::price()
 {
	 int price = 20 * (1 << (tier - 1));
	 if (enchantment != NULL) {
		 price *= 1.5;
	 }
	 return considerState(price);
 }

 Item* MeleeWeapon::random()
 {
	 Weapon::random();

	 if (Random::Int(10 + Level()) == 0) {
		 enchant();
	 }

	 return this;
 }

Dagger::Dagger()
:MeleeWeapon(1, 1.2f, 1.0f)
{
	name = BPT::getText("lang.dagger");
	image = ItemSpriteSheet::DAGGER;
}

Knuckles::Knuckles()
:MeleeWeapon(1, 1.0f, 0.5f)
{
	name = BPT::getText("lang.knuckleduster");
	image = ItemSpriteSheet::KNUCKLEDUSTER;
}

Quarterstaff::Quarterstaff()
:MeleeWeapon(2, 1.0f, 1.0f)
{
	name = BPT::getText("lang.quarterstaff");
	image = ItemSpriteSheet::QUARTERSTAFF;
}

Spear::Spear()
:MeleeWeapon(2, 1.0f, 1.5f)
{
	name = BPT::getText("lang.spear");
	image = ItemSpriteSheet::SPEAR;
}

Mace::Mace()
:MeleeWeapon(3, 1.0f, 0.8f)
{
	name = BPT::getText("lang.mace");
	image = ItemSpriteSheet::MACE;
}

Sword::Sword()
:MeleeWeapon(3, 1.0f, 1.0f)
{
	name = BPT::getText("lang.sword");
	image = ItemSpriteSheet::SWORD;
}

Longsword::Longsword()
:MeleeWeapon(4, 1.0f, 1.0f)
{
	name = BPT::getText("lang.longsword");
	image = ItemSpriteSheet::LONG_SWORD;
}

BattleAxe::BattleAxe()
:MeleeWeapon(4, 1.2f, 1.0f)
{
	name = BPT::getText("lang.battle_axe");
	image = ItemSpriteSheet::BATTLE_AXE;
}

WarHammer::WarHammer()
:MeleeWeapon(5, 1.2f, 1.0f)
{
	name = BPT::getText("lang.war_hammer");
	image = ItemSpriteSheet::WAR_HAMMER;
}

Glaive::Glaive()
:MeleeWeapon(5, 1.0f, 1.0f)
{
	name = BPT::getText("lang.glaive");
	image = ItemSpriteSheet::GLAIVE;
}

namespace{
	class WndBagListenerNew :public WndBag::Listener{
	public:
		ShortSword* ss;
		WndBagListenerNew(ShortSword* ssw) :ss(ssw){}

		virtual void onSelect(Item* item){
			if (item != NULL && !(dynamic_cast<Boomerang*>(item))) {

				//Sample.INSTANCE.play(Assets.SND_EVOKE);
				ScrollOfUpgrade::upgrade(Item::curUser);
				Item::evoke(Item::curUser);

				GLog::w(ShortSword::TXT_REFORGED.c_str(), item->Name());

				((MeleeWeapon*)item)->safeUpgrade();
				Item::curUser->spendAndNext(ShortSword::TIME_TO_REFORGE);

				//Badges::validateItemLevelAquired(item);

			}
			else {

				if (dynamic_cast<Boomerang*>(item)) {
					GLog::w(ShortSword::TXT_NOT_BOOMERANG.c_str());
				}

				if (ss->equipped) {
					Item::curUser->belongings->weapon = ss;
				}
				else {
					//ss->collect(Item::curUser->belongings->backpack);
				}
			}
		}
	};
}

const String ShortSword::AC_REFORGE = BPT::getText("lang.REFORGE");
const String ShortSword::TXT_SELECT_WEAPON = BPT::getText("lang.select_weapon_upgrade");
const String ShortSword::TXT_REFORGED = BPT::getText("lang.reforged_short_sword_upgrade");
const String ShortSword::TXT_NOT_BOOMERANG = BPT::getText("lang.not_upgrade_boomerang");

const float ShortSword::TIME_TO_REFORGE = 2.0f;

ShortSword::ShortSword()
:MeleeWeapon(1, 1.0f, 1.0f)
{
	name = BPT::getText("lang.short_sword");
	image = ItemSpriteSheet::SHORT_SWORD;

	STR = 11;

	itemSelector = new WndBagListenerNew(this);
}

void ShortSword::actions(Hero* hero, std::vector<std::string>& actions)
{
	MeleeWeapon::actions(hero, actions);
	if (Level() > 0) {
		actions.push_back(AC_REFORGE);
	}
}

void ShortSword::execute(Hero* hero, std::string action)
{
	if (action == AC_REFORGE) {

		if (hero->belongings->weapon == this) {
			equipped = true;
			hero->belongings->weapon = NULL;
		}
		else {
			equipped = false;
			detach(hero->belongings->backpack);
		}

		curUser = hero;

		GameScene::selectItem(itemSelector, WndBag::Mode::WEAPON, TXT_SELECT_WEAPON);

	}
	else {

		MeleeWeapon::execute(hero, action);

	}
}

const String MissileWeapon::TXT_MISSILES = BPT::getText("lang.Missile_weapon");
const String MissileWeapon::TXT_YES = BPT::getText("lang.i_know_im_doing");
const String MissileWeapon::TXT_NO = BPT::getText("lang.i_changed_my_mind");
const String MissileWeapon::TXT_R_U_SURE = BPT::getText("lang.you_really_equip_as_melee_weapon");

MissileWeapon::MissileWeapon()
{
	stackable = true;
	levelKnown = true;
	defaultAction = AC_THROW;
}

void MissileWeapon::actions(Hero* hero, std::vector<std::string>& actions)
{
	Weapon::actions(hero, actions);
	if (hero->heroClass.type() != HeroClass::E_HUNTRESS && hero->heroClass.type() != HeroClass::E_ROGUE) {
		for (std::vector<std::string>::iterator itr = actions.begin();
			itr != actions.end(); )
		{
			if ((*itr).compare(AC_EQUIP) == 0)
			{
				itr = actions.erase(itr);
			}
			else if ((*itr).compare(AC_UNEQUIP) == 0)
			{
				itr = actions.erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}
}

void MissileWeapon::proc(Char* attacker, Char* defender, int damage)
{
	Weapon::proc(attacker, defender, damage);

	Hero* hero = (Hero*)attacker;
	if (hero->rangedWeapon == NULL && stackable) {
		if (quantity == 1) {
			doUnequip(hero, false, false);
		}
		else {
			detach(NULL);
		}
	}
}

namespace{
	class WndOptionsNew :public WndOptions{
	public:
		MissileWeapon* mw;
		Hero* hero;
		WndOptionsNew(MissileWeapon* m, Hero* h, const std::string& title, const std::string& message, const std::vector<std::string>& options)
			:mw(m), hero(h), WndOptions(title, message, options)
		{}
	protected:
		virtual void onSelect(int index)
		{
			if (index == 0) {
				mw->Weapon::doEquip(hero);
			}
		}
	};
}
bool MissileWeapon::doEquip(Hero* hero)
{
	std::vector<std::string> op;
	op.push_back(TXT_YES);
	op.push_back(TXT_NO);
	GameScene::show(new WndOptionsNew(this, hero, TXT_MISSILES, TXT_R_U_SURE, op));

	return false;
}

String MissileWeapon::info()
{
	std::stringstream info;
	info << desc();
	
	int min = this->min();
	int max = this->max();

	info << GameMath::format(BPT::getText("lang.average_damage_weapon_hit").c_str(), (min + (max - min) / 2));
	
	if (Dungeon::hero->belongings->backpack->items.contains(this)) {
		if (STR > Dungeon::hero->sTR()) {
			info << GameMath::format(BPT::getText("lang.inadequate_strength_accuracy_speed").c_str(), name.c_str());
		}
		if (STR < Dungeon::hero->sTR()) {
			info << GameMath::format(BPT::getText("lang.excess_strength_damage_increased").c_str(), name.c_str());
		}
	}
	
	if (isEquipped(Dungeon::hero)) {
		info << GameMath::format(BPT::getText("lang.hold_ready").c_str(), name.c_str());
		info << ".";
	}
	
	return info.str();
}

void MissileWeapon::onThrow(int cell)
{
	Char* enemy = Actor::findChar(cell);
	if (enemy == NULL || enemy == curUser) {
		Weapon::onThrow(cell);
	}
	else {
		if (!curUser->shoot(enemy, this)) {
			miss(cell);
		}
	}
}

Boomerang::Boomerang()
{
	name = BPT::getText("lang.boomerang");
	image = ItemSpriteSheet::BOOMERANG;

	STR = 10;

	stackable = false;
}

void Boomerang::proc(Char* attacker, Char* defender, int damage)
{
	MissileWeapon::proc(attacker, defender, damage);
	if (dynamic_cast<Hero*>(attacker) && ((Hero*)attacker)->rangedWeapon == this) {
		circleBack(defender->pos, (Hero*)attacker);
	}
}

void Boomerang::cast(Hero* user, int dst)
{
	throwEquiped = isEquipped(user);
	MissileWeapon::cast(user, dst);
}

void Boomerang::circleBack(int from, Hero* owner)
{
	MissileSprite* ms = (MissileSprite*)curUser->sprite->parent->recycle("MissileSprite");
	if (ms == NULL)
	{
		ms = new MissileSprite();
		curUser->sprite->parent->add(ms);
	}
	ms->reset(from, curUser->pos, curItem, NULL);

	if (throwEquiped) {
		owner->belongings->weapon = this;
		owner->spend(-TIME_TO_EQUIP);
	}
	else
	if (!collect(curUser->belongings->backpack)) {
		Dungeon::level->drop(this, owner->pos)->sprite->drop();
	}
}

Dart::Dart()
:Dart(1)
{

}

Dart::Dart(int number)
{
	quantity = number;
	name = BPT::getText("lang.dart");
	image = ItemSpriteSheet::DART;
}

Item* Dart::random()
{
	quantity = Random::Int(5, 15);
	return this;
}

Javelin::Javelin()
:Javelin(1)
{

}

Javelin::Javelin(int number)
{
	quantity = number;
}

void Javelin::proc(Char* attacker, Char* defender, int damage)
{
	MissileWeapon::proc(attacker, defender, damage);
	Buff::prolong(defender, "Cripple", Cripple::DURATION);
}

Item* Javelin::random()
{
	quantity = Random::Int(5, 15);
	return this;
}

IncendiaryDart::IncendiaryDart()
:IncendiaryDart(1)
{

}

IncendiaryDart::IncendiaryDart(int number)
{
	quantity = number;
}

void IncendiaryDart::proc(Char* attacker, Char* defender, int damage)
{
	((Burning*)Buff::affect(defender, "Burning"))->reignite(defender);
	MissileWeapon::proc(attacker, defender, damage);
}

Item* IncendiaryDart::random()
{
	quantity = Random::Int(3, 6);
	return this;
}

void IncendiaryDart::onThrow(int cell)
{
	Char* enemy = Actor::findChar(cell);
	if (enemy == NULL || enemy == Item::curUser) {
		if (Level::flamable[cell]) {
			GameScene::addBlob(Blob::seed(cell, 4, "BlobFire"));
		}
		else {
			MissileWeapon::onThrow(cell);
		}
	}
	else {
		if (!Item::curUser->shoot(enemy, this)) {
			Dungeon::level->drop(this, cell)->sprite->drop();
		}
	}
}

const float CurareDart::DURATION = 3.0f;

CurareDart::CurareDart()
:CurareDart(1)
{

}

CurareDart::CurareDart(int number)
{
	quantity = number;
}

void CurareDart::proc(Char* attacker, Char* defender, int damage)
{
	Buff::prolong(defender, "BuffParalysis", DURATION);
	MissileWeapon::proc(attacker, defender, damage);
}

Item* CurareDart::random()
{
	quantity = Random::Int(2, 5);
	return this;
}

Shuriken::Shuriken()
:Shuriken(1)
{
	
}

Shuriken::Shuriken(int number)
{
	quantity = number;

	name = BPT::getText("lang.shuriken");
	image = ItemSpriteSheet::SHURIKEN;

	STR = 13;

	DLY = 0.5f;
}

Item* Shuriken::random()
{
	quantity = Random::Int(5, 15);
	return this;
}

Tamahawk::Tamahawk()
:Tamahawk(1)
{

}

Tamahawk::Tamahawk(int number)
{
	quantity = number;

	name = BPT::getText("lang.tomahawk");
	image = ItemSpriteSheet::TOMAHAWK;

	STR = 17;
}

void Tamahawk::proc(Char* attacker, Char* defender, int damage)
{
	MissileWeapon::proc(attacker, defender, damage);
	((Bleeding*)Buff::affect(defender, "Bleeding"))->set(damage);
}

Item* Tamahawk::random()
{
	quantity = Random::Int(5, 12);
	return this;
}
