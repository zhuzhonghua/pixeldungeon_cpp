#include "pathfinder.h"
#include "util.h"

std::vector<int> PathFinder::distance;

void PathFinder::setMapSize(int width, int height)
{
	int size = width * height;

	if (PathFinder::size != size) 
	{
		PathFinder::size = size;
		distance.resize(size);
		goals.resize(size);
		queue.resize(size);

		int tmp[] = {-1, +1, -width, +width, -width - 1, -width + 1, +width - 1, +width + 1};
		dir.assign(tmp, tmp+sizeof(tmp)/sizeof(int));
	}
}

int PathFinder::getStep(int from, int to, std::vector<bool>& passable)
{
	if (!buildDistanceMap(from, to, passable)) 
	{
		return -1;
	}

	// From the starting position we are making one step downwards
	int minD = distance[from];
	int best = from;

	int step, stepD;

	for (int i = 0; i < dir.size(); i++) 
	{
		if ((stepD = distance[step = from + dir[i]]) < minD) 
		{
			minD = stepD;
			best = step;
		}
	}

	return best;
}

int PathFinder::getStepBack(int cur, int from, std::vector<bool>& passable)
{
	int d = buildEscapeDistanceMap(cur, from, 2.0f, passable);
	for (int i = 0; i < size; i++) 
	{
		goals[i] = distance[i] == d;
	}
	if (!buildDistanceMap(cur, goals, passable)) 
	{
		return -1;
	}

	int s = cur;

	// From the starting position we are making one step downwards
	int minD = distance[s];
	int mins = s;

	for (int i = 0; i < dir.size(); i++) 
	{
		int n = s + dir[i];
		int thisD = distance[n];

		if (thisD < minD) 
		{
			minD = thisD;
			mins = n;
		}
	}

	return mins;
}

std::vector<bool> PathFinder::goals;
std::vector<int> PathFinder::queue;

int PathFinder::size = 0;

std::vector<int> PathFinder::dir;

bool PathFinder::buildDistanceMap(int from, int to, std::vector<bool>& passable)
{
	if (from == to) 
	{
		return false;
	}

	Arrays<int>::fill(distance, std::numeric_limits<int>::max());

	bool pathFound = false;

	int head = 0;
	int tail = 0;

	// Add to queue
	queue[tail++] = to;
	distance[to] = 0;

	while (head < tail) 
	{
		// Remove from queue
		int step = queue[head++];
		if (step == from) 
		{
			pathFound = true;
			break;
		}
		int nextDistance = distance[step] + 1;

		for (int i = 0; i < dir.size(); i++) 
		{
			int n = step + dir[i];
			if (n == from || (n >= 0 && n < size && passable[n] && (distance[n] > nextDistance))) 
			{
				// Add to queue
				queue[tail++] = n;
				distance[n] = nextDistance;
			}
		}
	}

	return pathFound;
}

void PathFinder::buildDistanceMap(int to, std::vector<bool>& passable, int limit)
{
	Arrays<int>::fill(distance, std::numeric_limits<int>::max());

	int head = 0;
	int tail = 0;

	// Add to queue
	queue[tail++] = to;
	distance[to] = 0;

	while (head < tail) {

		// Remove from queue
		int step = queue[head++];

		int nextDistance = distance[step] + 1;
		if (nextDistance > limit) {
			return;
		}

		for (int i = 0; i < dir.size(); i++) 
		{

			int n = step + dir[i];
			if (n >= 0 && n < size && passable[n] && (distance[n] > nextDistance)) {
				// Add to queue
				queue[tail++] = n;
				distance[n] = nextDistance;
			}

		}
	}
}

bool PathFinder::buildDistanceMap(int from, std::vector<bool>& to, std::vector<bool>& passable)
{
	if (to[from]) 
	{
		return false;
	}

	Arrays<int>::fill(distance, std::numeric_limits<int>::max());

	bool pathFound = false;

	int head = 0;
	int tail = 0;

	// Add to queue
	for (int i = 0; i < size; i++) 
	{
		if (to[i]) {
			queue[tail++] = i;
			distance[i] = 0;
		}
	}

	while (head < tail) 
	{
		// Remove from queue
		int step = queue[head++];
		if (step == from) {
			pathFound = true;
			break;
		}
		int nextDistance = distance[step] + 1;

		for (int i = 0; i < dir.size(); i++) 
		{
			int n = step + dir[i];
			if (n == from || (n >= 0 && n < size && passable[n] && (distance[n] > nextDistance))) {
				// Add to queue
				queue[tail++] = n;
				distance[n] = nextDistance;
			}

		}
	}

	return pathFound;
}

int PathFinder::buildEscapeDistanceMap(int cur, int from, float factor, std::vector<bool>& passable)
{
	Arrays<int>::fill(distance, std::numeric_limits<int>::max());

	int destDist = std::numeric_limits<int>::max();

	int head = 0;
	int tail = 0;

	// Add to queue
	queue[tail++] = from;
	distance[from] = 0;

	int dist = 0;

	while (head < tail) 
	{
		// Remove from queue
		int step = queue[head++];
		dist = distance[step];

		if (dist > destDist) 
		{
			return destDist;
		}

		if (step == cur) 
		{
			destDist = (int)(dist * factor) + 1;
		}

		int nextDistance = dist + 1;

		for (int i = 0; i < dir.size(); i++) 
		{
			int n = step + dir[i];
			if (n >= 0 && n < size && passable[n] && distance[n] > nextDistance) 
			{
				// Add to queue
				queue[tail++] = n;
				distance[n] = nextDistance;
			}
		}
	}

	return dist;
}
