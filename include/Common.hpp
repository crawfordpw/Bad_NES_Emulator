//////////////////////////////////////////////////////////////////////////////////////////
//
// Common.hpp
//
// Contains common types, functions, classes, etc.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdint.h>
#include <Config.h>
#include <new>

typedef uint8_t  DataType;
typedef uint16_t AddressType;

// Forware declaration to prevent circular dependencies.
class System;

//========//
// Device
//
// A base class representation of a device within a system.
//========//
//
class Device
{
    public:

        virtual DataType Read(AddressType lAddress, DataType lLastRead) = 0;
        virtual void     Write(AddressType lAddress, DataType lData)    = 0;
        void             Connect(System * lSystem) {mSystem = lSystem;}
        void             Disconnect(void)          {mSystem = NULL;}

    protected:

        System * mSystem;
};

//========//
// Functor
//
// Base Functor class.
//========//
//
class Functor
{
    public:
        Functor(void)              = default;
        virtual ~Functor(void)     = default;

        virtual void Execute(void) = 0;
};

//========//
// Bit
//
// Simple function to get value of a bit position.
//
// param[in]    lPosition  The position to get.
// returns  The value of the bit.
//========//
//
inline constexpr uint16_t Bit(uint16_t lPosition)
{
    return (1 << lPosition);
}

#endif
