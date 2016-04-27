#ifndef _SOLVER_EXIT_CODE_HPP
#define _SOLVER_EXIT_CODE_HPP

enum SolverExitCode 
{
    ok = 0,

    uncaught_exception = 3,

    out_of_memory       = 14142,    // arbitrary value that is unlikely to be used by Windows for internal codes
    command_line_error,
    shared_memory_error,
    model_error,
    solver_error,
    forced_termination
};

#endif
