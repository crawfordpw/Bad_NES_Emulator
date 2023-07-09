//////////////////////////////////////////////////////////////////////////////////////////
//
// Mapper.hpp
//
// Common mapper classes and functionality.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MAPPER_HPP
#define MAPPER_HPP

#include <stdint.h>

//========//
// Mapper
//
// Base class for every mapper.
//========//
//
class Mapper
{
    public:

        Mapper(void)          = default;
        virtual ~Mapper(void) = default;
};

// Global functions.
extern Mapper * MapperFactory(uint8_t lMapperId);

#endif
