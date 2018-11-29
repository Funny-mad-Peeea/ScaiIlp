#ifndef _ILP_DATA_HPP
#define _ILP_DATA_HPP

#include "ilp_solver_interface.hpp"
#include "ilp_solver_impl.hpp"

#include <limits>
#include <vector>

namespace ilp_solver
{
    struct ILPData
    {
        // Inner vectors are rows/constraints, so every entry in the outer vector represents a variable.
        // Thus, variables not occuring in any constraints can be stored (m x 0),
        // but constraints without any variables can not (0 x n).
        std::vector< std::vector<double> > matrix;
        std::vector<double> objective;
        std::vector<double> variable_lower;
        std::vector<double> variable_upper;
        std::vector<double> constraint_lower;
        std::vector<double> constraint_upper;
        std::vector<VariableType> variable_type;
        ObjectiveSense objective_sense;

        std::vector<double> start_solution;

        int num_threads;
        bool deterministic;
        int log_level;
        double max_seconds;

        ILPData()
            : objective_sense{ ObjectiveSense::MINIMIZE }
            , num_threads    { c_default_num_threads }
            , deterministic  { c_default_deterministic }
            , log_level      { c_default_log_level }
            , max_seconds    { c_default_max_seconds }
        { }
    };


    struct ILPSolutionData
    {
        std::vector<double> solution;
        double objective;
        SolutionStatus solution_status;

        ILPSolutionData()
            : objective(std::numeric_limits<double>::quiet_NaN()),
              solution_status(SolutionStatus::NO_SOLUTION)
            {}

        explicit ILPSolutionData(ObjectiveSense p_objective_sense)
            : objective(p_objective_sense == ObjectiveSense::MINIMIZE ? std::numeric_limits<double>::max()
                                                                      : std::numeric_limits<double>::lowest()),
              solution_status(SolutionStatus::NO_SOLUTION)
            {}
    };
}

#endif
