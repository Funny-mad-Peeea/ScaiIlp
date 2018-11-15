#include "ilp_solver_impl.hpp"

#include <cassert>
#include <limits>

using std::string;
using std::vector;

namespace ilp_solver
{
    ILPSolverImpl::ILPSolverImpl()
        : d_start_solution()
        {}

    void ILPSolverImpl::add_variable_boolean(double p_objective, const string& p_name)
    {
        add_variable_boolean(vector<int>(), vector<double>(), p_objective, p_name);
    }

    void ILPSolverImpl::add_variable_boolean(const vector<double>& p_row_values, double p_objective, const string& p_name)
    {
        assert(d_all_row_indices.size() == p_row_values.size());
        add_variable_boolean(d_all_row_indices, p_row_values, p_objective, p_name);
    }

    void ILPSolverImpl::add_variable_boolean(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, const string& p_name)
    {
        add_variable_and_update_index_vector(p_row_indices, p_row_values, p_objective, 0, 1, p_name, VariableType::INTEGER);
    }

    void ILPSolverImpl::add_variable_integer(double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_variable_integer(vector<int>(), vector<double>(), p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverImpl::add_variable_integer(const vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        assert(d_all_row_indices.size() == p_row_values.size());
        add_variable_integer(d_all_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverImpl::add_variable_integer(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_variable_and_update_index_vector(p_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name, VariableType::INTEGER);
    }

    void ILPSolverImpl::add_variable_continuous(double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_variable_continuous(vector<int>(), vector<double>(), p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverImpl::add_variable_continuous(const vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        assert(d_all_row_indices.size() == p_row_values.size());
        add_variable_continuous(d_all_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverImpl::add_variable_continuous(const vector<int>& p_row_indices, const vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        add_variable_and_update_index_vector(p_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name, VariableType::CONTINUOUS);
    }

    void ILPSolverImpl::add_constraint(const vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        assert(d_all_col_indices.size() == p_col_values.size());
        add_constraint(d_all_col_indices, p_col_values, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverImpl::add_constraint(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const string& p_name)
    {
        if ((p_lower_bound <= 0.5*std::numeric_limits<double>::lowest()) && (p_upper_bound >= 0.5*std::numeric_limits<double>::max()))    // no restriction
            return;
        add_constraint_and_update_index_vector(p_col_indices, p_col_values, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverImpl::add_constraint_upper(const vector<double>& p_col_values, double p_upper_bound, const string& p_name)
    {
        assert(d_all_col_indices.size() == p_col_values.size());
        add_constraint_upper(d_all_col_indices, p_col_values, p_upper_bound, p_name);
    }

    void ILPSolverImpl::add_constraint_upper(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_upper_bound, const string& p_name)
    {
        if (p_upper_bound >= 0.5*std::numeric_limits<double>::max())    // no restriction
            return;
        add_constraint_and_update_index_vector(p_col_indices, p_col_values, std::numeric_limits<double>::lowest(), p_upper_bound, p_name);
    }

    void ILPSolverImpl::add_constraint_lower(const vector<double>& p_col_values, double p_lower_bound, const string& p_name)
    {
        assert(d_all_col_indices.size() == p_col_values.size());
        add_constraint_lower(d_all_col_indices, p_col_values, p_lower_bound, p_name);
    }

    void ILPSolverImpl::add_constraint_lower(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_lower_bound, const string& p_name)
    {
        if (p_lower_bound <= 0.5*std::numeric_limits<double>::lowest())   // no restriction
            return;
        add_constraint_and_update_index_vector(p_col_indices, p_col_values, p_lower_bound, std::numeric_limits<double>::max(), p_name);
    }

    void ILPSolverImpl::add_constraint_equality(const vector<double>& p_col_values, double p_value, const string& p_name)
    {
        assert(d_all_col_indices.size() == p_col_values.size());
        add_constraint_equality(d_all_col_indices, p_col_values, p_value, p_name);
    }

    void ILPSolverImpl::add_constraint_equality(const vector<int>& p_col_indices, const vector<double>& p_col_values, double p_value, const string& p_name)
    {
        add_constraint_and_update_index_vector(p_col_indices, p_col_values, p_value, p_value, p_name);
    }

    void ILPSolverImpl::set_start_solution(const std::vector<double>& p_solution)
    {
        d_start_solution = p_solution;
    }

    void ILPSolverImpl::minimize()
    {
        do_set_objective_sense(ObjectiveSense::MINIMIZE);
        solve();
    }

    void ILPSolverImpl::maximize()
    {
        do_set_objective_sense(ObjectiveSense::MAXIMIZE);
        solve();
    }

    const vector<double> ILPSolverImpl::get_solution() const
    {
        auto solution = do_get_solution();
        if (solution == nullptr)
            return vector<double>();
        else
            return vector<double>(solution, solution + d_all_col_indices.size());
    }

    double ILPSolverImpl::get_objective() const
    {
        return do_get_objective();
    }

    SolutionStatus ILPSolverImpl::get_status() const
    {
        return do_get_status();
    }

    void ILPSolverImpl::add_variable_and_update_index_vector(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name, VariableType p_type)
    {
        d_all_col_indices.push_back((int) d_all_col_indices.size());    // update d_all_col_indices
        do_add_variable(p_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name, p_type);
    }

    void ILPSolverImpl::add_constraint_and_update_index_vector(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const std::string& p_name)
    {
        d_all_row_indices.push_back((int) d_all_row_indices.size());    // update d_all_row_indices
        do_add_constraint(p_col_indices, p_col_values, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverImpl::solve()
    {
        do_prepare_and_solve(d_start_solution);
        d_start_solution.clear();
    }

    void ILPSolverImpl::set_default_parameters()
    {
        set_num_threads(c_default_num_threads);
        set_deterministic_mode(c_default_deterministic);
        set_log_level(c_default_log_level);
        set_max_seconds(c_default_max_seconds);
    }

}
