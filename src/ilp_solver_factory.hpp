#ifndef _ILP_SOLVER_FACTORY_HPP
#define _ILP_SOLVER_FACTORY_HPP

#include "ilp_solver_interface.hpp"

namespace ilp_solver
{
    extern "C" ILPSolverInterface* __stdcall create_cbc_solver();
    extern "C" void __stdcall destroy_solver(ILPSolverInterface* p_solver);
}

#endif
