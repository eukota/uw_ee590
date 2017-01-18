#pragma once
#include <map>

extern int GLOBAL_ARRAY_WIDTH;
extern int GLOBAL_ARRAY_HEIGHT;
extern bool SKIP_VERIFICATION;


struct ResultsStruct
{
	ResultsStruct();
	double WindowsRunTime;
	double OpenCLRunTime;
	bool HasWindowsRunTime;
	bool HasOpenCLRunTime;
};
void PrintResults(const std::vector<ResultsStruct*>& results);

////////////// PROBLEMS
class Problem
{
public:
	Problem(int(*prob)(ResultsStruct*), const std::string& annotation) : problem_(prob), problem_annotation_(annotation) {}
	virtual int operator()(ResultsStruct* res)
	{
		return problem_(res);
	};

	std::string Annotation() { return problem_annotation_; }
private:
	std::string problem_annotation_;
	int(*problem_)(ResultsStruct*);
};

////////////// Groups
class ProblemGroup
{
public:
	ProblemGroup(int groupNum, const std::string& annotation) : problem_group_num_(groupNum), problem_group_annotation_(annotation) {}
	int operator()(int problem);

	std::map<int, Problem*> problems_;
	int GroupNum() { return problem_group_num_; }

	std::string Annotation() { return problem_group_annotation_; }
protected:
	std::string problem_group_annotation_;
	int problem_group_num_;
};


////////////////// MANAGER
class GroupManager
{
public:
	GroupManager(const std::string& name);
	~GroupManager();
	void PrintGroupMenu();
	int Run();
	virtual std::string ProblemGroupName() { return std::string(); }
	virtual std::string ProblemName() { return std::string(); }

protected:
	std::string GroupName;
	std::map<int, ProblemGroup*> groups_;
};

ProblemGroup* GroupManagerInputControlFactory();
int SetValueM(ResultsStruct* results);
int SetValueN(ResultsStruct* results);
int SkipVerify(ResultsStruct* results);
int RunCount(ResultsStruct* results);