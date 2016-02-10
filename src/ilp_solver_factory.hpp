#ifndef _ILP_SOLVER_FACTORY_HPP
#define _ILP_SOLVER_FACTORY_HPP

#include "ilp_solver_exception.hpp"
#include "ilp_solver_interface.hpp"

namespace ilp_solver
{
    extern "C" ILPSolverInterface* __stdcall create_cbc_solver();
    extern "C" void __stdcall destroy_solver(ILPSolverInterface* p_solver);

    // For tests only
    extern "C" ILPSolverException* __stdcall create_exception();
    extern "C" void __stdcall destroy_exception(ILPSolverException* p_exception);
}

#endif
