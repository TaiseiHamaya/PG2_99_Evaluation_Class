#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <thread>
#include <memory>

struct ThreadLoadData final {
	std::unique_ptr<std::thread> thread;
	bool is_end_load;
};

class ThreadLoad {
public:
	static void Initialize();
	static void Finalize();
	static void RegisterLoadFunction(const std::string& identifier_name, const std::function<void(void)>& func);
	static bool IsEndLoad(const std::string& identifier_name);
	static void Join(const std::string& identifier_name);

private:
	static void ThreadLoadFunction(const std::function<void(void)>& func, bool* const is_end);

private:
	static std::unordered_map<std::string, ThreadLoadData> load_list;
};