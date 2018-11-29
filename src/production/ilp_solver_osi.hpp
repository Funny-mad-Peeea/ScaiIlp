#ifndef _ILP_SOLVER_OSI_HPP
#define _ILP_SOLVER_OSI_HPP

#if WITH_OSI == 1

#include "ilp_solver_osi_model.hpp"

class OsiSolverInterface;

namespace ilp_solver
{
    // Wrapper class for all ILP solvers that implement the OsiSolverInterface
    class ILPSolverOsi: public ILPSolverOsiModel
    {
        public:
            explicit ILPSolverOsi(OsiSolverInterface* p_ilp_solver);

        private:
            OsiSolverInterface* d_ilp_solver;

            OsiSolverInterface* get_solver_osi_model   ()                                      override;

            std::vector<double> get_solution            () const                                override;
            void                set_start_solution      (const std::vector<double>& p_solution) override;
            double              get_objective           () const                                override;
            SolutionStatus      get_status              () const                                override;

            void                set_num_threads         (int p_num_threads)                     override;
            void                set_deterministic_mode  (bool p_deterministic)                  override;
            void                set_log_level           (int p_level)                           override;
            void                set_max_seconds         (double p_seconds)                      override;

            void                solve_impl              ()                                      override;
            void                set_objective_sense_impl(ObjectiveSense p_sense)                override;
    };
}

#endif

#endif
