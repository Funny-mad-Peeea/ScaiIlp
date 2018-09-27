#if WITH_CBC == 1

#ifndef _ILP_SOLVER_CBC_HPP
#define _ILP_SOLVER_CBC_HPP


#pragma comment(lib, "libCbc.lib")
#pragma comment(lib, "libCoinUtils.lib")
#pragma comment(lib, "libClp.lib")
#pragma comment(lib, "libOsi.lib")
#pragma comment(lib, "libOsiClp.lib")
#pragma comment(lib, "libCgl.lib")

#include "ilp_solver_osi_model.hpp"

#pragma warning(push)
#pragma warning(disable : 5033) // silence warning in CBC concerning the deprecated keyword 'register'
#pragma warning(disable : 4309) // silence warning in CBC concerning truncations of constant values in 64 bit.
#include "CbcModel.hpp"
#include "OsiClpSolverInterface.hpp"
#pragma warning(pop)

class OsiSolverInterface;

namespace ilp_solver
{
    class ILPSolverCbc : public ILPSolverOsiModel
    {
        private:
            OsiClpSolverInterface d_clp_solver;
            CbcModel d_model;

            OsiSolverInterface*       do_get_solver    ()       override;
            const OsiSolverInterface* do_get_solver    () const override;

            void                      do_solve         (const std::vector<double>& p_start_solution,
                                                        int p_num_threads, bool p_deterministic, int p_log_level, double p_max_seconds) override;
            const double*             do_get_solution  () const override;
            double                    do_get_objective () const override;
            SolutionStatus            do_get_status    () const override;
    };
}

#endif

#endif
