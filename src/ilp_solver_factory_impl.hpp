#ifndef _ILP_SOLVER_FACTORY_IMPL_HPP
#define _ILP_SOLVER_FACTORY_IMPL_HPP

#include "ilp_solver_interface.hpp"

#include "IlpSolverDll.hpp"

namespace ilp_solver
{
    extern "C" ILP_SOLVER_DLL_API ILPSolverInterface* APIENTRY create_cbc_solver_pointer();
    extern "C" ILP_SOLVER_DLL_API void APIENTRY destroy_solver(ILPSolverInterface* p_solver);

    struct SolverDeleter
    {
        void operator()(ILPSolverInterface* p_solver) const
        {
            destroy_solver(p_solver);   // instead of calling delete, ask the DLL to delete the pointer
        }
    };
}

#endif
