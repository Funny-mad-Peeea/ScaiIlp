#ifndef _SHARED_MEMORY_COMMUNICATION_HPP
#define _SHARED_MEMORY_COMMUNICATION_HPP

#include "ilp_data.hpp"

#include <memory>
#include <string>
#include <vector>

#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>

namespace ilp_solver
{
    class ParentCommunication
    {
        public:
            ParentCommunication(std::string& p_shared_memory_name);
            ~ParentCommunication();

            void write_ilp_data(const ILPData& p_data, const ILPSolutionData& p_solution_data);
            void read_solution_data(ILPSolutionData* r_solution_data);

        private:
            std::string d_shared_memory_name;
            boost::interprocess::windows_shared_memory* d_shared_memory;
            boost::interprocess::mapped_region* d_mapped_region;

            // non-owned pointer; do not delete
            void* d_address;
            void* d_result_address;

            void create_shared_memory(int p_size);
    };


    class ChildCommunication
    {
        public:
            ChildCommunication(std::string& p_shared_memory_name);

            void read_ilp_data(ILPData* r_data);
            void write_solution_data(const ILPSolutionData& p_solution_data);

        private:
            const boost::interprocess::windows_shared_memory d_shared_memory;
            const boost::interprocess::mapped_region d_mapped_region;

            // non-owned pointer; do not delete
            void* const d_address;
            void* d_result_address;
    };
}

#endif
