#include "ilp_solver_impl.hpp"

#include <cassert>
#include <limits>

using std::string;
using std::vector;

namespace ilp_solver
{
    void ILPSolverImpl::add_variable_boolean(double p_objective, const string& p_name)
    {
        add_variable_impl (VariableType::BINARY, p_objective, 0., 1., p_name);
    }

    void ILPSolverImpl::add_variable_boolean(const vector<double>& p_row_values, double p_objective, const string& p_name)
    {
        add_variable_impl (VariableType::BINARY, p_objective, 0., 1., p_name, &p_row_values);
    }

    void ILPSolverImpl::add_variable_boolean(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, const string& p_name)
    {
        assert(p_row_values.size() >= p_row_indices.size());
        add_variable_impl (VariableType::BINARY, p_objective, 0., 1., p_name, &p_row_values, &p_row_indices);
    }

    void ILPSolverImpl::add_variable_integer(double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_variable_impl (VariableType::INTEGER, p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverImpl::add_variable_integer(const vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_variable_impl (VariableType::INTEGER, p_objective, p_lower_bound, p_upper_bound, p_name, &p_row_values);
    }

    void ILPSolverImpl::add_variable_integer(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        assert(p_row_values.size() >= p_row_indices.size());
        add_variable_impl (VariableType::INTEGER, p_objective, p_lower_bound, p_upper_bound, p_name, &p_row_values, &p_row_indices);
    }

    void ILPSolverImpl::add_variable_continuous(double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_variable_impl (VariableType::CONTINUOUS, p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverImpl::add_variable_continuous(const vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_variable_impl (VariableType::CONTINUOUS, p_objective, p_lower_bound, p_upper_bound, p_name, &p_row_values);
    }

    void ILPSolverImpl::add_variable_continuous(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        assert(p_row_values.size() >= p_row_indices.size());
        add_variable_impl (VariableType::CONTINUOUS, p_objective, p_lower_bound, p_upper_bound, p_name, &p_row_values, &p_row_indices);
    }

    void ILPSolverImpl::add_constraint(const vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_constraint_impl (&p_lower_bound, &p_upper_bound, p_col_values, p_name);
    }

    void ILPSolverImpl::add_constraint(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_constraint_impl (&p_lower_bound, &p_upper_bound, p_col_values, p_name, &p_col_indices);
    }

    void ILPSolverImpl::add_constraint_upper(const vector<double>& p_col_values, double p_upper_bound, const string& p_name)
    {
        add_constraint_impl (nullptr, &p_upper_bound, p_col_values, p_name);
    }

    void ILPSolverImpl::add_constraint_upper(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_upper_bound, const string& p_name)
    {
        add_constraint_impl (nullptr, &p_upper_bound, p_col_values, p_name, &p_col_indices);
    }

    void ILPSolverImpl::add_constraint_lower(const vector<double>& p_col_values, double p_lower_bound, const string& p_name)
    {
        add_constraint_impl (&p_lower_bound, nullptr, p_col_values, p_name);
    }

    void ILPSolverImpl::add_constraint_lower(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_lower_bound, const string& p_name)
    {
        add_constraint_impl (&p_lower_bound, nullptr, p_col_values, p_name, &p_col_indices);
    }

    void ILPSolverImpl::add_constraint_equality(const vector<double>& p_col_values, double p_value, const string& p_name)
    {
        add_constraint_impl (&p_value, &p_value, p_col_values, p_name);
    }

    void ILPSolverImpl::add_constraint_equality(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_value, const string& p_name)
    {
        add_constraint_impl (&p_value, &p_value, p_col_values, p_name, &p_col_indices);
    }

    void ILPSolverImpl::set_default_parameters()
    {
        set_num_threads(c_default_num_threads);
        set_deterministic_mode(c_default_deterministic);
        set_log_level(c_default_log_level);
        set_max_seconds(c_default_max_seconds);
    }

    void ILPSolverImpl::minimize()
    {
        set_objective_sense(ObjectiveSense::MINIMIZE);
        solve_impl();
    }

    void ILPSolverImpl::maximize()
    {
        set_objective_sense(ObjectiveSense::MAXIMIZE);
        solve_impl();
    }
}
