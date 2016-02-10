#ifndef _ILP_SOLVER_FACTORY_HPP
#define _ILP_SOLVER_FACTORY_HPP

#include "ilp_solver_interface.hpp"

#include "IlpSolverDll.hpp"

namespace ilp_solver
{
    extern "C" ILP_SOLVER_DLL_API ILPSolverInterface* APIENTRY create_cbc_solver();
    extern "C" ILP_SOLVER_DLL_API void APIENTRY destroy_solver(ILPSolverInterface* p_solver);
}

#endif
