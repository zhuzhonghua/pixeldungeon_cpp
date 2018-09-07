#include "level.h"
#include "util.h"
#include "dungeon.h"
#include "terrain.h"
#include "mob.h"
#include "painter.h"
#include "glog.h"
#include "gamescene.h"
#include "hero.h"

std::vector<bool> Level::fieldOfView(LENGTH);

std::vector<bool> Level::passable(LENGTH);
std::vector<bool> Level::losBlocking(LENGTH);
std::vector<bool> Level::flamable(LENGTH);
std::vector<bool> Level::secret(LENGTH);
std::vector<bool> Level::solid(LENGTH);
std::vector<bool> Level::avoid(LENGTH);
std::vector<bool> Level::water(LENGTH);
std::vector<bool> Level::pit(LENGTH);

std::vector<bool> Level::discoverable(LENGTH);

const int Level::NEIGHBOURS4[] = { -Level::WIDTH, +1, +Level::WIDTH, -1 };
const int Level::NEIGHBOURS8[] = { +1, -1, +Level::WIDTH, -Level::WIDTH, +1 + Level::WIDTH, +1 - Level::WIDTH, -1 + Level::WIDTH, -1 - Level::WIDTH };
const int Level::NEIGHBOURS9[] = { 0, +1, -1, +Level::WIDTH, -Level::WIDTH, +1 + Level::WIDTH, +1 - Level::WIDTH, -1 + Level::WIDTH, -1 - Level::WIDTH };

const float Level::TIME_TO_RESPAWN = 50;
const std::string Level::TXT_HIDDEN_PLATE_CLICKS = "A hidden pressure plate clicks!";

bool Level::resizingNeeded;
int Level::loadedMapSize;

bool Level::pitRoomNeeded = false;
bool Level::weakFloorCreated = false;

const std::string Level::MAP = "map";
const std::string Level::VISITED = "visited";
const std::string Level::MAPPED = "mapped";
const std::string Level::ENTRANCE = "entrance";
const std::string Level::EXIT = "exit";
const std::string Level::HEAPS = "heaps";
const std::string Level::PLANTS = "plants";
const std::string Level::MOBS = "mobs";
const std::string Level::BLOBS = "blobs";

void Level::buildFlagMaps()
{
	for (int i = 0; i < LENGTH; i++) 
	{
		int flags = Terrain::flags[map[i]];
		passable[i] = (flags & Terrain::PASSABLE) != 0;
		losBlocking[i] = (flags & Terrain::LOS_BLOCKING) != 0;
		flamable[i] = (flags & Terrain::FLAMABLE) != 0;
		secret[i] = (flags & Terrain::SECRET) != 0;
		solid[i] = (flags & Terrain::SOLID) != 0;
		avoid[i] = (flags & Terrain::AVOID) != 0;
		water[i] = (flags & Terrain::LIQUID) != 0;
		pit[i] = (flags & Terrain::PIT) != 0;
	}

	int lastRow = LENGTH - WIDTH;
	for (int i = 0; i < WIDTH; i++) 
	{
		passable[i] = avoid[i] = false;
		passable[lastRow + i] = avoid[lastRow + i] = false;
	}
	for (int i = WIDTH; i < lastRow; i += WIDTH) 
	{
		passable[i] = avoid[i] = false;
		passable[i + WIDTH - 1] = avoid[i + WIDTH - 1] = false;
	}

	for (int i = WIDTH; i < LENGTH - WIDTH; i++) 
	{
		if (water[i]) 
		{
			map[i] = getWaterTile(i);
		}

		if (pit[i]) 
		{
			if (!pit[i - WIDTH]) 
			{
				int c = map[i - WIDTH];
				if (c == Terrain::EMPTY_SP || c == Terrain::STATUE_SP) 
				{
					map[i] = Terrain::CHASM_FLOOR_SP;
				}
				else if (water[i - WIDTH]) 
				{
					map[i] = Terrain::CHASM_WATER;
				}
				else if ((Terrain::flags[c] & Terrain::UNSTITCHABLE) != 0) 
				{
					map[i] = Terrain::CHASM_WALL;
				}
				else 
				{
					map[i] = Terrain::CHASM_FLOOR;
				}
			}
		}
	}
}

int Level::getWaterTile(int pos)
{
	int t = Terrain::WATER_TILES;
	for (int j = 0; j < sizeof(NEIGHBOURS4)/sizeof(int); j++) 
	{
		if ((Terrain::flags[map[pos + NEIGHBOURS4[j]]] & Terrain::UNSTITCHABLE) != 0) 
		{
			t += 1 << j;
		}
	}
	return t;
}

void Level::cleanWalls()
{
	for (int i = 0; i < LENGTH; i++) 
	{
		bool d = false;

		for (int j = 0; j < sizeof(NEIGHBOURS9)/sizeof(int); j++) {
			int n = i + NEIGHBOURS9[j];
			if (n >= 0 && n < LENGTH && map[n] != Terrain::WALL && map[n] != Terrain::WALL_DECO) 
			{
				d = true;
				break;
			}
		}

		if (d) 
		{
			d = false;

			for (int j = 0; j < sizeof(NEIGHBOURS9) / sizeof(int); j++) 
			{
				int n = i + NEIGHBOURS9[j];
				if (n >= 0 && n < LENGTH && !pit[n]) 
				{
					d = true;
					break;
				}
			}
		}

		discoverable[i] = d;
	}
}

Level::Level()
{
	color1 = 0x004400;
	color2 = 0x88CC44;

	feeling = Feeling::NONE;

	viewDistance = Dungeon::isChallenged(Challenges::DARKNESS) ? 3 : 8;
}

Level::~Level()
{
	clear();
}

void Level::create()
{
	resizingNeeded = false;
	
	map.resize(LENGTH);
	visited.resize(LENGTH);
	Arrays<bool>::fill(visited, false);

	mapped.resize(LENGTH);
	Arrays<bool>::fill(mapped, false);

	clear();
	//heaps = new SparseArray<Heap>();

	//plants = new SparseArray<Plant>();
	//
	//if (!Dungeon.bossLevel()) {
	//	addItemToSpawn(Generator.random(Generator.Category.FOOD));
	//	if (Dungeon.posNeeded()) {
	//		addItemToSpawn(new PotionOfStrength());
	//		Dungeon.potionOfStrength++;
	//	}
	//	if (Dungeon.souNeeded()) {
	//		addItemToSpawn(new ScrollOfUpgrade());
	//		Dungeon.scrollsOfUpgrade++;
	//	}
	//	if (Dungeon.soeNeeded()) {
	//		addItemToSpawn(new ScrollOfEnchantment());
	//		Dungeon.scrollsOfEnchantment++;
	//	}
	//
	//	if (Dungeon.depth > 1) {
	//		switch (Random.Int(10)) {
	//		case 0:
	//			if (!Dungeon.bossLevel(Dungeon.depth + 1)) {
	//				feeling = Feeling.CHASM;
	//			}
	//			break;
	//		case 1:
	//			feeling = Feeling.WATER;
	//			break;
	//		case 2:
	//			feeling = Feeling.GRASS;
	//			break;
	//		}
	//	}
	//}
	//
	bool pitNeeded = Dungeon::depth > 1 && weakFloorCreated;
	
	do {
		Arrays<int>::fill(map, feeling == Feeling::CHASM ? Terrain::CHASM : Terrain::WALL);
	
		pitRoomNeeded = pitNeeded;
		weakFloorCreated = false;
	
	} while (!build());
	decorate();
	
	buildFlagMaps();
	cleanWalls();

	createMobs();
	createItems();
}

void Level::clear()
{
	for (std::set<Mob*>::iterator itr = mobs.begin();
		itr != mobs.end(); itr++)
	{
		delete *itr;
	}
	mobs.clear();

	for (std::map<std::string, Blob*>::iterator itr = blobs.begin();
		itr != blobs.end(); itr++)
	{
		delete itr->second;
	}
	blobs.clear();
}

int Level::pitCell()
{
	return randomRespawnCell();
}

int Level::randomRespawnCell()
{
	int cell;
	do {
		cell = Random::Int(LENGTH);
	} while (!passable[cell] || Dungeon::visible[cell] || Actor::findChar(cell) != NULL);
	return cell;
}

int Level::adjustPos(int pos)
{
	return (pos / loadedMapSize) * WIDTH + (pos % loadedMapSize);
}

void Level::reset()
{
	//for (Mob mob : mobs.toArray(new Mob[0])) {
	//	if (!mob.reset()) {
	//		mobs.remove(mob);
	//	}
	//}
	//createMobs();
}

std::vector<bool> Level::updateFieldOfView(Char* c)
{
	int cx = c->pos % WIDTH;
	int cy = c->pos / WIDTH;

	bool sighted = false;
	//boolean sighted = c->buff(Blindness.class) == null && c.buff(Shadows.class) == null && c.isAlive();
	//if (sighted) {
	//	ShadowCaster.castShadow(cx, cy, fieldOfView, c.viewDistance);
	//}
	//else {
		Arrays<bool>::fill(fieldOfView, false);
	//}
	//
	int sense = 1;
	//if (c.isAlive()) {
	//	for (Buff b : c.buffs(MindVision.class)) {
	//		sense = Math.max(((MindVision)b).distance, sense);
	//	}
	//}
	
	if ((sighted && sense > 1) || !sighted) 
	{
	
		int ax = std::max(0, cx - sense);
		int bx = std::min(cx + sense, WIDTH - 1);
		int ay = std::max(0, cy - sense);
		int by = std::min(cy + sense, HEIGHT - 1);
	
		int len = bx - ax + 1;
		int pos = ax + ay * WIDTH;
		for (int y = ay; y <= by; y++, pos += WIDTH) 
		{
			Arrays<bool>::fill(fieldOfView, pos, pos + len, true);
		}
	
		for (int i = 0; i < LENGTH; i++) 
		{
			fieldOfView[i] = fieldOfView[i] & discoverable[i];
		}
	}
	
	if (c->isAlive()) {
		//if (c.buff(MindVision.class) != null) {
		//	for (Mob mob : mobs) {
		//		int p = mob.pos;
		//		fieldOfView[p] = true;
		//		fieldOfView[p + 1] = true;
		//		fieldOfView[p - 1] = true;
		//		fieldOfView[p + WIDTH + 1] = true;
		//		fieldOfView[p + WIDTH - 1] = true;
		//		fieldOfView[p - WIDTH + 1] = true;
		//		fieldOfView[p - WIDTH - 1] = true;
		//		fieldOfView[p + WIDTH] = true;
		//		fieldOfView[p - WIDTH] = true;
		//	}
		//}
		//else 
		if (c == Dungeon::hero && ((Hero*)c)->heroClass.type() == HeroClass::E_HUNTRESS) 
		{
			for (std::set<Mob*>::iterator itr = mobs.begin();
				itr != mobs.end(); itr++)
			{
				Mob* mob = *itr;;
				int p = mob->pos;
				if (distance(c->pos, p) == 2) 
				{
					fieldOfView[p] = true;
					fieldOfView[p + 1] = true;
					fieldOfView[p - 1] = true;
					fieldOfView[p + WIDTH + 1] = true;
					fieldOfView[p + WIDTH - 1] = true;
					fieldOfView[p - WIDTH + 1] = true;
					fieldOfView[p - WIDTH - 1] = true;
					fieldOfView[p + WIDTH] = true;
					fieldOfView[p - WIDTH] = true;
				}
			}
		}
		//if (c.buff(Awareness.class) != null) {
		//	for (Heap heap : heaps.values()) {
		//		int p = heap.pos;
		//		fieldOfView[p] = true;
		//		fieldOfView[p + 1] = true;
		//		fieldOfView[p - 1] = true;
		//		fieldOfView[p + WIDTH + 1] = true;
		//		fieldOfView[p + WIDTH - 1] = true;
		//		fieldOfView[p - WIDTH + 1] = true;
		//		fieldOfView[p - WIDTH - 1] = true;
		//		fieldOfView[p + WIDTH] = true;
		//		fieldOfView[p - WIDTH] = true;
		//	}
		//}
	}

	return fieldOfView;
}

int Level::distance(int a, int b)
{
	int ax = a % WIDTH;
	int ay = a / WIDTH;
	int bx = b % WIDTH;
	int by = b / WIDTH;
	return std::max(std::abs(ax - bx), std::abs(ay - by));
}

int Level::tunnelTile()
{
	return feeling == Feeling::CHASM ? Terrain::EMPTY_SP : Terrain::EMPTY;
}

int Level::randomDestination()
{
	int cell;
	do 
	{
		cell = Random::Int(LENGTH);
	} while (!passable[cell]);
	return cell;
}

bool Level::adjacent(int a, int b)
{
	int diff = std::abs(a - b);
	return diff == 1 || diff == WIDTH || diff == WIDTH + 1 || diff == WIDTH - 1;
}

void Level::mobPress(Mob* mob)
{
	//int cell = mob.pos;
	//
	//if (pit[cell] && !mob.flying) {
	//	Chasm.mobFall(mob);
	//	return;
	//}
	//
	//boolean trap = true;
	//switch (map[cell]) {
	//
	//case Terrain::TOXIC_TRAP:
	//	ToxicTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::FIRE_TRAP:
	//	FireTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::PARALYTIC_TRAP:
	//	ParalyticTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::POISON_TRAP:
	//	PoisonTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::ALARM_TRAP:
	//	AlarmTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::LIGHTNING_TRAP:
	//	LightningTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::GRIPPING_TRAP:
	//	GrippingTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::SUMMONING_TRAP:
	//	SummoningTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::DOOR:
	//	Door.enter(cell);
	//
	//default:
	//	trap = false;
	//}
	//
	//if (trap) {
	//	if (Dungeon.visible[cell]) {
	//		Sample.INSTANCE.play(Assets.SND_TRAP);
	//	}
	//	set(cell, Terrain::INACTIVE_TRAP);
	//	GameScene.updateMap(cell);
	//}
	//
	//Plant plant = plants.get(cell);
	//if (plant != null) {
	//	plant.activate(mob);
	//}
}

void Level::set(int cell, int terrain)
{
	Painter::set(Dungeon::level, cell, terrain);

	int flags = Terrain::flags[terrain];
	passable[cell] = (flags & Terrain::PASSABLE) != 0;
	losBlocking[cell] = (flags & Terrain::LOS_BLOCKING) != 0;
	flamable[cell] = (flags & Terrain::FLAMABLE) != 0;
	secret[cell] = (flags & Terrain::SECRET) != 0;
	solid[cell] = (flags & Terrain::SOLID) != 0;
	avoid[cell] = (flags & Terrain::AVOID) != 0;
	pit[cell] = (flags & Terrain::PIT) != 0;
	water[cell] = terrain == Terrain::WATER || terrain >= Terrain::WATER_TILES;
}

Heap* Level::drop(Item* item, int cell)
{
	//if (Dungeon.isChallenged(Challenges.NO_FOOD) && item instanceof Food) {
	//	item = new Gold(item.price());
	//}
	//else
	//if (Dungeon.isChallenged(Challenges.NO_ARMOR) && item instanceof Armor) {
	//	item = new Gold(item.price());
	//}
	//else
	//if (Dungeon.isChallenged(Challenges.NO_HEALING) && item instanceof PotionOfHealing) {
	//	item = new Gold(item.price());
	//}
	//else
	//if (Dungeon.isChallenged(Challenges.NO_HERBALISM) && item instanceof SeedPouch) {
	//	item = new Gold(item.price());
	//}
	//else
	//if (Dungeon.isChallenged(Challenges.NO_SCROLLS) && (item instanceof Scroll || item instanceof ScrollHolder)) {
	//	if (item instanceof ScrollOfUpgrade) {
	//		// These scrolls still can be found
	//	}
	//	else {
	//		item = new Gold(item.price());
	//	}
	//}
	//
	//if ((map[cell] == Terrain.ALCHEMY) && !(item instanceof Plant.Seed)) {
	//	int n;
	//	do {
	//		n = cell + NEIGHBOURS8[Random.Int(8)];
	//	} while (map[n] != Terrain.EMPTY_SP);
	//	cell = n;
	//}
	//
	//Heap heap = heaps.get(cell);
	//if (heap == null) {
	//
	//	heap = new Heap();
	//	heap.pos = cell;
	//	if (map[cell] == Terrain.CHASM || (Dungeon.level != null && pit[cell])) {
	//		Dungeon.dropToChasm(item);
	//		GameScene.discard(heap);
	//	}
	//	else {
	//		heaps.put(cell, heap);
	//		GameScene.add(heap);
	//	}
	//
	//}
	//else if (heap.type == Heap.Type.LOCKED_CHEST || heap.type == Heap.Type.CRYSTAL_CHEST) {
	//
	//	int n;
	//	do {
	//		n = cell + Level.NEIGHBOURS8[Random.Int(8)];
	//	} while (!Level.passable[n] && !Level.avoid[n]);
	//	return drop(item, n);
	//
	//}
	//heap.drop(item);
	//
	//if (Dungeon.level != null) {
	//	press(cell, null);
	//}
	//
	//return heap;

	return NULL;
}

void Level::uproot(int pos)
{
	//plants.delete(pos);
}

Plant* Level::plant(Plant::Seed* seed, int pos)
{
	//Plant plant = plants.get(pos);
	//if (plant != null) {
	//	plant.wither();
	//}
	//
	//plant = seed.couch(pos);
	//plants.put(pos, plant);
	//
	//GameScene.add(plant);
	//
	//return plant;
	return NULL;
}

void Level::press(int cell, Char* ch)
{
	if (pit[cell] && ch == Dungeon::hero) 
	{
		//Chasm.heroFall(cell);
		return;
	}

	bool trap = false;

	switch (map[cell]) 
	{
	case Terrain::SECRET_TOXIC_TRAP:
		GLog::i(TXT_HIDDEN_PLATE_CLICKS);
	case Terrain::TOXIC_TRAP:
		trap = true;
		//ToxicTrap.trigger(cell, ch);
		break;

	case Terrain::SECRET_FIRE_TRAP:
		GLog::i(TXT_HIDDEN_PLATE_CLICKS);
	case Terrain::FIRE_TRAP:
		trap = true;
		//FireTrap.trigger(cell, ch);
		break;

	case Terrain::SECRET_PARALYTIC_TRAP:
		GLog::i(TXT_HIDDEN_PLATE_CLICKS);
	case Terrain::PARALYTIC_TRAP:
		trap = true;
		//ParalyticTrap.trigger(cell, ch);
		break;

	case Terrain::SECRET_POISON_TRAP:
		GLog::i(TXT_HIDDEN_PLATE_CLICKS);
	case Terrain::POISON_TRAP:
		trap = true;
		//PoisonTrap.trigger(cell, ch);
		break;

	case Terrain::SECRET_ALARM_TRAP:
		GLog::i(TXT_HIDDEN_PLATE_CLICKS);
	case Terrain::ALARM_TRAP:
		trap = true;
		//AlarmTrap.trigger(cell, ch);
		break;

	case Terrain::SECRET_LIGHTNING_TRAP:
		GLog::i(TXT_HIDDEN_PLATE_CLICKS);
	case Terrain::LIGHTNING_TRAP:
		trap = true;
		//LightningTrap.trigger(cell, ch);
		break;

	case Terrain::SECRET_GRIPPING_TRAP:
		GLog::i(TXT_HIDDEN_PLATE_CLICKS);
	case Terrain::GRIPPING_TRAP:
		trap = true;
		//GrippingTrap.trigger(cell, ch);
		break;

	case Terrain::SECRET_SUMMONING_TRAP:
		GLog::i(TXT_HIDDEN_PLATE_CLICKS);
	case Terrain::SUMMONING_TRAP:
		trap = true;
		//SummoningTrap.trigger(cell, ch);
		break;

	case Terrain::HIGH_GRASS:
		//HighGrass.trample(this, cell, ch);
		break;

	case Terrain::WELL:
		//WellWater.affectCell(cell);
		break;

	case Terrain::ALCHEMY:
		if (ch == NULL) 
		{
			//Alchemy.transmute(cell);
		}
		break;

	case Terrain::DOOR:
		//Door.enter(cell);
		break;
	}

	if (trap) 
	{
		//Sample.INSTANCE.play(Assets.SND_TRAP);
		if (ch == Dungeon::hero) 
		{
			Dungeon::hero->interrupt();
		}
		set(cell, Terrain::INACTIVE_TRAP);
		GameScene::updateMap(cell);
	}

	//Plant plant = plants.get(cell);
	//if (plant != null) {
	//	plant.activate(ch);
	//}
}

void Level::destroy(int pos)
{
	if ((Terrain::flags[map[pos]] & Terrain::UNSTITCHABLE) == 0) {

		set(pos, Terrain::EMBERS);

	}
	else {
		boolean flood = false;
		for (int j = 0; j < 4; j++) {
			if (water[pos + NEIGHBOURS4[j]]) {
				flood = true;
				break;
			}
		}
		if (flood) {
			set(pos, getWaterTile(pos));
		}
		else {
			set(pos, Terrain::EMBERS);
		}
	}
}

String Level::tileName(int tile)
{
	if (tile >= Terrain::WATER_TILES) {
		return tileName(Terrain::WATER);
	}

	if (tile != Terrain::CHASM && (Terrain::flags[tile] & Terrain::PIT) != 0) {
		return tileName(Terrain::CHASM);
	}

	switch (tile) {
	case Terrain::CHASM:
		return "Chasm";
	case Terrain::EMPTY:
	case Terrain::EMPTY_SP:
	case Terrain::EMPTY_DECO:
	case Terrain::SECRET_TOXIC_TRAP:
	case Terrain::SECRET_FIRE_TRAP:
	case Terrain::SECRET_PARALYTIC_TRAP:
	case Terrain::SECRET_POISON_TRAP:
	case Terrain::SECRET_ALARM_TRAP:
	case Terrain::SECRET_LIGHTNING_TRAP:
		return "Floor";
	case Terrain::GRASS:
		return "Grass";
	case Terrain::WATER:
		return "Water";
	case Terrain::WALL:
	case Terrain::WALL_DECO:
	case Terrain::SECRET_DOOR:
		return "Wall";
	case Terrain::DOOR:
		return "Closed door";
	case Terrain::OPEN_DOOR:
		return "Open door";
	case Terrain::ENTRANCE:
		return "Depth entrance";
	case Terrain::EXIT:
		return "Depth exit";
	case Terrain::EMBERS:
		return "Embers";
	case Terrain::LOCKED_DOOR:
		return "Locked door";
	case Terrain::PEDESTAL:
		return "Pedestal";
	case Terrain::BARRICADE:
		return "Barricade";
	case Terrain::HIGH_GRASS:
		return "High grass";
	case Terrain::LOCKED_EXIT:
		return "Locked depth exit";
	case Terrain::UNLOCKED_EXIT:
		return "Unlocked depth exit";
	case Terrain::SIGN:
		return "Sign";
	case Terrain::WELL:
		return "Well";
	case Terrain::EMPTY_WELL:
		return "Empty well";
	case Terrain::STATUE:
	case Terrain::STATUE_SP:
		return "Statue";
	case Terrain::TOXIC_TRAP:
		return "Toxic gas trap";
	case Terrain::FIRE_TRAP:
		return "Fire trap";
	case Terrain::PARALYTIC_TRAP:
		return "Paralytic gas trap";
	case Terrain::POISON_TRAP:
		return "Poison dart trap";
	case Terrain::ALARM_TRAP:
		return "Alarm trap";
	case Terrain::LIGHTNING_TRAP:
		return "Lightning trap";
	case Terrain::GRIPPING_TRAP:
		return "Gripping trap";
	case Terrain::SUMMONING_TRAP:
		return "Summoning trap";
	case Terrain::INACTIVE_TRAP:
		return "Triggered trap";
	case Terrain::BOOKSHELF:
		return "Bookshelf";
	case Terrain::ALCHEMY:
		return "Alchemy pot";
	default:
		return "???";
	}
}

String Level::tileDesc(int tile)
{
	switch (tile) {
	case Terrain::CHASM:
		return "You can't see the bottom.";
	case Terrain::WATER:
		return "In case of burning step into the water to extinguish the fire.";
	case Terrain::ENTRANCE:
		return "Stairs lead up to the upper depth.";
	case Terrain::EXIT:
	case Terrain::UNLOCKED_EXIT:
		return "Stairs lead down to the lower depth.";
	case Terrain::EMBERS:
		return "Embers cover the floor.";
	case Terrain::HIGH_GRASS:
		return "Dense vegetation blocks the view.";
	case Terrain::LOCKED_DOOR:
		return "This door is locked, you need a matching key to unlock it.";
	case Terrain::LOCKED_EXIT:
		return "Heavy bars block the stairs leading down.";
	case Terrain::BARRICADE:
		return "The wooden barricade is firmly set but has dried over the years. Might it burn?";
	case Terrain::SIGN:
		return "You can't read the text from here.";
	case Terrain::TOXIC_TRAP:
	case Terrain::FIRE_TRAP:
	case Terrain::PARALYTIC_TRAP:
	case Terrain::POISON_TRAP:
	case Terrain::ALARM_TRAP:
	case Terrain::LIGHTNING_TRAP:
	case Terrain::GRIPPING_TRAP:
	case Terrain::SUMMONING_TRAP:
		return "Stepping onto a hidden pressure plate will activate the trap.";
	case Terrain::INACTIVE_TRAP:
		return "The trap has been triggered before and it's not dangerous anymore.";
	case Terrain::STATUE:
	case Terrain::STATUE_SP:
		return "Someone wanted to adorn this place, but failed, obviously.";
	case Terrain::ALCHEMY:
		return "Drop some seeds here to cook a potion.";
	case Terrain::EMPTY_WELL:
		return "The well has run dry.";
	default:
		if (tile >= Terrain::WATER_TILES) {
			return tileDesc(Terrain::WATER);
		}
		if ((Terrain::flags[tile] & Terrain::PIT) != 0) {
			return tileDesc(Terrain::CHASM);
		}
		return "";
	}

}

void Level::addVisuals(Scene* scene)
{
	//for (int i = 0; i < LENGTH; i++) {
	//	if (pit[i]) {
	//		scene.add(new WindParticle.Wind(i));
	//		if (i >= WIDTH && water[i - WIDTH]) {
	//			scene.add(new FlowParticle.Flow(i - WIDTH));
	//		}
	//	}
	//}
}
