#include "pch.h"
#include "Solution7.h"

namespace AoC2022_D7 {
	using namespace std;

	// --------------------------- Types ----------------------------------
	typedef struct file_t {
		string name;
		uint64_t size;

		file_t() :name(""), size(0) {};
		file_t(const uint64_t& sz, const string& n) :name(n), size(sz) {};
	};

	typedef struct folder_t {
		string name;
		vector<file_t> files;
		vector<folder_t*> dirs;
		folder_t* root;
		uint64_t size = 0;

		folder_t(string iName) : name(iName), files(), dirs(), root(nullptr), size(0) {}
		~folder_t() {
			std::for_each(dirs.begin(), dirs.end(), [](folder_t* pFolder) {delete pFolder; });
			root = nullptr;
		}
		void addFile(file_t file) { files.push_back(file); updateSize(file.size); }
		void addFolder(folder_t* folder) { folder->root = this; dirs.push_back(folder); updateSize(folder->size); }
		void updateSize(uint64_t sz) { folder_t* tmp = this; while (tmp) { tmp->size += sz; tmp = tmp->root; } }
	};

	// ------------------------- Constants --------------------------------
	static constexpr uint64_t DISK_CAPACITY = 70000000;
	static constexpr uint64_t UPDATE_SIZE = 30000000;

	// -------------------------- Functions -------------------------------
	folder_t* populateList(folder_t* head, const vector<string>& lines, int start, int length) {
		for (int i = start; i < start + length; i++) {
			string command = lines[i];
			auto info = aoc::parseString(command, " ");
			if (info[0] == "dir") {
				folder_t* dir = new folder_t(info[1]);
				head->addFolder(dir);
			}
			else {
				file_t file(std::stoull(info[0]), info[1]);
				head->addFile(file);
			}
		}
		return head;
	}

	folder_t* changeDir(folder_t* head, string opt) {
		if (opt == "/") {
			while (head->root != nullptr)
				head = head->root;
		}
		else if (opt == "..") {
			if (head->root != nullptr)
				head = head->root;
		}
		else {
			for (int i = 0; i < head->dirs.size(); i++) {
				if (head->dirs[i]->name == opt) {
					head = head->dirs[i];
					break;
				}
			}
		}
		return head;
	}

	folder_t* parseInput(const vector<string>& lines) {
		folder_t* root = new folder_t("/");
		folder_t* head = root;
		int i = 0;
		while (i < lines.size()) {
			string line = lines[i];
			auto info = aoc::parseString(line, " ");
			if (info[0] == "$" && info[1] == "cd") {
				head = changeDir(head, info[2]);
			}
			else if (info[0] == "$" && info[1] == "ls") {
				int len = 0;
				i++;
				while (i + len < lines.size() && lines[i + len][0] != '$') len++;
				head = populateList(head, lines, i, len);
				i += len - 1;
			}
			i++;
		}
		return root;
	}

	vector<folder_t*> findDirs(folder_t* head, uint64_t minSize, uint64_t maxSize) {
		if (head->dirs.size() == 0) return {};
		vector<folder_t*> res;
		if (minSize <= head->size && head->size <= maxSize)
			res.push_back(head);
		for (auto dir : head->dirs) {
			if (minSize <= dir->size && dir->size <= maxSize)
				res.push_back(dir);
			if (dir->dirs.size() > 0) {
				auto subRes = findDirs(dir, minSize, maxSize);
				res.insert(res.end(), subRes.begin(), subRes.end());
			}
		}
		return res;
	}

	// --------------------------- Part 1 ---------------------------------
	uint64_t solvePart1(const vector<string>& lines) {
		folder_t* root = parseInput(lines);
		auto smallDirs = findDirs(root, 0, 100000);
		uint64_t sum = 0;
		for (auto dir : smallDirs) {
			sum += dir->size;
		}
		delete root;
		return sum;
	}

	// --------------------------- Part 2 ---------------------------------
	int solvePart2(const vector<string>& lines) {
		folder_t* root = parseInput(lines);
		auto freeSpace = DISK_CAPACITY - root->size;
		auto minSize = UPDATE_SIZE - freeSpace;
		auto dirs = findDirs(root, minSize, DISK_CAPACITY);
		if (dirs.size() == 0) return 0;
		std::sort(dirs.begin(), dirs.end(), [](folder_t* a, folder_t* b) {return a->size < b->size; });
		uint64_t sizeToDelete = dirs.front()->size;
		delete root;
		return sizeToDelete;
	}
}


int AoC2022_D7::solve() {
#if 0 // test
	auto lines = aoc::readFile("./src/D7_No_Space_Left_On_Device/small.txt");
	if (lines.empty()) return 1;
#else
	auto lines = aoc::readFile("./src/D7_No_Space_Left_On_Device/input.txt");
	if (lines.empty()) return 1;
#endif
	aoc::prettyPrint(lines);

	std::cout << "-----------------------------------------------------\r\n";
	std::cout << "Part 1 = " << solvePart1(lines) << "\r\n"; // 1792222
	std::cout << "Part 2 = " << solvePart2(lines) << "\r\n"; // 2851
	std::cout << "-----------------------------------------------------\r\n";

	return 0;
}