#include "ilp_solver_interface.hpp"

#include <cassert>

using std::string;
using std::vector;

namespace ilp_solver
{
    void ILPSolverInterface::add_variable_boolean(double p_objective, const string& p_name)
    {
        add_variable_boolean(vector<int>(), vector<double>(), p_objective, p_name);
    }

    void ILPSolverInterface::add_variable_boolean(const vector<double>& p_row_values, double p_objective, const string& p_name)
    {
        assert(all_row_indices().size() == p_row_values.size());
        add_variable_boolean(all_row_indices(), p_row_values, p_objective, p_name);
    }

    void ILPSolverInterface::add_variable_boolean(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, const string& p_name)
    {
        do_add_variable_and_update_index_vector(p_row_indices, p_row_values, p_objective, 0, 1, p_name, VariableType::INTEGER);
    }
    
    void ILPSolverInterface::add_variable_integer(double p_objective, int p_lower_bound, int p_upper_bound, const string& p_name)
    {
        add_variable_integer(vector<int>(), vector<double>(), p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverInterface::add_variable_integer(const vector<double>& p_row_values, double p_objective, int p_lower_bound, int p_upper_bound, const string& p_name)
    {
        assert(all_row_indices().size() == p_row_values.size());
        add_variable_integer(all_row_indices(), p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverInterface::add_variable_integer(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, int p_lower_bound, int p_upper_bound, const string& p_name)
    {
        do_add_variable_and_update_index_vector(p_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name, VariableType::INTEGER);
    }
    
    void ILPSolverInterface::add_variable_continuous(double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_variable_continuous(vector<int>(), vector<double>(), p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverInterface::add_variable_continuous(const vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        assert(all_row_indices().size() == p_row_values.size());
        add_variable_continuous(all_row_indices(), p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverInterface::add_variable_continuous(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        do_add_variable_and_update_index_vector(p_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name, VariableType::CONTINUOUS);
    }
    
    void ILPSolverInterface::add_constraint(const vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        assert(all_col_indices().size() == p_col_values.size());
        add_constraint(all_col_indices(), p_col_values, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverInterface::add_constraint(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        do_add_constraint_and_update_index_vector(p_col_indices, p_col_values, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverInterface::add_constraint_upper(const vector<double>& p_col_values, double p_upper_bound, const string& p_name)
    {
        assert(all_col_indices().size() == p_col_values.size());
        add_constraint_upper(all_col_indices(), p_col_values, p_upper_bound, p_name);
    }
    
    void ILPSolverInterface::add_constraint_upper(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_upper_bound, const string& p_name)
    {
        if (p_upper_bound >= 0.5*std::numeric_limits<double>::max())    // no restriction
            return;
        do_add_constraint_and_update_index_vector(p_col_indices, p_col_values, -std::numeric_limits<double>::max(), p_upper_bound, p_name);
    }

    void ILPSolverInterface::add_constraint_lower(const vector<double>& p_col_values, double p_lower_bound, const string& p_name)
    {
        assert(all_col_indices().size() == p_col_values.size());
        add_constraint_lower(all_col_indices(), p_col_values, p_lower_bound, p_name);
    }
    
    void ILPSolverInterface::add_constraint_lower(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_lower_bound, const string& p_name)
    {
        if (p_lower_bound <= -0.5*std::numeric_limits<double>::max())   // no restriction
            return;
        do_add_constraint_and_update_index_vector(p_col_indices, p_col_values, p_lower_bound, std::numeric_limits<double>::max(), p_name);
    }

    void ILPSolverInterface::add_constraint_equality(const vector<double>& p_col_values, double p_value, const string& p_name)
    {
        assert(all_col_indices().size() == p_col_values.size());
        add_constraint_equality(all_col_indices(), p_col_values, p_value, p_name);
    }
    
    void ILPSolverInterface::add_constraint_equality(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_value, const string& p_name)
    {
        do_add_constraint_and_update_index_vector(p_col_indices, p_col_values, p_value, p_value, p_name);
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
    
    const vector<double> ILPSolverInterface::get_solution() const
    {
        auto solution = do_get_solution();
        if (solution == nullptr)
            return vector<double>();
        else
            return vector<double>(solution, solution + all_col_indices().size());
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
