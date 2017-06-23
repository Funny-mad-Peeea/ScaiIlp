#ifndef _ILP_SOLVER_INTERFACE_T_HPP
#define _ILP_SOLVER_INTERFACE_T_HPP

#include <functional>
#include <string>

namespace ilp_solver
{
    class ILPSolverInterface;

    void execute_test_and_destroy_solver(ILPSolverInterface* p_solver, const std::string& p_solver_name, std::function<void(ILPSolverInterface*, const std::string&)> p_test);

    // For stub solver only
    void test_bad_alloc(ILPSolverInterface* p_solver, const std::string& p_solver_name);
}

#endif
