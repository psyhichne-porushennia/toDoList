#include "App.h"

void to_json(nlohmann::json& j, const Task& task)
{
	j = nlohmann::json
	{
		{"taskID", task.getTaskID()},
		{"text", task.getText()},
		{"status", static_cast<int>(task.getStatus())}
	};
}

void from_json(const nlohmann::json& j, Task& task) {
	size_t id = j.at("taskID").get<size_t>();
	std::string text = j.at("text").get<std::string>();
	Status status = static_cast<Status>(j.at("status").get<int>());
	task = Task(id, text, status);
}

iTaskRepository::~iTaskRepository() {}


bool FileRepository::saveTasks(const std::vector<Task>& tasks)
{
	nlohmann::json j = tasks;
	std::ofstream file(filename);
	if (file.is_open())
	{
		file << j.dump(4);
		return true;
	}
	return false;
}

std::vector<Task> FileRepository::loadTasks()
{
	std::vector<Task> tasks;
	std::ifstream file(filename);
	
	if (file.is_open())
	{
		nlohmann::json j;
		file >> j;
		tasks = j.get<std::vector<Task>>();
	}

	return tasks;
}

std::vector<Task> TaskManager::getTasks() const
{	
	return tasks;
}

void TaskManager::addtask(std::string text)
{
	tasks.emplace_back(tasks.size(), text);
}

void TaskManager::deleteTask(size_t taskID)
{
	auto it = std::remove_if(tasks.begin(), tasks.end(),
		[taskID](const Task& task) {
			return task.getTaskID() == taskID;
		});

	if (it != tasks.end())
	{
		tasks.erase(it, tasks.end());

		for (size_t i = 0; i < tasks.size(); ++i)
		{
			tasks[i] = Task(i, tasks[i].getText(), tasks[i].getStatus());
		}
	}
}

void TaskManager::saveTasks()
{
	repository->saveTasks(tasks);
}


