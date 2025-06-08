#include "App.h"
#include <iostream>


int main()
{
	std::string path = "file.txt";
	auto repo = std::make_unique<FileRepository>(path);
	TaskManager manager(std::move(repo));


	auto loadedTasks = manager.getTasks();
	for (const auto& task : loadedTasks)
		std::cout << task.getTaskID() << ": " << task.getText() << std::endl;

	std::cout << "\n--- After Deletion ---\n";

	manager.deleteTask(0); 

	auto updatedTasks = manager.getTasks();
	for (const auto& task : updatedTasks)
		std::cout << task.getTaskID() << ": " << task.getText() << std::endl;

	manager.saveTasks(); 

	return 0;
}


