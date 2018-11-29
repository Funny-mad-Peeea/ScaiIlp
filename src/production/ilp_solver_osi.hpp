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

            OsiSolverInterface* get_solver_osi         ()                     override;

            void                set_num_threads        (int p_num_threads)    override;
            void                set_deterministic_mode (bool p_deterministic) override;
            void                set_log_level          (int p_level)          override;
            void                set_max_seconds        (double p_seconds)     override;
    };
}

#endif

#endif
