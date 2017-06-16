#ifndef _ILP_SOLVER_EXCEPTION_HPP
#define _ILP_SOLVER_EXCEPTION_HPP

#include <string>

namespace ilp_solver
{
    // For tests only

    struct ILPSolverException
    {
        virtual void throw_exception(const std::string& p_message) const = 0;
        virtual ~ILPSolverException() {}
    };


    struct ILPSolverExceptionImpl : ILPSolverException
    {
        void throw_exception(const std::string& p_message) const override
        {
            throw std::exception(p_message.c_str());
        }
    };
}

#endif
