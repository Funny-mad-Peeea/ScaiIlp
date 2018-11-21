#if WITH_OSI == 1

#include "ilp_solver_osi_model.hpp"

#include "CoinPackedVector.hpp"


#pragma warning(push)
#pragma warning(disable : 4309) // silence warning in CBC concerning truncations of constant values in 64 bit.
#include "OsiSolverInterface.hpp"
#pragma warning(pop)

#include <cassert>

using std::string;
using std::vector;

// Specify name of variable or constraint only in debug mode:
#ifdef _DEBUG
#define DO_FORWARD_NAME true
#endif

// States whether consecutive elements of each column are contiguous in memory.
// (If not, consecutive elements of each row are contiguous in memory.)
constexpr auto c_column_ordered = false;

constexpr auto c_test_for_duplicate_index = false;

namespace ilp_solver
{
    ILPSolverOsiModel::ILPSolverOsiModel()
    { }

    void ILPSolverOsiModel::add_variable_impl (VariableType p_type, double p_objective, double p_lower_bound, double p_upper_bound,
        [[maybe_unused]] const std::string& p_name, const std::vector<double>* p_row_values,
        const std::vector<int>* p_row_indices)
    {
        auto*         solver{get_solver()};
        CoinPackedVector col;

        if (p_row_values)
        {
            if (p_row_indices)
            {
                assert (p_row_indices->size() == p_row_values->size());
                assert (static_cast<int>(p_row_indices->size()) <= solver->getNumRows());
                col = CoinPackedVector(static_cast<int>(p_row_indices->size()), p_row_indices->data(), p_row_values->data(), true); // do check for duplicate indices
            }
            else
            {
                assert (static_cast<int>(p_row_values->size()) == solver->getNumRows());
                col = CoinPackedVector(solver->getNumRows(), p_row_values->data(), false); // do not check for duplicate indices
            }
        }

        auto [lower, upper] = handle_bounds(&p_lower_bound, &p_upper_bound);

#if DO_FORWARD_NAME == true
        if (!p_name.empty())
            solver->addCol(col, lower, upper, p_objective, p_name);
        else
#endif
            solver->addCol(col, lower, upper, p_objective);

        int index = solver->getNumCols() - 1;
        switch ( p_type )
        {
            case VariableType::BINARY:    [[fallthrough]];
            // OSI does not support a dedicated binary type. A binary variable is just an integer variable with bounds [0,1] or fixed to 0 or 1.
            case VariableType::INTEGER:
                solver->setInteger(index);
            break;

            case VariableType::CONTINUOUS:
                solver->setContinuous(index);
            break;
        }
    }

    void ILPSolverOsiModel::set_infinity()
    {
        auto*         solver{ get_solver() };
        d_pos_infinity =  solver->getInfinity();
        d_neg_infinity = -solver->getInfinity();
    }

    void ILPSolverOsiModel::add_constraint_impl (const double* p_lower_bound, const double* p_upper_bound,
        const std::vector<double>& p_col_values, [[maybe_unused]] const std::string& p_name,
        const std::vector<int>* p_col_indices)
    {
        CoinPackedVector row;
        auto*         solver{ get_solver() };

        if (p_col_indices)
        {
            assert( p_col_values.size()   == p_col_indices->size() );
            assert( static_cast<int>(p_col_indices->size()) <= solver->getNumCols() );
            int n_cols = static_cast<int>(p_col_indices->size());

            row = CoinPackedVector(n_cols, p_col_indices->data(), p_col_values.data(), true); // do check for duplicate indicies
        }
        else
        {
            assert( static_cast<int>(p_col_values.size()) == solver->getNumCols() );
            row = CoinPackedVector(solver->getNumCols(), p_col_values.data(), false); // do not check for duplicate indicies
        }

        auto[lower, upper] = handle_bounds(p_lower_bound, p_upper_bound);
        // If there are no bounds, we do not need to do anything.
        if (lower == d_neg_infinity && upper == d_pos_infinity) return;

#if DO_FORWARD_NAME == true
        if (!p_name.empty())
            solver->addRow(row, lower, upper, p_name);
        else
#endif
            solver->addRow(row, lower, upper);
    }

    std::vector<double> ILPSolverOsiModel::get_solution() const
    {
        auto* solver{ get_solver() };
        const auto* solution_array = solver->getColSolution(); // Returns nullptr if no solution was found.

        if (!solution_array) return std::vector<double>();
        return std::vector<double>(solution_array, solution_array + solver->getNumCols());
    }

    void ILPSolverOsiModel::set_start_solution(const std::vector<double>& p_solution)
    {
        auto* solver{ get_solver() };
        assert( static_cast<int>(p_solution.size()) == solver->getNumCols() );

        solver->setColSolution(p_solution.data());
    }

    double ILPSolverOsiModel::get_objective() const
    {
        auto* solver{ get_solver() };
        return solver->getObjValue();
    }

    SolutionStatus ILPSolverOsiModel::get_status() const
    {
        auto* solver{ get_solver() };

        if ( solver->isProvenOptimal() )
            return SolutionStatus::PROVEN_OPTIMAL;
        if ( solver->isProvenPrimalInfeasible() )
            return SolutionStatus::PROVEN_INFEASIBLE;
        if ( solver->isProvenDualInfeasible() )
            return SolutionStatus::PROVEN_UNBOUNDED;

        const auto* solution_array = solver->getColSolution();
        if (solution_array) return SolutionStatus::SUBOPTIMAL;
        return SolutionStatus::NO_SOLUTION;
    }

    void ILPSolverOsiModel::set_objective_sense_impl(ObjectiveSense p_sense)
    {
        auto* solver{ get_solver() };
        if (p_sense == ObjectiveSense::MINIMIZE )
            solver->setObjSense( 1.);
        else
            solver->setObjSense(-1.);
    }

    void ILPSolverOsiModel::solve_impl()
    {
        auto* solver{ get_solver() };
        solver->branchAndBound();
    }

    const OsiSolverInterface* ILPSolverOsiModel::get_solver () const
    {
        return const_cast<ILPSolverOsiModel*>(this)->get_solver();
    }
}

#endif
