#ifndef _ILP_SOLVER_FACTORY_HPP
#define _ILP_SOLVER_FACTORY_HPP

#include "ilp_solver_interface.hpp"

#include "IlpSolverDll.hpp"

#include <memory>

namespace ILPSolver
{
    ILP_SOLVER_DLL_API std::unique_ptr<ILPSolverInterface> create_cbc_solver();
}

#endif
