#include "ilp_solver_factory.hpp"

#include "ilp_solver_cbc.hpp"
#include "ilp_solver_stub.hpp"

namespace ilp_solver
{
    extern "C" ILPSolverInterface* __stdcall create_cbc_solver()
    {
        return new ILPSolverCbc();
    }

    extern "C" ILPSolverInterface* __stdcall create_stub_solver(const char* p_executable_basename, const char* p_shared_memory_name)
    {
        return new ILPSolverStub(p_executable_basename, p_shared_memory_name);
    }

    extern "C" void __stdcall destroy_solver(ILPSolverInterface* p_solver)
    {
        delete p_solver;
    }
}
