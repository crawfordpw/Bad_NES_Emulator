//////////////////////////////////////////////////////////////////////////////////////////
//
// Cartridge.hpp
//
// File that manages the concept of a physical NES cartridge.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include "Common.hpp"

//========//
// Cartridge
//
// Represents a physical cartridge.
//========//
//
class Cartridge : public Device
{
    public:

        Cartridge(void);
        virtual ~Cartridge(void) = default;

        virtual DataType Read(AddressType lAddress, DataType lLastRead) override;
        virtual void     Write(AddressType lAddress, DataType lData)    override;

    protected:

        AddressType mAddressStart;
        AddressType mAddressEnd;
};

#endif
