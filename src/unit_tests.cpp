#include "ilp_solver_cbc_t.hpp"
#include "serialization_t.hpp"

void main()
{
    ilp_solver::test_sorting_cbc();
    ilp_solver::test_linear_programming_cbc();
    test_serialization();
}
