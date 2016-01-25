#ifndef _ILP_SOLVER_FACTORY_HPP
#define _ILP_SOLVER_FACTORY_HPP

#include "ilp_solver_interface.hpp"

#include "IlpSolverDll.hpp"

#include <memory>

namespace ilp_solver
{
    // Do not call this explicitly:
    ILP_SOLVER_DLL_API void destroy_solver(ILPSolverInterface* p_solver);

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
    ILP_SOLVER_DLL_API UniquePtrSolver create_cbc_solver();
    
    ILP_SOLVER_DLL_API void create_exception(const std::string& p_exception_message);   // for testing purpose
}

#endif
