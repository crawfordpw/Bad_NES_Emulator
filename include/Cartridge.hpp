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
#include "Memory.hpp"
#include <Mappers/Mapper.hpp>

//========//
// Cartridge
//
// Represents a physical cartridge.
//========//
//
class Cartridge : public Device
{
    public:

        explicit Cartridge(const char * lFilename);
        virtual ~Cartridge(void);

        bool             IsValidImage(void) {return mValidImage;}
        virtual DataType Read(AddressType lAddress) override;
        virtual void     Write(AddressType lAddress, DataType lData)    override;

        bool             IsPrgMirror(void) {return mPrgMirror;}

    protected:

        struct Header
        {
            char       mId[4];      // Bytes 0-3
            uint8_t    mPrgBanks;   // Byte  4
            uint8_t    mChrBanks;   // Byte  5
            uint8_t    mFlags6;     // Byte  6
            uint8_t    mFlags7;     // Byte  7
            uint8_t    mPrgRamSize; // Byte  8
            uint8_t    mTvSystem;   // Byte  9
            uint8_t    mFlags10;    // Byte  10
            char       mUnused[5];  // Byte  11-15
        };

        enum
        {
            DEFAULT_PRG_SIZE = 0x4000,
            DEFAULT_CHR_SIZE = 0x2000,
            TRAINER_SIZE     = 512
        };

        AddressType  mAddressStart;         // Start of cartridge address space.
        AddressType  mAddressEnd;           // End of cartridge address space.
        Header       mHeader;               // Contains header informations provided by file.
        uint8_t      mMapperId;             // Which mapper does the cartridge use.
        Mapper *     mMapper;               // The mapper.
        MemoryRam    mPrgMemory;            // Program ROM memory space or mapper registers.
        MemoryRam    mChrMemory;            // Character ROM memory space or mapper registers.
        uint8_t      mMirrorType;           // Mirroring mode, horizontal or vertical.
        bool         mNes20Format;          // Is the provided file in NES 2.0 format.
        bool         mPrgMirror;            // If the number of program banks is 1, the address space is 32k with the second half mirrored.
        bool         mChrRam;               // If the number of chracter banks is 0, the memory acts as a RAM instead.
        bool         mValidImage;           // Flag for determing if the file loaded is valid.

    protected:

        enum Flags6Bits
        {
            MIRRORING             = Bit(0),
            BATTERY               = Bit(1),
            TRAINER               = Bit(2),
            FOUR_SCREEN_MODE      = Bit(3),
            LOW_NIBBLE_MAPPER_ID  = 0xF0,

            // Meaning of MIRRORING bit.
            HORIZONTAL            = 0,
            VERTICAL              = 1
        };

        enum Flags7Bits
        {
            VS_UNISYSTEM          = Bit(0),
            PLAYCHOICE_10         = Bit(1),
            NES_20                = 0x0C,
            HIGH_NIBBLE_MAPPER_ID = 0xF0,

            // Is NES 2.0 format
            NES_20_ID             = 0x08
        };

        enum TVSystemBits
        {
            TV_SYSTEM             = Bit(0),

            // Meaning of TV_SYSTEM bit.
            NTSC                  = 0,
            PAL                   = 1
        };

        enum Flags10Bits
        {
            TV_SYSTEM_F10         = 0x03,
            PRG_RAM               = Bit(4),
            HAS_BUS_CONFLICTS     = Bit(5),

            // Meaning of TV_SYSTEM bit.
            NTSC_F10              = 0,
            PAL_F10               = 2,
            DUAL                  = 3
        };

        uint8_t GetMirroring(void)       {return mHeader.mFlags6   & Flags6Bits::MIRRORING;}
        uint8_t GetBattery(void)         {return mHeader.mFlags6   & Flags6Bits::BATTERY;}
        uint8_t GetTrainer(void)         {return mHeader.mFlags6   & Flags6Bits::TRAINER;}
        uint8_t GetFourScreen(void)      {return mHeader.mFlags6   & Flags6Bits::FOUR_SCREEN_MODE;}
        uint8_t GetLowNibbleMapId(void)  {return mHeader.mFlags6   & Flags6Bits::LOW_NIBBLE_MAPPER_ID;}
        uint8_t GetVsSystem(void)        {return mHeader.mFlags7   & Flags7Bits::VS_UNISYSTEM;}
        uint8_t GetPlayChoice(void)      {return mHeader.mFlags7   & Flags7Bits::PLAYCHOICE_10;}
        uint8_t GetNes20 (void)          {return mHeader.mFlags7   & Flags7Bits::NES_20;}
        uint8_t GetHighNibbleMapId(void) {return mHeader.mFlags7   & Flags7Bits::HIGH_NIBBLE_MAPPER_ID;}
        uint8_t GetTvSystem(void)        {return mHeader.mTvSystem & TVSystemBits::TV_SYSTEM;}
        uint8_t GetTvSystemF10(void)     {return mHeader.mFlags10  & Flags10Bits::TV_SYSTEM_F10;}
        uint8_t GetTPrgRam(void)         {return mHeader.mFlags10  & Flags10Bits::PRG_RAM;}
        uint8_t GetHasBusConflicts(void) {return mHeader.mFlags10  & Flags10Bits::HAS_BUS_CONFLICTS;}
};

#endif
