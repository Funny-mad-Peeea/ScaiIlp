#include "ilp_solver_factory.hpp"

#include "ilp_solver_cbc.hpp"

using std::unique_ptr;

namespace ILPSolver
{
    template<class T, class Deleter>
    unique_ptr<T, Deleter> make_unique()
    {
        return unique_ptr<T, Deleter>(new T());
    }

    unique_ptr<ILPSolverInterface, SolverDeleter> create_cbc_solver()
    {
        return make_unique<ILPSolverCbc, SolverDeleter>();
    }

    void destroy_solver(ILPSolverInterface* p_solver)
    {
        delete p_solver;
    }
}
