#pragma once

#include "ilp_solver_interface.hpp"


namespace ilp_solver
{
    // Necessary forward declarations of structs and typedefs.
    struct  Scip;
    typedef Scip SCIP;
    struct  SCIP_Cons;
    typedef SCIP_Cons SCIP_CONS;
    struct  SCIP_Var;
    typedef SCIP_Var SCIP_VAR;
    enum    SCIP_Vartype : int;
    typedef SCIP_Vartype SCIP_VARTYPE;

    class ILPSolverSCIP : public ILPSolverInterface
    {
    public:

        void add_variable_boolean(double p_objective, const std::string& p_name = "") override;
        void add_variable_boolean(const std::vector<double>& p_row_values, double p_objective, const std::string& p_name = "") override;
        void add_variable_boolean(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, const std::string& p_name = "") override;

        void add_variable_integer(double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
        void add_variable_integer(const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
        void add_variable_integer(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;

        void add_variable_continuous(double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
        void add_variable_continuous(const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
        void add_variable_continuous(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;

        void add_constraint(const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
        void add_constraint(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;

        void add_constraint_upper(const std::vector<double>& p_col_values, double p_upper_bound, const std::string& p_name = "") override;
        void add_constraint_upper(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_upper_bound, const std::string& p_name = "") override;

        void add_constraint_lower(const std::vector<double>& p_col_values, double p_lower_bound, const std::string& p_name = "") override;
        void add_constraint_lower(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, const std::string& p_name = "") override;

        void add_constraint_equality(const std::vector<double>& p_col_values, double p_value, const std::string& p_name = "") override;
        void add_constraint_equality(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_value, const std::string& p_name = "") override;

        void set_start_solution(const std::vector<double>& p_solution) override;

        void minimize() override;
        void maximize() override;

        const std::vector<double> get_solution()  const override;
        double                    get_objective() const override;
        SolutionStatus            get_status()    const override;

        void set_num_threads(int p_num_threads)           override;
        void set_deterministic_mode(bool p_deterministic) override;
        void set_log_level(int p_level)                   override;
        void set_max_seconds(double p_seconds)            override;


        ILPSolverSCIP();
        ~ILPSolverSCIP();
    private:
        SCIP* d_scip;

        std::vector<SCIP_CONS*>   d_rows;
        std::vector<SCIP_VAR*>    d_cols;

        void add_variable(const std::vector<int>* p_row_indices, const std::vector<double>* p_row_values, double p_lower_bound, double p_upper_bound, double p_objective, SCIP_VARTYPE p_type, const std::string& p_name);
        void add_constraint_intern(const std::vector<int>* p_col_indices, const std::vector<double>& p_col_values, double p_lhs, double p_rhs, const std::string& p_name);
    };
}
