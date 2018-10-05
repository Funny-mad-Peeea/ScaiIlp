#ifndef _ILP_SOLVER_FACTORY_HPP
#define _ILP_SOLVER_FACTORY_HPP

#include "ilp_solver_interface.hpp"


#if (WITH_CBC == 1)
#define MAY_USE_CBC
#else
#define MAY_USE_CBC [[deprecated("CBC is deactivated. You can not create a CBC Solver.")]]
#endif

namespace ilp_solver
{
    extern "C" MAY_USE_CBC ILPSolverInterface* __stdcall create_solver_cbc();
    extern "C"             ILPSolverInterface* __stdcall create_solver_stub(const char* p_executable_basename);

    extern "C" void __stdcall destroy_solver(ILPSolverInterface* p_solver);
}

#undef MAY_USE_CBC

#endif
