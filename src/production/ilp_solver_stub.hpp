#ifndef _ILP_SOLVER_STUB_HPP
#define _ILP_SOLVER_STUB_HPP

#include "ilp_data.hpp"
#include "ilp_solver_collect.hpp"

#include <string>

namespace ilp_solver
{
    // Receives data about the ILP, writes it into shared memory,
    // and starts a new solver process that solves the ILP.
    class ILPSolverStub : public ILPSolverCollect
    {
        public:
            ILPSolverStub(const std::string& p_executable_basename, const std::string& p_shared_memory_name);

        private:
            std::string d_executable_basename;
            std::string d_shared_memory_name;

            ILPSolutionData d_ilp_solution_data;

            void do_solve(const ILPData& p_data) override;

            const double*  do_get_solution  () const override;
            double         do_get_objective () const override;
            SolutionStatus do_get_status    () const override;
    };
}

#endif
