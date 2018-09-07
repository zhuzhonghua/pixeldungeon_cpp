#include "itemstatushandler.h"
#include "util.h"
#include "bundle.h"

const std::string ItemStatusHandler::PFX_IMAGE = "_image";
const std::string ItemStatusHandler::PFX_LABEL = "_label";
const std::string ItemStatusHandler::PFX_KNOWN = "_known";

void ItemStatusHandler::restore(Bundle* bundle, std::vector<std::string>& allLabels, std::vector<int> allImages)
{
	std::list<std::string> labelsLeft(allLabels.begin(), allLabels.end());
	std::list<int> imagesLeft(allImages.begin(), allImages.end());

	for (int i = 0; i < items.size(); i++) 
	{
		std::string item = items[i];
		std::string itemName = item;

		if (bundle->contains(itemName + PFX_LABEL)) 
		{
			std::string label = bundle->getString(itemName + PFX_LABEL);
			labels.insert(std::make_pair(item, label));
			labelsLeft.remove(label);

			int image = bundle->getInt(itemName + PFX_IMAGE);
			images.insert(std::make_pair(item, image));
			imagesLeft.remove(image);

			if (bundle->getBoolean(itemName + PFX_KNOWN)) 
			{
				known.insert(item);
			}
		}
		else 
		{
			int index = Random::Int(labelsLeft.size());

			std::list<std::string>::iterator itr;
			int c = 0;
			for (itr = labelsLeft.begin(); itr != labelsLeft.end(); itr++, c++)
			{
				if (c == index) break;
			}

			labels.insert(std::make_pair(item, *itr));
			labelsLeft.erase(itr);

			std::list<int>::iterator itr2;
			c = 0;
			for (itr2 = imagesLeft.begin(); itr2 != imagesLeft.end(); itr2++, c++)
			{
				if (c == index) break;
			}
			images.insert(std::make_pair(item, *itr2));
			imagesLeft.erase(itr2);
		}
	}
}

ItemStatusHandler::ItemStatusHandler(std::vector<std::string>& items, std::vector<std::string> allLabels, std::vector<int> allImages)
{
	this->items = items;
	
	std::list<std::string> labelsLeft(allLabels.begin(), allLabels.end());
	std::list<int> imagesLeft(allImages.begin(), allImages.end());

	for (int i = 0; i < items.size(); i++) 
	{
		std::string item = (items[i]);

		int index = Random::Int(labelsLeft.size());

		std::list<std::string>::iterator itr;
		int c = 0;
		for (itr = labelsLeft.begin(); itr != labelsLeft.end(); itr++,c++)
		{
			if (c == index) break;
		}

		labels.insert(std::make_pair(item, *itr));
		labelsLeft.erase(itr);

		std::list<int>::iterator itr2;
		c = 0;
		for (itr2 = imagesLeft.begin(); itr2 != imagesLeft.end(); itr2++, c++)
		{
			if (c == index) break;
		}
		images.insert(std::make_pair(item, *itr2));
		imagesLeft.erase(itr2);
	}
}

ItemStatusHandler::ItemStatusHandler(std::vector<std::string>& items, std::vector<std::string> labels, std::vector<int> images, Bundle* bundle)
{
	this->items = items;
	restore(bundle, labels, images);
}

void ItemStatusHandler::save(Bundle* bundle)
{
	for (int i = 0; i < items.size(); i++) {
		std::string itemName = items[i];

		bundle->put(itemName + PFX_IMAGE, images[items[i]]);
		bundle->put(itemName + PFX_LABEL, labels[items[i]]);
		bundle->put(itemName + PFX_KNOWN, known.find(items[i]) == known.end());
	}
}

int ItemStatusHandler::image(const std::string& item)
{
	std::map<std::string, int>::iterator itr = images.find(item);
	return itr->second;
}

std::string ItemStatusHandler::label(const std::string& item)
{
	std::map<std::string, std::string>::iterator itr = labels.find(item);
	return itr->second;
}

bool ItemStatusHandler::isKnown(const std::string& item)
{
	return known.find(item) == known.end();
}

void ItemStatusHandler::know(const std::string& item)
{
	known.insert(item);

	if (known.size() == items.size() - 1) 
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (known.find(items[i]) == known.end()) 
			{
				known.insert(items[i]);
				break;
			}
		}
	}
}

std::set<std::string> ItemStatusHandler::unknown()
{
	std::set<std::string> result;
	for (int i = 0; i < items.size(); i++) 
	{
		if (known.find(items[i]) == known.end()) 
		{
			result.insert(items[i]);
		}
	}
	return result;
}
