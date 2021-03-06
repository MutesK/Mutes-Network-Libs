#include "Common.h"
#include "TaskManager.h"
#include <random>

using namespace Util;
using namespace std;


TaskManager::TaskManager()
{
	_Tasklist.clear();

	CreateAgent();
}

// TO DO
TaskManager::~TaskManager()
{
	_Tasklist.clear();
}


void TaskManager::CreateAgent()
{
	for (uint32_t i = 0; i < std::thread::hardware_concurrency() * 4; i++)
	{
		auto agent = CreateAgent(i);
	}
}

bool TaskManager::AddTaskAgentid(Task* t, uint64_t agentNum)
{
	const auto agentName = "TaskAgent" + to_string(agentNum);

	return AddTaskAgentid(t, agentName);
}

bool TaskManager::AddTaskAgentid(Task* t, std::string agentName)
{
	const auto result = _Tasklist.find(agentName);
	if (result == _Tasklist.end())
	{
		return false;
	}

	auto agent = result->second;
	agent->AddTask(*t);
	return true;
}

bool TaskManager::AddTask(Task* t)
{
	auto tasksize = _Tasklist.size();

	random_device rn;
	mt19937_64 rnd(rn());

	uniform_int_distribution<int> range(0, static_cast<int>(_Tasklist.size()));
	return AddTaskAgentid(t, range(rnd));
}

std::shared_ptr<TaskAgent> TaskManager::CreateAgent(uint64_t agentNum)
{
	const auto agentName = "TaskAgent" + to_string(agentNum);

	return CreateAgent(agentName);
}

std::shared_ptr<TaskAgent> TaskManager::CreateAgent(std::string agentName)
{
	auto Agent = make_shared<TaskAgent>(agentName);

	_Tasklist[agentName] = Agent;
	return Agent;
}
