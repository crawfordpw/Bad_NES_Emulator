//////////////////////////////////////////////////////////////////////////////////////////
//
// System.hpp
//
// File for the entire system.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Memory.hpp"
#include "Cpu6502.hpp"
#include "Cartridge.hpp"

//========//
// System
//
// Contains the main components for the system.
//========//
//
class System
{
    public:

        enum MemoryMap
        {
            RAM_START               = 0x0000,
            RAM_SIZE                = 0x0800,
            RAM_RANGE               = 0x1FFF,

            PPU_REGISTER_START      = 0x2000,
            PPU_REGISTER_SIZE       = 0x0008,
            PPU_REGISTER_RANGE      = 0x3FFF,

            APU_IO_REGISTER_START   = 0x4000,
            APU_IO_REGISTER_SIZE    = 0x0018,
            APU_IO_FUNC_START       = 0x4018,
            APU_IO_FUNC_SIZE        = 0x0008,

            CARTRIDGE_START         = 0x4020,
            CARTRIDGE_SIZE          = 0xBFE0,
            CARTRIDGE_RANGE         = 0xFFFF,
        };

        System(void);
        ~System(void);

        void     Start(void);
        DataType Read(AddressType lAddress, DataType lLastRead);
        void     Write(AddressType lAddress, DataType lData);

        void     InsertCartridge(Cartridge * lCartridge);
        void     RemoveCartridge(void);

        void     LoadMemory(char * lProgram, AddressType lSize, AddressType lOffset);

        void     CpuTest(void);

    public:

        Cpu6502      mCpu;
        MemoryMapped mRam;

    private:

        void     DumpMemoryAsHex(const char * lFilename);
        void     DumpMemoryAsRaw(const char * lFilename);

        Cartridge *   mCartridge;
};

//========//
// TestNesFunctor
//
// Functor for testing the cpu using testnes rom.
//========//
//
class TestNesFunctor : public Functor
{
    public:

        enum
        {
            ERROR_BUFFER_SIZE = Cpu6502::BUFFER_SIZE * 2 + 50
        };

        TestNesFunctor(Cpu6502 * lCpu, bool lStopAtFirstFail);
        virtual ~TestNesFunctor(void);

        virtual void Execute(void);

        bool     mStopExecution;

    protected:

        Cpu6502 *   mCpu;
        File *      mTrace;
        char *      mLogFile;
        long int    mFileSize;
        bool        mStopAtFirstFail;
        long int    mCurrentPosition;
        int         mLineNum;
        static char cLineBuffer[];
        static char cErrorBuffer[];
};

#endif
