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

typedef uint8_t DataType;
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

        void Connect(System * lSystem) {mSystem = lSystem;}

    protected:

        System * mSystem;
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
