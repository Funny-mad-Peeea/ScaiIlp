#include "ilp_solver_cbc_t.hpp"

#include "ilp_solver_interface_t.hpp"
#include "ilp_solver_factory.hpp"

namespace ilp_solver
{
    void test_sorting_cbc()
    {
        execute_test_and_destroy_solver(create_cbc_solver(), "Cbc Solver", test_sorting);
    }

    void test_linear_programming_cbc()
    {
        execute_test_and_destroy_solver(create_cbc_solver(), "Cbc Solver", test_linear_programming);
    }

    void test_performance_cbc()
    {
        execute_test_and_destroy_solver(create_cbc_solver(), "Cbc Solver", test_performance);
    }
}
