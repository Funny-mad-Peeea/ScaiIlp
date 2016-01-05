#ifndef _ILP_SOLVER_OSI_MODEL_HPP
#define _ILP_SOLVER_OSI_MODEL_HPP

#include "ilp_solver_interface.hpp"

#include <string>
#include <vector>

class OsiSolverInterface;

namespace ILPSolver
{
    // Implements all methods from ILPSolverInterface that can be realized
    // via the model of the OsiSolverInterface.
    class ILPSolverOsiModel : public ILPSolverInterface
    {
        private:
            void    do_add_variable         (std::vector<int> p_row_indices, std::vector<double> p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, std::string p_name, VariableType p_type) override;
            void    do_add_constraint       (std::vector<int> p_col_indices, std::vector<double> p_col_values, double p_lower_bound, double p_upper_bound, std::string p_name) override;
            void    do_set_objective_sense  (ObjectiveSense p_sense) override;
    
            virtual OsiSolverInterface&         solver() = 0;
            virtual const OsiSolverInterface&   solver() const = 0;
    };
}

#endif
