#include "ThreadLoad.h"

#include <cassert>

std::unordered_map<std::string, ThreadLoadData> ThreadLoad::load_list;

void ThreadLoad::Initialize() {
	assert(load_list.size() == 0);
	load_list.clear();
}

void ThreadLoad::Finalize() {
	for (auto itr = load_list.begin(); itr != load_list.end(); ++itr) {
		if (itr->second.thread->joinable()) {
			itr->second.thread->join();
		}
		itr->second.is_end_load = true;
	}
}

void ThreadLoad::RegisterLoadFunction(const std::string& identifier_name, const std::function<void(void)>& func) {
	assert(load_list.find(identifier_name) == load_list.end());
	load_list.emplace(identifier_name, ThreadLoadData{ nullptr,false });
	load_list.at(identifier_name).thread = std::make_unique<std::thread>(std::thread(&ThreadLoad::ThreadLoadFunction, func, &load_list.at(identifier_name).is_end_load));
}

bool ThreadLoad::IsEndLoad(const std::string& identifier_name) {
	assert(load_list.find(identifier_name) != load_list.end());
	return load_list.at(identifier_name).is_end_load;
}

void ThreadLoad::Join(const std::string& identifier_name) {
	assert(load_list.find(identifier_name) != load_list.end());
	if (load_list.at(identifier_name).thread->joinable()) {
		load_list.at(identifier_name).thread->join();
	}
}

void ThreadLoad::ThreadLoadFunction(const std::function<void(void)>& func, bool* const is_end) {
	func();
	*is_end = true;
}
