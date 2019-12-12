#include "gurobi.h"
#include <gurobi_c++.h>
#include "staff_manager.h"
#include "interval_manager.h"



Gurobi::Gurobi()
{
}


Gurobi::~Gurobi()
{
}

std::string Gurobi::_var_name(std::string str, const std::vector<size_t>& idx) const
{
	for (size_t i : idx)
	{
		str += "_";
		str += std::to_string(i);
	}
	return str;
}

int Gurobi::run()
{
	try {
		GRBEnv env = GRBEnv();

		GRBModel model = GRBModel(env);

		// Create variables
		bool max_eff = true;
		bool ne = true;
		bool budget = true;
		if (!max_eff && budget)
		{
			printf("error: cannot minimize budget and add budget constraint at the same time\n");
		}

		const std::vector<Staff>& staff = StaffManager::get_inst().get_staff();
	    const std::vector<Interval>& intervals = IntervalManager::get_inst().get_intervals();

		const size_t M = staff.size();
		const size_t I = intervals.size();
		const size_t S = 5;

		std::vector<std::vector<GRBVar>> x(M);
		std::vector<std::vector<std::vector<GRBVar>>> y(M);
		for (size_t m = 0; m < M; ++m)
		{
			x[m].resize(I);
			for (size_t i = 0; i < I; ++i)
			{
				//x[m][i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, _var_name("x", {m, i}));
				x[m][i] = model.addVar(0.0, 1.0, 0.0, GRB_INTEGER, _var_name("x", {m, i}));
			}
		}
		for (size_t m = 0; m < M; ++m)
		{
			y[m].resize(S);
			for (size_t s = 0; s < S; ++s)
			{
				y[m][s].resize(I);
			    for (size_t i = 0; i < I; ++i)
		    	{
				    y[m][s][i] = model.addVar(0.0, 1.0, 0.0, GRB_INTEGER, _var_name("y", {m, s, i}));
			    }
			}
		}

		//for (size_t m = 0; m < M; ++m)
		//{
		//	for (size_t s = 0; s < S; ++s)
		//	{
		//	    for (size_t i = 0; i < I; ++i)
		//    	{
		//		    std::cout << y[m][s][i].get(GRB_StringAttr_VarName) << std::endl;
		//	    }
		//	}
		//}

		// Set objective

		GRBQuadExpr obj;

		//for (size_t m = 0; m < M; ++m)
		//{
		//	for (size_t i = 0; i < I; ++i)
		//	{
		//		obj += staff[m].communication_eff * x[m][i];
		//	}
		//}

		for (size_t m = 0; m < M; ++m)
		{
			for (size_t s = 0; s < S; ++s)
			{
				for (size_t i = 0; i < I; ++i)
				{
					//obj += ( staff[m].skill[s].se + staff[m].skill[s].ne ) * y[m][s][i];
			        const Interval& interval = intervals.at(i);
				    if (interval.item_skill.at(s).number == 0)
			    	{
				    	continue;
				    }
				    const Staff& member = staff.at(m);
					if (ne)
					{
					    obj += ( member.communication_eff + member.skill[s].se + member.skill[s].ne ) * y[m][s][i];
					}
					else
					{
						// don't consider communcation and negative efficiency
						obj += (member.skill[s].se) * y[m][s][i];
					}
				}
			}
		}

		if(max_eff)
		{ 
		    model.setObjective(obj, GRB_MAXIMIZE);
		}

		for (size_t i = 0; i < I; ++i)
		{
			const Interval& interval = intervals.at(i);
		    for (size_t m = 0; m < M; ++m)
	    	{
			    GRBLinExpr constr = 0.0;
		        for (size_t s = 0; s < S; ++s)
		    	{
					constr += y[m][s][i];
				}
			    model.addConstr(constr <= 1);
			}
		}

		for (size_t i = 0; i < I; ++i)
		{
			const Interval& interval = intervals.at(i);
		    for (size_t s = 0; s < S; ++s)
	    	{
				if (interval.item_skill.at(s).number == 0)
				{
					continue;
				}
			    GRBLinExpr constr = 0.0;
		        for (size_t m = 0; m < M; ++m)
		    	{
					constr += y[m][s][i];
				}
			    model.addConstr(constr == (double)interval.item_skill.at(s).number);
			}
		}

		for (size_t i = 0; i < I; ++i)
		{
			const Interval& interval = intervals.at(i);
			GRBLinExpr constr = 0.0;
		    for (size_t m = 0; m < M; ++m)
	    	{
				const Staff& member = staff.at(m);
    			//constr += member.communication_eff * x[m][i];
		        for (size_t s = 0; s < S; ++s)
		    	{
					if (interval.item_skill.at(s).number == 0)
					{
						continue;
					}
					if (ne)
					{
					    constr += (member.communication_eff + member.skill.at(s).se + member.skill.at(s).ne ) * y[m][s][i];
					}
					else
					{
						// don't consider communcation and negative efficiency
					    constr += ( member.skill.at(s).se ) * y[m][s][i];
					}
					//constr += ( member.skill.at(s).se + member.skill.at(s).ne ) * y[m][s][i];
				}
			}
			model.addConstr(constr >= interval.tolerance_eff);
		}

		GRBQuadExpr constr = 0.0;

		for (size_t m = 0; m < M; ++m)
		{
			const Staff& member = staff.at(m);
			GRBLinExpr days = 0;
			for (size_t i = 0; i < I; ++i)
			{
			    const Interval& interval = intervals.at(i);
		        for (size_t s = 0; s < S; ++s)
		    	{
				    days += interval.duration * y[m][s][i];
				}
			}
			constr += member.cost / 30 * days;

		 //   for (size_t s = 0; s < S; ++s)
			//{
			//    for (size_t i = 1; i < I; ++i)
		 //       {
			//        const Interval& interval = intervals.at(i);
			//	    if (interval.item_skill.at(s).number == 0)
			//		{
			//			continue;
			//		}
			//		constr += member.skill.at(s).tc;
			//	}
			//}

		    for (size_t s1 = 0; s1 < S; ++s1)
			{
				for (size_t s2 = 0; s2 < S; ++s2)
				{
					if (s1 == s2)
					{
						continue;
					}
					for (size_t i = 1; i < I; ++i)
					{
						const Interval& interval = intervals.at(i);
						if (interval.item_skill.at(s2).number == 0)
						{
							continue;
						}
						constr += (y[m][s1][i - 1] * y[m][s2][i] * member.skill.at(s2).tc);
					}
				}
			}
		}
		//int B = 4000000;
		int B = (int)(4000000.0 * 1.5);
	    //int B = 5320000;
		if (budget)
		{
			model.addQConstr(constr <= B);
		}
		else if (!max_eff)
		{
			model.setObjective(constr, GRB_MINIMIZE);
		}

		// Optimize model

		model.optimize();

		if (ne)
		{
			printf("Consider communication and negative effective\n");
		}
		else
		{
			printf("Don't Consider communication and negative effective\n");
		}
		if (budget)
		{
			printf("Consider budget\n");
		}
		else
		{
			printf("Don't consider budget\n");
		}
		for (size_t i = 0; i < I; ++i)
		{
			for (size_t s = 0; s < S; ++s)
			{
		        for (size_t m = 0; m < M; ++m)
		    	{
					//if (y[m][s][i].get(GRB_DoubleAttr_X) == 1)
					if (abs(y[m][s][i].get(GRB_DoubleAttr_X) - 1.0) < 0.00001)
					{
				        std::cout << y[m][s][i].get(GRB_StringAttr_VarName) << " " << y[m][s][i].get(GRB_DoubleAttr_X) << std::endl;
					}
			    }
			}
			std::cout << std::endl;
		}

		for (size_t i = 0; i < I; ++i)
		{
			const Interval& interval = intervals.at(i);
			printf("No %zu %s:\n", i + 1, interval.item.c_str());
			for (size_t s = 0; s < S; ++s)
			{
				if (interval.item_skill.at(s).number == 0)
				{
					continue;
				}
				printf("    Skill %s: ", get_skill_type_str(static_cast<SkillType>(s)).c_str());
		        for (size_t m = 0; m < M; ++m)
		    	{
					int count = 0;
					//if (y[m][s][i].get(GRB_DoubleAttr_X) == 1)
					if (abs(y[m][s][i].get(GRB_DoubleAttr_X) - 1.0) < 0.00001)
					{
						if(0 < count)
						{ 
							printf(",");
						}
						++count;
						printf("%zu ", m);
					}
			    }
			}
			std::cout << std::endl;
		}

		std::cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;

		double obj_value = 0.0;
		for (size_t m = 0; m < M; ++m)
		{
			for (size_t s = 0; s < S; ++s)
			{
				for (size_t i = 0; i < I; ++i)
				{
			        const Interval& interval = intervals.at(i);
				    if (interval.item_skill.at(s).number == 0)
			    	{
				    	continue;
				    }
				    const Staff& member = staff.at(m);
					//if (y[m][s][i].get(GRB_DoubleAttr_X) == 1)
					if (abs(y[m][s][i].get(GRB_DoubleAttr_X) - 1.0) < 0.00001)
					{
						if (ne)
						{
							obj_value += (member.communication_eff + member.skill[s].se + member.skill[s].ne);
						}
						else
						{
							// don't consider communcation and negative efficiency
							obj_value += (member.skill[s].se);
						}
					}
				}
			}
		}

		printf("Obj value: %f\n", obj_value);

		double obj_cost = 0.0;

		for (size_t m = 0; m < M; ++m)
		{
			const Staff& member = staff.at(m);
			int days = 0;
			for (size_t i = 0; i < I; ++i)
			{
			    const Interval& interval = intervals.at(i);
		        for (size_t s = 0; s < S; ++s)
		    	{
					//if (y[m][s][i].get(GRB_DoubleAttr_X) == 1)
					if (abs(y[m][s][i].get(GRB_DoubleAttr_X) - 1.0) < 0.00001)
					{
						days += interval.duration;
					}
				}
			}
			obj_cost += member.cost / 30 * days;

		    for (size_t s1 = 0; s1 < S; ++s1)
			{
				for (size_t s2 = 0; s2 < S; ++s2)
				{
					if (s1 == s2)
					{
						continue;
					}
					for (size_t i = 1; i < I; ++i)
					{
						const Interval& interval = intervals.at(i);
						if (interval.item_skill.at(s2).number == 0)
						{
							continue;
						}
						//if (y[m][s1][i - 1].get(GRB_DoubleAttr_X) != 1)
					    if (abs(y[m][s1][i - 1].get(GRB_DoubleAttr_X) - 1.0) >= 0.00001)
						{
							continue;
					    }
						//if (y[m][s2][i].get(GRB_DoubleAttr_X) != 1)
					    if (abs(y[m][s2][i].get(GRB_DoubleAttr_X) - 1.0) >= 0.00001)
						{
							continue;
					    }
						obj_cost += member.skill.at(s2).tc;
					}
				}
			}
		}

		printf("Obj cost: %f\n", obj_cost);
	}
	catch (GRBException e) {
		std::cout << "Error code = " << e.getErrorCode() << std::endl;
		std::cout << e.getMessage() << std::endl;
	}
	catch (...) {
		std::cout << "Exception during optimization" << std::endl;
	}

	return 0;
}
