#if WITH_CBC == 1

#include <algorithm>
#include "ilp_solver_cbc.hpp"

#pragma warning(push)
#pragma warning(disable : 5033) // silence warning in CBC concerning the deprecated keyword 'register'
#include "CoinMessageHandler.hpp"
#include "OsiSolverInterface.hpp"
#pragma warning(pop)


namespace ilp_solver
{
    OsiSolverInterface* ILPSolverCbc::do_get_solver()
    {
        return d_model.solver();
    }

    const OsiSolverInterface* ILPSolverCbc::do_get_solver() const
    {
        return d_model.solver();
    }

    ILPSolverCbc::ILPSolverCbc()
    {
        OsiSolverInterface* solver = new OsiClpSolverInterface();
        d_model.assignSolver(solver, true);

        set_num_threads(1);
        set_deterministic_mode(true);
        set_log_level(0);
        set_max_seconds(std::numeric_limits<double>::max());
    }

    void ILPSolverCbc::do_solve(const std::vector<double>& p_start_solution)
    {
        d_model.resetModel();

        if (!p_start_solution.empty())
        {
            assert((int) p_start_solution.size() == d_model.getNumCols());
            d_model.setBestSolution(p_start_solution.data(), static_cast<int>(p_start_solution.size()), COIN_DBL_MAX, true);
        }
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

    SolutionStatus ILPSolverCbc::do_get_status() const
    {
        if (d_model.isProvenOptimal())
            return SolutionStatus::PROVEN_OPTIMAL;
        else if (d_model.isProvenInfeasible())
            return SolutionStatus::PROVEN_INFEASIBLE;
        else
            return (do_get_solution() == nullptr ? SolutionStatus::NO_SOLUTION
                                                 : SolutionStatus::SUBOPTIMAL);
    }

    void ILPSolverCbc::set_num_threads        (int p_num_threads)
    {
        const auto cbc_num_threads = (p_num_threads == 1 ? 0 : p_num_threads);      // peculiarity of Cbc
        d_model.setNumberThreads(cbc_num_threads);
    }

    void ILPSolverCbc::set_deterministic_mode (bool p_deterministic)
    {
        const auto cbc_thread_mode = ((d_model.getNumberThreads() > 1 && p_deterministic) ? 1 : 0);
        d_model.setThreadMode(cbc_thread_mode);
    }

    void ILPSolverCbc::set_log_level          (int p_level)
    {
        d_model.messageHandler()->setLogLevel(std::clamp(p_level, 0, 4));   // log level must be between 0 and 4
    }

    void ILPSolverCbc::set_max_seconds        (double p_seconds)
    {
        d_model.setMaximumSeconds(p_seconds);
    }

}

#endif
