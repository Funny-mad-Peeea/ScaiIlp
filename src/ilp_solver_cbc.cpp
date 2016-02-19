#include "ilp_solver_cbc.hpp"

#include "CoinMessageHandler.hpp"
#include "OsiSolverInterface.hpp"

namespace ilp_solver
{
    OsiSolverInterface* ILPSolverCbc::solver()
    {
        return &d_clp_solver;
    }

    const OsiSolverInterface* ILPSolverCbc::solver() const
    {
        return &d_clp_solver;
    }

    void ILPSolverCbc::do_solve()
    {
        d_model = CbcModel(*solver());
        d_model.messageHandler()->setLogLevel(std::min(std::max(log_level(), 0), 4));   // log level must be between 0 and 4
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

    void ILPSolverCbc::do_set_num_threads(int p_num_threads)
    {
        d_model.setNumberThreads(p_num_threads);
    }
}
