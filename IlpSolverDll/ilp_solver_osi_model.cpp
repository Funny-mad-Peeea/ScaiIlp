#include "ilp_solver_osi_model.hpp"

#include "OsiSolverInterface.hpp"

using std::string;

// Specify name of variable or constraint only in debug mode:
#ifdef _DEBUG
    #define FORWARD_NAME(name) , name
    #define DO_FORWARD_NAME true
#else
    #define FORWARD_NAME(name)
#endif

namespace ILPSolver
{
    void ILPSolverOsiModel::do_add_variable(std::vector<int> p_row_indices, std::vector<double> p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, string p_name, VariableType p_type)
    {
        assert(p_row_indices.size() == p_row_values.size());

        solver().addCol((int) p_row_indices.size(), p_row_indices.data(), p_row_values.data(), p_lower_bound, p_upper_bound, p_objective FORWARD_NAME(p_name));

        const auto variable_index = solver().getNumCols();

        if (p_type == VariableType::INTEGER)
            solver().setInteger(variable_index);
        else
            solver().setContinuous(variable_index);
    }

    void ILPSolverOsiModel::do_add_constraint(std::vector<int> p_col_indices, std::vector<double> p_col_values, double p_lower_bound, double p_upper_bound, string p_name)
    {
        assert(p_col_indices.size() == p_col_values.size());

        solver().addRow((int) p_col_indices.size(), p_col_indices.data(), p_col_values.data(), p_lower_bound, p_upper_bound);

        const auto constraint_index = solver().getNumRows();

        #ifdef DO_FORWARD_NAME
            solver().setRowName(constraint_index, p_name);
        #endif
    }

    void ILPSolverOsiModel::do_set_objective_sense(ObjectiveSense p_sense)
    {
        solver().setObjSense(p_sense == ILPSolverInterface::ObjectiveSense::MINIMIZE ? 1 : -1);
    }
}
