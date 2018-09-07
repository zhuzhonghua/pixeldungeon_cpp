#pragma once

#include "bundlable.h"
#include "typedefine.h"

class Journal{
public:
	class Feature{
	public:
		std::string desc;

		Feature(const std::string& desc)
		{
			this->desc = desc;
		}
		Feature(){}

		virtual bool operator == (const Feature& other)
		{
			return desc.compare(other.desc) == 0;
		}
	};

	static Feature WELL_OF_HEALTH;
	static Feature WELL_OF_AWARENESS;
	static Feature WELL_OF_TRANSMUTATION;
	static Feature SACRIFICIAL_FIRE;
	static Feature ALCHEMY;
	static Feature GARDEN;
	static Feature STATUE;
	static Feature GHOST;
	static Feature WANDMAKER;
	static Feature TROLL;
	static Feature IMP;

	class Record :public Bundlable{
	private:
		static const std::string FEATURE;
		static const std::string DEPTH;

	public:
		Feature feature;
		int depth;

		Record();
		Record(Feature feature, int depth);

		bool operator <(const Record& another) {
			return another.depth - depth < 0;
		}

		virtual void restoreFromBundle(Bundle* bundle);
		virtual void storeInBundle(Bundle* bundle);
		virtual std::string getClassName() { return "Journal::Record";  };
	};

	static ArrayList<Record> records;

	static void reset();
	static void storeInBundle(Bundle* bundle);
	static void restoreFromBundle(Bundle* bundle);

	static void add(Feature feature);
	static void remove(Feature feature);
private:
	static const String JOURNAL;
};