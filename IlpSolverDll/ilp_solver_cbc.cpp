#include "ilp_solver_cbc.hpp"

#include "OsiSolverInterface.hpp"

namespace ILPSolver
{
    OsiSolverInterface& ILPSolverCbc::solver()
    {
        return d_clp_solver;
    }

    const OsiSolverInterface& ILPSolverCbc::solver() const
    {
        return d_clp_solver;
    }

    void ILPSolverCbc::do_solve()
    {
        d_model = CbcModel(solver());
        d_model.branchAndBound();
    }

    const double* ILPSolverCbc::do_get_solution() const
    {
        return d_model.bestSolution();
    }

    double ILPSolverCbc::do_get_objective() const
    {
        return d_model.getObjValue();
    }

}
