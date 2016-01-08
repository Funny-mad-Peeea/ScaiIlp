#ifndef _ILP_SOLVER_OSI_MODEL_HPP
#define _ILP_SOLVER_OSI_MODEL_HPP

#include "ilp_solver_interface.hpp"

#include "CoinPackedMatrix.hpp"

#include <string>
#include <vector>

class OsiSolverInterface;

namespace ILPSolver
{
    // Implements all methods from ILPSolverInterface that can be realized
    // via the model of the OsiSolverInterface.
    class ILPSolverOsiModel : public ILPSolverInterface
    {
        public:
            ILPSolverOsiModel();

        private:
            CoinPackedMatrix d_matrix;
            std::vector<double> d_objective;
            std::vector<double> d_variable_lower, d_variable_upper;
            std::vector<double> d_constraint_lower, d_constraint_upper;
            std::vector<VariableType> d_variable_type;
            std::vector<std::string> d_variable_name;
            std::vector<std::string> d_constraint_name;

            void    do_add_variable         (const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, std::string p_name, VariableType p_type) override;
            void    do_add_constraint       (const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, std::string p_name) override;
            void    do_set_objective_sense  (ObjectiveSense p_sense) override;

            void    do_prepare_and_solve    () override;

            void    prepare                 ();
    
            virtual OsiSolverInterface*         solver() = 0;
            virtual const OsiSolverInterface*   solver() const = 0;
            virtual void                        do_solve() = 0;     // not always implemented as solver().branchAndBound() (see ILPSolverCbc)
    };
}

#endif
