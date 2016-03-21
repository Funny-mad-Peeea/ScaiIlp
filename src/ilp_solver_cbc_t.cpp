#include "ilp_solver_cbc_t.hpp"

#include "ilp_solver_interface_t.hpp"
#include "ilp_solver_factory.hpp"

namespace ilp_solver
{
    void test_sorting_cbc()
    {
        auto solver = create_cbc_solver();
        try
        {
            test_sorting(solver);
        }
        catch (...)
        {
            destroy_solver(solver);
            throw;
        }
        destroy_solver(solver);
    }

    void test_linear_programming_cbc()
    {
        auto solver = create_cbc_solver();
        try
        {
            test_linear_programming(solver);
        }
        catch (...)
        {
            destroy_solver(solver);
            throw;
        }
        destroy_solver(solver);
    }
}
