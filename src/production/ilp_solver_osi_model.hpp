#ifndef _ILP_SOLVER_OSI_MODEL_HPP
#define _ILP_SOLVER_OSI_MODEL_HPP

#if WITH_OSI == 1

// Link with the CoinUtils and Osi Libraries.
#pragma comment(lib, "libCoinUtils.lib")
#pragma comment(lib, "libOsi.lib")
#pragma comment(lib, "libOsiClp.lib")

#include "ilp_solver_impl.hpp"

#include "CoinPackedMatrix.hpp"

#include <string>
#include <vector>

class OsiSolverInterface;

namespace ilp_solver
{
    // Implements all methods from ILPSolverInterfaceImpl that can be realized
    // via the model of the OsiSolverInterface.
    class ILPSolverOsiModel : public ILPSolverImpl
    {
        public:
            ILPSolverOsiModel();

            std::vector<double> get_solution       () const                                override;
            void                set_start_solution (const std::vector<double>& p_solution) override;

            double              get_objective      () const                                override;
            SolutionStatus      get_status         () const                                override;

        private:
            std::vector<int> d_rows;
            std::vector<int> d_cols;

            virtual OsiSolverInterface*       get_solver ()       = 0;

            const OsiSolverInterface* get_solver () const;
            void add_variable_impl (VariableType p_type, double p_objective, double p_lower_bound, double p_upper_bound,
                [[maybe_unused]] const std::string& p_name = "", const std::vector<double>* p_row_values = nullptr,
                const std::vector<int>* p_row_indices = nullptr) override;

            void add_constraint_impl (const double* p_lower_bound, const double* p_upper_bound,
                const std::vector<double>& p_col_values, [[maybe_unused]] const std::string& p_name = "",
                const std::vector<int>* p_col_indices = nullptr) override;

            void solve_impl() override;
            void set_objective_sense(ObjectiveSense p_sense) override;
    };
}

#endif

#endif
