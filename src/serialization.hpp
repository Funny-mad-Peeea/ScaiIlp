#ifndef _SERIALIZATION_HPP
#define _SERIALIZATION_HPP

#include <memory>
#include <vector>

/**************************
* Byte management classes *
* used in MemoryAligner   *
**************************/
class ByteVector
{
    public:
        virtual int    size   ()                 const = 0;
        virtual void   resize (int p_size)             = 0;
        virtual void*  pos    (int p_offset = 0) const = 0;
};


class NonOwnedByteVector : public ByteVector
{
    public:
        NonOwnedByteVector(void* p_memory) : d_memory(static_cast<char*>(p_memory)) {}

        int    size   ()             const override { return std::numeric_limits<int>::max(); }
        void   resize (int p_size)         override { throw std::logic_error("Cannot resize a non-owned byte vector."); }
        void*  pos    (int p_offset) const override { return d_memory + p_offset; }
    private:
        char* const d_memory;
};


class OwnedByteVector : public ByteVector
{
    public:
        int    size   ()             const override { return (int) d_vector.size(); }
        void   resize (int p_size)         override { d_vector.resize(p_size); }
        void*  pos    (int p_offset) const override { return const_cast<char*>(d_vector.data() + p_offset); }

    private:
        std::vector<char> d_vector;
};

/***************************************************************
* Serializes and deserializes plain old data types (POD types) *
* and vectors and maps them to a specific memory region.       *
***************************************************************/
class MemoryAligner
{
    public:
        MemoryAligner() :               d_offset(0), d_memory(new OwnedByteVector()),            d_owned_buffer(true)  {}
        MemoryAligner(void* p_memory) : d_offset(0), d_memory(new NonOwnedByteVector(p_memory)), d_owned_buffer(false) {}

        void set_offset(int p_offset)
        {
            d_offset = p_offset;
        }


        int required_bytes() const
        {
            if (!d_owned_buffer)
                throw std::logic_error("Cannot query number of required bytes of non-owned vector.");
            return d_memory->size();
        }


        void copy_memory(void* p_memory) const
        {
            if (!d_owned_buffer)
                throw std::logic_error("Copying a non-owned vector is valid, but indicates a logic error.");
            std::memcpy(p_memory, d_memory->pos(), d_memory->size());
        }

        // All functions return the value of d_offset before the function call

        // Serialization
        template<typename POD_type>             int serialize(const POD_type& p_value);
        template<typename POD_type>             int serialize(const std::vector<POD_type>& p_vector);
        template<typename POD_type_or_vector>   int serialize(const std::vector< std::vector<POD_type_or_vector> >& p_vector_of_vectors);

        // Like serialization, but the space is zeroed
        template<typename POD_type>             int reserve_and_zero(const POD_type& p_value);
        template<typename POD_type>             int reserve_and_zero(const std::vector<POD_type>& p_vector);
        template<typename POD_type_or_vector>   int reserve_and_zero(const std::vector< std::vector<POD_type_or_vector> >& p_vector_of_vectors);

        // Deserialization
        template<typename POD_type>             int deserialize(POD_type* r_value);
        template<typename POD_type>             int deserialize(std::vector<POD_type>* r_vector);
        template<typename POD_type_or_vector>   int deserialize(std::vector< std::vector<POD_type_or_vector> >* r_vector_of_vectors);

    private:
        int d_offset;
        std::unique_ptr<ByteVector> d_memory;
        bool d_owned_buffer;
};

/*****************************************************
* Embedded DSL for serialization and deserialization *
*****************************************************/

// Simple serialization: serializer << xyz
// =======================================
template<typename Serializable>
MemoryAligner& operator<<(MemoryAligner& p_serializer, const Serializable& p_serializable)
{
    p_serializer.serialize(p_serializable);
    return p_serializer;
}


// Simple deserialization: deserializer >> xyz
// ===========================================
template<typename Deserializable>
MemoryAligner& operator>>(MemoryAligner& p_deserializer, Deserializable& p_deserializable)
{
    p_deserializer.deserialize(&p_deserializable);
    return p_deserializer;
}


// Reserving space and storing the offset: serializer << reserve(xyz, offset)
// ==========================================================================
template<typename Serializable>
std::pair<const Serializable*, int*> reserve(const Serializable& p_serializable, int& p_offset)
{
    return std::make_pair<const Serializable*, int*>(&p_serializable, &p_offset);
}


template<typename Serializable>
MemoryAligner& operator<<(MemoryAligner& p_serializer, const std::pair<const Serializable*, int*>& p_serializable_offset)
{
    *(p_serializable_offset.second) = p_serializer.reserve_and_zero(*(p_serializable_offset.first));
    return p_serializer;
}


// Deserialization and storing the offset: deserializer >> store_offset(xyz, offset)
// =================================================================================
template<typename Deserializable>
std::pair<Deserializable*, int*> store_offset(Deserializable& p_deserializable, int& p_offset)
{
    return std::make_pair<Deserializable*, int*>(&p_deserializable, &p_offset);
}


template<typename Deserializable>
MemoryAligner& operator>>(MemoryAligner& p_deserializer, std::pair<Deserializable*, int*>& p_deserializable_offset)
{
    *(p_deserializable_offset.second) = p_deserializer.deserialize(p_deserializable_offset.first);
    return p_deserializer;
}


// (De-) Serialization at a certain offset: serializer << at_offset(xyz, offset) or deserializer >> at_offset(xyz, offset)
// =======================================================================================================================
template<typename DeSerializable>
std::pair<DeSerializable*, const int*> at_offset(DeSerializable& p_de_serializable, const int& p_offset)
{
    return std::make_pair<DeSerializable*, const int*>(&p_de_serializable, &p_offset);
}


template<typename Serializable>
MemoryAligner& operator<<(MemoryAligner& p_serializer, const std::pair<const Serializable*, const int*>& p_serializable_offset)
{
    p_serializer.set_offset(*(p_serializable_offset.second));
    p_serializer.serialize(*(p_serializable_offset.first));
    return p_serializer;
}


template<typename Deserializable>
MemoryAligner& operator>>(MemoryAligner& p_deserializer, const std::pair<Deserializable*, const int*>& p_deserializable_offset)
{
    p_deserializer.set_offset(*(p_deserializable_offset.second));
    p_deserializer.deserialize(p_deserializable_offset.first);
    return p_deserializer;
}


/*****************
* Implementation *
*****************/

// (De-) Serialization of a POD type
// =================================
template<typename POD_type>
int MemoryAligner::serialize(const POD_type& p_value)
{
    const auto old_offset = d_offset;
    const auto num_bytes = sizeof(POD_type);
    d_offset += num_bytes;
    if (d_offset > d_memory->size())
        d_memory->resize(d_offset);
    auto pointer = static_cast<POD_type*>(d_memory->pos(old_offset));
    *pointer = p_value;
    return old_offset;
}


template<typename POD_type>
int MemoryAligner::reserve_and_zero(const POD_type& p_value)
{
    const auto old_offset = d_offset;
    const auto num_bytes = sizeof(p_value);
    d_offset += num_bytes;
    if (d_offset > d_memory->size())
        d_memory->resize(d_offset);
    std::memset(d_memory->pos(old_offset), 0, num_bytes);
    return old_offset;
}


template<typename POD_type>
int MemoryAligner::deserialize(POD_type* r_value)
{
    const auto old_offset = d_offset;
    const auto num_bytes = sizeof(POD_type);
    d_offset += num_bytes;
    auto pointer = static_cast<POD_type*>(d_memory->pos(old_offset));
    *r_value = *pointer;
    return old_offset;
}


// (De-) Serialization of a POD type vector
// ========================================
template<typename POD_type>
int MemoryAligner::serialize(const std::vector<POD_type>& p_vector)
{
    const auto vector_size = (int) p_vector.size();
    const auto reserved_size = vector_size;
    const auto old_offset = serialize(reserved_size);
    serialize(vector_size);
    const auto num_bytes = vector_size*sizeof(POD_type);
    const auto offset_data = d_offset;
    d_offset += num_bytes;
    if (d_offset > d_memory->size())
        d_memory->resize(d_offset);
    std::memcpy(d_memory->pos(offset_data), p_vector.data(), num_bytes);
    return old_offset;
}


template<typename POD_type>
int MemoryAligner::reserve_and_zero(const std::vector<POD_type>& p_vector)
{
    const auto vector_size = 0;
    const auto reserved_size = (int) p_vector.size();
    const auto old_offset = serialize(reserved_size);
    serialize(vector_size);
    const auto num_bytes = reserved_size*sizeof(POD_type);
    const auto offset_data = d_offset;
    d_offset += num_bytes;
    if (d_offset > d_memory->size())
        d_memory->resize(d_offset);
    std::memset(d_memory->pos(offset_data), 0, num_bytes);
    return old_offset;
}


template<typename POD_type>
int MemoryAligner::deserialize(std::vector<POD_type>* r_vector)
{
    int reserved_size, vector_size;
    const auto old_offset = deserialize(&reserved_size);
    deserialize(&vector_size);
    const auto num_vector_bytes = vector_size*sizeof(POD_type);
    const auto num_reserved_bytes = reserved_size*sizeof(POD_type);
    const auto offset_data = d_offset;
    d_offset += num_reserved_bytes;
    r_vector->resize(vector_size);
    std::memcpy(r_vector->data(), d_memory->pos(offset_data), num_vector_bytes);
    return old_offset;
}


// (De-) Serialization of a vector of vectors
// ==========================================
template<typename POD_type_or_vector>
int MemoryAligner::serialize(const std::vector< std::vector<POD_type_or_vector> >& p_vector_of_vectors)
{
    const auto vector_size = (int) p_vector_of_vectors.size();
    const auto reserved_size = vector_size;
    const auto old_offset = serialize(reserved_size);
    serialize(vector_size);
    for (const auto& vector: p_vector_of_vectors)
        serialize(vector);
    return old_offset;
}


template<typename POD_type_or_vector>
int MemoryAligner::reserve_and_zero(const std::vector< std::vector<POD_type_or_vector> >& p_vector_of_vectors)
{
    const auto vector_size = 0;
    const auto reserved_size = (int) p_vector_of_vectors.size();
    const auto old_offset = serialize(reserved_size);
    serialize(vector_size);
    for (const auto& vector: p_vector_of_vectors)
        reserve_and_zero(vector);
    return old_offset;
}


template<typename POD_type_or_vector>
int MemoryAligner::deserialize(std::vector< std::vector<POD_type_or_vector> >* r_vector_of_vectors)
{
    int reserved_size, vector_size;
    const auto old_offset = deserialize(&reserved_size);
    deserialize(&vector_size);
    r_vector_of_vectors->resize(reserved_size);
    for (auto& vector: *r_vector_of_vectors)
        deserialize(&vector);
    r_vector_of_vectors->resize(vector_size);
    return old_offset;
}

#endif
