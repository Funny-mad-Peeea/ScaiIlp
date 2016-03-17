#include "serialization.hpp"

Serializer& operator>>(Serializer& p_serializer, void** r_address)
{
    *r_address = p_serializer.current_address();
    return p_serializer;
}


Deserializer& operator>>(Deserializer& p_deserializer, void** r_address)
{
    *r_address = p_deserializer.current_address();
    return p_deserializer;
}
