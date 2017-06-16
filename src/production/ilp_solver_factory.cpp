#include "ilp_solver_factory.hpp"

#include "ilp_solver_cbc.hpp"
#include "ilp_solver_stub.hpp"

namespace ilp_solver
{
    extern "C" ILPSolverInterface* __stdcall create_solver_cbc()
    {
        return new ILPSolverCbc();
    }

    extern "C" ILPSolverInterface* __stdcall create_solver_stub(const char* p_executable_basename)
    {
        return new ILPSolverStub(p_executable_basename);
    }

    extern "C" void __stdcall destroy_solver(ILPSolverInterface* p_solver)
    {
        delete p_solver;
    }

    extern "C" ILPSolverException* __stdcall create_exception()
    {
        return new ILPSolverExceptionImpl();
    }

    extern "C" void __stdcall destroy_exception(ILPSolverException* p_exception)
    {
        delete p_exception;
    }
}
