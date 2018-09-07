#pragma once

#include <set>
#include <list>

class Graph{
public:
	class Node {
	public:
		virtual int Distance() = 0;
		virtual void Distance(int value) = 0;

		virtual int Price() = 0;
		virtual void Price(int value) = 0;

		virtual std::set<Node*> edges() = 0;

	};
public:
	static void setPrice(std::list<Node*>& nodes, int value);
	static void buildDistanceMap(std::set<Node*>& nodes, Node* focus);
	static std::list<Node*> buildPath(std::set<Node*>& nodes, Node* from, Node* to);
};