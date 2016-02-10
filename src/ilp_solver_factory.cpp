#include "ilp_solver_factory.hpp"

#include "ilp_solver_cbc.hpp"

namespace ilp_solver
{
    extern "C" ILP_SOLVER_DLL_API ILPSolverInterface* APIENTRY create_cbc_solver()
    {
        return new ILPSolverCbc();
    }

    extern "C" ILP_SOLVER_DLL_API void APIENTRY destroy_solver(ILPSolverInterface* p_solver)
    {
        delete p_solver;
    }

    extern "C" ILP_SOLVER_DLL_API ILPSolverException* APIENTRY create_exception()
    {
        return new ILPSolverExceptionImpl();
    }

    extern "C" ILP_SOLVER_DLL_API void APIENTRY destroy_exception(ILPSolverException* p_exception)
    {
        delete p_exception;
    }
}
