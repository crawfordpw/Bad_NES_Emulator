/////////////////////////////////////////////////////////////////////
//
// Mapper_000.cpp
//
// Implementation file for mapper 000.
//
/////////////////////////////////////////////////////////////////////

#include <Mappers/Mapper_000.hpp>
#include <Cartridge.hpp>

//--------//
// MapRead
//
// Maps a given address to a mapped address for a read operation if the address
// is for ROM. If its for RAM, handle the read directly.
//
// param[in]       lAddress        The address to map.
// param[out]      lMappedAddress  The mapped address.
// param[in,out]   lData           Last data read as IN, the data read from RAM as OUT.
//                                 This only is set if the return is false.
// returns  If the cartridge should read PRG ROM.
//--------//
//
bool Mapper000::MapRead(AddressType lAddress, AddressType * lMappedAddress, DataType * lData)
{
#ifdef FAMILY_BASIC
    // Reading from RAM.
    if (lAddress >= Mapper000::PRG_RAM_START && lAddress <= Mapper000::PRG_RAM_END)
    {
        *lData = mRam.Read(lAddress & (PRG_RAM_SIZE - 1), *lData);
        return false;
    }
#endif

    // Map given address to ROM.
    if (lAddress >= PRG_ROM_START && lAddress <= PRG_ROM_END)
    {
        *lMappedAddress = lAddress & (mCartridge->IsPrgMirror() ? PRG_ROM_MIRROR_SIZE : PRG_ROM_NO_MIRROR_SIZE);
        return true;
    }
    return false;
}

//--------//
// MapWrite
//
// Maps a given address to a mapped address for a write operation if the address
// is for ROM. If its for RAM, handle the write directly.
//
// param[in]   lAddress        The address to map.
// param[out]  lMappedAddress  The mapped address.
// param[in]   lData           The data to write.
// returns  If the cartridge should write PRG ROM.
//--------//
//
bool Mapper000::MapWrite(AddressType lAddress, AddressType * lMappedAddress, DataType lData)
{
#ifdef FAMILY_BASIC
    // Writing to RAM.
    if (lAddress >= Mapper000::PRG_RAM_START && lAddress <= Mapper000::PRG_RAM_END)
    {
        mRam.Write(lAddress & (PRG_RAM_SIZE - 1), lData);
        return false;
    }
#endif

    // Map given address to ROM.
    if (lAddress >= PRG_ROM_START && lAddress <= PRG_ROM_END)
    {
        *lMappedAddress = lAddress & (mCartridge->IsPrgMirror() ? PRG_ROM_MIRROR_SIZE : PRG_ROM_NO_MIRROR_SIZE);
        return true;
    }
    return false;
}
