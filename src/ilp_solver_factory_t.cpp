#include "ilp_solver_factory_t.hpp"

#include "ilp_solver_factory.hpp"

#include <cassert>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

namespace ilp_solver
{
    void test_create_exception()
    {
        cout << endl
             << "Exception test" << endl
             << "==============" << endl
             << endl;

        const auto exception_message = string("This is a test exception.");
        try
        {
            const auto exception = ilp_solver::create_exception();
            
            cout << "Throwing the following exception: " << exception_message << endl;
            exception->throw_exception(exception_message);
            
            cout << "Error: Could not catch the exception." << endl;
            assert(false);
        }
        catch (std::exception& e)
        {
            const auto exception_message_2 = string(e.what());
            cout << "Caught the following exception: " << exception_message_2 << endl;
            assert(exception_message_2 == exception_message);
        }
    }
}
