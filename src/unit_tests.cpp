#include "ilp_solver_cbc_t.hpp"
#include "ilp_solver_stub_t.hpp"
#include "serialization_t.hpp"

void main()
{
    ilp_solver::test_sorting_cbc();
    ilp_solver::test_linear_programming_cbc();

    test_serialization();

    ilp_solver::test_sorting_cbc_stub();
    ilp_solver::test_linear_programming_cbc_stub();
}
