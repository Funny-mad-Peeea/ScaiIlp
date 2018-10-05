#if (WITH_GUROBI == 1) && (_WIN64 == 1)

#include <cassert>
#include "ilp_solver_gurobi.hpp"


namespace ilp_solver
{
        ILPSolverGurobi::ILPSolverGurobi()
        {
            set_log_level(0); // Set the default log_level to "no output".
        }

        void ILPSolverGurobi::add_variable_boolean(double p_objective, const std::string& p_name)
        {
            add_variable(0., 1., p_objective, p_name, GRB_BINARY);
        }
        void ILPSolverGurobi::add_variable_boolean(const std::vector<double>& p_row_values, double p_objective, const std::string& p_name)
        {
            add_variable(p_row_values, 0., 1., p_objective, p_name, GRB_BINARY);
        }
        void ILPSolverGurobi::add_variable_boolean(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, const std::string& p_name)
        {
            add_variable(p_row_indices, p_row_values, 0., 1., p_objective, p_name, GRB_BINARY);
        }


        void ILPSolverGurobi::add_variable_integer(double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
        {
            add_variable(p_lower_bound, p_upper_bound, p_objective, p_name, GRB_INTEGER);
        }
        void ILPSolverGurobi::add_variable_integer(const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
        {
            add_variable(p_row_values, p_lower_bound, p_upper_bound, p_objective, p_name, GRB_INTEGER);
        }
        void ILPSolverGurobi::add_variable_integer(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
        {
            add_variable(p_row_indices, p_row_values, p_lower_bound, p_upper_bound, p_objective, p_name, GRB_INTEGER);
        }


        void ILPSolverGurobi::add_variable_continuous(double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
        {
            add_variable(p_lower_bound, p_upper_bound, p_objective, p_name, GRB_CONTINUOUS);
        }
        void ILPSolverGurobi::add_variable_continuous(const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
        {
            add_variable(p_row_values, p_lower_bound, p_upper_bound, p_objective, p_name, GRB_CONTINUOUS);
        }
        void ILPSolverGurobi::add_variable_continuous(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
        {
            add_variable(p_row_indices, p_row_values, p_lower_bound, p_upper_bound, p_objective, p_name, GRB_CONTINUOUS);
        }


        void ILPSolverGurobi::add_constraint(const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const std::string& p_name)
        {
            add_constraint_range(build_row(p_col_values), p_lower_bound, p_upper_bound, p_name);
        }
        void ILPSolverGurobi::add_constraint(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const std::string& p_name)
        {
            add_constraint_range(build_row(p_col_indices, p_col_values), p_lower_bound, p_upper_bound, p_name);
        }


        void ILPSolverGurobi::add_constraint_upper(const std::vector<double>& p_col_values, double p_upper_bound, const std::string& p_name)
        {
            add_constraint_sense(build_row(p_col_values), p_upper_bound, p_name, GRB_LESS_EQUAL);
        }
        void ILPSolverGurobi::add_constraint_upper(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_upper_bound, const std::string& p_name)
        {
            add_constraint_sense(build_row(p_col_indices, p_col_values), p_upper_bound, p_name, GRB_LESS_EQUAL);
        }


        void ILPSolverGurobi::add_constraint_lower(const std::vector<double>& p_col_values, double p_lower_bound, const std::string& p_name)
        {
            add_constraint_sense(build_row(p_col_values), p_lower_bound, p_name, GRB_GREATER_EQUAL);
        }
        void ILPSolverGurobi::add_constraint_lower(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, const std::string& p_name)
        {
            add_constraint_sense(build_row(p_col_indices, p_col_values), p_lower_bound, p_name, GRB_GREATER_EQUAL);
        }


        void ILPSolverGurobi::add_constraint_equality(const std::vector<double>& p_col_values, double p_value, const std::string& p_name)
        {
            add_constraint_sense(build_row(p_col_values), p_value, p_name, GRB_EQUAL);
        }
        void ILPSolverGurobi::add_constraint_equality(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_value, const std::string& p_name)
        {
            add_constraint_sense(build_row(p_col_indices, p_col_values), p_value, p_name, GRB_EQUAL);
        }

        void ILPSolverGurobi::set_start_solution(const std::vector<double>& p_solution)
        {
            assert( p_solution.size() >= d_cols.size() );

            for (size_t i = 0; i < d_cols.size(); i++)
            {
                d_cols[i].set(GRB_DoubleAttr_Start, p_solution[i]);
            }
        }

        void ILPSolverGurobi::minimize()
        {
            d_model.set(GRB_IntAttr_ModelSense, 1); // 1 means 'minimize'.
            d_model.update();
            d_model.optimize();
        }
        void ILPSolverGurobi::maximize()
        {
            d_model.set(GRB_IntAttr_ModelSense, -1); // -1 means 'maximize'.
            d_model.update();
            d_model.optimize();
        }


        const std::vector<double> ILPSolverGurobi::get_solution() const
        {
            std::vector<double> solution;
            if (d_model.get(GRB_IntAttr_SolCount) > 0) // If we obtained at least one (possibly suboptimal) feasible solution...
            {
                solution.reserve(d_cols.size());
                for (auto& var : d_cols)
                {
                    // Store all the variables of the best solution in the return-value vector.
                    solution.emplace_back(var.get(GRB_DoubleAttr_X));
                }
            }
            return solution;
        }

        double ILPSolverGurobi::get_objective() const
        {
            return d_model.get(GRB_DoubleAttr_ObjVal);
        }

        SolutionStatus ILPSolverGurobi::get_status() const
        {
            int status = d_model.get(GRB_IntAttr_Status);
            int sol_count = d_model.get(GRB_IntAttr_SolCount);

            SolutionStatus current_status = (sol_count > 0) ? SolutionStatus::SUBOPTIMAL : SolutionStatus::NO_SOLUTION;
            switch (status)
            {
            case GRB_OPTIMAL:         return SolutionStatus::PROVEN_OPTIMAL;    // The only two cases were current_status
            case GRB_INFEASIBLE:      return SolutionStatus::PROVEN_INFEASIBLE; // does not hold the correct value.
            case GRB_LOADED:          [[fallthrough]];
            case GRB_INF_OR_UNBD:     [[fallthrough]];
            case GRB_UNBOUNDED:       [[fallthrough]];
            case GRB_CUTOFF:          [[fallthrough]];
            case GRB_ITERATION_LIMIT: [[fallthrough]];
            case GRB_NODE_LIMIT:      [[fallthrough]];
            case GRB_TIME_LIMIT:      [[fallthrough]];
            case GRB_SOLUTION_LIMIT:  [[fallthrough]];
            case GRB_INTERRUPTED:     [[fallthrough]];
            case GRB_NUMERIC:         [[fallthrough]];
            case GRB_SUBOPTIMAL:      [[fallthrough]];
            case GRB_INPROGRESS:      [[fallthrough]];
            case GRB_USER_OBJ_LIMIT:  [[fallthrough]];
            default:                  return current_status;
            }
        }

        void ILPSolverGurobi::set_num_threads(int p_num_threads)
        {
            d_model.set(GRB_IntParam_Threads, p_num_threads);
        }

        void ILPSolverGurobi::set_deterministic_mode([[maybe_unused]] bool  p_deterministic) // Since the function is empty, the parameter is unused.
        {
            // Gurobi should be deterministic by default.
            // Therefore, this function is empty.
        }

        void ILPSolverGurobi::set_log_level(int p_level)
        {
            p_level = (p_level < 0) ? 0 : p_level;
            if (p_level == 0)
                d_model.set(GRB_IntParam_OutputFlag, 0); // 0 means no output.
            else
            {
                d_model.set(GRB_IntParam_OutputFlag, 1);
                d_model.set(GRB_IntParam_LogToConsole, 1);
                d_model.set(GRB_IntParam_DisplayInterval, p_level); // Messages are displayed more often the higher p_level is.
            }
        }

        void ILPSolverGurobi::set_max_seconds(double p_seconds)
        {
            d_model.set(GRB_DoubleParam_TimeLimit, p_seconds);
        }

        void ILPSolverGurobi::add_constraint_sense(GRBLinExpr&& row, double p_bound, const std::string& name, char sense)
        {
            d_model.addConstr(row, sense, p_bound, name);
        }
        void ILPSolverGurobi::add_constraint_range(GRBLinExpr&& row, double p_lower_bound, double p_upper_bound, const std::string& p_name)
        {

            if (p_lower_bound == p_upper_bound)
                add_constraint_sense(std::move(row), p_lower_bound, p_name, GRB_EQUAL);
            else
            {
                assert(p_lower_bound < p_upper_bound);
                d_model.addRange(std::move(row), p_lower_bound, p_upper_bound, p_name);
            }
        }

        // Build a new Gurobi Linear Expression from a vector of coefficients at least the size of the variables.
        GRBLinExpr ILPSolverGurobi::build_row(const std::vector<double>& p_col_values)
        {
            assert(p_col_values.size() >= d_cols.size());

            GRBLinExpr row = 0;
            row.addTerms(p_col_values.data(), d_cols.data(), static_cast<int>(d_cols.size()));
            return row;
        }

        // Build a new Gurobi Linear Expression from a vector of variable indices and their coefficients.
        GRBLinExpr ILPSolverGurobi::build_row(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values)
        {
            assert(p_col_values.size() >= p_col_indices.size());
            assert(p_col_indices.size() <= d_cols.size()); // Can not use more different indices than we have variables.

            GRBLinExpr row = 0;
            for (size_t i = 0; i < p_col_indices.size(); i++)
            {
                int ind = p_col_indices[i];
                assert(ind < static_cast<int>(d_cols.size())); // Assert that the index is valid.

                row += d_cols[ind] * p_col_values[i];
            }
            return row;
        }

        void ILPSolverGurobi::add_variable(double p_lower_bound, double p_upper_bound, double p_objective, const std::string& p_name, char p_type)
        {
            auto new_var = d_model.addVar(p_lower_bound, p_upper_bound, p_objective, p_type, p_name);
            d_cols.emplace_back(std::move(new_var));
        }

        void ILPSolverGurobi::add_variable(const std::vector<double>& p_row_values, double p_lower_bound, double p_upper_bound, double p_objective, const std::string& p_name, char p_type)
        {
            assert(p_row_values.size() >= d_rows.size());

            auto new_var = d_model.addVar(p_lower_bound, p_upper_bound, p_objective, p_type, static_cast<int>(d_rows.size()), d_rows.data(), p_row_values.data(), p_name);
            d_cols.emplace_back(std::move(new_var));
        }

        void ILPSolverGurobi::add_variable(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_lower_bound, double p_upper_bound, double p_objective, const std::string& p_name, char p_type)
        {
            assert(p_row_values.size() >= p_row_indices.size());
            assert(p_row_indices.size() <= d_rows.size());

            // We need to transform the indices of constraints we are given to an actual vector of constraints,
            // to add the variable and it's coefficients to each referenced constraint.
            std::vector<GRBConstr> tmp_rows;
            tmp_rows.reserve(p_row_indices.size());
            for (int ind : p_row_indices)
            {
                assert(ind < static_cast<int>(d_rows.size()));
                tmp_rows.push_back(d_rows[ind]);
            }
            auto new_var = d_model.addVar(p_lower_bound, p_upper_bound, p_objective, p_type, static_cast<int>(tmp_rows.size()), tmp_rows.data(), p_row_values.data(), p_name);
            d_cols.emplace_back(std::move(new_var));
        }
}

#endif
