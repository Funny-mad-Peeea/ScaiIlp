#include "ilp_solver_interface_impl.hpp"

using std::vector;

namespace ilp_solver
{
    vector<int> ILPSolverInterfaceImpl::all_col_indices()
    {
        return d_all_col_indices;
    }

    vector<int> ILPSolverInterfaceImpl::all_row_indices()
    {
        return d_all_row_indices;
    }

    void ILPSolverInterfaceImpl::do_add_variable_and_update_index_vector(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name, VariableType p_type)
    {
        d_all_col_indices.push_back((int) d_all_col_indices.size());    // update d_all_col_indices
        do_add_variable(p_row_indices, p_row_values, p_objective, p_lower_bound, p_upper_bound, p_name, p_type);
    }

    void ILPSolverInterfaceImpl::do_add_constraint_and_update_index_vector(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const std::string& p_name)
    {
        d_all_row_indices.push_back((int) d_all_row_indices.size());    // update d_all_row_indices
        do_add_constraint(p_col_indices, p_col_values, p_lower_bound, p_upper_bound, p_name);
    }
}
