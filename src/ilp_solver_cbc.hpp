#ifndef _ILP_SOLVER_CBC_HPP
#define _ILP_SOLVER_CBC_HPP

#include "ilp_solver_osi_model.hpp"

#include "CbcModel.hpp"
#include "OsiClpSolverInterface.hpp"

class OsiSolverInterface;

namespace ilp_solver
{
    class ILPSolverCbc : public ILPSolverOsiModel
    {
        private:
            OsiClpSolverInterface d_clp_solver;
            CbcModel d_model;

            OsiSolverInterface*         do_get_solver              () override;
            const OsiSolverInterface*   do_get_solver              () const override;

            void                        do_solve            () override;
            const double*               do_get_solution     () const override;
            double                      do_get_objective    () const override;
    };
}

#endif
