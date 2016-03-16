#ifndef _SERIALIZATION_HPP
#define _SERIALIZATION_HPP

#include <vector>

/**********************************
* Serializes plain old data types *
* (POD types) and vectors         *
**********************************/
class Serializer
{
    public:
        // p_memory == nullptr simulates serialization. This allows calculating
        // the amount of required bytes without writing the data to a temporary
        // memory location.
        explicit Serializer(void* p_memory)
            : d_memory(static_cast<char*>(p_memory)),
              d_offset(static_cast<char*>(p_memory)),
              d_simulate(d_memory == nullptr ? true : false)
            {}

        int required_bytes() const
        {
            return d_offset - d_memory;
        }

        template<typename POD_type>             void serialize(const POD_type& p_value);
        template<typename POD_type>             void serialize(const std::vector<POD_type>& p_vector);
        template<typename POD_type_or_vector>   void serialize(const std::vector< std::vector<POD_type_or_vector> >& p_vector_of_vectors);

        template<typename POD_type>             void* reserve(const POD_type& p_value);
        template<typename POD_type>             void* reserve(const std::vector<POD_type>& p_vector);
        template<typename POD_type_or_vector>   void* reserve(const std::vector< std::vector<POD_type_or_vector> >& p_vector_of_vectors);

    private:
        const char* d_memory;
        char* d_offset;
        const bool d_simulate;
};


/************************************
* Deserializes plain old data types *
* (POD types) and vectors           *
************************************/
class Deserializer
{
    public:
        explicit Deserializer(void* p_memory) : d_offset(static_cast<char*>(p_memory)) {}

        void* offset() const { return d_offset; }

        template<typename POD_type>             void deserialize(POD_type* r_value);
        template<typename POD_type>             void deserialize(std::vector<POD_type>* r_vector);
        template<typename POD_type_or_vector>   void deserialize(std::vector< std::vector<POD_type_or_vector> >* r_vector_of_vectors);

    private:
        char* d_offset;
};


/*****************************************************
* Embedded DSL for serialization and deserialization *
*****************************************************/

// Simple serialization: serializer << xyz
// =======================================
template<typename Serializable>
Serializer& operator<<(Serializer& p_serializer, const Serializable& p_serializable)
{
    p_serializer.serialize(p_serializable);
    return p_serializer;
}


// Simple deserialization: deserializer >> xyz
// ===========================================
template<typename Deserializable>
Deserializer& operator>>(Deserializer& p_deserializer, Deserializable& p_deserializable)
{
    p_deserializer.deserialize(&p_deserializable);
    return p_deserializer;
}


// Reserving space: serializer << reserve(xyz[, offset])
// =====================================================
template<typename Serializable>
std::pair<const Serializable*, void**> reserve(const Serializable& p_serializable, void** p_offset = nullptr)
{
    return std::make_pair(&p_serializable, p_offset);
}


template<typename Serializable>
Serializer& operator<<(Serializer& p_serializer, const std::pair<const Serializable*, void**>& p_serializable_offset)
{
    auto offset = p_serializer.reserve(*(p_serializable_offset.first));
    if (p_serializable_offset.second != nullptr)
        *(p_serializable_offset.second) = offset;
    return p_serializer;
}


/*****************
* Implementation *
*****************/

// (De-) Serialization of a POD type
// =================================
template<typename POD_type>
void Serializer::serialize(const POD_type& p_value)
{
    const auto num_bytes = sizeof(POD_type);
    auto pointer = static_cast<POD_type*>(static_cast<void*>(d_offset));
    if (!d_simulate)
        *pointer = p_value;
    d_offset += num_bytes;
}


template<typename POD_type>
void* Serializer::reserve(const POD_type& p_value)
{
    const auto offset = d_offset;
    const auto num_bytes = sizeof(p_value);
    d_offset += num_bytes;
    return offset;
}


template<typename POD_type>
void Deserializer::deserialize(POD_type* r_value)
{
    const auto num_bytes = sizeof(POD_type);
    auto pointer = static_cast<POD_type*>(static_cast<void*>(d_offset));
    *r_value = *pointer;
    d_offset += num_bytes;
}


// (De-) Serialization of a POD type vector
// ========================================
template<typename POD_type>
void Serializer::serialize(const std::vector<POD_type>& p_vector)
{
    const auto vector_size = (int) p_vector.size();
    const auto reserved_size = vector_size;
    serialize(reserved_size);
    serialize(vector_size);
    const auto num_bytes = vector_size*sizeof(POD_type);
    const auto offset_data = d_offset;
    d_offset += num_bytes;
    if (!d_simulate)
        std::memcpy(offset_data, p_vector.data(), num_bytes);
}


template<typename POD_type>
void* Serializer::reserve(const std::vector<POD_type>& p_vector)
{
    const auto offset = d_offset;
    const auto vector_size = 0;
    const auto reserved_size = (int) p_vector.size();
    serialize(reserved_size);
    serialize(vector_size);
    const auto num_bytes = reserved_size*sizeof(POD_type);
    d_offset += num_bytes;
    return offset;
}


template<typename POD_type>
void Deserializer::deserialize(std::vector<POD_type>* r_vector)
{
    int reserved_size, vector_size;
    deserialize(&reserved_size);
    deserialize(&vector_size);
    const auto num_vector_bytes = vector_size*sizeof(POD_type);
    r_vector->resize(vector_size);
    std::memcpy(r_vector->data(), d_offset, num_vector_bytes);
    const auto num_reserved_bytes = reserved_size*sizeof(POD_type);
    d_offset += num_reserved_bytes;
}


// (De-) Serialization of a vector of vectors
// ==========================================
template<typename POD_type_or_vector>
void Serializer::serialize(const std::vector< std::vector<POD_type_or_vector> >& p_vector_of_vectors)
{
    const auto vector_size = (int) p_vector_of_vectors.size();
    const auto reserved_size = vector_size;
    serialize(reserved_size);
    serialize(vector_size);
    for (const auto& vector: p_vector_of_vectors)
        serialize(vector);
}


template<typename POD_type_or_vector>
void* Serializer::reserve(const std::vector< std::vector<POD_type_or_vector> >& p_vector_of_vectors)
{
    const auto offset = d_offset;
    const auto vector_size = 0;
    const auto reserved_size = (int) p_vector_of_vectors.size();
    serialize(reserved_size);
    serialize(vector_size);
    for (const auto& vector: p_vector_of_vectors)
        reserve(vector);
    return offset;
}


template<typename POD_type_or_vector>
void Deserializer::deserialize(std::vector< std::vector<POD_type_or_vector> >* r_vector_of_vectors)
{
    int reserved_size, vector_size;
    deserialize(&reserved_size);
    deserialize(&vector_size);
    r_vector_of_vectors->resize(reserved_size);
    for (auto& vector: *r_vector_of_vectors)
        deserialize(&vector);
    r_vector_of_vectors->resize(vector_size);
}

#endif
