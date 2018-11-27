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

            // We need to reimplement those, since CbcModel stores solution values independently of its solver.
            std::vector<double> get_solution  () const override;
            double              get_objective () const override;
            SolutionStatus      get_status    () const override;
            void set_start_solution(const std::vector<double>& p_solution) override;

            void set_num_threads        (int p_num_threads)    override;
            void set_deterministic_mode (bool p_deterministic) override;
            void set_log_level          (int p_level)          override;
            void set_max_seconds        (double p_seconds)     override;
        private:
            CbcModel d_model;

            OsiSolverInterface*       get_solver_osi    ()       override;

            // Need to reimplement this again.
            void solve_impl() override;
            void set_objective_sense_impl(ObjectiveSense p_sense) override;
    };
}

#endif

#endif
