#pragma once

#include <string>
#include <vector>

struct Skill
{
	Skill(int e, int t, double n) :
		se(e),
		tc(t),
		ne(n)
	{}
	int se;
	int tc;
	double ne;
};

struct Staff
{
	Staff(std::string n, int c, Skill pm, Skill sa, Skill sd, Skill pg, Skill qa, int ce) :
		name(std::move(n)),
		cost(c),
		skill({pm, sa, sd, pg, qa}),
		communication_eff(ce)
	{
	}
	std::string name;
	int cost;
    std::vector<Skill> skill;
	int communication_eff;
};

class StaffManager
{
public:
	static StaffManager& get_inst()
	{
		static StaffManager inst;
		return inst;
	}
public:
	StaffManager();
	~StaffManager();

	const std::vector<Staff>& get_staff() const { return _staff; }
private:
	std::vector<Staff> _staff;
};

