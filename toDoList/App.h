#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>


enum Status
{
	toDO,
	pending,
	done,
	archived
};

class Task
{
public:
	Task() = default;

	Task(size_t id, const std::string& text, Status status = toDO)
		: taskID(id), text(text), status(status) {}


	size_t getTaskID() const { return taskID; }
	std::string getText() const { return text; }
	Status getStatus() const { return status; }

	friend void to_json(nlohmann::json& j, const Task& task);
	friend void from_json(const nlohmann::json& j, Task& task);
private:
	size_t taskID;
	std::string text;
	Status status;
};

void to_json(nlohmann::json& j, const Task& task);
void from_json(const nlohmann::json& j, Task& task);

class iTaskRepository
{
public:
	virtual ~iTaskRepository() = 0;

	virtual bool saveTasks(const std::vector<Task>& tasks) = 0;
	virtual std::vector<Task> loadTasks() = 0;
};

class FileRepository : public iTaskRepository
{
public:
	FileRepository(const std::string& filename)
		: filename(filename) {}

	bool saveTasks(const std::vector<Task>& tasks) override;
	std::vector<Task> loadTasks() override;
private:
	std::string filename;
};

class TaskManager
{
public:
	TaskManager(std::unique_ptr<iTaskRepository> repo) : repository{ std::move(repo) } 
	{
		tasks = repository->loadTasks();
	}
	void addtask(std::string text);
	std::vector<Task> getTasks() const;
	void saveTasks();
	void deleteTask(size_t taskID);

private:
	std::vector<Task> tasks;
	std::unique_ptr<iTaskRepository> repository;
};
