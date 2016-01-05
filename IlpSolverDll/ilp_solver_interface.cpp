#include "ilp_solver_interface.hpp"

using std::string;
using std::vector;

namespace ILPSolver
{
    void ILPSolverInterface::add_variable_boolean(double p_objective, string p_name)
    {
        add_variable_boolean(vector<int>(), vector<double>(), p_objective, p_name);
    }

    void ILPSolverInterface::add_variable_boolean(vector<int> p_row_indices, vector<double> p_row_values, double p_objective, string p_name)
    {
        do_add_variable(p_row_indices, p_row_values, p_objective, 0, 1, p_name, VariableType::INTEGER);
    }
    
    void ILPSolverInterface::add_variable_integer(double p_objective, double p_lower_bound, double p_upper_bound, string p_name)
    {
        add_variable_integer(vector<int>(), vector<double>(), p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverInterface::add_variable_integer(vector<int> p_row_indices, vector<double> p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, string p_name)
    {
        do_add_variable(p_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name, VariableType::INTEGER);
    }
    
    void ILPSolverInterface::add_variable_continuous(double p_objective, double p_lower_bound, double p_upper_bound, string p_name)
    {
        add_variable_continuous(vector<int>(), vector<double>(), p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverInterface::add_variable_continuous(vector<int> p_row_indices, vector<double> p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, string p_name)
    {
        do_add_variable(p_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name, VariableType::CONTINUOUS);
    }
    
    void ILPSolverInterface::add_constraint(vector<int> p_col_indices, vector<double> p_col_values, double p_lower_bound, double p_upper_bound, string p_name)
    {
        do_add_constraint(p_col_indices, p_col_values, p_lower_bound, p_upper_bound, p_name);
    }
    
    void ILPSolverInterface::add_constraint_upper(vector<int> p_col_indices, vector<double> p_col_values, double p_upper_bound, string p_name)
    {
        do_add_constraint(p_col_indices, p_col_values, -DBL_MAX, p_upper_bound, p_name);
    }
    
    void ILPSolverInterface::add_constraint_lower(vector<int> p_col_indices, vector<double> p_col_values, double p_lower_bound, string p_name)
    {
        do_add_constraint(p_col_indices, p_col_values, p_lower_bound, DBL_MAX, p_name);
    }
    
    void ILPSolverInterface::add_constraint_equality(vector<int> p_col_indices, vector<double> p_col_values, double p_value, string p_name)
    {
        do_add_constraint(p_col_indices, p_col_values, p_value, p_value, p_name);
    }
    
    void ILPSolverInterface::minimize()
    {
        do_set_objective_sense(ObjectiveSense::MINIMIZE);
        do_solve();
    }
    
    void ILPSolverInterface::maximize() 
    {
        do_set_objective_sense(ObjectiveSense::MAXIMIZE);
        do_solve();
    }
    
    const double* ILPSolverInterface::get_solution() const
    {
        return do_get_solution();
    }
    
    double ILPSolverInterface::get_objective() const
    {
        return do_get_objective();
    }
}