
#if 0
#include <cassert>
#include "ilp_solver_scip.hpp"
#include "scip/scip.h"
#include "scip/scipdefplugins.h"

namespace ilp_solver
{
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

    }

        void ILPSolverSCIP::minimize()
    {
        SCIP_CALL_EXC(SCIPsetObjsense(d_scip, SCIP_OBJSENSE_MINIMIZE));
        SCIP_CALL_EXC(SCIPsolve(d_scip));
    }
    void ILPSolverSCIP::maximize()
    {
        SCIP_CALL_EXC(SCIPsetObjsense(d_scip, SCIP_OBJSENSE_MAXIMIZE));
        SCIP_CALL_EXC(SCIPsolve(d_scip));
    }

    const std::vector<double> ILPSolverSCIP::get_solution()  const
    {
        std::vector<double> res;
        SCIP_SOL* sol = SCIPgetBestSol(d_scip);
        if (sol)
        {
            res.resize(d_cols.size());
            for (size_t i = 0; i < d_cols.size(); i++)
            {
                res[i] = SCIPgetSolVal(d_scip, sol, d_cols[i]);
            }
        }
    }
    double ILPSolverSCIP::get_objective() const
    {

    }
    SolutionStatus ILPSolverSCIP::get_status() const
    {

    }

    void ILPSolverSCIP::set_num_threads(int p_num_threads)
    {

    }
    void ILPSolverSCIP::set_deterministic_mode(bool p_deterministic)
    {

    }
    void ILPSolverSCIP::set_log_level(int p_level)
    {

    }
    void ILPSolverSCIP::set_max_seconds(double p_seconds)
    {

    }


    ILPSolverSCIP::ILPSolverSCIP()
    {
        SCIP_CALL_EXC(SCIPcreate(&d_scip));
        SCIP_CALL_EXC(SCIPincludeDefaultPlugins(d_scip));
        SCIP_CALL_EXC(SCIPsetMessagehdlr(NULL));

        SCIP_CALL_EXC(SCIPcreateProb(d_scip, "problem", NULL, NULL, NULL, NULL, NULL, NULL, NULL));
        SCIP_CALL_EXC(SCIPsetObjsense(d_scip, SCIP_OBJSENSE_MINIMIZE));
    }

    ILPSolverSCIP::~ILPSolverSCIP()
    {
        SCIP_CALL_EXC(SCIPreleaseVar(d_scip, d_cols.data()));
        SCIP_CALL_EXC(SCIPreleaseCons(d_scip, d_rows.data()));
        SCIP_CALL_EXC(SCIPfree(&d_scip));
    }

    void ILPSolverSCIP::add_variable(const std::vector<int>* p_row_indices, const std::vector<double>* p_row_values, double p_lower_bound, double p_upper_bound, double p_objective, int p_type, const std::string& p_name)
    {
        SCIP_VAR* var;
        SCIP_CALL_EXC(SCIPcreateVar(d_scip, &var, p_name.c_str(), p_lower_bound, p_upper_bound, p_objective, p_type, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL));
        d_cols.push_back(var);
        SKIP_CALL_EXC(SCIPaddVar(d_scip, var));

        if (p_row_values)
        {
            if (p_row_indices)
            {
                assert( p_row_values->size() >= p_row_indices->size() );
                for (int i : p_row_indices)
                {
                    assert( i < d_rows.size() );
                    SCIP_CONS* cons = d_rows[i];
                    SCIP_CALL_EXC(SCIPaddCoefLinear(d_scip, cons, var, p_row_values[i]));
                }
            }
            else
            {
                assert( p_row_values->size() >= d_rows.size() );
                for (size_t i = 0; i < d_rows.size(); i++)
                {
                    SCIP_CALL_EXC(SCIPaddCoefLinear(d_scip, d_rows[i], var, p_row_values[i]));
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

        SCIP_CALL_EXC(SCIPcreateConsLinear(scip, &cons, p_name.c_str(), size, vars, p_col_values.data(),
                p_lhs, p_rhs, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE));
        d_rows.push_back(cons);
    }
}

#endif
