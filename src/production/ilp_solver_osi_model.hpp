#ifndef _ILP_SOLVER_OSI_MODEL_HPP
#define _ILP_SOLVER_OSI_MODEL_HPP

#if WITH_OSI == 1

// Link with the CoinUtils and Osi Libraries.
#pragma comment(lib, "libCoinUtils.lib")
#pragma comment(lib, "libOsi.lib")
#pragma comment(lib, "libOsiClp.lib")

#include "ilp_solver_impl.hpp"

#include "CoinPackedMatrix.hpp"
#include "CoinModel.hpp"

#include <string>
#include <vector>

class OsiSolverInterface;

namespace ilp_solver
{
    // Implements all methods from ILPSolverInterfaceImpl that can be realized
    // via the model of the OsiSolverInterface.
    // If inheriting from ILPSolverOsiModel, you only need to implement
    //     get_solver
    //     set_num_threads
    //     set_deterministic_mode
    //     set_log_level
    //     set_max_seconds
    // although you might want to override
    //     get_solution
    //     set_start_solution
    //     get_objective
    //     get_status
    //     solve_impl
    //     set_objective_sense_impl
    //     set_infinity
    // for the concrete solver.
    class ILPSolverOsiModel : public ILPSolverImpl
    {
        public:
            ILPSolverOsiModel();

            std::vector<double> get_solution       () const                                override;
            void                set_start_solution (const std::vector<double>& p_solution) override;
            double              get_objective      () const                                override;
            SolutionStatus      get_status         () const                                override;
            int                 get_num_constraints() const                                override;
            int                 get_num_variables  () const                                override;
        protected:
            void prepare_impl() override;
        private:
            CoinModel d_cache{};
            bool      d_cache_changed{false};

            // Obtain a pointer to a solver fulfilling the OsiSolverInterface.
            virtual OsiSolverInterface* get_solver_osi() = 0;
              const OsiSolverInterface* get_solver_osi() const;

            void add_variable_impl (VariableType p_type, double p_objective, double p_lower_bound, double p_upper_bound,
                [[maybe_unused]] const std::string& p_name = "", const std::vector<double>* p_row_values = nullptr,
                const std::vector<int>* p_row_indices = nullptr) override;

            void add_constraint_impl (double p_lower_bound, double p_upper_bound,
                const std::vector<double>& p_col_values, [[maybe_unused]] const std::string& p_name = "",
                const std::vector<int>* p_col_indices = nullptr) override;

            void solve_impl() override;
            void set_objective_sense_impl(ObjectiveSense p_sense) override;
            std::pair<double, double> get_infinity_impl() override;
    };
}

#endif

#endif
