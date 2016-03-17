#include "serialization_t.hpp"

#include "serialization.hpp"

#include <cassert>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

const auto c_result_vector_size = 4;

struct Data
{
    enum class Enumeration { ENUM1, ENUM2, ENUM3 };

    bool        value_bool;
    char        value_char;
    int         value_int;
    float       value_float;
    double      value_double;
    Enumeration value_enum;

    vector<int>                         vector_1_int;
    vector< vector<double> >            vector_2_double;
    vector< vector< vector<char> > >    vector_3_char;

};


struct Result
{
    enum class Enumeration { ENUM1, ENUM2 };

    int value_int;
    Enumeration value_enum;

    vector<int>                         vector_1_int;
    vector< vector<double> >            vector_2_double;
    vector< vector< vector<char> > >    vector_3_char;
};


struct ResultAddress
{
    void* address_values;
    void* address_vectors;
};


template<class T>
bool operator!=(const vector<T>& p_vector_1, const vector<T>& p_vector_2)
{
    if (p_vector_1.size() != p_vector_2.size())
        return true;

    for (auto i = 0; i < (int) p_vector_1.size(); ++i)
        if (p_vector_1[i] != p_vector_2[i])
            return true;

    return false;
}


template<class T>
bool operator==(const vector<T>& p_vector_1, const vector<T>& p_vector_2)
{
    return !(p_vector_1 != p_vector_2);
}


static bool rand_bool()     { return ((rand() % 2) == 0 ? false : true); }
static char rand_char()     { return (char) (rand() % 256); }
static int rand_int()       { return rand(); }
static double rand_double() { return -0.5 + (1.0*rand())/RAND_MAX; }
static float rand_float()   { return (float) rand_double(); }

static int rand_size()      { return 1 + (rand() % 4); }

static Data generate_random_data()
{
    srand(3);

    Data data;

    data.value_bool   = rand_bool();
    data.value_char   = rand_char();
    data.value_int    = rand_int();
    data.value_float  = rand_float();
    data.value_double = rand_double();
    data.value_enum = ((rand() % 3) == 0 ? Data::Enumeration::ENUM3 :
                      ((rand() % 2) == 0 ? Data::Enumeration::ENUM2
                                         : Data::Enumeration::ENUM1));

    const auto size_1 = rand_size();
    data.vector_1_int.resize(size_1);
    for (auto i = 0; i < size_1; ++i)
        data.vector_1_int[i] = rand_int();

    const auto size_2_1 = rand_size();
    const auto size_2_2 = rand_size();
    data.vector_2_double.resize(size_2_1);
    for (auto i = 0; i < size_2_1; ++i)
    {
        data.vector_2_double[i].resize(size_2_2);
        for (auto j = 0; j < size_2_2; ++j)
            data.vector_2_double[i][j] = rand_double();
    }

    const auto size_3_1 = rand_size();
    const auto size_3_2 = rand_size();
    const auto size_3_3 = rand_size();
    data.vector_3_char.resize(size_3_1);
    for (auto i = 0; i < size_3_1; ++i)
    {
        data.vector_3_char[i].resize(size_3_2);
        for (auto j = 0; j < size_3_2; ++j)
        {
            data.vector_3_char[i][j].resize(size_3_3);
            for (auto k = 0; k < size_3_3; ++k)
                data.vector_3_char[i][j][k] = rand_char();
        }
    }

    return data;
}


static Result generate_random_result()
{
    Result result;

    result.value_int = rand_int();
    result.value_enum = ((rand() % 2) == 0 ? Result::Enumeration::ENUM2
                                           : Result::Enumeration::ENUM1);

    result.vector_1_int.resize(c_result_vector_size);
    for (auto i = 0; i < c_result_vector_size; ++i)
        result.vector_1_int[i] = rand_int();

    result.vector_2_double.resize(c_result_vector_size);
    for (auto i = 0; i < c_result_vector_size; ++i)
    {
        result.vector_2_double[i].resize(c_result_vector_size);
        for (auto j = 0; j < c_result_vector_size; ++j)
            result.vector_2_double[i][j] = rand_double();
    }

    result.vector_3_char.resize(c_result_vector_size);
    for (auto i = 0; i < c_result_vector_size; ++i)
    {
        result.vector_3_char[i].resize(c_result_vector_size);
        for (auto j = 0; j < c_result_vector_size; ++j)
        {
            result.vector_3_char[i][j].resize(c_result_vector_size);
            for (auto k = 0; k < c_result_vector_size; ++k)
                result.vector_3_char[i][j][k] = rand_char();
        }
    }

    return result;
}


static ResultAddress serialize(const Data& p_data, std::vector<char>* r_memory)
{
    Result result;

    result.vector_1_int.resize(c_result_vector_size);

    result.vector_2_double.resize(c_result_vector_size);
    for (auto& vector: result.vector_2_double)
        vector.resize(c_result_vector_size);

    result.vector_3_char.resize(c_result_vector_size);
    for (auto& vectors: result.vector_3_char)
    {
        vectors.resize(c_result_vector_size);
        for (auto& vector: vectors)
            vector.resize(c_result_vector_size);
    }

    ResultAddress address;

    for (auto i = 0; i < 2; ++i)
    {
        // i == 0: simulation to determine required amount of bytes
        // i == 1: write to memory
        Serializer serializer(i == 0 ? nullptr : r_memory->data());

        serializer << p_data.vector_2_double
                   << p_data.value_bool
                   << reserve(result.vector_3_char, &(address.address_vectors))
                   << reserve(result.vector_1_int)
                   << reserve(result.vector_2_double)
                   << p_data.value_enum
                   << p_data.vector_1_int
                   << p_data.value_int
                   << reserve(result.value_enum, &(address.address_values))
                   << reserve(result.value_int)
                   << p_data.value_float
                   << p_data.vector_3_char
                   << p_data.value_double
                   << p_data.value_char;

        if (i == 0)
            r_memory->resize(serializer.required_bytes());
    }

    return address;
}


static Data deserialize(void* p_address, ResultAddress* v_result_address)
{
    Data data;
    Result result;
    
    Deserializer deserializer(p_address);

    deserializer >> data.vector_2_double
                 >> data.value_bool;
                 
    v_result_address->address_vectors = deserializer.current_address(); 

    deserializer >> result.vector_3_char
                 >> result.vector_1_int
                 >> result.vector_2_double
                 >> data.value_enum
                 >> data.vector_1_int
                 >> data.value_int;
                 
    v_result_address->address_values = deserializer.current_address();

    deserializer >> result.value_enum
                 >> result.value_int
                 >> data.value_float
                 >> data.vector_3_char
                 >> data.value_double
                 >> data.value_char;

    return data;
}


static void serialize_result(const Result& p_result, const ResultAddress& p_address)
{
    Serializer(p_address.address_values)  << p_result.value_enum
                                          << p_result.value_int;

    Serializer(p_address.address_vectors) << p_result.vector_3_char
                                          << p_result.vector_1_int
                                          << p_result.vector_2_double;
};


static Result deserialize_result(const ResultAddress& p_result_address)
{
    Result result;

    Deserializer(p_result_address.address_vectors) >> result.vector_3_char
                                                   >> result.vector_1_int
                                                   >> result.vector_2_double;

    Deserializer(p_result_address.address_values)  >> result.value_enum
                                                   >> result.value_int;

    return result;
}


static void verify_zero(const ResultAddress& p_result_address)
{
    auto result = deserialize_result(p_result_address);

    assert(result.value_int == 0);
    assert(static_cast<int>(result.value_enum) == 0);
    assert(result.vector_1_int.size() == 0);
    assert(result.vector_2_double.size() == 0);
    assert(result.vector_3_char.size() == 0);
}


static void verify_equality(const Data& p_data_1, const Data& p_data_2)
{
    assert(p_data_1.value_bool      == p_data_2.value_bool);
    assert(p_data_1.value_char      == p_data_2.value_char);
    assert(p_data_1.value_int       == p_data_2.value_int);
    assert(p_data_1.value_float     == p_data_2.value_float);
    assert(p_data_1.value_double    == p_data_2.value_double);
    assert(p_data_1.vector_1_int    == p_data_2.vector_1_int);
    assert(p_data_1.vector_2_double == p_data_2.vector_2_double);
    assert(p_data_1.vector_3_char   == p_data_2.vector_3_char);
}


static void verify_equality(const ResultAddress& p_address_1, const ResultAddress& p_address_2)
{
    assert(p_address_1.address_values  == p_address_2.address_values);
    assert(p_address_1.address_vectors == p_address_2.address_vectors);
}


static void verify_equality(const Result& p_result_1, const Result& p_result_2)
{
    assert(p_result_1.value_int       == p_result_2.value_int);
    assert(p_result_1.value_enum      == p_result_2.value_enum);
    assert(p_result_1.vector_1_int    == p_result_2.vector_1_int);
    assert(p_result_1.vector_2_double == p_result_2.vector_2_double);
    assert(p_result_1.vector_3_char   == p_result_2.vector_3_char);
}


void test_serialization()
{
    cout << endl
         << "Serialization test" << endl
         << "==================" << endl
         << endl;

    // Alice: Generate data and serialize it
    const auto data_alice = generate_random_data();

    vector<char> memory;
    const auto result_address_alice = serialize(data_alice, &memory);

    verify_zero(result_address_alice);   // result is zero unless Bob serialized his result

    // Bob: Deserialize data and serialize result in the same memory
    ResultAddress result_address_bob;
    auto data_bob = deserialize(memory.data(), &result_address_bob);

    verify_equality(data_alice, data_bob);
    verify_equality(result_address_alice, result_address_bob);

    const auto result_bob = generate_random_result();
    serialize_result(result_bob, result_address_bob);

    // Alice: Deserialize result
    const auto result_alice = deserialize_result(result_address_alice);

    verify_equality(result_alice, result_bob);

    cout << "Serialization and deserialization successful." << endl;
}