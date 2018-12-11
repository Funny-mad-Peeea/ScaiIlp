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
        // Inner vectors are rows/constraints. The size of the outer vector is the number of constraints.
        // The size of the inner vectors is the number of variables.
        // If no constraints are given, we can not know the number of variables. (m x 0 can be stored, 0 x n can not).
        using Matrix = std::vector<std::vector<double>>;

        Matrix              matrix;
        std::vector<double> objective;
        std::vector<double> variable_lower;
        std::vector<double> variable_upper;
        std::vector<double> constraint_lower;
        std::vector<double> constraint_upper;
        std::vector<VariableType> variable_type;
        ObjectiveSense objective_sense{ ObjectiveSense::MINIMIZE };

        std::vector<double> start_solution;

        // Defaults will be overwritten in ilp_solver_collect.
        int    num_threads    { c_default_num_threads   };
        bool   deterministic  { c_default_deterministic };
        int    log_level      { c_default_log_level     };
        double max_seconds    { c_default_max_seconds   };
        bool   presolve       { c_default_presolve      };

        ILPData() = default;
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
