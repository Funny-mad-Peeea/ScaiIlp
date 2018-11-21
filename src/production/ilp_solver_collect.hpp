#ifndef _ILP_SOLVER_COLLECT_HPP
#define _ILP_SOLVER_COLLECT_HPP

#include "ilp_data.hpp"
#include "ilp_solver_impl.hpp"

namespace ilp_solver
{
    // Stores all information about the ILP and the solver.
    // Is used to create a stub.
    class ILPSolverCollect : public ILPSolverImpl
    {
        protected:
            ILPData d_ilp_data;
        private:

            void add_variable_impl (VariableType p_type, double p_objective, double p_lower_bound, double p_upper_bound,
                const std::string& p_name = "", const std::vector<double>* p_row_values = nullptr,
                const std::vector<int>* p_row_indices = nullptr) override;

            void add_constraint_impl (const double* p_lower_bound, const double* p_upper_bound,
                const std::vector<double>& p_col_values, const std::string& p_name = "",
                const std::vector<int>* p_col_indices = nullptr) override;
            void set_objective_sense_impl(ObjectiveSense p_sense) override;

            void set_start_solution     (const std::vector<double>& p_solution) override;

            void set_num_threads        (int p_num_threads)    override;
            void set_deterministic_mode (bool p_deterministic) override;
            void set_log_level          (int p_level)          override;
            void set_max_seconds        (double p_seconds)     override;
    };
}

#endif
