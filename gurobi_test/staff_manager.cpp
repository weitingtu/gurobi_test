#include "staff_manager.h"



StaffManager::StaffManager() :
	_staff()
{
	// name, c, pm, sa, sd, pg, qa, ce
	_staff.push_back(Staff("A", 70000, Skill(80, 0, 0.0),   Skill(90,  18, -0.5), Skill(70, 41, -1),   Skill(20,  37, -4),   Skill(60,  41, -2.5), 25));
	_staff.push_back(Staff("B", 64000, Skill(75, 15, -4),   Skill(90,  15, -1),   Skill(80, 53, -2.5), Skill(70,  72, -7),   Skill(80,  24, -2.5), 20));
	_staff.push_back(Staff("C", 65000, Skill(60, 30, -6),   Skill(100,  0,  0),   Skill(80, 20, -1),   Skill(20,  62, -5),   Skill(50,  20, -0.5), 30));
	_staff.push_back(Staff("D", 49000, Skill(30, 35, -5),   Skill(60,  27, -6),   Skill(90, 40, -1.5), Skill(80,  27, -0.5), Skill(45,  35, -4), 10));
	_staff.push_back(Staff("E", 55000, Skill(60, 31, -5.5), Skill(70,  32, -5),   Skill(75, 15, -3),   Skill(90,  38, -1),   Skill(50,  24, -2), 5));
	_staff.push_back(Staff("F", 67000, Skill(20, 69, -7),   Skill(40,  66, -4.5), Skill(90, 24, -1),   Skill(100, 15, 0),    Skill(65,  31, -3), 25));
	_staff.push_back(Staff("G", 42000, Skill(10, 76, -6.5), Skill(60,  58, -4),   Skill(20, 20, -7),   Skill(50,  21, -5.5), Skill(75,  22, -2), 15));
	_staff.push_back(Staff("H", 45000, Skill(10, 84, -6),   Skill(20,  60, -7),   Skill(20, 20, -7.5), Skill(40,  22, -7.5), Skill(85,  40,  0), 50));
	_staff.push_back(Staff("I", 51000, Skill(70, 33, -3.5), Skill(25,  73, -6.5), Skill(10, 47, -8),   Skill(15,  15, -5.5), Skill(100,  0,  0), 20));
	_staff.push_back(Staff("J", 63000, Skill(70, 14, 0.0),  Skill(80,  13, -3),   Skill(80, 50, -6),   Skill(30,  88, -8),   Skill(90,   0,  0), 30));
	_staff.push_back(Staff("K", 56000, Skill(50, 27, -8),   Skill(30,  42, -6.5), Skill(50, 10, -3.5), Skill(95,   0, 0),    Skill(70,   0,  0), 10));
	_staff.push_back(Staff("L", 60000, Skill(65, 42, -4.5), Skill(55,  34, -2.5), Skill(45, 32, -4),   Skill(75,  16, -2.5), Skill(65,  13, -3), 5));
}


StaffManager::~StaffManager()
{
}
