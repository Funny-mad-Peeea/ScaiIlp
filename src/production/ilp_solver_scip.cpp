#if WITH_SCIP == 1

#include <cassert>
#include "ilp_solver_scip.hpp"

namespace ilp_solver
{
    #include "scip/scip.h"
    #include "scip/scipdefplugins.h"

    namespace
    {
        // A lambda instead of the macro expansion to handle return codes.
        // If the SCIP function call was unsuccessful, throw an error.
        template<typename F, typename... Args>
        static __forceinline void call_scip(F p_f, Args... p_args)
        {
            auto retcode = p_f(p_args...);
            if (retcode != SCIP_OKAY)
                throw std::exception("SCIP Error!\n");
        };
    }

    void ILPSolverSCIP::add_variable_boolean(double p_objective, const std::string& p_name)
    {
        add_variable(nullptr, nullptr, 0., 1., p_objective, SCIP_VARTYPE_BINARY, p_name);
    }
    void ILPSolverSCIP::add_variable_boolean(const std::vector<double>& p_row_values, double p_objective, const std::string& p_name)
    {
        add_variable(nullptr, &p_row_values, 0., 1., p_objective, SCIP_VARTYPE_BINARY, p_name);
    }
    void ILPSolverSCIP::add_variable_boolean(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, const std::string& p_name)
    {
        add_variable(&p_row_indices, &p_row_values, 0., 1., p_objective, SCIP_VARTYPE_BINARY, p_name);
    }

    void ILPSolverSCIP::add_variable_integer(double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
    {
        add_variable(nullptr, nullptr, p_lower_bound, p_upper_bound, p_objective, SCIP_VARTYPE_INTEGER, p_name);
    }
    void ILPSolverSCIP::add_variable_integer(const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
    {
        add_variable(nullptr, &p_row_values, p_lower_bound, p_upper_bound, p_objective, SCIP_VARTYPE_INTEGER, p_name);
    }
    void ILPSolverSCIP::add_variable_integer(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
    {
        add_variable(&p_row_indices, &p_row_values, p_lower_bound, p_upper_bound, p_objective, SCIP_VARTYPE_INTEGER, p_name);
    }

    void ILPSolverSCIP::add_variable_continuous(double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
    {
        add_variable(nullptr, nullptr, p_lower_bound, p_upper_bound, p_objective, SCIP_VARTYPE_CONTINUOUS, p_name);
    }
    void ILPSolverSCIP::add_variable_continuous(const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
    {
        add_variable(nullptr, &p_row_values, p_lower_bound, p_upper_bound, p_objective, SCIP_VARTYPE_CONTINUOUS, p_name);
    }
    void ILPSolverSCIP::add_variable_continuous(const std::vector<int>& p_row_indices, const std::vector<double>& p_row_values, double p_objective, double p_lower_bound, double p_upper_bound, const std::string& p_name)
    {
        add_variable(&p_row_indices, &p_row_values, p_lower_bound, p_upper_bound, p_objective, SCIP_VARTYPE_CONTINUOUS, p_name);
    }

    void ILPSolverSCIP::add_constraint(const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const std::string& p_name)
    {
        add_constraint_intern(nullptr, p_col_values, p_lower_bound, p_upper_bound, p_name);
    }
    void ILPSolverSCIP::add_constraint(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, double p_upper_bound, const std::string& p_name)
    {
        add_constraint_intern(&p_col_indices, p_col_values, p_lower_bound, p_upper_bound, p_name);
    }

    void ILPSolverSCIP::add_constraint_upper(const std::vector<double>& p_col_values, double p_upper_bound, const std::string& p_name)
    {
        add_constraint_intern(nullptr, p_col_values, -SCIPinfinity(d_scip), p_upper_bound, p_name);
    }
    void ILPSolverSCIP::add_constraint_upper(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_upper_bound, const std::string& p_name)
    {
        add_constraint_intern(&p_col_indices, p_col_values, -SCIPinfinity(d_scip), p_upper_bound, p_name);
    }

    void ILPSolverSCIP::add_constraint_lower(const std::vector<double>& p_col_values, double p_lower_bound, const std::string& p_name)
    {
        add_constraint_intern(nullptr, p_col_values, p_lower_bound, SCIPinfinity(d_scip), p_name);
    }
    void ILPSolverSCIP::add_constraint_lower(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_lower_bound, const std::string& p_name)
    {
        add_constraint_intern(&p_col_indices, p_col_values, p_lower_bound, SCIPinfinity(d_scip), p_name);
    }

    void ILPSolverSCIP::add_constraint_equality(const std::vector<double>& p_col_values, double p_value, const std::string& p_name)
    {
        add_constraint_intern(nullptr, p_col_values, p_value, p_value, p_name);
    }
    void ILPSolverSCIP::add_constraint_equality(const std::vector<int>& p_col_indices, const std::vector<double>& p_col_values, double p_value, const std::string& p_name)
    {
        add_constraint_intern(&p_col_indices, p_col_values, p_value, p_value, p_name);
    }

    void ILPSolverSCIP::set_start_solution(const std::vector<double>& p_solution)
    {
        assert(p_solution.size() >= d_cols.size());

        SCIP_SOL* sol;
        SCIP_Bool ignored{false};
        call_scip(SCIPcreateSol, d_scip, &sol, nullptr);

        // SCIP uses a double*, not a const double*, but ScaiILP demands a const std::vector<double>&.
        // Internally, SCIP calls a single-variable setter for every variable with a by-value pass of the corresponding double,
        // so the const_cast should not violate actual const-ness.
        // Sadly, it is not avoidable since SCIP is not const-correct. (SCIP 6.0)
        call_scip(SCIPsetSolVals, d_scip, sol, static_cast<int>(d_cols.size()), d_cols.data(), const_cast<double*>(p_solution.data()));
        call_scip(SCIPaddSol, d_scip, sol, &ignored);
    }

    void ILPSolverSCIP::minimize()
    {
        call_scip(SCIPsetObjsense, d_scip, SCIP_OBJSENSE_MINIMIZE);
        call_scip(SCIPsolve, d_scip);
    }
    void ILPSolverSCIP::maximize()
    {
        call_scip(SCIPsetObjsense, d_scip, SCIP_OBJSENSE_MAXIMIZE);
        call_scip(SCIPsolve, d_scip);
    }

    const std::vector<double> ILPSolverSCIP::get_solution()  const
    {
        assert( get_status() == SolutionStatus::SUBOPTIMAL || get_status() == SolutionStatus::PROVEN_OPTIMAL );

        std::vector<double> res;
        res.reserve(d_cols.size());

        // Obtain the best solution value for each variable in the current best solution.
        auto sol = SCIPgetBestSol(d_scip);
        for (auto& s : d_cols)
            res.push_back(SCIPgetSolVal(d_scip, sol, s));

        return res;
    }
    double ILPSolverSCIP::get_objective() const
    {
        // The current primal bound is the best objective value attained.
        double val = SCIPgetPrimalbound(d_scip);

        // Handle infinity cases.
        if (val ==  SCIPinfinity(d_scip)) return std::numeric_limits<double>::max();
        if (val == -SCIPinfinity(d_scip)) return std::numeric_limits<double>::min();

        return val;
    }
    SolutionStatus ILPSolverSCIP::get_status() const
    {
        auto n = SCIPgetNSolsFound(d_scip);
        SolutionStatus ret = (n > 0) ? SolutionStatus::SUBOPTIMAL : SolutionStatus::NO_SOLUTION;

        // Handle all possible status values. Almost all will be reduced to SUBOPTIMAL or NO_SOLUTION.
        switch (SCIPgetStatus(d_scip))
        {
            case SCIP_STATUS_OPTIMAL:    return SolutionStatus::PROVEN_OPTIMAL;
            case SCIP_STATUS_INFEASIBLE: return SolutionStatus::PROVEN_INFEASIBLE;
            case SCIP_STATUS_UNKNOWN:        [[fallthrough]];
            case SCIP_STATUS_UNBOUNDED:      [[fallthrough]];
            case SCIP_STATUS_INFORUNBD:      [[fallthrough]];
            case SCIP_STATUS_NODELIMIT:      [[fallthrough]];
            case SCIP_STATUS_TOTALNODELIMIT: [[fallthrough]];
            case SCIP_STATUS_STALLNODELIMIT: [[fallthrough]];
            case SCIP_STATUS_TIMELIMIT:      [[fallthrough]];
            case SCIP_STATUS_MEMLIMIT:       [[fallthrough]];
            case SCIP_STATUS_GAPLIMIT:       [[fallthrough]];
            case SCIP_STATUS_SOLLIMIT:       [[fallthrough]];
            case SCIP_STATUS_BESTSOLLIMIT:   [[fallthrough]];
            case SCIP_STATUS_RESTARTLIMIT:   [[fallthrough]];
            case SCIP_STATUS_USERINTERRUPT:  [[fallthrough]];
            case SCIP_STATUS_TERMINATE:      [[fallthrough]];
            default:                     return ret;
        }
    }

    void ILPSolverSCIP::set_num_threads(int p_num_threads)
    {
        // Possibly does nothing if not using FiberSCIP or some other parallelization method.
        call_scip(SCIPsetIntParam, d_scip, "parallel/maxnthreads", p_num_threads);
    }
    void ILPSolverSCIP::set_deterministic_mode(bool p_deterministic)
    {
        // Possibly does nothing if not using FiberSCIP or some other parallelization method.
        call_scip(SCIPsetIntParam, d_scip, "parallel/mode", p_deterministic);
    }
    void ILPSolverSCIP::set_log_level(int p_level)
    {
        p_level = (p_level < 0) ? 0 : p_level; // Minimum level for verbosity is 0 (no output).
        p_level = (p_level > 5) ? 5 : p_level; // Maximum level for verbosity is 5.
        call_scip(SCIPsetIntParam, d_scip, "display/verblevel", p_level);
    }
    void ILPSolverSCIP::set_max_seconds(double p_seconds)
    {
        call_scip(SCIPsetRealParam, d_scip, "limits/time", p_seconds);
    }


    ILPSolverSCIP::ILPSolverSCIP()
    {
        call_scip(SCIPcreate, &d_scip);
        call_scip(SCIPincludeDefaultPlugins, d_scip);

        // All the nullptr's are possible User-data.
        call_scip(SCIPcreateProb, d_scip, "problem", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        call_scip(SCIPsetObjsense, d_scip, SCIP_OBJSENSE_MINIMIZE);
    }

    ILPSolverSCIP::~ILPSolverSCIP()
    {
        // Variables and Constraints need to be released separately.
        for(auto& p : d_cols)
            call_scip(SCIPreleaseVar, d_scip, &p);
        for(auto& p : d_rows)
            call_scip(SCIPreleaseCons, d_scip, &p);
        call_scip(SCIPfree, &d_scip);
    }

    void ILPSolverSCIP::add_variable(const std::vector<int>* p_row_indices, const std::vector<double>* p_row_values, double p_lower_bound, double p_upper_bound, double p_objective, SCIP_VARTYPE p_type, const std::string& p_name)
    {
        SCIP_VAR* var;
        const char* name = p_name.c_str();
        // Creates a variable of type p_type with corresponding objectives and bounds.
        // The parameters after p_type are:
        //     initial:   true  (the column belonging to var is present in the initial root LP.)
        //     removable: false (the column belonging to var is not removable from the LP.)
        //     User Data pointers.
        call_scip(SCIPcreateVar, d_scip, &var, name, p_lower_bound, p_upper_bound, p_objective, p_type, TRUE, FALSE, nullptr, nullptr, nullptr, nullptr, nullptr);
        call_scip(SCIPaddVar, d_scip, var);
        d_cols.push_back(var); // We need to store the variables seperately to access them later on.

        if (p_row_values) // If we have coefficients given...
        {
            if (p_row_indices) // and indices...
            {
                assert( p_row_values->size() >= p_row_indices->size() );
                // Add the corresponding coefficient to every constraint indexed.
                for (auto i : *p_row_indices)
                {
                    assert( i < static_cast<int>(d_rows.size()) );
                    call_scip(SCIPaddCoefLinear, d_scip, d_rows[i], var, (*p_row_values)[i]);
                }
            }
            else
            {
                assert( p_row_values->size() >= d_rows.size() );
                // Add the corresponding coefficient to every constraint in the problem.
                for (size_t i = 0; i < d_rows.size(); i++)
                {
                    call_scip(SCIPaddCoefLinear, d_scip, d_rows[i], var, (*p_row_values)[i]);
                }
            }
        }
    }

    void ILPSolverSCIP::add_constraint_intern(const std::vector<int>* p_col_indices, const std::vector<double>& p_col_values, double p_lhs, double p_rhs, const std::string& p_name)
    {
        SCIP_CONS* cons;
        SCIP_VAR**  vars;
        std::vector<SCIP_VAR*> tmp;
        int size{0};

        const char* name = p_name.c_str();

        // If we have no indices given, we need to have a coefficient for every variable in the problem.
        if (!p_col_indices)
        {
            assert(p_col_values.size() >= d_cols.size());
            vars = d_cols.data();
            size = static_cast<int>(d_cols.size());
        }
        else
        {
            // Otherwise we need to create a vector of the correct variables given by their indices.
            assert(p_col_values.size() >= p_col_indices->size());
            tmp.reserve(p_col_indices->size());
            for (int i : *p_col_indices)
            {
                assert(i < static_cast<int>(d_cols.size()));
                tmp.push_back(d_cols[i]);
            }
            vars = tmp.data();
            size = static_cast<int>(tmp.size());
        }

        // SCIP uses a double*, not a const double*, but ScaiILP demands a const std::vector<double>&.
        // Internally, SCIP copies the buffer, so the const_cast should not violate actual const-ness.
        // Sadly, it is not avoidable since SCIP is not const-correct. (SCIP 6.0)
        // The parameters after p_rhs are:
        //    initial:        true  (the relaxed constraint is in the initial LP.)
        //    separate:       true  (the constraint should be separated during LP processing.)
        //    enforce:        true  (the constraint is enforced during node processing.)
        //    check:          true  (the constraint is checked for feasibility.)
        //    propagate:      true  (the constraint is propagated during node processing.)
        //    local:          false (the constraint is valid globally.)
        //    modifiable:     false (the constraint is not subject to column generation.)
        //    dynamic:        false (the constraint is not subject to aging.)
        //    removable:      false (the constraint may not be removed during aging or cleanup.)
        //    stickingatnode: false (the constraint should not be kept at the node where it was added.)
        call_scip(SCIPcreateConsLinear, d_scip, &cons, name, size, vars, const_cast<double*>(p_col_values.data()),
                p_lhs, p_rhs, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
        call_scip(SCIPaddCons, d_scip, cons);
        d_rows.push_back(cons);
    }
}

#endif
