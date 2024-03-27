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

        virtual DataType Read(AddressType lAddress)                     = 0;
        virtual void     Write(AddressType lAddress, DataType lData)    = 0;
        void             Connect(System * lSystem) {mSystem = lSystem; mDisconnectedError = false;}
        void             Disconnect(void)          {mSystem = nullptr;}

        Device(void) : mSystem(nullptr), mDisconnectedError(false) {};
        virtual ~Device(void) = default;

    protected:

        bool IsDisconnected(void);

        System * mSystem;
        bool mDisconnectedError;    // Have we posted an error already due to being disconnected?
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
// param[in]    lPosition  The bit position to get the value of.
// returns  The value of the bit.
//========//
//
template <typename UINT>
inline constexpr UINT Bit(UINT lPosition)
{
    return (1 << static_cast<unsigned>(lPosition));
}

//========//
// BitMask
//
// Simple function to mask off a number of bits at a specific location.
//
// param[in]    lWidth      How many consecutive bits in the mask.
// param[in]    lPosition   How much to shift the mask left by.
// returns  The value of the bit.
//========//
//
template <typename UINT>
inline constexpr UINT BitMask(UINT lWidth, UINT lPosition)
{
    return (~(~0U << static_cast<unsigned>(lWidth)) << static_cast<unsigned>(lPosition));
}

//========//
// BitMask
//
// Simple function to mask off a number of bits from bit 0.
//
// param[in]    lWidth      How many consecutive bits in the mask.
// returns  The value of the bit.
//========//
//
template <typename UINT>
inline constexpr UINT BitMask(UINT lWidth)
{
    return BitMask(lWidth, static_cast<UINT>(0));
}

#ifdef USE_LOGGER
    #define CAPTURE_LOG(lMessage) ::ApiLogger::Log(lMessage)
    #define CAPTURE_LOG_SIZE(lMessage, lSize) ::ApiLogger::Log(lMessage, lSize)
#else
    #define CAPTURE_LOG(lMessage)
    #define CAPTURE_LOG_SIZE(lMessage, lSize)
#endif

#endif
