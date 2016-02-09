#ifndef _ILP_SOLVER_FACTORY_HPP
#define _ILP_SOLVER_FACTORY_HPP

#include "ilp_solver_factory_impl.hpp"

#include <memory>

namespace ilp_solver
{
    typedef std::unique_ptr<ILPSolverInterface, SolverDeleter> UniquePtrSolver;
    UniquePtrSolver create_cbc_solver();
}

#endif
