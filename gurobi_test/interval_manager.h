#pragma once

#include "type.h"
#include <vector>
#include <string>

struct ItemSkill
{
	ItemSkill() :
		skill(SkillType::NONE),
		number(),
		tolerance_eff()
	{}

	ItemSkill(SkillType s, size_t n, int t) :
		skill(s),
		number(n),
		tolerance_eff(t)
	{}
	SkillType skill;
	size_t number;
	int tolerance_eff;
};

struct Interval {
	Interval(std::string n, int d, int t, std::vector<ItemSkill> i) : 
		item(std::move(n)),
		duration(d),
		tolerance_eff(t),
		item_skill(std::move(i)) 
	{}
	std::string item;
	int duration;
	int tolerance_eff;
	std::vector<ItemSkill> item_skill;
};

class IntervalManager
{
public:
	static IntervalManager& get_inst()
	{
		static IntervalManager inst;
		return inst;
	}
public:
	IntervalManager();
	~IntervalManager();
	const std::vector<Interval>& get_intervals() const { return _intervals; }

private:
	std::vector<Interval> _intervals;
};

