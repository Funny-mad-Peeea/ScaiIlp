#include "ilp_solver_cbc_t.hpp"

#include "ilp_solver_interface_t.hpp"
#include "ilp_solver_factory.hpp"

namespace ILPSolver
{
    void test_sorting_cbc()
    {
        test_sorting(create_cbc_solver().get());
    }

    void test_linear_programming_cbc()
    {
        test_linear_programming(create_cbc_solver().get());
    }
}