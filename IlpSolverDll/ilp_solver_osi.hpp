#ifndef _ILP_SOLVER_OSI_HPP
#define _ILP_SOLVER_OSI_HPP

#include "ilp_solver_osi_model.hpp"

class OsiSolverInterface;

namespace ILPSolver
{
    // Wrapper class for all ILP solvers that implement the OsiSolverInterface
    class ILPSolverOsi: public ILPSolverOsiModel
    {
    public:
        ILPSolverOsi(OsiSolverInterface& p_ilp_solver);

        private:
            OsiSolverInterface& d_ilp_solver;

            OsiSolverInterface&         solver              () override         { return d_ilp_solver; }
            const OsiSolverInterface&   solver              () const override   { return d_ilp_solver; }

            void                        do_solve            () override;
            const double*               do_get_solution     () const override;
            double                      do_get_objective    () const override;
    };
}

#endif
