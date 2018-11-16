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
        OsiSolverInterface* solver = new OsiClpSolverInterface();
        d_model.assignSolver(solver, true);

        set_default_parameters();
    }

    std::vector<double> ILPSolverCbc::get_solution() const
    {
        const auto* result = d_model.bestSolution();
        if (!result) return std::vector<double>();
        return std::vector<double>(result, result + d_model.getNumCols());
    }

    double ILPSolverCbc::get_objective() const
    {
        return d_model.getObjValue();
    }

    SolutionStatus ILPSolverCbc::get_status() const
    {
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
        std::cout << "MODEL SENSE: " << d_model.getObjSense() << ' ' << d_model.getObjValue() << ' ';
        d_model.setBestSolution(p_solution.data(), p_solution.size(), COIN_DBL_MAX, true);
        std::cout << d_model.getObjValue() << ' ' << d_model.bestSolution()[0]  <<std::endl;
    }

    void ILPSolverCbc::set_num_threads        (int p_num_threads)
    {
        const auto cbc_num_threads = (p_num_threads == 1 ? 0 : p_num_threads);      // peculiarity of Cbc
        d_model.setNumberThreads(cbc_num_threads);
    }

    void ILPSolverCbc::solve_impl()
    {
        d_model.branchAndBound();
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
