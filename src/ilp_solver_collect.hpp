#ifndef _ILP_SOLVER_COLLECT_HPP
#define _ILP_SOLVER_COLLECT_HPP

#include "ilp_solver_interface_impl.hpp"

namespace ilp_solver
{
    // Stores all information about the ILP and the solver.
    class ILPSolverCollect : public ILPSolverInterfaceImpl
    {
        private:
            std::vector< std::vector<double> > d_matrix;    // Note: mx0 matrices can be stored, but 0xn matrices cannot.
            std::vector<double>       d_objective;
            std::vector<double>       d_variable_lower, d_variable_upper;
            std::vector<double>       d_constraint_lower, d_constraint_upper;
            std::vector<VariableType> d_variable_type;
            ObjectiveSense d_objective_sense;

            virtual void do_solve(const std::vector< std::vector<double> >& p_matrix,
                                  const std::vector<double>& p_objective,
                                  const std::vector<double>& p_variable_lower,
                                  const std::vector<double>& p_variable_upper,
                                  const std::vector<double>& p_constraint_lower,
                                  const std::vector<double>& p_constraint_upper,
                                  const std::vector<VariableType>& p_variable_type,
                                  ObjectiveSense p_objective_sense,
                                  int p_num_threads,
                                  int p_log_level,
                                  double p_max_seconds) = 0;

            void do_add_variable   (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& /* p_name */, VariableType p_type) override;
            void do_add_constraint (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const std::string& /* p_name */)                                          override;

            void do_set_objective_sense (ObjectiveSense p_sense)                                   override;
            void do_prepare_and_solve   (int p_num_threads, int p_log_level, double p_max_seconds) override;
    };
}

#endif
