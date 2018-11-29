#include "ilp_solver_collect.hpp"

#include <cassert>

using std::string;
using std::vector;
typedef vector< vector<double> > Matrix;

namespace ilp_solver
{
    static void append_column(Matrix* v_matrix, const vector<double>& p_row_values)
    {
        if (v_matrix->empty())
            return;

        // set specified values
        for (int i = 0; i < static_cast<int>(v_matrix->size()); i++)
            (*v_matrix)[i].push_back(p_row_values[i]);
    }


    static void append_column(Matrix* v_matrix, const vector<int>& p_row_indices, const vector<double>& p_row_values)
    {
        if (v_matrix->empty())
            return;

        // enlarge matrix
        for (auto& row: *v_matrix)
            row.push_back(0.0);

        // set specified values
        for (auto i = 0; i < (int) p_row_indices.size(); ++i)
        {
            const auto row_index = p_row_indices[i];
            const auto value = p_row_values[i];
            (*v_matrix)[row_index].back() = value;
        }
    }


    static void append_row(Matrix* v_matrix, const vector<double>& p_col_values)
    {
        // enlarge matrix
        v_matrix->emplace_back(p_col_values);
    }


    static void append_row(Matrix* v_matrix, int p_num_cols, const vector<int>& p_col_indices, const vector<double>& p_col_values)
    {
        // enlarge matrix
        v_matrix->emplace_back(p_num_cols, 0.0);

        // set specified values
        auto& row = v_matrix->back();
        for (auto i = 0; i < (int) p_col_indices.size(); ++i)
        {
            const auto col_index = p_col_indices[i];
            const auto value = p_col_values[i];
            row[col_index] = value;
        }
    }


    int ILPSolverCollect::get_num_constraints() const
    {
        return static_cast<int>(d_ilp_data.constraint_lower.size());
    }


    int ILPSolverCollect::get_num_variables()   const
    {
        return static_cast<int>(d_ilp_data.variable_lower.size());
    }


    std::pair<double, double> ILPSolverCollect::get_infinity_impl()
    {
        return {std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max()};
    }


    void ILPSolverCollect::add_variable_impl (VariableType p_type, double p_objective, double p_lower_bound, double p_upper_bound,
        const std::string& /* p_name */, const std::vector<double>* p_row_values,
        const std::vector<int>* p_row_indices)
    {
        if (p_row_values)
        {
            if (!p_row_indices)
            {
                assert (p_row_values->size() == d_ilp_data.matrix.size());
                append_column(&d_ilp_data.matrix, *p_row_values);
            }
            else
            {
                assert (p_row_values->size() == p_row_indices->size());
                assert (p_row_indices->size() <= d_ilp_data.matrix.size());
                append_column(&d_ilp_data.matrix, *p_row_indices, *p_row_values);
            }
        }
        else
        {
            assert( p_row_indices == nullptr );
            append_column(&d_ilp_data.matrix, std::vector<int>(), std::vector<double>());
        }

        d_ilp_data.objective.push_back(p_objective);
        d_ilp_data.variable_lower.push_back(p_lower_bound);
        d_ilp_data.variable_upper.push_back(p_upper_bound);
        d_ilp_data.variable_type.push_back(p_type);
    }


    void ILPSolverCollect::add_constraint_impl (double p_lower_bound, double p_upper_bound,
        const std::vector<double>& p_col_values, const std::string& /* p_name */,
        const std::vector<int>* p_col_indices)
    {
        if (!p_col_indices)
        {
            assert( p_col_values.size() == d_ilp_data.objective.size() );
            append_row(&d_ilp_data.matrix, p_col_values);
        }
        else
        {
            assert(p_col_values.size() == p_col_indices->size());
            assert(p_col_indices->size() <= d_ilp_data.objective.size());

            int n_cols = static_cast<int>(d_ilp_data.objective.size());
            append_row(&d_ilp_data.matrix, n_cols, *p_col_indices, p_col_values);
        }

        d_ilp_data.constraint_lower.push_back(p_lower_bound);
        d_ilp_data.constraint_upper.push_back(p_upper_bound);
    }


    void ILPSolverCollect::set_objective_sense_impl(ObjectiveSense p_sense)
    {
        d_ilp_data.objective_sense = p_sense;
    }


    void ILPSolverCollect::set_start_solution(const std::vector<double>& p_solution)
    {
        d_ilp_data.start_solution = p_solution;
    }


    void ILPSolverCollect::set_num_threads        (int p_num_threads)
    {
        d_ilp_data.num_threads = p_num_threads;
    }


    void ILPSolverCollect::set_deterministic_mode (bool p_deterministic)
    {
        d_ilp_data.deterministic = p_deterministic;
    }


    void ILPSolverCollect::set_log_level          (int p_level)
    {
        d_ilp_data.log_level = p_level;
    }


    void ILPSolverCollect::set_max_seconds        (double p_seconds)
    {
        d_ilp_data.max_seconds = p_seconds;
    }
}
