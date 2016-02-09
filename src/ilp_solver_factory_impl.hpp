#ifndef _ILP_SOLVER_FACTORY_IMPL_HPP
#define _ILP_SOLVER_FACTORY_IMPL_HPP

#include "ilp_solver_exception.hpp"
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

    // For tests only
    extern "C" ILP_SOLVER_DLL_API ILPSolverException* APIENTRY create_exception_pointer();
    extern "C" ILP_SOLVER_DLL_API void APIENTRY destroy_exception(ILPSolverException* p_exception);

    struct ExceptionDeleter
    {
        void operator()(ILPSolverException* p_exception) const
        {
            destroy_exception(p_exception); // instead of calling delete, ask the DLL to delete the pointer
        }
    };
}

#endif
