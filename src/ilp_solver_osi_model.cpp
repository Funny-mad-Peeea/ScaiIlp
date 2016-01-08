#include "ilp_solver_osi_model.hpp"

#include "CoinPackedVector.hpp"
#include "OsiSolverInterface.hpp"

using std::string;

// Specify name of variable or constraint only in debug mode:
#ifdef _DEBUG
    #define FORWARD_NAME(name) , name
    #define DO_FORWARD_NAME true
#else
    #define FORWARD_NAME(name)
#endif

// States whether consecutive elements of each column are contiguous in memory.
// (If not, consecutive elements of each row are contiguous in memory.)
const auto column_ordered = false;

const auto test_for_duplicate_index = false;

namespace ILPSolver
{
    ILPSolverOsiModel::ILPSolverOsiModel() : d_matrix(column_ordered, 0, 0)
    {
        d_matrix.setDimensions(0, 0);
    }

    void ILPSolverOsiModel::do_add_variable(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, string p_name, VariableType p_type)
    {
        const auto num_indices = (int) p_row_indices.size();
        assert((int) p_row_values.size() == num_indices);

        const auto column = CoinPackedVector(num_indices, p_row_indices.data(), p_row_values.data(), test_for_duplicate_index);

        d_matrix.appendCol(column);
        d_objective.push_back(p_objective);
        d_variable_lower.push_back(p_lower_bound);
        d_variable_upper.push_back(p_upper_bound);
        d_variable_type.push_back(p_type);

        #ifdef DO_FORWARD_NAME
            d_variable_name.push_back(p_name);
        #endif
    }

    void ILPSolverOsiModel::do_add_constraint(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, string p_name)
    {
        const auto num_indices = (int) p_col_indices.size();
        assert((int) p_col_values.size() == num_indices);

        const auto row = CoinPackedVector(num_indices, p_col_indices.data(), p_col_values.data(), test_for_duplicate_index);

        d_matrix.appendRow(row);
        d_constraint_lower.push_back(p_lower_bound);
        d_constraint_upper.push_back(p_upper_bound);

        #ifdef DO_FORWARD_NAME
            d_constraint_name.push_back(p_name);
        #endif
    }

    void ILPSolverOsiModel::do_set_objective_sense(ObjectiveSense p_sense)
    {
        solver().setObjSense(p_sense == ILPSolverInterface::ObjectiveSense::MINIMIZE ? 1 : -1);
    }

    void ILPSolverOsiModel::do_prepare_and_solve()
    {
        prepare();        
        do_solve();
    }

    void ILPSolverOsiModel::prepare()
    {
        auto& ilp_solver = solver();

        // Add variables and constraints
        ilp_solver.loadProblem(d_matrix,
                               d_variable_lower.data(),
                               d_variable_upper.data(),
                               d_objective.data(),
                               d_constraint_lower.data(),
                               d_constraint_upper.data());

        // Set variable types
        for (auto i = 0; i < (int) d_variable_type.size(); ++i)
        {
            if (d_variable_type[i] == VariableType::INTEGER)
                ilp_solver.setInteger(i);
            else
                ilp_solver.setContinuous(i);
        }

        #ifdef DO_FORWARD_NAME
            // Set variable names
            for (auto i = 0; i < (int) d_variable_name.size(); ++i)
                ilp_solver.setColName(i, d_variable_name[i]);
    
            // Set constraint names
            for (auto j = 0; j < (int) d_constraint_name.size(); ++j)
                ilp_solver.setRowName(j, d_constraint_name[j]);
        #endif

        // Deactivate solvers console output
        ilp_solver.setHintParam(OsiDoReducePrint, 1);
    }
}
