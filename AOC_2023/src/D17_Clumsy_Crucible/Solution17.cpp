#include "pch.h"
#include "Solution17.h"

namespace AoC2023_D17 {
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

		struct hash {
			size_t operator()(const pos_t& lhs) const { return std::hash<int>()(lhs.x) ^ std::hash<int>()(lhs.y); }
		};

		bool inRange(const int& xMax, const int& yMax, const int& xMin = 0, const int& yMin = 0) { return xMin <= x && x < xMax && yMin <= y && y < yMax; }
		friend std::ostream& operator<<(std::ostream& out, const pos_t& pos) { return out << "(" << pos.x << ", " << pos.y << ")"; }
	};

	enum class DIRECTION :int { NONE = -1, NORTH = 0, EAST, SOUTH, WEST };
	static constexpr pos_t NORTH = { -1,0 };
	static constexpr pos_t SOUTH = { 1,0 };
	static constexpr pos_t WEST = { 0,-1 };
	static constexpr pos_t EAST = { 0,1 };
	static constexpr array<pos_t, 4> DIRECTIONS = { NORTH,EAST,SOUTH,WEST };
	static constexpr int TOTAL_DIRECTIONS = 4;

	typedef struct state_t {
		state_t() {}
		state_t(pos_t pos, DIRECTION dir, int steps, int heatLoss) :
			pos(pos), dir(dir), steps(steps), heatLoss(heatLoss) {}
		pos_t pos = { 0,0 };
		DIRECTION dir = DIRECTION::NONE;
		int steps = 1;
		int heatLoss = 0;

		vector<state_t> getNeighbors(int minSteps, int maxSteps) {
			vector<state_t> neighbors;
			// move ahead, in the same direction, bounded by maxSteps
			if (steps < maxSteps) {
				state_t neighbor = *this;
				neighbor.pos = pos + DIRECTIONS[(int)neighbor.dir];
				neighbor.steps++;
				neighbors.push_back(neighbor);
			}
			// can only turn if the minimum of steps is reached
			if (steps >= minSteps) {
				// to the left of current direction
				state_t neighbor = *this;
				neighbor.dir = static_cast<DIRECTION>(((int)dir + 3) % TOTAL_DIRECTIONS);
				neighbor.pos = pos + DIRECTIONS[(int)neighbor.dir];
				neighbor.steps = 1;
				neighbors.push_back(neighbor);

				// to the right of current direction
				neighbor = *this;
				neighbor.dir = static_cast<DIRECTION>(((int)dir + 1) % TOTAL_DIRECTIONS);
				neighbor.pos = pos + DIRECTIONS[(int)neighbor.dir];
				neighbor.steps = 1;
				neighbors.push_back(neighbor);
			}
			return neighbors;
		}

		bool operator==(const state_t& rhs) const { return pos == rhs.pos && dir == rhs.dir && steps == rhs.steps; }
		bool operator!=(const state_t& rhs) const { return !(*this == rhs); }
		bool operator<(const state_t& rhs) const { return heatLoss < rhs.heatLoss; };
		bool operator>(const state_t& rhs) const { return heatLoss > rhs.heatLoss; };
		bool operator>=(const state_t& rhs) const { return !(*this < rhs); };
		bool operator<=(const state_t& rhs) const { return !(*this > rhs); };
		struct hash {
			size_t operator()(const state_t& lhs) const { return pos_t::hash()(lhs.pos) ^ std::hash<int>()((int)lhs.dir); }
		};
	}state_t;

	typedef struct map_t {
		map_t(const vector<string>& lines) : grid(lines.size(), vector<int>()) {
			std::for_each(lines.begin(), lines.end(),
				[&](const string& line) {
					std::transform(line.begin(), line.end(), std::back_inserter(grid[height]), [](const char& c)->int {return (int)(c - '0'); });
					width = line.size(), height++;
				});
		}

		vector<vector<int>> grid = {};
		int height = 0, width = 0;

		int operator[](const pos_t& pos) const { return grid[pos.x][pos.y]; }
		int& operator[](const pos_t& pos) { return grid[pos.x][pos.y]; }
	}map_t;

	// -------------------------- Functions -------------------------------

	state_t dijkstra(const map_t& map, const pos_t& beginPos, const pos_t& endPos, int minSteps = 0, int maxSteps = 3) {
		std::unordered_set<state_t, state_t::hash> closed;

		std::priority_queue<state_t, vector<state_t>, std::greater<state_t>> open;
		for (auto& dir : { DIRECTION::NORTH, DIRECTION::EAST, DIRECTION::SOUTH, DIRECTION::WEST }) {
			auto next = beginPos + DIRECTIONS[(int)dir];
			if (next.inRange(map.height, map.width)) {
				open.push(state_t(beginPos, dir, 0, 0));
			}
		}
		state_t endState;
		while (!open.empty()) {
			state_t currState = open.top();
			open.pop();

			// position already visited
			if (closed.find(currState) != closed.end()) {
				continue;
			}
			closed.insert(currState);

			// stop condition
			if (currState.pos == endPos && currState.steps >= minSteps) {
				endState = currState;
				break;
			}

			// checking the neighbors
			for (state_t nextState : currState.getNeighbors(minSteps, maxSteps)) {
				if (nextState.pos.inRange(map.height, map.width)) {
					nextState.heatLoss += map[nextState.pos];
					open.push(nextState);
				}
			}
		}
		// the destination was not reached
		if (open.empty())
			return state_t();
		return endState;
	}

	// --------------------------- Part 1 ---------------------------------
	int solvePart1(const vector<string>& lines) {
		map_t map(lines);
		auto endState = dijkstra(map, { 0,0 }, { map.height - 1, map.width - 1 });
		return endState.heatLoss;
	}

	// --------------------------- Part 2 ---------------------------------
	int solvePart2(const vector<string>& lines) {
		map_t map(lines);
		auto endState = dijkstra(map, { 0,0 }, { map.height - 1, map.width - 1 }, 4, 10);
		return endState.heatLoss;
	}
}

int AoC2023_D17::solve() {
#if 0 // tests
	auto lines = AoC::readFile("./src/D17_Clumsy_Crucible/small.txt");
	if (lines.empty()) return 1;
#elif 0
	vector<string> lines = {
		"111111111111",
		"999999999991",
		"999999999991",
		"999999999991",
		"999999999991",
	};
#else
	auto lines = AoC::readFile("./src/D17_Clumsy_Crucible/input.txt");
	if (lines.empty()) return 1;
#endif
	std::ostringstream oss;
	for (auto& line : lines) {
		oss << line << "\r\n";
	}
	oss << "\r\n";
	cout << oss.str();

	auto part1 = solvePart1(lines);
	auto part2 = solvePart2(lines);

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << part1 << "\r\n"; // 953
	std::cout << "Part 2 = " << part2 << "\r\n"; // 1180
	std::cout << "-----------------------------------------------------\r\n";
	return 0;
}
