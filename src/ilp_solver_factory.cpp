#include "ilp_solver_factory.hpp"

#include "ilp_solver_cbc.hpp"

namespace ilp_solver
{
    extern "C" ILPSolverInterface* __stdcall create_cbc_solver()
    {
        return new ILPSolverCbc();
    }

    extern "C" void __stdcall destroy_solver(ILPSolverInterface* p_solver)
    {
        delete p_solver;
    }
}
