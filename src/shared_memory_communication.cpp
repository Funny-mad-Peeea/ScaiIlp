#include "shared_memory_communication.hpp"

#include "serialization.hpp"

using namespace boost::interprocess;

namespace ilp_solver
{
    /***************************************
    * (De-) Serialization of solution data *
    ***************************************/
    static void serialize_result(Serializer* v_serializer, const ILPSolutionData& p_solution_data)
    {
        *v_serializer << p_solution_data.solution
                      << p_solution_data.objective
                      << p_solution_data.solution_status;
    }


    static void deserialize_result(Deserializer* v_deserializer, ILPSolutionData* r_solution_data)
    {
        *v_deserializer >> r_solution_data->solution
                        >> r_solution_data->objective
                        >> r_solution_data->solution_status;
    }

    
    /**********************************
    * (De-) Serialization of ILP data *
    **********************************/
    static void* serialize_ilp_data(Serializer* v_serializer, const ILPData& p_data, const ILPSolutionData& p_solution_data)
    {
        *v_serializer << p_data.matrix
                      << p_data.objective
                      << p_data.variable_lower
                      << p_data.variable_upper
                      << p_data.constraint_lower
                      << p_data.constraint_upper
                      << p_data.variable_type
                      << p_data.objective_sense
                      << p_data.num_threads
                      << p_data.log_level
                      << p_data.max_seconds;
                      
        auto result_address = v_serializer->current_address();

        serialize_result(v_serializer, p_solution_data);

        return result_address; 
    }


    static void* deserialize_ilp_data(Deserializer* v_deserializer, ILPData* r_data)
    {
        *v_deserializer >> r_data->matrix
                        >> r_data->objective
                        >> r_data->variable_lower
                        >> r_data->variable_upper
                        >> r_data->constraint_lower
                        >> r_data->constraint_upper
                        >> r_data->variable_type
                        >> r_data->objective_sense
                        >> r_data->num_threads
                        >> r_data->log_level
                        >> r_data->max_seconds;

        return v_deserializer->current_address();
    }


    static int determine_required_size(const ILPData& p_data, const ILPSolutionData& p_solution_data)
    {
        Serializer serializer(nullptr);
        serialize_ilp_data(&serializer, p_data, p_solution_data);
        return serializer.required_bytes();
    }


    static void* serialize_ilp_data(void* p_address, const ILPData& p_data)
    {
        Serializer serializer(p_address);
        return serialize_ilp_data(&serializer, p_data, ILPSolutionData());
    }


    /***********************
    * Parent communication *
    ***********************/
    ParentCommunication::ParentCommunication(std::string& p_shared_memory_name)
        : d_shared_memory(nullptr),
          d_mapped_region(nullptr),
          d_address(nullptr),
          d_result_address(nullptr)
        {}


    ParentCommunication::~ParentCommunication()
    {
        delete d_shared_memory;
        delete d_mapped_region;
    }


    void ParentCommunication::create_shared_memory(int p_size)
    {
        if (d_shared_memory != nullptr)
            return;

        d_shared_memory = new windows_shared_memory(create_only, d_shared_memory_name.c_str(), read_write, p_size);
        d_mapped_region = new mapped_region(*d_shared_memory, read_write);
        d_address = d_mapped_region->get_address();
    }


    void ParentCommunication::write_ilp_data(const ILPData& p_data, const ILPSolutionData& p_solution_data)
    {
        const auto size = determine_required_size(p_data, p_solution_data);
        create_shared_memory(size);
        d_result_address = serialize_ilp_data(d_address, p_data);
    }


    void ParentCommunication::read_solution_data(ILPSolutionData* r_solution_data)
    {
        Deserializer deserializer(d_result_address);
        deserialize_result(&deserializer, r_solution_data);
    }


    /**********************
    * Child communication *
    **********************/
    ChildCommunication::ChildCommunication(std::string& p_shared_memory_name)
        : d_shared_memory(open_only, p_shared_memory_name.c_str(), read_write),
          d_mapped_region(d_shared_memory, read_write),
          d_address(d_mapped_region.get_address()),
          d_result_address(nullptr)
        {}


    void ChildCommunication::read_ilp_data(ILPData* r_data)
    {
        Deserializer deserializer(d_address);
        d_result_address = deserialize_ilp_data(&deserializer, r_data);
    }


    void ChildCommunication::write_solution_data(const ILPSolutionData& p_solution_data)
    {
        Serializer serializer(d_result_address);
        serialize_result(&serializer, p_solution_data);
    }
}
