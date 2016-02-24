#ifndef _ILP_SOLVER_OSI_HPP
#define _ILP_SOLVER_OSI_HPP

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

            OsiSolverInterface*       do_get_solver    ()       override { return d_ilp_solver; }
            const OsiSolverInterface* do_get_solver    () const override { return d_ilp_solver; }

            void                      do_solve         ()       override;
            const double*             do_get_solution  () const override;
            double                    do_get_objective () const override;
    };
}

#endif
