#ifndef _ILP_SOLVER_CBC_HPP
#define _ILP_SOLVER_CBC_HPP

#include "ilp_solver_osi_model.hpp"

#include "CbcModel.hpp"
#include "OsiClpSolverInterface.hpp"

class OsiSolverInterface;

namespace ILPSolver
{
    class ILPSolverCbc : public ILPSolverOsiModel
    {
        private:
            OsiClpSolverInterface d_clp_solver;
            CbcModel d_model;

            OsiSolverInterface*         solver              () override;
            const OsiSolverInterface*   solver              () const override;

            void                        do_solve            () override;
            const double*               do_get_solution     () const override;
            double                      do_get_objective    () const override;
            void                        do_set_num_threads  (int p_num_threads) override;
    };
}

#endif
