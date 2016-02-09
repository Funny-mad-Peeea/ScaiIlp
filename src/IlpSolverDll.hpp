#ifndef _ILP_SOLVER_DLL_HPP
#define _ILP_SOLVER_DLL_HPP

#define APIENTRY __stdcall

#ifdef ILP_SOLVER_DLL_EXPORTS
    #define ILP_SOLVER_DLL_API __declspec(dllexport) 
#else
    #define ILP_SOLVER_DLL_API __declspec(dllimport)
#endif

#endif