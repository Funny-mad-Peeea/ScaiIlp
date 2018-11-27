#if WITH_OSI == 1

#include "ilp_solver_osi_model.hpp"

#include "CoinPackedVector.hpp"


#pragma warning(push)
#pragma warning(disable : 4309) // silence warning in CBC concerning truncations of constant values in 64 bit.
#include "OsiSolverInterface.hpp"
#pragma warning(pop)

#include <optional>
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
    namespace
    {

        // Prune zeros before constructing a coin-packed vector.
        // Returns newly constructed vectors only if there are zeroes in p_values.
        //     If p_indices == nullptr, constructs a new index array for all non-zeroes in p_values.
        //     Otherwise, only keeps the indices corresponding to non-zero values.
        std::optional<std::pair<std::vector<int>, std::vector<double>>> prune_zeros(const std::vector<double>& p_values, const std::vector<int>* p_indices = nullptr)
        {
            assert ( (p_indices != nullptr) ? p_indices->size() == p_values.size() : true);

            std::vector<int>    indices;
            std::vector<double> values;

            int num_zeros { static_cast<int>(std::count(p_values.begin(), p_values.end(), 0.)) };

            if (num_zeros > 0)
            {
                // New size is independent from the existence of an index vector.
                int new_size = static_cast<int>(p_values.size()) - num_zeros;
                indices.reserve( new_size );
                 values.reserve( new_size );

                for (int i = 0; i < static_cast<int>(p_values.size()); i++)
                {
                    auto value = p_values[i];
                    // Construct the new vectors. If we have no indices, use the current index.
                    if (value != 0.)
                    {
                        indices.push_back( (p_indices != nullptr) ? (*p_indices)[i] : i );
                        values.push_back(value);
                    }
                }

                return {std::pair(std::move(indices), std::move(values))};
            }
            // Return a default-constructed (empty) optional.
            return {};
        }

        std::vector<int> global_index_vector;

        void update_global_index_vector(int new_size)
        {
            if (static_cast<int>(global_index_vector.size()) < new_size)
            {
                global_index_vector.reserve(std::max(static_cast<size_t>(new_size), 2 * global_index_vector.capacity()));
                for (int i = static_cast<int>(global_index_vector.size()); i < new_size; i++)
                    global_index_vector.push_back(i);
            }
        }
    }

    int ILPSolverOsiModel::get_num_variables() const
    {
        return d_cache.numberColumns();
    }

    int ILPSolverOsiModel::get_num_constraints() const
    {
        return d_cache.numberRows();
    }

    ILPSolverOsiModel::ILPSolverOsiModel()
    {
    }

    void ILPSolverOsiModel::add_variable_impl (VariableType p_type, double p_objective, double p_lower_bound, double p_upper_bound,
        [[maybe_unused]] const std::string& p_name, const std::vector<double>* p_row_values,
        const std::vector<int>* p_row_indices)
    {
        const int*    rows{nullptr};
        int         n_rows{0};
        const double* vals{nullptr};

        if (p_row_values)
        {
            // Reduce the vectors, if necessary.
            auto optional_reduction = prune_zeros(*p_row_values, p_row_indices);
            if (optional_reduction)
            {
                rows   = optional_reduction->first.data();
                vals   = optional_reduction->second.data();
                n_rows = optional_reduction->first.size();
            }
            else
            {
                vals   = p_row_values->data();
                n_rows = p_row_values->size();
                if (p_row_indices)
                    rows = p_row_indices->data();
                else
                {
                    update_global_index_vector(n_rows);
                    rows = global_index_vector.data();
                }
            }

            assert (n_rows <= get_num_constraints() );
        }

        // OSI has no special case for binary variables.
        bool is_integer_or_binary{ (p_type == VariableType::CONTINUOUS) ? false : true };
#if DO_FORWARD_NAME == true
        if (!p_name.empty())
            d_cache.addCol(n_rows, rows, vals, p_lower_bound, p_upper_bound, p_objective, p_name.c_str(), is_integer_or_binary);
        else
#endif
            d_cache.addCol(n_rows, rows, vals, p_lower_bound, p_upper_bound, p_objective, NULL, is_integer_or_binary);
        d_cache_changed = true;
    }

    void ILPSolverOsiModel::prepare_impl()
    {
        auto* solver{ get_solver_osi() };
        if (d_cache_changed && (d_cache.numberColumns() > 0 || d_cache.numberRows() > 0))
        {
            solver->loadFromCoinModel(d_cache, true);
            d_cache_changed = false;
        }
    }

    std::pair<double, double> ILPSolverOsiModel::get_infinity_impl()
    {
        auto*             solver{ get_solver_osi() };
        auto inf = solver->getInfinity();
        return {-inf, inf};
    }

    void ILPSolverOsiModel::add_constraint_impl (double p_lower_bound, double p_upper_bound,
        const std::vector<double>& p_col_values, [[maybe_unused]] const std::string& p_name,
        const std::vector<int>* p_col_indices)
    {
        const int*    cols{nullptr};
        int         n_cols{0};
        const double* vals{nullptr};

        // Reduce the vectors, if necessary.
        auto optional_reduction{ prune_zeros(p_col_values, p_col_indices) };
        if (optional_reduction)
        {
            cols = optional_reduction->first.data();
            vals = optional_reduction->second.data();
            n_cols = optional_reduction->first.size();
        }
        else
        {
            vals = p_col_values.data();
            n_cols = p_col_values.size();
            if (p_col_indices)
                cols = p_col_indices->data();
            else
            {
                update_global_index_vector(n_cols);
                cols =  global_index_vector.data();
            }
        }

#if DO_FORWARD_NAME == true
        if (!p_name.empty())
            d_cache.addRow(n_cols, cols, vals, p_lower_bound, p_upper_bound, p_name.c_str());
        else
#endif
            d_cache.addRow(n_cols, cols, vals, p_lower_bound, p_upper_bound);
        d_cache_changed = true;
    }

    std::vector<double> ILPSolverOsiModel::get_solution() const
    {
        auto* solver{ get_solver_osi() };
        const auto* solution_array = solver->getColSolution(); // Returns nullptr if no solution was found.

        if (!solution_array) return std::vector<double>();
        return std::vector<double>(solution_array, solution_array + solver->getNumCols());
    }

    void ILPSolverOsiModel::set_start_solution(const std::vector<double>& p_solution)
    {
        assert( static_cast<int>(p_solution.size()) == get_num_variables() );
        auto* solver{ get_solver_osi() };

        solver->setColSolution(p_solution.data());
    }

    double ILPSolverOsiModel::get_objective() const
    {
        auto* solver{ get_solver_osi() };
        return solver->getObjValue();
    }

    SolutionStatus ILPSolverOsiModel::get_status() const
    {
        auto* solver{ get_solver_osi() };

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
        auto* solver{ get_solver_osi() };
        if (p_sense == ObjectiveSense::MINIMIZE )
            solver->setObjSense( 1.);
        else
            solver->setObjSense(-1.);
    }

    void ILPSolverOsiModel::solve_impl()
    {
        auto* solver{ get_solver_osi() };
        solver->branchAndBound();
    }

    const OsiSolverInterface* ILPSolverOsiModel::get_solver_osi () const
    {
        return const_cast<ILPSolverOsiModel*>(this)->get_solver_osi();
    }
}

#endif
