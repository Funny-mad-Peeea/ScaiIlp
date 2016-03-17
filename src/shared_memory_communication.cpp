#include "shared_memory_communication.hpp"

#include "serialization.hpp"

using namespace boost::interprocess;

/**********************************
* (De-) Serialization of ILP data *
**********************************/
static void* serialize_ilp_data(Serializer* v_serializer)
{
    void* result_address;
    
    /* Serialize data here */
    *v_serializer >> &result_address;

    return result_address; 
}


static void* deserialize_ilp_data(Deserializer* v_deserializer)
{
    void* result_address;

    /* Deserialize data here */
    *v_deserializer >> &result_address;

    return result_address;
}


/***************************************
* (De-) Serialization of solution data *
***************************************/
static void serialize_result(Serializer* v_serializer)
{
    /* Serialize data here */
}


static void deserialize_result(Deserializer* v_deserializer)
{
    /* Deserialize data here */
}


/***********************
* Parent communication *
***********************/
ParentCommunication::ParentCommunication(std::string& p_memory_name)
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

    d_shared_memory = new windows_shared_memory(create_only, d_memory_name.c_str(), read_write, p_size);
    d_mapped_region = new mapped_region(*d_shared_memory, read_write);
    d_address = d_mapped_region->get_address();
}


int ParentCommunication::determine_required_size()
{
    Serializer serializer(nullptr);
    ::serialize_ilp_data(&serializer);
    return serializer.required_bytes();
}


void* ParentCommunication::serialize_ilp_data()
{
    Serializer serializer(d_address);
    return ::serialize_ilp_data(&serializer);
}


void ParentCommunication::write_ilp_data()
{
    const auto size = determine_required_size();
    create_shared_memory(size);
    d_result_address = serialize_ilp_data();
}


void ParentCommunication::read_solution_data()
{
    Deserializer deserializer(d_result_address);
    deserialize_result(&deserializer);
}


/**********************
* Child communication *
**********************/
ChildCommunication::ChildCommunication(std::string& p_memory_name)
    : d_shared_memory(open_only, p_memory_name.c_str(), read_write),
      d_mapped_region(d_shared_memory, read_write),
      d_address(d_mapped_region.get_address()),
      d_result_address(nullptr)
    {}


void ChildCommunication::read_ilp_data()
{
    Deserializer deserializer(d_address);
    d_result_address = ::deserialize_ilp_data(&deserializer);
}


void ChildCommunication::write_solution_data()
{
    Serializer serializer(d_result_address);
    serialize_result(&serializer);
}
