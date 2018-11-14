#if WITH_OSI == 1

#include "ilp_solver_osi.hpp"

#pragma warning(push)
#pragma warning(disable : 4309) // silence warning in CBC concerning truncations of constant values in 64 bit.
#include "OsiSolverInterface.hpp"
#pragma warning(pop)

namespace ilp_solver
{
    ILPSolverOsi::ILPSolverOsi(OsiSolverInterface* p_ilp_solver) : d_ilp_solver(p_ilp_solver) {}

    void ILPSolverOsi::do_solve(const std::vector<double>& p_start_solution)
    {
        auto solver = do_get_solver();
        if (!p_start_solution.empty())
            solver->setColSolution(p_start_solution.data());
        solver->branchAndBound();
    }

    const double* ILPSolverOsi::do_get_solution() const
    {
        return do_get_solver()->getColSolution();
    }

    double ILPSolverOsi::do_get_objective() const
    {
        return do_get_solver()->getObjValue();
    }

    SolutionStatus ILPSolverOsi::do_get_status() const
    {
        const auto solver = do_get_solver();
        if (solver->isProvenOptimal())
            return SolutionStatus::PROVEN_OPTIMAL;
        else if (solver->isProvenPrimalInfeasible())
            return SolutionStatus::PROVEN_INFEASIBLE;
        else
            return (do_get_solution() == nullptr ? SolutionStatus::NO_SOLUTION
                                                 : SolutionStatus::SUBOPTIMAL);
    }

    void ILPSolverOsi::set_num_threads        (int)
    {
        // Not supported by OsiSolverInterface.
    }

    void ILPSolverOsi::set_deterministic_mode (bool)
    {
        // Not supported by OsiSolverInterface.
    }

    void ILPSolverOsi::set_log_level          (int p_level)
    {
        do_get_solver()->messageHandler()->setLogLevel(p_level);
    }

    void ILPSolverOsi::set_max_seconds        (double)
    {
        // Not supported by OsiSolverInterface.
    }
}

#endif
