#include "graph.h"

void Graph::setPrice(std::list<Node*>& nodes, int value)
{
	for (std::list<Node*>::iterator itr = nodes.begin();
		itr != nodes.end(); itr++) 
	{
		(*itr)->Price(value);
	}
}

void Graph::buildDistanceMap(std::set<Node*>& nodes, Node* focus)
{
	for (std::set<Node*>::iterator itr = nodes.begin();
		itr != nodes.end(); itr++)
	{
		(*itr)->Distance(std::numeric_limits<int>::max());
	}

	std::list<Node*> queue;

	focus->Distance(0);
	queue.push_back(focus);

	while (!queue.empty()) 
	{

		Node* node = queue.front();
		queue.pop_front();

		int distance = node->Distance();
		int price = node->Price();

		std::set<Node*> edges = node->edges();
		for (std::set<Node*>::iterator itr = edges.begin();
			itr != edges.end(); itr++)
		{
			Node* edge = *itr;
			if (edge->Distance() > distance + price) 
			{
				queue.push_back(edge);
				edge->Distance(distance + price);
			}
		}
	}
}

std::list<Graph::Node*> Graph::buildPath(std::set<Node*>& nodes, Node* from, Node* to)
{
	std::list<Node*> path;

	Node* room = from;
	while (room != to) 
	{
		int min = room->Distance();
		Node* next = NULL;

		std::set<Node*> edges = room->edges();
		for (std::set<Node*>::iterator itr = edges.begin();
			itr != edges.end(); itr++)
		{
			Node* edge = *itr;
			int distance = edge->Distance();
			if (distance < min) 
			{
				min = distance;
				next = edge;
			}
		}

		if (next == NULL) 
		{
			return std::list<Node*>();
		}

		path.push_back(next);
		room = next;
	}

	return path;
}
