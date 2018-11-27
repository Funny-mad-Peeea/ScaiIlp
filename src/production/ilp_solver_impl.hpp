#ifndef _ILP_SOLVER_IMPL_HPP
#define _ILP_SOLVER_IMPL_HPP

#include "ilp_solver_interface.hpp"


// The implementation serves to avoid redundant code duplication.
// When inheriting from ILPSolverImpl, you only need to implement the following functions:
//     add_variable_impl
//     add_constraint_impl
//     solve_impl
//     set_objective_sense_impl
//     set_start_solution
//     get_solution
//     get_objective
//     get_status
//     set_num_threads
//     set_deterministic_mode
//     set_log_level
//     set_max_seconds
//     set_infinity
//     get_num_constraints
//     get_num_variables
// If there is any action you need to be done after constructing the problem, but before solving it,
// (e.g. integrating a cache) you may also want to implement
//     prepare_impl
#include <iostream>
namespace ilp_solver
{
    enum class VariableType   { INTEGER, CONTINUOUS, BINARY };
    enum class ObjectiveSense { MINIMIZE, MAXIMIZE };


    // You may call this free function in the constructor of your fully implemented class.
    // Can not be called in the constructor of ILPSolverImpl since the functions are not yet implemented.
    static inline void set_default_parameters(ILPSolverInterface* p_solver)
    {
        p_solver->set_num_threads(c_default_num_threads);
        p_solver->set_deterministic_mode(c_default_deterministic);
        p_solver->set_log_level(c_default_log_level);
        p_solver->set_max_seconds(c_default_max_seconds);
    }

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
            ILPSolverImpl()
            { }


        private:
            virtual void prepare_impl();
            virtual std::pair<double, double> get_infinity_impl() = 0;

            virtual void add_variable_impl (VariableType p_type, double p_objective, double p_lower_bound, double p_upper_bound,
                                            const std::string& p_name = "", const std::vector<double>* p_row_values = nullptr,
                                            const std::vector<int>* p_row_indices = nullptr) = 0;


            virtual void add_constraint_impl (double p_lower_bound, double p_upper_bound,
                                              const std::vector<double>& p_col_values, const std::string& p_name = "",
                                              const std::vector<int>* p_col_indices = nullptr) = 0;

            virtual void solve_impl() = 0;
            virtual void set_objective_sense_impl(ObjectiveSense p_sense) = 0;


            std::pair<double, double> handle_bounds(double p_lower_bound, double p_upper_bound);
    };
}

#endif
