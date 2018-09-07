#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>

class Bundle;
class ItemStatusHandler{
private:
	std::vector<std::string> items;

	std::map<std::string, int> images;
	std::map<std::string, std::string> labels;
	
	static const std::string PFX_IMAGE;
	static const std::string PFX_LABEL;
	static const std::string PFX_KNOWN;

	void restore(Bundle* bundle, std::vector<std::string>& allLabels, std::vector<int> allImages);
public:
	std::set<std::string> known;

	ItemStatusHandler(std::vector<std::string>& items, std::vector<std::string> allLabels, std::vector<int> allImages);
	ItemStatusHandler(std::vector<std::string>& items, std::vector<std::string> labels, std::vector<int> images, Bundle* bundle);

	void save(Bundle* bundle);
	int image(const std::string& item);
	std::string label(const std::string& item);
	bool isKnown(const std::string& item);
	void know(const std::string& item);
	std::set<std::string> unknown();
};