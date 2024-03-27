//////////////////////////////////////////////////////////////////////////////////////////
//
// Mapper.hpp
//
// Common mapper classes and functionality.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MAPPER_HPP
#define MAPPER_HPP

#include "Common.hpp"
#include "Memory.hpp"

class Cartridge;

//========//
// Mapper
//
// Base class for every mapper.
//========//
//
class Mapper
{
    public:

        explicit Mapper(Cartridge * lCartridge) : mCartridge(lCartridge) {}
        virtual ~Mapper(void) = default;

        virtual bool MapRead(AddressType lAddress, AddressType * lMappedAddress, DataType * lData) = 0;
        virtual bool MapWrite(AddressType lAddress, AddressType * lMappedAddress, DataType lData)  = 0;

    protected:

        Cartridge *  mCartridge;
};

// Global functions.
extern Mapper * MapperFactory(uint8_t lMapperId, Cartridge * lCartridge);

#endif
