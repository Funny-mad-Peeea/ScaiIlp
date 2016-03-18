#include "ilp_solver_stub.hpp"

#include "shared_memory_communication.hpp"

namespace ilp_solver
{
    static int execute_solver(const std::string& p_executable_name, const std::string& p_shared_memory_name)
    {
        auto quote = [](const std::string& p_string)
        {
            return '"' + p_string + '"';
        };
        const auto command_line = quote(p_executable_name) + ' ' + quote(p_shared_memory_name);
        return std::system(command_line.c_str());
    }


    ILPSolverStub::ILPSolverStub(const std::string& p_executable_name, const std::string& p_shared_memory_name)
        : d_executable_name(p_executable_name),
          d_shared_memory_name(p_shared_memory_name)          
        {}


    const double* ILPSolverStub::do_get_solution() const
    {
        return d_ilp_solution_data.solution.data();
    }


    double ILPSolverStub::do_get_objective() const
    {
        return d_ilp_solution_data.objective;
    }


    SolutionStatus ILPSolverStub::do_get_status() const
    {
        return d_ilp_solution_data.solution_status;
    }


    void ILPSolverStub::do_solve(const ILPData& p_data)
    {
        d_ilp_solution_data.objective = (p_data.objective_sense == ObjectiveSense::MINIMIZE
                                                                 ? std::numeric_limits<double>::max()
                                                                 : std::numeric_limits<double>::lowest());
        ParentCommunication communicator(d_shared_memory_name);

        d_ilp_solution_data.solution.resize(p_data.num_variables());

        communicator.write_ilp_data(p_data, d_ilp_solution_data);

        auto exit_code = execute_solver(d_executable_name, d_shared_memory_name);
        if (exit_code != 0)
            d_ilp_solution_data.solution.clear();
        else
            communicator.read_solution_data(&d_ilp_solution_data);
    }
}
