#pragma once
#include <string>
#include <vector>

class Gurobi
{
public:
	Gurobi();
	~Gurobi();

	int run();
private:
	std::string _var_name(std::string str, const std::vector<size_t>& idx) const;
};

