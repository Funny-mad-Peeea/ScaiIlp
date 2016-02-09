#include "ilp_solver_factory.hpp"

namespace ilp_solver
{
    UniquePtrSolver create_cbc_solver()
    {
        return UniquePtrSolver(create_cbc_solver_pointer());
    }

    UniquePtrException create_exception()
    {
        return UniquePtrException(create_exception_pointer());
    }
}
