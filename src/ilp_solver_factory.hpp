#ifndef _ILP_SOLVER_FACTORY_HPP
#define _ILP_SOLVER_FACTORY_HPP

#include "ilp_solver_interface.hpp"

#include "IlpSolverDll.hpp"

#include <memory>

namespace ilp_solver
{
    // Do not call this explicitly:
    extern "C" ILP_SOLVER_DLL_API ILPSolverInterface* APIENTRY create_cbc_solver_pointer();
    extern "C" ILP_SOLVER_DLL_API void APIENTRY destroy_solver(ILPSolverInterface* p_solver);

    struct SolverDeleter
    {
        void operator()(ILPSolverInterface* p_solver) const
        {
            destroy_solver(p_solver);   // instead of calling delete, ask the DLL to delete the pointer
        }
    };

    /**********************************
    * (Logically) published interface *
    **********************************/
    typedef std::unique_ptr<ILPSolverInterface, SolverDeleter> UniquePtrSolver;
    UniquePtrSolver create_cbc_solver()
    {
        return UniquePtrSolver(create_cbc_solver_pointer());
    }
}

#endif
