#include "interval_manager.h"



IntervalManager::IntervalManager()
{
	_intervals.push_back(Interval("Project Kick Off", 10, 70, { ItemSkill(SkillType::PM, 1, 70), ItemSkill(), ItemSkill(), ItemSkill(), ItemSkill() }));
	_intervals.push_back(Interval("Schedule Report", 15, 50, { ItemSkill(SkillType::PM, 1, 50), ItemSkill(), ItemSkill(), ItemSkill(), ItemSkill() }));
	_intervals.push_back(Interval("System Analysis", 60, 300, { ItemSkill(), ItemSkill(SkillType::SA, 4, 300), ItemSkill(), ItemSkill(), ItemSkill() }));
	_intervals.push_back(Interval("System Design", 90, 310, { ItemSkill(), ItemSkill(SkillType::SA, 2, 70), ItemSkill(SkillType::SD, 2, 85), ItemSkill(), ItemSkill() }));
	_intervals.push_back(Interval("System Development", 210, 760, { ItemSkill(), ItemSkill(SkillType::SA, 4, 65), ItemSkill(SkillType::SD, 2, 80), ItemSkill(SkillType::PG, 4, 85), ItemSkill() }));
	_intervals.push_back(Interval("Test and Verification", 60, 460, { ItemSkill(), ItemSkill(SkillType::SA, 2, 60), ItemSkill(), ItemSkill(), ItemSkill(SkillType::QA, 4, 85) }));
	_intervals.push_back(Interval("On Product Preparation", 45, 350, { ItemSkill(SkillType::PM, 1, 50), ItemSkill(SkillType::SA, 4, 75), ItemSkill(), ItemSkill(), ItemSkill() }));
}


IntervalManager::~IntervalManager()
{
}
