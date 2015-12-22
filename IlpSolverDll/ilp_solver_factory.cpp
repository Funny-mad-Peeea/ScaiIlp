#include "ilp_solver_factory.hpp"

#include "ilp_solver_cbc.hpp"

using std::unique_ptr;

namespace ILPSolver
{
    template<class T>
    unique_ptr<T> make_unique()
    {
        return unique_ptr<T>(new T());
    }

    unique_ptr<ILPSolverInterface> create_cbc_solver()
    {
        return make_unique<ILPSolverCbc>();
    }
}
