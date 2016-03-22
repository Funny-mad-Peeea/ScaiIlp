#include "ilp_solver_stub_t.hpp"

#include "ilp_solver_interface_t.hpp"
#include "ilp_solver_stub.hpp"

#include <algorithm>
#include <cassert>

const auto c_solver_name = "ScaiCbc.exe";
const auto c_shared_memory_name = "ILP_SOLVER_SHARED_MEMORY";

namespace ilp_solver
{
    /*********************
    * Internal functions *
    *********************/
    static double rand_double()
    {
        return -0.5 + (1.0*rand())/RAND_MAX;
    }


    static void test_bad_alloc()
    {
        srand(3);

        ILPSolverStub solver(c_solver_name, c_shared_memory_name);

        // It is not clear that this is sufficient to provoke a bad_alloc
        const auto variable_scaling = 10.0;
        const auto num_variables = 500000;
        for (auto j = 0; j < num_variables; ++j)
            solver.add_variable_integer(rand_double(), variable_scaling*rand_double(), variable_scaling*(1.0 + rand_double()));

        const auto constraint_scaling = num_variables*variable_scaling;
        const auto num_constraints = 100;
        std::vector<double> constraint_vector(num_variables);
        for (auto i = 0; i < num_constraints; ++i)
        {
            std::generate(std::begin(constraint_vector), std::end(constraint_vector), [](){ return rand_double(); });
            solver.add_constraint(constraint_vector, constraint_scaling*rand_double(), constraint_scaling*(1.0 + rand_double()));
        }

        try
        {
            // bad_alloc should be treated as "no solution"
            solver.minimize();
            assert(solver.get_status() == SolutionStatus::NO_SOLUTION);
            assert(solver.get_solution().size() == 0);
        }
        catch (...)
        {
            assert(false);
        }

    }

    /**********************
    * Published functions *
    **********************/
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


    void test_cbc_stub_exception_handling()
    {
        test_bad_alloc();
    }
}
