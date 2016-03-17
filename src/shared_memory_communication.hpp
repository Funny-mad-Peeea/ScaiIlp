#ifndef _SHARED_MEMORY_COMMUNICATION_HPP
#define _SHARED_MEMORY_COMMUNICATION_HPP

#include <memory>
#include <string>
#include <vector>

#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>

class ParentCommunication
{
    public:
        ParentCommunication(std::string& p_memory_name);
        ~ParentCommunication();

        void write_ilp_data();
        void read_solution_data();

    private:
        std::string d_memory_name;
        boost::interprocess::windows_shared_memory* d_shared_memory;
        boost::interprocess::mapped_region* d_mapped_region;

        // non-owned pointer; do not delete
        void* d_address;
        void* d_result_address;

        void create_shared_memory(int p_size);
        int determine_required_size();
        void* serialize_ilp_data();
};


class ChildCommunication
{
    public:
        ChildCommunication(std::string& p_memory_name);

        void read_ilp_data();
        void write_solution_data();

    private:
        const boost::interprocess::windows_shared_memory d_shared_memory;
        const boost::interprocess::mapped_region d_mapped_region;

        // non-owned pointer; do not delete
        void* const d_address;
        void* d_result_address;
};

#endif
