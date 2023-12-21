#include "pch.h"
#include "Solution12.h"

namespace AoC2022_D12 {
	using namespace std;

	// --------------------------- Types ----------------------------------

	// (x,y) coordinates -> x = row, y = col
	typedef struct pos_t {
		int x = 0, y = 0;

		bool operator==(const pos_t& rhs) const { return x == rhs.x && y == rhs.y; }
		bool operator!=(const pos_t& rhs) const { return !(*this == rhs); }
		pos_t& operator+=(const pos_t& rhs) { x += rhs.x; y += rhs.y; return *this; }
		friend pos_t operator+(pos_t lhs, const pos_t& rhs) { lhs += rhs; return lhs; }
		pos_t& operator-=(const pos_t& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
		friend pos_t operator-(pos_t lhs, const pos_t& rhs) { lhs -= rhs; return lhs; }
		friend pos_t operator*(int lhs, pos_t rhs) { rhs.x *= lhs; rhs.y *= lhs; return rhs; }

		struct hash {
			size_t operator()(const pos_t& lhs) const { return std::hash<int>()(lhs.x) ^ std::hash<int>()(lhs.y); }
		};
	};

	typedef struct map_t {
		pos_t start = { 0,0 }, end = { 0,0 };
		vector<vector<int>> grid = {};
		int height = 0, width = 0;

		bool valid(const pos_t& pos) const { return 0 <= pos.x && pos.x < height && 0 <= pos.y && pos.y < width; }
		int at(const pos_t& pos) const { return grid[pos.x][pos.y]; }
		int& at(const pos_t& pos) { return grid[pos.x][pos.y]; }
	}map_t;

	enum class DIRECTION :int { NONE = -1, NORTH = 0, EAST, SOUTH, WEST };
	static constexpr pos_t NORTH = { -1,0 };
	static constexpr pos_t SOUTH = { 1,0 };
	static constexpr pos_t WEST = { 0,-1 };
	static constexpr pos_t EAST = { 0,1 };
	static constexpr array<pos_t, 4> DIRECTIONS = { NORTH,EAST,SOUTH,WEST };
	static constexpr int TOTAL_DIRECTIONS = 4;

	typedef struct state_t {
		state_t() {}
		state_t(pos_t pos, DIRECTION dir, int steps, const map_t* mapPtr) :
			pos(pos), dir(dir), steps(steps), map(mapPtr) {
			updateScore();
		}
		pos_t pos = { 0,0 };
		DIRECTION dir = DIRECTION::NONE;
		int steps = 1;
		const map_t* map = nullptr;
		int score = 0;

		//vector<state_t> getNeighbors(int minSteps, int maxSteps) {
		//	vector<state_t> neighbors;
		//	// move ahead, in the same direction, bounded by maxSteps
		//	if (steps < maxSteps) {
		//		state_t neighbor = *this;
		//		neighbor.pos = pos + DIRECTIONS[(int)neighbor.dir];
		//		neighbor.steps++;
		//		neighbors.push_back(neighbor);
		//	}
		//	// can only turn if the minimum of steps is reached
		//	if (steps >= minSteps) {
		//		// to the left of current direction
		//		state_t neighbor = *this;
		//		neighbor.dir = static_cast<DIRECTION>(((int)dir + 3) % TOTAL_DIRECTIONS);
		//		neighbor.pos = pos + DIRECTIONS[(int)neighbor.dir];
		//		neighbor.steps = 1;
		//		neighbors.push_back(neighbor);

		//		// to the right of current direction
		//		neighbor = *this;
		//		neighbor.dir = static_cast<DIRECTION>(((int)dir + 1) % TOTAL_DIRECTIONS);
		//		neighbor.pos = pos + DIRECTIONS[(int)neighbor.dir];
		//		neighbor.steps = 1;
		//		neighbors.push_back(neighbor);
		//	}
		//	return neighbors;
		//}

		void updateScore() { score = std::abs(pos.x - map->end.x) + std::abs(pos.y - map->end.y) + std::abs(map->at(pos) - map->at(map->end)); }
		bool operator==(const state_t& rhs) const { return pos == rhs.pos && dir == rhs.dir && steps == rhs.steps; }
		bool operator!=(const state_t& rhs) const { return !(*this == rhs); }
		bool operator<(const state_t& rhs) const { return score < rhs.score; };
		bool operator<(const state_t& rhs) { updateScore(); return *this < rhs; };
		bool operator>(const state_t& rhs) const { return score > rhs.score; };
		bool operator>(const state_t& rhs) { updateScore(); return *this < rhs; };
		bool operator>=(const state_t& rhs) const { return !(*this < rhs); };
		bool operator>=(const state_t& rhs) { return !(*this < rhs); };
		bool operator<=(const state_t& rhs) const { return !(*this > rhs); };
		bool operator<=(const state_t& rhs) { return !(*this > rhs); };
		struct hash {
			size_t operator()(const state_t& lhs) const { return pos_t::hash()(lhs.pos) ^ std::hash<int>()((int)lhs.dir); }
		};
	}state_t;



	// -------------------------- Functions -------------------------------
	map_t parseMap(const vector<string>& lines) {
		map_t map;
		map.height = lines.size(), map.width = lines.front().length();
		map.grid = vector<vector<int>>(map.height, vector<int>(map.width, 0));
		for (int i = 0; i < lines.size(); i++) {
			auto& line = lines[i];
			for (int j = 0; j < line.length(); j++) {
				if (line[j] == 'S') map.grid[i][j] = 0, map.start = { i,j };
				else if (line[j] == 'E') map.grid[i][j] = 25, map.end = { i,j };
				else map.grid[i][j] = (int)(line[j] - 'a');
			}
		}
		return map;
	}

	state_t dijkstra(const map_t& map, int minHeightDiff = 0, int HeightDiff = 1) {
		std::unordered_set<state_t, state_t::hash> closedSet;
		std::priority_queue<state_t, vector<state_t>, std::greater<state_t>> openSet;
		for (auto& dir : { DIRECTION::NORTH, DIRECTION::EAST, DIRECTION::SOUTH, DIRECTION::WEST }) {
			auto next = map.start + DIRECTIONS[(int)dir];
			if (map.valid(next)) {
				openSet.push(state_t(map.start, dir, 0, &map));
			}
		}
		state_t endState;
		//while (!open.empty()) {
		//	state_t currState = open.top();
		//	open.pop();
		//	// position already visited
		//	if (closed.find(currState) != closed.end()) {
		//		continue;
		//	}
		//	closed.insert(currState);
		//	// stop condition
		//	if (currState.pos == endPos && currState.steps >= minSteps) {
		//		endState = currState;
		//		break;
		//	}
		//	// checking the neighbors
		//	for (state_t nextState : currState.getNeighbors(minSteps, maxSteps)) {
		//		if (nextState.pos.inRange(map.height, map.width)) {
		//			nextState.heatLoss += map[nextState.pos];
		//			open.push(nextState);
		//		}
		//	}
		//}
		// the destination was not reached
		if (openSet.empty())
			return state_t();
		return endState;
	}

	// --------------------------- Part 1 ---------------------------------
	int64_t solvePart1(const vector<string>& lines) {
		map_t map = parseMap(lines);
		auto s = dijkstra(map);
		return 0;
	}

	// --------------------------- Part 2 ---------------------------------
	int64_t solvePart2(const vector<string>& lines) {
		return 0;
	}
}

int AoC2022_D12::solve() {
#if 1 // smaller test
	auto lines = aoc::readFile("./src/D12_Hill_Climbing_Algorithm/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = aoc::readFile("./src/D12_Hill_Climbing_Algorithm/input.txt");
	if (lines.empty()) return 1;
#endif
	// pretty print
	aoc::prettyPrint(lines);

	auto part1 = solvePart1(lines);
	auto part2 = solvePart2(lines);

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << part1 << "\r\n"; // 
	std::cout << "Part 2 = " << part2 << "\r\n"; // 
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
}
