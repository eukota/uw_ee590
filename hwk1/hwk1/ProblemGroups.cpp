#include <iostream>
#include <string>
#include <vector>
#include "tools.h"
#include "utils.h"
#include "ProblemGroups.h"
#include "arithmetic.h"

ResultsStruct::ResultsStruct()
	: WindowsRunTime(0.0)
	, OpenCLRunTime(0.0)
	, HasWindowsRunTime(false)
	, HasOpenCLRunTime(false)
{
}

void PrintResults(const std::vector<ResultsStruct*>& results)
{
	if (results.empty())
		return;

	double totalWindowsTimes = 0.0;
	double totalOpenCLTimes = 0.0;
	int num = 1;
	for (std::vector<ResultsStruct*>::const_iterator i = results.begin(), e = results.end(); i != e; ++i, ++num)
	{
		totalWindowsTimes += (*i)->WindowsRunTime;
		totalOpenCLTimes += (*i)->OpenCLRunTime;
		printf("Run: %d: Windows Profiler Runtime: %f. OpenCL Profiler Runtime: %f.\n", num, (*i)->WindowsRunTime, (*i)->OpenCLRunTime);
	}

	const double WindowsAvg = totalWindowsTimes / (double)num;
	const double OpenCLAvg = totalOpenCLTimes / (double)num;
	printf("---------------------------\n");
	if(results.front()->HasWindowsRunTime)
		printf("Average Windows Profiler Runtime: %f.\n", WindowsAvg);
	if (results.front()->HasOpenCLRunTime)
		printf("Average OpenCL Profiler Runtime : %f.\n", OpenCLAvg);
}

int ProblemGroup::operator()(int problem)
{
	if (problems_.find(problem) == problems_.end())
	{
		std::cout << "WARNING: " << problem_group_num_ << "." << problem << " not found" << std::endl;
		return 0;
	}

	std::vector<ResultsStruct*> results;
	int retVal = 0;
	const size_t runCount = (GroupNum() == 0 ? 1 : dmath::RUN_COUNT);
	for (int i = 0; i < runCount; i++)
	{
		if(GroupNum() != 0)
			printf("\r Running... %d", i);
		ResultsStruct* result = new ResultsStruct();
		retVal = problems_[problem]->operator()(result);
		results.push_back(result);
	}
	printf("\n");
	if(GroupNum() != 0)
		PrintResults(results);
	
	// @TODO verify that results vector destructing deletes members
	return retVal;
}
GroupManager::GroupManager(const std::string& name)
	: GroupName(name)
{
}

GroupManager::~GroupManager()
{
	for (std::map<int, ProblemGroup*>::iterator i = groups_.begin(), e = groups_.end(); i != e; ++i)
		delete i->second;
}

void GroupManager::PrintGroupMenu()
{
	std::cout << "Group: " << GroupName.c_str() << std::endl;
	for (std::map<int, ProblemGroup*>::const_iterator i = groups_.begin(), e = groups_.end(); i != e; ++i)
	{
		for (std::map<int, Problem*>::const_iterator g = i->second->problems_.begin(), h = i->second->problems_.end(); h != g; ++g)
		{
			std::cout << i->first << "." << g->first << ": " << g->second->Annotation().c_str() << std::endl;
		}
	}
}

int GroupManager::Run()
{
	int result = 0;
	std::cout << "Running new " << GroupName.c_str() << " Tests" << std::endl;
	std::string input;
	do
	{
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Enter " << ProblemGroupName().c_str() << "." << ProblemName().c_str() << " to run: " << std::endl;
		std::cout << "(enter \"?\" for list of functions)" << std::endl;

		std::cin >> input;
		if (input == "?")
		{
			PrintGroupMenu();
			continue;
		}
		if (atoi(input.c_str()) == -1)
			return 0;

		std::vector<std::string> selection = (tools::split(input, "."));
		int problemgroup = atoi(selection[0].c_str());
		int problem = atoi(selection[1].c_str());
		std::cout << "-------------------------------------------------------" << std::endl;
		if (groups_.find(problemgroup) == groups_.end())
		{
			std::cout << "WARNING: " << ProblemGroupName().c_str() << " " << problemgroup << " not found." << std::endl;
			continue;
		}
		std::cout << "Running " << ProblemGroupName() << " " << problemgroup << ": " /*<< ProblemName() << " " */ << groups_[problemgroup]->problems_[problem]->Annotation() << std::endl;
		result = groups_[problemgroup]->operator()(problem);
		if (result != 0)
			LogInfo("Possible issue with result %d.\n", result);

	} while (atoi(input.c_str()) != -1);
	return result;
}