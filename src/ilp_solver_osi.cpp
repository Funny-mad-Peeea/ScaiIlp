#include "ilp_solver_osi.hpp"

#include "OsiSolverInterface.hpp"

namespace ilp_solver
{
    ILPSolverOsi::ILPSolverOsi(OsiSolverInterface* p_ilp_solver) : d_ilp_solver(p_ilp_solver) {}

    void ILPSolverOsi::do_solve()
    {
        do_get_solver()->branchAndBound();
    }

    const double* ILPSolverOsi::do_get_solution() const
    {
        return do_get_solver()->getColSolution();
    }

    double ILPSolverOsi::do_get_objective() const
    {
        return do_get_solver()->getObjValue();
    }
}
