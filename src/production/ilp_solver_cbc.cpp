#if WITH_CBC == 1

#include <algorithm>
#include "ilp_solver_cbc.hpp"
#include "CglTreeInfo.hpp" // Needed to deal with the probing_info memory leak in Cbc

#pragma warning(push)
#pragma warning(disable : 5033) // silence warning in CBC concerning the deprecated keyword 'register'
#include "CoinMessageHandler.hpp"
#include "OsiSolverInterface.hpp"
#pragma warning(pop)


namespace ilp_solver
{
    OsiSolverInterface* ILPSolverCbc::get_solver()
    {
        return d_model.solver();
    }

    ILPSolverCbc::ILPSolverCbc()
    {
        // CbcModel assumes ownership over solver and deletes it in its destructor.
        OsiSolverInterface* solver = new OsiClpSolverInterface();
        d_model.assignSolver(solver, true);

        set_default_parameters();
    }

    std::vector<double> ILPSolverCbc::get_solution() const
    {
        // The best solution is stored by CbcModel, not by the solver, thus reimplementation.
        const auto* result = d_model.bestSolution();
        if (!result) return std::vector<double>();
        return std::vector<double>(result, result + d_model.getNumCols());
    }

    double ILPSolverCbc::get_objective() const
    {
        // The best objective value is stored by CbcModel, not by the solver, thus reimplementation.
        return d_model.getObjValue();
    }

    SolutionStatus ILPSolverCbc::get_status() const
    {
        // Solution status is stored by CbcModel.
        if (d_model.isProvenOptimal())
            return SolutionStatus::PROVEN_OPTIMAL;
        else if (d_model.isProvenInfeasible())
            return SolutionStatus::PROVEN_INFEASIBLE;
        else
            return (d_model.bestSolution() == nullptr ? SolutionStatus::NO_SOLUTION
                : SolutionStatus::SUBOPTIMAL);
    }

    void ILPSolverCbc::set_start_solution(const std::vector<double>& p_solution)
    {
        // Set the current best solution of Cbc to the given solution, check for feasibility, but not for better objective value.
        d_model.setBestSolution(p_solution.data(), static_cast<int>(p_solution.size()), COIN_DBL_MAX, true);
    }

    void ILPSolverCbc::set_num_threads        (int p_num_threads)
    {
        const auto cbc_num_threads = (p_num_threads == 1 ? 0 : p_num_threads); // peculiarity of Cbc (1 is 'for testing').
        d_model.setNumberThreads(cbc_num_threads);
    }

    void ILPSolverCbc::solve_impl()
    {
        // The probingInfo is not deleted on successive solves,
        // but overwritten. Thus, it produces memory leaks if not deleted here.
        auto probing_ptr = d_model.probingInfo();
        if (probing_ptr)
            delete probing_ptr;

        d_model.branchAndBound();
    }

    void ILPSolverCbc::set_deterministic_mode (bool p_deterministic)
    {
        const auto cbc_thread_mode = ((d_model.getNumberThreads() > 1 && p_deterministic) ? 1 : 0);
        d_model.setThreadMode(cbc_thread_mode);
    }

    void ILPSolverCbc::set_log_level          (int p_level)
    {
        int level = std::clamp(p_level, 0, 4); // log level must be between 0 and 4
        d_model.messageHandler()->setLogLevel(level);   // Both CBC and the CLP solver must be set.
        d_model.solver()->messageHandler()->setLogLevel(level);
    }

    void ILPSolverCbc::set_max_seconds        (double p_seconds)
    {
        d_model.setMaximumSeconds(p_seconds);
    }

}

#endif
