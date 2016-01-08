#ifndef _ILP_SOLVER_INTERFACE_HPP
#define _ILP_SOLVER_INTERFACE_HPP

#include <string>
#include <vector>

#include "IlpSolverDll.hpp"

namespace ilp_solver
{
    class ILP_SOLVER_DLL_API ILPSolverInterface
    {
        public:
            void            add_variable_boolean    (                                                                                double p_objective,                                             std::string p_name = "");
            void            add_variable_boolean    (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective,                                             std::string p_name = "");
            void            add_variable_integer    (                                                                                double p_objective, int    p_lower_bound, int    p_upper_bound, std::string p_name = "");
            void            add_variable_integer    (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, int    p_lower_bound, int    p_upper_bound, std::string p_name = "");
            void            add_variable_continuous (                                                                                double p_objective, double p_lower_bound, double p_upper_bound, std::string p_name = "");
            void            add_variable_continuous (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, std::string p_name = "");
    
            void            add_constraint          (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound,                      std::string p_name = "");   // l <= a*x <= r
            void            add_constraint_upper    (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values,                       double p_upper_bound,                      std::string p_name = "");   //      a*x <= r
            void            add_constraint_lower    (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound,                                            std::string p_name = "");   // l <= a*x
            void            add_constraint_equality (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values,                                              double p_value,     std::string p_name = "");   //      a*x = v
    
            void            minimize                ();
            void            maximize                ();
    
            const double*   get_solution            () const;
            double          get_objective           () const;

            void            set_num_threads         (int p_num_threads);
    
            virtual         ~ILPSolverInterface     () {}

        protected:
            enum class VariableType     {INTEGER, CONTINUOUS};
            enum class ObjectiveSense   {MINIMIZE, MAXIMIZE};

        private:
            virtual void            do_add_variable             (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, std::string p_name, VariableType p_type) = 0;
            virtual void            do_add_constraint           (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, std::string p_name) = 0;
            virtual void            do_set_objective_sense      (ObjectiveSense p_sense) = 0;
            virtual void            do_prepare_and_solve        () = 0;
            virtual const double*   do_get_solution             () const = 0;
            virtual double          do_get_objective            () const = 0;
            virtual void            do_set_num_threads          (int p_num_threads) = 0;
    };
}

#endif
