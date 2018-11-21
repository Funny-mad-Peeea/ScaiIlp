#if (WITH_GUROBI == 1) && (_WIN64 == 1)

#include <cassert>
#include "ilp_solver_gurobi.hpp"

namespace {

    inline void check_gurobi_error(int p_ret, const char* p_error = "");

    class GurobiEnvSingleton
    {
    public:
        GurobiEnvSingleton()
        {}

        void create_environment()
        {
            if (!d_environment_set){
                check_gurobi_error( GRBloadenv(&d_environment_ptr, nullptr)
                                  , "Could not create Gurobi environment.");
                d_environment_set = true;
            }
        }

        GRBenv* environment()
        {
            return d_environment_ptr;
        }

        const GRBenv* environment() const
        {
            return d_environment_ptr;
        }


        ~GurobiEnvSingleton() noexcept
        {
            if (d_environment_set)
                GRBfreeenv(d_environment_ptr);
        }
    private:
        GRBenv* d_environment_ptr{ nullptr };
        bool    d_environment_set{ false };
    };

    GurobiEnvSingleton global_gurobi_environment{};

    inline void check_gurobi_error(int p_ret, const char* p_error)
    {
        if (p_ret != 0)
        {
            const char* error = GRBgeterrormsg(global_gurobi_environment.environment());
            auto ret = std::string(p_error) + "[" + error + "]";
            throw std::exception(ret.c_str());
        }
    }

}

namespace ilp_solver
{
    ILPSolverGurobi::ILPSolverGurobi()
    {
        global_gurobi_environment.create_environment();
        check_gurobi_error( GRBnewmodel(global_gurobi_environment.environment(), &d_model, "", 0, nullptr, nullptr, nullptr, nullptr, nullptr)
                          , "Could not create empty Gurobi model.");
        set_default_parameters();
    }

    ILPSolverGurobi::~ILPSolverGurobi() noexcept
    {
        check_gurobi_error( GRBfreemodel(d_model)
                          , "Could not destroy Gurobi model.");
    }

    void ILPSolverGurobi::set_start_solution(const std::vector<double>& p_solution)
    {
        assert( p_solution.size() >= d_cols.size() );
        check_gurobi_error ( GRBsetdblattrarray(d_model, GRB_DBL_ATTR_START, 0, static_cast<int>(p_solution.size()), const_cast<double*>(p_solution.data()))
                           , "Could not set Gurobi start solution");
    }

    std::vector<double> ILPSolverGurobi::get_solution() const
    {
        int cols{ static_cast<int>(d_cols.size()) };
        std::vector<double> solution(cols);
        check_gurobi_error( GRBgetdblattrarray(d_model, GRB_DBL_ATTR_X, 0, cols, &solution.front())
                          , "Could not query the solution.");

        return solution;
    }

    double ILPSolverGurobi::get_objective() const
    {
        double result{0.};
        check_gurobi_error( GRBgetdblattr(d_model, GRB_DBL_ATTR_OBJVAL, &result)
                          , "Could not query the Gurobi objective value.");
        return result;
    }

    SolutionStatus ILPSolverGurobi::get_status() const
    {
        int status{0};
        check_gurobi_error( GRBgetintattr(d_model, GRB_INT_ATTR_STATUS, &status)
                          , "Could not query problem status.");
        int sol_count{0};
        check_gurobi_error( GRBgetintattr(d_model, GRB_INT_ATTR_SOLCOUNT, &sol_count)
                          , "Could not query number of feasible solutions found.");

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
        assert( p_num_threads >= 0 );
        check_gurobi_error( GRBsetintparam(GRBgetenv(d_model), GRB_INT_PAR_THREADS, p_num_threads)
                          , "Could not set number of threads for Gurobi.");
    }

    void ILPSolverGurobi::set_deterministic_mode(bool)
    {
        // Gurobi should be deterministic by default.
        // Therefore, this function is empty.
    }

    void ILPSolverGurobi::set_log_level(int p_level)
    {
        p_level = (p_level < 0) ? 0 : p_level;
        if (p_level == 0)
            check_gurobi_error( GRBsetintparam(GRBgetenv(d_model), GRB_INT_PAR_OUTPUTFLAG, 0) // 0 means no output.
                              , "Could not turn off Gurobi logging.");
        else
        {
            check_gurobi_error( GRBsetintparam(GRBgetenv(d_model), GRB_INT_PAR_OUTPUTFLAG, 1)
                              , "Could not turn on Gurobi logging.");
            check_gurobi_error( GRBsetintparam(GRBgetenv(d_model), GRB_INT_PAR_LOGTOCONSOLE, 1)
                              , "Could not turn on Gurobi Console logging.");
            p_level = 1 + 9 / p_level;
            // Gurobi prints log lines every DisplayInterval seconds.
            // We chose 10 seconds as the maximum, while 1 second is the Gurobi minimum (reached for p_level > 9).
            check_gurobi_error( GRBsetintparam(GRBgetenv(d_model), GRB_INT_PAR_DISPLAYINTERVAL, p_level)
                              , "Could not set logging frequency.");
        }
    }

    void ILPSolverGurobi::set_max_seconds(double p_seconds)
    {
        assert( p_seconds > 0 );
        check_gurobi_error(GRBsetdblparam(GRBgetenv(d_model), GRB_DBL_PAR_TIMELIMIT, p_seconds)
            , "Could not set time limit for Gurobi.");
    }

    void ILPSolverGurobi::add_variable_impl (VariableType p_type, double p_objective, double p_lower_bound, double p_upper_bound,
        const std::string& p_name, const std::vector<double>* p_row_values, const std::vector<int>* p_row_indices)
    {
        int     num{0};
        int*    indices{nullptr};
        double* values{nullptr};
        if (p_row_values)
        {
            p_row_indices = (p_row_indices) ? p_row_indices : &d_rows;
            assert( p_row_values->size() >= p_row_indices->size() );
            assert( p_row_indices->size() <= d_rows.size() );

            num     = static_cast<int>(p_row_indices->size());
            indices = const_cast<int*>(p_row_indices->data());
            values  = const_cast<double*>(p_row_values->data());
        }
        auto [lower, upper] = handle_bounds(&p_lower_bound, &p_upper_bound);
        char type = (p_type == VariableType::INTEGER)    ? GRB_INTEGER
                  : (p_type == VariableType::CONTINUOUS) ? GRB_CONTINUOUS
                  :                                        GRB_BINARY;

        check_gurobi_error( GRBaddvar(d_model, num, indices, values, p_objective, lower, upper, type, p_name.c_str())
                          , "Could not add variable to Gurobi model.");

    }


    void ILPSolverGurobi::add_constraint_impl (const double* p_lower_bound, const double* p_upper_bound,
        const std::vector<double>& p_col_values, const std::string& p_name, const std::vector<int>* p_col_indices)
    {
        p_col_indices = (p_col_indices) ? p_col_indices : &d_cols;
        assert( p_col_indices->size() <= p_col_values.size() );
        assert( p_col_indices->size() <= d_cols.size() );

        int     num    { static_cast<int>    ((p_col_indices) ? p_col_indices->size() : d_cols.size()) };
        int*    indices{  const_cast<int*>   ((p_col_indices) ? p_col_indices->data() : d_cols.data()) };
        double* values {  const_cast<double*>(p_col_values.data()) };

        auto [lower, upper] = handle_bounds(p_lower_bound, p_upper_bound);
        if (lower == upper)
            check_gurobi_error( GRBaddconstr(d_model, num, indices, values, GRB_EQUAL, lower, p_name.c_str())
                              , "Could not add a Gurobi equality constraint.");
        else
        {
            if (lower != d_neg_infinity)
            {
                if (upper != d_pos_infinity)
                    check_gurobi_error( GRBaddrangeconstr(d_model, num, indices, values, lower, upper, p_name.c_str())
                                      , "Could not add a Gurobi constraint.");
                else
                    check_gurobi_error( GRBaddconstr(d_model, num, indices, values, GRB_GREATER_EQUAL, lower, p_name.c_str())
                                      , "Could not add a Gurobi constraint.");
            }
            else
            {
                if (upper != d_pos_infinity)
                    check_gurobi_error( GRBaddrangeconstr(d_model, num, indices, values, GRB_LESS_EQUAL, upper, p_name.c_str())
                                      , "Could not add a Gurobi constraint.");
            }
        }
    }

    void ILPSolverGurobi::set_infinity()
    {
        d_pos_infinity =  GRB_INFINITY;
        d_neg_infinity = -GRB_INFINITY;
    }

    void ILPSolverGurobi::solve_impl()
    {
        check_gurobi_error( GRBoptimize(d_model)
                          , "Could not optimize the Gurobi model.");
    }

    void ILPSolverGurobi::set_objective_sense_impl(ObjectiveSense p_sense)
    {
        int sense{ (p_sense == ObjectiveSense::MINIMIZE) ? 1 : -1 };
        check_gurobi_error( GRBsetintattr(d_model, GRB_INT_ATTR_MODELSENSE, sense)
                          , "Could not set Gurobi model sense.");
    }

}

#endif
