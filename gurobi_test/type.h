#pragma once

#include <string>

enum class SkillType
{
	NONE = -1,
	PM = 0,
	SA = 1,
	SD = 2,
	PG = 3,
	QA = 4,
	SkillTypeSize = 5
};

static std::string get_skill_type_str(SkillType type)
{
	switch (type)
	{
	case SkillType::NONE:
			return "None";
	case SkillType::PM:
			return "PM";
	case SkillType::SA:
			return "SA";
	case SkillType::SD:
			return "SD";
	case SkillType::PG:
			return "PG";
	case SkillType::QA:
			return "QA";
	default:
		break;
	}

	return "Error";
}
