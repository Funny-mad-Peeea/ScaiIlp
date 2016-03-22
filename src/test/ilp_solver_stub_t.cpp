#include "ilp_solver_stub_t.hpp"

#include "ilp_solver_interface_t.hpp"
#include "ilp_solver_stub.hpp"

const auto c_solver_name = "ScaiCbc.exe";
const auto c_shared_memory_name = "ILP_SOLVER_SHARED_MEMORY";

namespace ilp_solver
{
    void test_sorting_cbc_stub()
    {
        ILPSolverStub solver(c_solver_name, c_shared_memory_name);
        test_sorting(&solver);
    }

    void test_linear_programming_cbc_stub()
    {
        ILPSolverStub solver(c_solver_name, c_shared_memory_name);
        test_linear_programming(&solver);
    }
}
