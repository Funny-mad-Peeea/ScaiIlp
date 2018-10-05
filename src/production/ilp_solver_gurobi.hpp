#ifndef _ILP_SOLVER_GUROBI_HPP
#define _ILP_SOLVER_GUROBI_HPP

//Gurobi is only supported on 64-bit builds.
#if (WITH_GUROBI == 1) && (_WIN64 == 1)

#pragma comment(lib, "gurobi80.lib")

// We need to link against a different library in debug and release mode.
#ifndef NDEBUG
#pragma comment(lib, "gurobi_c++mdd2017.lib")
#else
#pragma comment(lib, "gurobi_c++md2017.lib")
#endif

#include "ilp_solver_interface.hpp"
#include "gurobi_c++.h"

namespace ilp_solver
{
    class ILPSolverGurobi : public ILPSolverInterface
    {
        public:

            void add_variable_boolean   (                                                                                double p_objective,                                             const std::string& p_name = "") override;
            void add_variable_boolean   (                                       const std::vector<double>& p_row_values, double p_objective,                                             const std::string& p_name = "") override;
            void add_variable_boolean   (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective,                                             const std::string& p_name = "") override;

            void add_variable_integer   (                                                                                double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
            void add_variable_integer   (                                       const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
            void add_variable_integer   (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;

            void add_variable_continuous(                                                                                double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
            void add_variable_continuous(                                       const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
            void add_variable_continuous(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;

            void add_constraint         (                                       const std::vector<double>& p_col_values,                     double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
            void add_constraint         (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values,                     double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;

            void add_constraint_upper   (                                       const std::vector<double>& p_col_values,                                           double p_upper_bound, const std::string& p_name = "") override;
            void add_constraint_upper   (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values,                                           double p_upper_bound, const std::string& p_name = "") override;

            void add_constraint_lower   (                                       const std::vector<double>& p_col_values,                     double p_lower_bound,                       const std::string& p_name = "") override;
            void add_constraint_lower   (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values,                     double p_lower_bound,                       const std::string& p_name = "") override;

            void add_constraint_equality(                                       const std::vector<double>& p_col_values, double p_value,                                                 const std::string& p_name = "") override;
            void add_constraint_equality(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_value,                                                 const std::string& p_name = "") override;

            void set_start_solution(const std::vector<double>& p_solution) override;

            void minimize() override;
            void maximize() override;

            const std::vector<double> get_solution()  const override;
            double                    get_objective() const override;
            SolutionStatus            get_status()    const override;

            void set_num_threads       (int p_num_threads)    override;
            void set_deterministic_mode(bool p_deterministic) override;
            void set_log_level         (int p_level)          override;
            void set_max_seconds       (double p_seconds)     override;


            ILPSolverGurobi();
            ~ILPSolverGurobi() = default;
        private:
            GRBEnv d_gurobi_environment{};

            std::vector<GRBConstr> d_rows;
            std::vector<GRBVar>    d_cols;
            GRBModel d_model{d_gurobi_environment};


            void add_constraint_sense(GRBLinExpr&& p_row, double p_bound,                             const std::string& p_name, char p_sense);
            void add_constraint_range(GRBLinExpr&& p_row, double p_lower_bound, double p_upper_bound, const std::string& p_name);

            GRBLinExpr build_row(                                       const std::vector<double>& p_col_values);
            GRBLinExpr build_row(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values);

            void add_variable(                                                                                double p_lower_bound, double p_upper_bound, double p_objective, const std::string& p_name, char p_type);
            void add_variable(                                       const std::vector<double>& p_row_values, double p_lower_bound, double p_upper_bound, double p_objective, const std::string& p_name, char p_type);
            void add_variable(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_lower_bound, double p_upper_bound, double p_objective, const std::string& p_name, char p_type);
    };
}

#endif
#endif
