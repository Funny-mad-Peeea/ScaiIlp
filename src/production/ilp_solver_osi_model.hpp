#ifndef _ILP_SOLVER_OSI_MODEL_HPP
#define _ILP_SOLVER_OSI_MODEL_HPP

#if WITH_OSI == 1

// Link with the CoinUtils and Osi Libraries.
#pragma comment(lib, "libCoinUtils.lib")
#pragma comment(lib, "libOsi.lib")
#pragma comment(lib, "libOsiClp.lib")

#include "ilp_solver_impl.hpp"
#include "CoinModel.hpp"

#include <string>
#include <vector>

class OsiSolverInterface;

namespace ilp_solver
{
    // Implements all methods from ILPSolverInterfaceImpl that can be realized
    // via the pure problem model of the OsiSolverInterface.
    class ILPSolverOsiModel : public ILPSolverImpl
    {
        public:
            int  get_num_constraints() const override;
            int  get_num_variables  () const override;
        protected:
            ILPSolverOsiModel();

            void prepare_impl() override;

            // Const version is set here and does not need to be overwritten.
            const OsiSolverInterface* const_get_solver_osi_model() const;
        private:
            CoinModel d_cache{};
            bool      d_cache_changed{false};

            // Obtain a pointer to a solver fulfilling the OsiSolverInterface.
            virtual OsiSolverInterface* get_solver_osi_model() = 0;

            void add_variable_impl (VariableType p_type, double p_objective, double p_lower_bound, double p_upper_bound,
                [[maybe_unused]] const std::string& p_name = "", const std::vector<double>* p_row_values = nullptr,
                const std::vector<int>* p_row_indices = nullptr) override;

            void add_constraint_impl (double p_lower_bound, double p_upper_bound,
                const std::vector<double>& p_col_values, [[maybe_unused]] const std::string& p_name = "",
                const std::vector<int>* p_col_indices = nullptr) override;

            std::pair<double, double> get_infinity_impl() override;
    };
}

#endif

#endif
