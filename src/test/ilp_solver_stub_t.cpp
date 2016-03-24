#include "ilp_solver_stub_t.hpp"

#include "ilp_solver_interface_t.hpp"
#include "ilp_solver_factory.hpp"

const auto c_solver_name = "ScaiIlp.exe";
const auto c_shared_memory_name = "ILP_SOLVER_SHARED_MEMORY";

namespace ilp_solver
{
    static ILPSolverInterface* create_solver()
    {
        return create_stub_solver(c_solver_name, c_shared_memory_name);
    }


    void test_sorting_cbc_stub()
    {
        execute_test_and_destroy_solver(create_solver(), "Cbc Stub Solver", test_sorting);
    }


    void test_linear_programming_cbc_stub()
    {
        execute_test_and_destroy_solver(create_solver(), "Cbc Stub Solver", test_linear_programming);
    }


    void test_bad_alloc_cbc_stub()
    {
        execute_test_and_destroy_solver(create_solver(), "Cbc Stub Solver", test_bad_alloc);
    }
}
