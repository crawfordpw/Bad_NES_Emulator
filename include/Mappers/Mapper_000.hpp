//////////////////////////////////////////////////////////////////////////////////////////
//
// Mapper_000.hpp
//
// Mapper for id 000, https://www.nesdev.org/wiki/NROM
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MAPPER_000_HPP
#define MAPPER_000_HPP

#include "Mapper.hpp"

//========//
// Mapper000
//
// Class for mapper 000.
//========//
//
class Mapper000 : public Mapper
{
    public:
        enum
        {
#ifdef FAMILY_BASIC
            PRG_RAM_START           = 0x6000,
            PRG_RAM_END             = 0x7FFF,
            PRG_RAM_SIZE            = PRG_RAM_END - PRG_RAM_START + 1,
#else
            PRG_RAM_SIZE            = 0,
#endif

            PRG_ROM_START           = 0x8000,
            PRG_ROM_END             = 0xFFFF,
            PRG_ROM_NO_MIRROR_SIZE  = PRG_ROM_END - PRG_ROM_START,
            PRG_ROM_MIRROR_SIZE     = PRG_ROM_NO_MIRROR_SIZE / 2
        };

        // According to https://www.nesdev.org/wiki/NROM most emulators just provide 8kb of program ram.
        explicit Mapper000(Cartridge * lCartridge) : Mapper(lCartridge), mRam(PRG_RAM_SIZE) {}
        virtual ~Mapper000(void) = default;

        virtual bool MapRead(AddressType lAddress, AddressType * lMappedAddress, DataType * lData)  override;
        virtual bool MapWrite(AddressType lAddress, AddressType * lMappedAddress, DataType lData) override;

    protected:

        MemoryRam mRam;
};

#endif
