#ifndef _ILP_SOLVER_IMPL_HPP
#define _ILP_SOLVER_IMPL_HPP

#include "ilp_solver_interface.hpp"


// The implementation serves to avoid redundant code duplication.
// When inheriting from ILPSolverImpl, you only need to implement the following functions:
//     add_variable_impl
//     add_constraint_impl
//     solve_impl
//     set_objective_sense
//     set_start_solution
//     get_solution
//     get_objective
//     get_status
//     set_num_threads
//     set_deterministic_mode
//     set_log_level
//     set_max_seconds
//     set_infinity
namespace ilp_solver
{
    enum class VariableType   { INTEGER, CONTINUOUS, BINARY };
    enum class ObjectiveSense { MINIMIZE, MAXIMIZE };

    class ILPSolverImpl : public ILPSolverInterface
    {
        public:
            void add_variable_boolean    (                                                                                double p_objective,                                             const std::string& p_name = "") override;
            void add_variable_boolean    (                                       const std::vector<double>& p_row_values, double p_objective,                                             const std::string& p_name = "") override;
            void add_variable_boolean    (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective,                                             const std::string& p_name = "") override;
            void add_variable_integer    (                                                                                double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
            void add_variable_integer    (                                       const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
            void add_variable_integer    (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
            void add_variable_continuous (                                                                                double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
            void add_variable_continuous (                                       const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;
            void add_variable_continuous (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name = "") override;

            void add_constraint          (                                       const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound,                     const std::string& p_name = "") override;
            void add_constraint          (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound,                     const std::string& p_name = "") override;
            void add_constraint_upper    (                                       const std::vector<double>& p_col_values,                       double p_upper_bound,                     const std::string& p_name = "") override;
            void add_constraint_upper    (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values,                       double p_upper_bound,                     const std::string& p_name = "") override;
            void add_constraint_lower    (                                       const std::vector<double>& p_col_values, double p_lower_bound,                                           const std::string& p_name = "") override;
            void add_constraint_lower    (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound,                                           const std::string& p_name = "") override;
            void add_constraint_equality (                                       const std::vector<double>& p_col_values,                                              double p_value,    const std::string& p_name = "") override;
            void add_constraint_equality (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values,                                              double p_value,    const std::string& p_name = "") override;

            void minimize() override;
            void maximize() override;

        protected:
            void set_default_parameters();
            std::pair<double, double> handle_bounds(const double* p_lower_bound, const double* p_upper_bound);

            double d_pos_infinity;
            double d_neg_infinity;

        private:
            virtual void set_infinity();

            virtual void add_variable_impl (VariableType p_type, double p_objective, double p_lower_bound, double p_upper_bound,
                                            const std::string& p_name = "", const std::vector<double>* p_row_values = nullptr,
                                            const std::vector<int>* p_row_indices = nullptr) = 0;


            virtual void add_constraint_impl (const double* p_lower_bound, const double* p_upper_bound,
                                              const std::vector<double>& p_col_values, const std::string& p_name = "",
                                              const std::vector<int>* p_col_indices = nullptr) = 0;

            virtual void solve_impl() = 0;
            virtual void set_objective_sense(ObjectiveSense p_sense) = 0;
    };
}

#endif
