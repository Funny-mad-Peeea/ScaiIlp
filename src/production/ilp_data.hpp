#ifndef _ILP_DATA_HPP
#define _ILP_DATA_HPP

#include <limits>
#include <vector>

namespace ilp_solver
{
    enum class VariableType   { INTEGER, CONTINUOUS };
    enum class ObjectiveSense { MINIMIZE, MAXIMIZE };
    enum class SolutionStatus { PROVEN_OPTIMAL, PROVEN_INFEASIBLE, SUBOPTIMAL, NO_SOLUTION };

    struct ILPData
    {
        std::vector< std::vector<double> > matrix;  // Note: mx0 matrices can be stored, but 0xn matrices cannot.
        std::vector<double> objective;
        std::vector<double> variable_lower;
        std::vector<double> variable_upper;
        std::vector<double> constraint_lower;
        std::vector<double> constraint_upper;
        std::vector<VariableType> variable_type;
        ObjectiveSense objective_sense;

        int num_threads;
        int log_level;
        double max_seconds;

        ILPData() : objective_sense(ObjectiveSense::MINIMIZE),
                    num_threads(0), log_level(0), max_seconds(std::numeric_limits<double>::max()) {}
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
