#ifndef _SOLVER_EXIT_CODE_HPP
#define _SOLVER_EXIT_CODE_HPP

enum SolverExitCode 
{
    ok = 0,
    out_of_memory       = 14142,
    command_line_error,
    shared_memory_error,
    model_error,
    solver_error,
    forced_termination,

    unknown_error       = 14200
};

#endif
