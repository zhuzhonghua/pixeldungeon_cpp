#include "journal.h"

Journal::Feature Journal::WELL_OF_HEALTH("Well of Health");
Journal::Feature Journal::WELL_OF_AWARENESS("Well of Awareness");
Journal::Feature Journal::WELL_OF_TRANSMUTATION("Well of Transmutation");
Journal::Feature Journal::SACRIFICIAL_FIRE("Sacrificial chamber");
Journal::Feature Journal::ALCHEMY("Alchemy pot");
Journal::Feature Journal::GARDEN("Garden");
Journal::Feature Journal::STATUE("Animated statue");
Journal::Feature Journal::GHOST("Sad ghost");
Journal::Feature Journal::WANDMAKER("Old wandmaker");
Journal::Feature Journal::TROLL("Troll blacksmith");
Journal::Feature Journal::IMP("Ambitious imp");

ArrayList<Journal::Record> Journal::records;

void Journal::reset()
{
	records.clear();
}

void Journal::storeInBundle(Bundle* bundle)
{
	//bundle.put( JOURNAL, records );
}

void Journal::restoreFromBundle(Bundle* bundle)
{
	//records = new ArrayList<Record>();
	//for (Bundlable rec : bundle.getCollection(JOURNAL)) {
	//	records.add((Record)rec);
	//}
}

void Journal::add(Feature feature)
{
	//int size = records.size();
	//for (int i = 0; i < size; i++) {
	//	Record rec = records.get(i);
	//	if (rec.feature == feature && rec.depth == Dungeon.depth) {
	//		return;
	//	}
	//}
	//
	//records.add(new Record(feature, Dungeon.depth));
}

void Journal::remove(Feature feature)
{
	//int size = records.size();
	//for (int i = 0; i < size; i++) {
	//	Record rec = records.get(i);
	//	if (rec.feature == feature && rec.depth == Dungeon.depth) {
	//		records.remove(i);
	//		return;
	//	}
	//}
}

const String Journal::JOURNAL = "journal";

const std::string Journal::Record::FEATURE = "feature";

const std::string Journal::Record::DEPTH = "depth";

Journal::Record::Record()
{
	depth = 0;
}

Journal::Record::Record(Feature feature, int depth)
{
	this->feature = feature;
	this->depth = depth;
}

void Journal::Record::restoreFromBundle(Bundle* bundle)
{
	//feature = Feature.valueOf(bundle.getString(FEATURE));
	//depth = bundle.getInt(DEPTH);
}

void Journal::Record::storeInBundle(Bundle* bundle)
{
	//bundle.put(FEATURE, feature.toString());
	//bundle.put(DEPTH, depth);
}
