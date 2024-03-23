/////////////////////////////////////////////////////////////////////
//
// Mapper.cpp
//
// Implementation file for common mapper functionality.
//
/////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <Mappers/Mapper_000.hpp>

//--------//
// MapperFactory
//
// Helper class to create a mapper given an id.
//
// param[in]    lMapperId  Id of mapper to create.
// param[in]    lCartridge The Cartridge this mapper is for.
// returns  Dynamically created mapper. Calling functions owns the memory.
//--------//
//
Mapper * MapperFactory(uint8_t lMapperId, Cartridge * lCartridge)
{
    Mapper * lMapper;

    switch (lMapperId)
    {
        case 0:
            lMapper = new Mapper000(lCartridge);
            break;
        
        default:
            lMapper = nullptr;
    }

    return lMapper;
}
