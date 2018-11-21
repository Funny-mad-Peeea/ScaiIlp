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
        else if (d_model.isProvenDualInfeasible())
            return SolutionStatus::PROVEN_UNBOUNDED;
        else
            return ((d_model.bestSolution() == nullptr) ? SolutionStatus::NO_SOLUTION
                                                        : SolutionStatus::SUBOPTIMAL);
    }

    void ILPSolverCbc::set_start_solution(const std::vector<double>& p_solution)
    {
        // Set the current best solution of Cbc to the given solution, check for feasibility, but not for better objective value.
        assert( static_cast<int>(p_solution.size()) == d_model.getNumCols() );
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

    void ILPSolverCbc::set_objective_sense_impl(ObjectiveSense p_sense)
    {
        // CBCs objective value depends on the sense.
        // When changing the sense, the objective value gets multiplied by -1.
        // If we have set a start solution, we probably do not want that, and fix it.

        double old_sense{ d_model.getObjSense() };
        double sense{ (p_sense == ObjectiveSense::MINIMIZE) ? 1.0 : -1.0 };
        double old_value{ d_model.getObjValue() };

        d_model.setObjSense(sense); // ObjValue will be flipped here, if the senses differ.

        // If the senses differ, we need to fix the change from beforehand.
        // Both infinities are special cases.
        if (sense * old_sense < 0.)
        {
            old_value = (old_value == d_neg_infinity) ? d_pos_infinity
                      : (old_value == d_pos_infinity) ? d_neg_infinity
                      :  old_value;
            d_model.setObjValue( old_value );

        }
    }

}

#endif
