#include "ilp_solver_interface.hpp"

using std::string;
using std::vector;

namespace ilp_solver
{
    void ILPSolverInterface::add_variable_boolean(double p_objective, const string& p_name)
    {
        add_variable_boolean(vector<int>(), vector<double>(), p_objective, p_name);
    }

    void ILPSolverInterface::add_variable_boolean(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, const string& p_name)
    {
        do_add_variable(p_row_indices, p_row_values, p_objective, 0, 1, p_name, VariableType::INTEGER);
    }
    
    void ILPSolverInterface::add_variable_integer(double p_objective, int p_lower_bound, int p_upper_bound, const string& p_name)
    {
        add_variable_integer(vector<int>(), vector<double>(), p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverInterface::add_variable_integer(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, int p_lower_bound, int p_upper_bound, const string& p_name)
    {
        do_add_variable(p_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name, VariableType::INTEGER);
    }
    
    void ILPSolverInterface::add_variable_continuous(double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_variable_continuous(vector<int>(), vector<double>(), p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverInterface::add_variable_continuous(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        do_add_variable(p_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name, VariableType::CONTINUOUS);
    }
    
    void ILPSolverInterface::add_constraint(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        do_add_constraint(p_col_indices, p_col_values, p_lower_bound, p_upper_bound, p_name);
    }
    
    void ILPSolverInterface::add_constraint_upper(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_upper_bound, const string& p_name)
    {
        if (p_upper_bound >= 0.5*std::numeric_limits<double>::max())    // no restriction
            return;
        do_add_constraint(p_col_indices, p_col_values, -std::numeric_limits<double>::max(), p_upper_bound, p_name);
    }
    
    void ILPSolverInterface::add_constraint_lower(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_lower_bound, const string& p_name)
    {
        if (p_lower_bound <= -0.5*std::numeric_limits<double>::max())   // no restriction
            return;
        do_add_constraint(p_col_indices, p_col_values, p_lower_bound, std::numeric_limits<double>::max(), p_name);
    }
    
    void ILPSolverInterface::add_constraint_equality(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_value, const string& p_name)
    {
        do_add_constraint(p_col_indices, p_col_values, p_value, p_value, p_name);
    }
    
    void ILPSolverInterface::minimize()
    {
        do_set_objective_sense(ObjectiveSense::MINIMIZE);
        do_prepare_and_solve();
    }
    
    void ILPSolverInterface::maximize() 
    {
        do_set_objective_sense(ObjectiveSense::MAXIMIZE);
        do_prepare_and_solve();
    }
    
    const double* ILPSolverInterface::get_solution() const
    {
        return do_get_solution();
    }
    
    double ILPSolverInterface::get_objective() const
    {
        return do_get_objective();
    }

    void ILPSolverInterface::set_num_threads(int p_num_threads)
    {
        do_set_num_threads(p_num_threads);
    }
}
