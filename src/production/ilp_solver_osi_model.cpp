#if WITH_OSI == 1

#include "ilp_solver_osi_model.hpp"

#pragma warning(push)
#pragma warning(disable : 4309) // silence warning in CBC concerning truncations of constant values in 64 bit.
#include "OsiSolverInterface.hpp"
#pragma warning(pop)

#include <optional>
#include <cassert>
#include <algorithm>
#include <numeric>

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
        // Returns newly constructed vectors only if there are zeroes in p_values or if no indices are given.
        //     If p_indices == nullptr, constructs a new index array for all non-zeroes in p_values.
        //     Otherwise, only keeps the indices corresponding to non-zero values.
        std::pair<std::optional<std::vector<int>>, std::optional<std::vector<double>>> prune_zeros(const std::vector<double>& p_values, const std::vector<int>* p_indices = nullptr)
        {
            assert ( (p_indices != nullptr) ? p_indices->size() == p_values.size() : true);

            std::pair<std::optional<std::vector<int>>, std::optional<std::vector<double>>> return_value;

            int num_zeros { static_cast<int>(std::count(p_values.begin(), p_values.end(), 0.)) };

            if (num_zeros > 0)
            {
                return_value.first.emplace();
                return_value.second.emplace();

                std::vector<int>&    indices = *return_value.first;
                std::vector<double>& values  = *return_value.second;

                // New size is independent from the existence of an index vector.
                int new_size = static_cast<int>(p_values.size()) - num_zeros;
                indices.reserve(new_size);
                values.reserve (new_size);

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
            }
            else if (!p_indices)
            {
                // Create a full vector of 0 to size - 1.
                return_value.first.emplace(p_values.size(), 0);
                std::vector<int>& indices = *return_value.first;
                std::iota(indices.begin(), indices.end(), 0);
            }

            return return_value;
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
            auto [indices, values] = prune_zeros(*p_row_values, p_row_indices);
            if (indices)
            {
                if (values) vals = values->data();
                else        vals = p_row_values->data();

                rows   = indices->data();
                n_rows = indices->size();
            }
            else
            {
                vals   = p_row_values->data();
                rows   = p_row_indices->data();
                n_rows = p_row_indices->size();
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
            d_cache.addCol(n_rows, rows, vals, p_lower_bound, p_upper_bound, p_objective, nullptr, is_integer_or_binary);
        d_cache_changed = true;
    }

    void ILPSolverOsiModel::prepare_impl()
    {
        auto* solver{ get_solver_osi_model() };
        if (d_cache_changed && (d_cache.numberColumns() > 0 || d_cache.numberRows() > 0))
        {
            solver->loadFromCoinModel(d_cache, true);
            d_cache_changed = false;
        }
    }


    void ILPSolverOsiModel::add_constraint_impl (double p_lower_bound, double p_upper_bound,
        const std::vector<double>& p_col_values, [[maybe_unused]] const std::string& p_name,
        const std::vector<int>* p_col_indices)
    {
        const int*    cols{nullptr};
        int         n_cols{0};
        const double* vals{nullptr};

        // Reduce the vectors, if necessary.
        auto[indices, values] = prune_zeros(p_col_values, p_col_indices);
        if (indices)
        {
            if (values) vals = values->data();
            else        vals = p_col_values.data();

            cols = indices->data();
            n_cols = indices->size();
        }
        else
        {
            vals = p_col_values.data();
            cols = p_col_indices->data();
            n_cols = p_col_indices->size();
        }

#if DO_FORWARD_NAME == true
        if (!p_name.empty())
            d_cache.addRow(n_cols, cols, vals, p_lower_bound, p_upper_bound, p_name.c_str());
        else
#endif
            d_cache.addRow(n_cols, cols, vals, p_lower_bound, p_upper_bound);
        d_cache_changed = true;
    }

    const OsiSolverInterface* ILPSolverOsiModel::const_get_solver_osi_model () const
    {
        return const_cast<ILPSolverOsiModel*>(this)->get_solver_osi_model();
    }
}

#endif
