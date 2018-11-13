#ifndef _ILP_SOLVER_CBC_HPP
#define _ILP_SOLVER_CBC_HPP

#if WITH_CBC == 1

static_assert(WITH_OSI == 1,
    "CBC requires the Osi-Interface and the CoinUtils contained therein. "
    "Please set WITH_OSI=1 or deactivate CBC with WITH_CBC=0.");

// Link with the required CBC Libraries.
#pragma comment(lib, "libCbc.lib")
#pragma comment(lib, "libClp.lib")
#pragma comment(lib, "libCgl.lib")

#include "ilp_solver_osi_model.hpp" // Including this also links with the required COIN Libraries.

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
        public:
            ILPSolverCbc();
        private:
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
