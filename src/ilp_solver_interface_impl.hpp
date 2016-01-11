#ifndef _ILP_SOLVER_INTERFACE_IMPL_HPP
#define _ILP_SOLVER_INTERFACE_IMPL_HPP

#include "ilp_solver_interface.hpp"

namespace ilp_solver
{
    class ILPSolverInterfaceImpl : public ILPSolverInterface
    {
    private:
        std::vector<int> d_all_col_indices;
        std::vector<int> d_all_row_indices;

        std::vector<int> all_col_indices() override;
        std::vector<int> all_row_indices() override;

        void             do_add_variable_and_update_index_vector    (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name, VariableType p_type) override;
        void             do_add_constraint_and_update_index_vector  (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const std::string& p_name) override;
    };
}

#endif
