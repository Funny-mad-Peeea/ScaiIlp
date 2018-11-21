#ifndef _ILP_SOLVER_GUROBI_HPP
#define _ILP_SOLVER_GUROBI_HPP

//Gurobi is only supported on 64-bit builds.
#if (WITH_GUROBI == 1) && (_WIN64 == 1)

#pragma comment(lib, "gurobi80.lib")

#include "ilp_solver_impl.hpp"
#include "gurobi_c.h"

namespace ilp_solver
{
    class ILPSolverGurobi : public ILPSolverImpl
    {
        public:
            void set_start_solution(const std::vector<double>& p_solution) override;
            std::vector<double> get_solution()  const override;
            double              get_objective() const override;
            SolutionStatus      get_status()    const override;

            void set_num_threads       (int p_num_threads)    override;
            void set_deterministic_mode(bool p_deterministic) override;
            void set_log_level         (int p_level)          override;
            void set_max_seconds       (double p_seconds)     override;


            ILPSolverGurobi();
            ~ILPSolverGurobi() noexcept;
        private:
            GRBmodel* d_model;

            // The indices of constraints and variables, should just be [0, ... , N - 1] and [0, ... , M - 1].
            std::vector<int> d_rows;
            std::vector<int> d_cols;

            void add_variable_impl (VariableType p_type, double p_objective, double p_lower_bound, double p_upper_bound,
                const std::string& p_name = "", const std::vector<double>* p_row_values = nullptr,
                const std::vector<int>* p_row_indices = nullptr) override;


            void add_constraint_impl (const double* p_lower_bound, const double* p_upper_bound,
                const std::vector<double>& p_col_values, const std::string& p_name = "",
                const std::vector<int>* p_col_indices = nullptr) override;

            void solve_impl() override;
            void set_objective_sense_impl(ObjectiveSense p_sense) override;

            void set_infinity() override;
    };
}

#endif
#endif
