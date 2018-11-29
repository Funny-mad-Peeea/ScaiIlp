#if WITH_OSI == 1

#include "ilp_solver_osi.hpp"

#pragma warning(push)
#pragma warning(disable : 4309) // silence warning in CBC concerning truncations of constant values in 64 bit.
#include "OsiSolverInterface.hpp"
#pragma warning(pop)

namespace ilp_solver
{
    ILPSolverOsi::ILPSolverOsi(OsiSolverInterface* p_ilp_solver)
        : d_ilp_solver(p_ilp_solver)
    {
        set_default_parameters(this);
    }

    OsiSolverInterface* ILPSolverOsi::get_solver_osi()
    {
        return d_ilp_solver;
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
        get_solver_osi()->messageHandler()->setLogLevel(p_level);
    }

    void ILPSolverOsi::set_max_seconds        (double)
    {
        // Not supported by OsiSolverInterface.
    }
}

#endif
