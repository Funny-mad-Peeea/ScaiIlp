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
        const int*    rows{nullptr};
        const double* vals{nullptr};
        int         n_rows{0};

        if (p_row_values)
        {
            if (!p_row_indices) p_row_indices = &d_rows;
            assert (p_row_values->size()  >= p_row_indices->size());
            assert (p_row_indices->size() <= d_rows.size());

            n_rows = static_cast<int>(p_row_indices->size());
            rows   = p_row_indices->data();
            vals   = p_row_values->data();
        }

#if DO_FORWARD_NAME == true
        if (!p_name.empty())
            solver->addCol(n_rows, rows, vals, p_lower_bound, p_upper_bound, p_objective, p_name);
        else
#endif
            solver->addCol(n_rows, rows, vals, p_lower_bound, p_upper_bound, p_objective);

        d_cols.push_back(d_cols.size());
        switch ( p_type )
        {
            case VariableType::BINARY:    [[fallthrough]];
            // OSI does not support a dedicated binary type. A binary variable is just an integer variable with bounds [0,1] or fixed to 0 or 1.
            case VariableType::INTEGER:
                solver->setInteger(d_cols.back());
            break;

            case VariableType::CONTINUOUS:
                solver->setContinuous(d_cols.back());
            break;
        }
    }

    void ILPSolverOsiModel::add_constraint_impl (const double* p_lower_bound, const double* p_upper_bound,
        const std::vector<double>& p_col_values, [[maybe_unused]] const std::string& p_name,
        const std::vector<int>* p_col_indices)
    {
        auto*         solver{ get_solver() };
        const int*    cols{ nullptr };
        const double* vals{ p_col_values.data() };
        int         n_cols{ 0 };

        const double max{  solver->getInfinity() };
        const double min{ -solver->getInfinity() };

        if (!p_col_indices) p_col_indices = &d_cols;

        assert( p_col_values.size()   >= p_col_indices->size() );
        assert( p_col_indices->size() <= d_cols.size() );

        n_cols = static_cast<int>(p_col_indices->size());
        cols   = p_col_indices->data();

        double lower = (p_lower_bound) ? *p_lower_bound : min;
               lower = (lower <= 0.5 * min) ? min : lower;
        double upper = (p_upper_bound) ? *p_upper_bound : max;
               upper = (upper >= 0.5 * max) ? max : upper;

        if (lower == min && upper == max) return;

        solver->addRow(n_cols, cols, vals, lower, upper);

        d_rows.push_back(d_rows.size());
#if DO_FORWARD_NAME == true
            if (!p_name.empty()) solver->setRowName(d_rows.back(), p_name);
#endif
    }

    std::vector<double> ILPSolverOsiModel::get_solution() const
    {
        auto* solver{ get_solver() };
        const auto* solution_array = solver->getColSolution();

        if (!solution_array) return std::vector<double>();
        return std::vector<double>(solution_array, solution_array + solver->getNumCols());
    }

    void ILPSolverOsiModel::set_start_solution(const std::vector<double>& p_solution)
    {
        auto* solver{ get_solver() };
        assert( static_cast<int>(p_solution.size()) >= solver->getNumCols() );

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
        if ( solver->isProvenPrimalInfeasible() || solver->isProvenDualInfeasible() )
            return SolutionStatus::PROVEN_INFEASIBLE;

        const auto* solution_array = solver->getColSolution();
        if (solution_array) return SolutionStatus::SUBOPTIMAL;
        return SolutionStatus::NO_SOLUTION;
    }

    void ILPSolverOsiModel::set_objective_sense(ObjectiveSense p_sense)
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
