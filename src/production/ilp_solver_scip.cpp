#include <cassert>
#include "ilp_solver_scip.hpp"
#include <iostream>

namespace ilp_solver
{
    #include "scip/scip.h"
    #include "scip/scipdefplugins.h"

    namespace
    {
        // A lambda instead of the macro expansion to handle return codes.
        // If the SCIP function call was unsuccessful, throw an error.
        // REVIEW: Which error should be thrown?
        static const auto SCIP_call_exec{
            [](auto f, auto... args) -> void
            {
                auto retcode = f(args...);
                if (retcode != SCIP_OKAY)
                    throw std::runtime_error("SCIP Error!\n");
            }
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
        SCIP_call_exec(SCIPcreateSol, d_scip, &sol, nullptr);

        // SCIP uses a double*, not a const double*, but ScaiILP demands a const std::vector<double>&.
        // Internally, SCIP calls a single-variable setter for every variable with a by-value pass of the corresponding double,
        // so the const_cast should not violate actual const-ness.
        // Sadly, it is not avoidable since SCIP is not const-correct. (SCIP 6.0)
        SCIP_call_exec(SCIPsetSolVals, d_scip, sol, d_cols.size(), d_cols.data(), const_cast<double*>(p_solution.data()));
        SCIP_call_exec(SCIPaddSol, d_scip, sol, &ignored);
    }

    void ILPSolverSCIP::minimize()
    {
        SCIP_call_exec(SCIPsetObjsense, d_scip, SCIP_OBJSENSE_MINIMIZE);
        SCIP_call_exec(SCIPsolve, d_scip);
    }
    void ILPSolverSCIP::maximize()
    {
        SCIP_call_exec(SCIPsetObjsense, d_scip, SCIP_OBJSENSE_MAXIMIZE);
        SCIP_call_exec(SCIPsolve, d_scip);
    }

    const std::vector<double> ILPSolverSCIP::get_solution()  const
    {
        std::vector<double> res;
        res.reserve(d_cols.size());
        auto sol = SCIPgetBestSol(d_scip);
        for (auto& s : d_cols)
            res.push_back(SCIPgetSolVal(d_scip, sol, s));
        return res;
    }
    double ILPSolverSCIP::get_objective() const
    {
        double val = SCIPgetPrimalbound(d_scip);
        if (val ==  SCIPinfinity(d_scip)) return std::numeric_limits<double>::max();
        if (val == -SCIPinfinity(d_scip)) return std::numeric_limits<double>::min();
        return val;
    }
    SolutionStatus ILPSolverSCIP::get_status() const
    {
        auto n = SCIPgetNSolsFound(d_scip);
        SolutionStatus ret = (n > 0) ? SolutionStatus::SUBOPTIMAL : SolutionStatus::NO_SOLUTION;
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
        SCIP_call_exec(SCIPsetIntParam, d_scip, "parallel/maxnthreads", p_num_threads);
    }
    void ILPSolverSCIP::set_deterministic_mode(bool p_deterministic)
    {
        SCIP_call_exec(SCIPsetIntParam, d_scip, "parallel/mode", p_deterministic);
    }
    void ILPSolverSCIP::set_log_level(int p_level)
    {
        p_level = (p_level < 0) ? 0 : p_level;
        p_level = (p_level > 5) ? 5 : p_level;
        SCIP_call_exec(SCIPsetIntParam, d_scip, "display/verblevel", p_level);
    }
    void ILPSolverSCIP::set_max_seconds(double p_seconds)
    {
        SCIP_call_exec(SCIPsetRealParam, d_scip, "limits/time", p_seconds);
    }


    ILPSolverSCIP::ILPSolverSCIP()
    {
        SCIP_call_exec(SCIPcreate, &d_scip);
        SCIP_call_exec(SCIPincludeDefaultPlugins, d_scip);

        SCIP_call_exec(SCIPcreateProb, d_scip, "problem", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        SCIP_call_exec(SCIPsetObjsense, d_scip, SCIP_OBJSENSE_MINIMIZE);
    }

    ILPSolverSCIP::~ILPSolverSCIP()
    {
         for(auto& p : d_cols) SCIP_call_exec(SCIPreleaseVar, d_scip, &p);
         for(auto& p : d_rows) SCIP_call_exec(SCIPreleaseCons, d_scip, &p);
         SCIP_call_exec(SCIPfree, &d_scip);
    }

    void ILPSolverSCIP::add_variable(const std::vector<int>* p_row_indices, const std::vector<double>* p_row_values, double p_lower_bound, double p_upper_bound, double p_objective, SCIP_VARTYPE p_type, const std::string& p_name)
    {
        SCIP_VAR* var;
        const char* name = p_name.c_str();
        SCIP_call_exec(SCIPcreateVar, d_scip, &var, name, p_lower_bound, p_upper_bound, p_objective, p_type, TRUE, FALSE, nullptr, nullptr, nullptr, nullptr, nullptr);
        d_cols.push_back(var);
        SCIP_call_exec(SCIPaddVar, d_scip, var);

        if (p_row_values)
        {
            if (p_row_indices)
            {
                assert( p_row_values->size() >= p_row_indices->size() );
                for (auto i : *p_row_indices)
                {
                    assert( i < d_rows.size() );
                    SCIP_CONS* cons = d_rows[i];
                    SCIP_call_exec(SCIPaddCoefLinear, d_scip, cons, var, (*p_row_values)[i]);
                }
            }
            else
            {
                assert( p_row_values->size() >= d_rows.size() );
                for (size_t i = 0; i < d_rows.size(); i++)
                {
                    SCIP_call_exec(SCIPaddCoefLinear, d_scip, d_rows[i], var, (*p_row_values)[i]);
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
        if (!p_col_indices)
        {
            assert(p_col_values.size() >= d_cols.size());
            vars = d_cols.data();
            size = static_cast<int>(d_cols.size());
        }
        else
        {
            assert(p_col_values.size() >= p_col_indices->size());
            tmp.reserve(p_col_indices->size());
            for (int i : *p_col_indices)
            {
                assert(i < d_cols.size());
                tmp.push_back(d_cols[i]);
            }
            vars = tmp.data();
            size = static_cast<int>(tmp.size());
        }

        // SCIP uses a double*, not a const double*, but ScaiILP demands a const std::vector<double>&.
        // Internally, SCIP copies the buffer, so the const_cast should not violate actual const-ness.
        // Sadly, it is not avoidable since SCIP is not const-correct. (SCIP 6.0)
        SCIP_call_exec(SCIPcreateConsLinear, d_scip, &cons, name, size, vars, const_cast<double*>(p_col_values.data()),
                p_lhs, p_rhs, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
        SCIP_call_exec(SCIPaddCons, d_scip, cons);
        d_rows.push_back(cons);
    }
}