/////////////////////////////////////////////////////////////////////
//
// System.cpp
//
// Implementation file for the system.
//
/////////////////////////////////////////////////////////////////////

#include <System.hpp>
#include <File/ApiFile.hpp>

//--------//
// ValidHexCharacter
//
// Helper function to check if a character is a valid hex character.
// If it is, then convert it to a number.
//
// param[in]  lChar   The character to check. 
// param[out] lByte   The converted byte if it was valid. 
// returns  True if its a valid hex character, fals otherwise. 
//--------//
//
bool ValidHexCharacter(char lChar, uint8_t * lByte)
{
    if (lChar >= 'a' && lChar <= 'f')
    {
        *lByte = static_cast<uint8_t>(lChar - 'a' + 10);
        return true;
    }

    if (lChar >= 'A' && lChar <= 'F')
    {
        *lByte = static_cast<uint8_t>(lChar - 'A' + 10);
        return true;
    }

    if (lChar >= '0' && lChar <= '9')
    {
        *lByte = static_cast<uint8_t>(lChar - '0');
        return true;
    }
    return false;
}

//--------//
//
// System
//
//--------//

//--------//
// System
//
// Constructor.
//--------//
//
System::System()
  : mRam(RAM_SIZE)
{
    mCpu.Connect(this);
    mRam.Connect(this);

    Write(mCpu.mInterruptVectors[Cpu6502::RESET_VECTOR].mLowByte, 0x00);
    Write(mCpu.mInterruptVectors[Cpu6502::RESET_VECTOR].mHighByte, 0x00);
    mCpu.Reset();
}

//--------//
// ~System
//
// Destructor.
//--------//
//
System::~System()
{
}

//--------//
// Start
//
// Starts running a program already loaded into memory.
//
// NOTE: This structure is just in for testing purposes.
//
//--------//
//
void System::Start(void)
{
    uint8_t lCycles;
    uint8_t lInstructions = 0;

    while (true)
    {
        mCpu.StepClock();
        lCycles = mCpu.GetCyclesLeft();
        if (lCycles == 0)
        {
            ++lInstructions;
        }
        if (lInstructions > 7)
        {
            mCpu.Reset();
            lInstructions = 0;
            DumpMemoryAsRaw("../MemDump.hex");
            break;
        }
    }
}

//--------//
// Read
//
// Reads some data out of memory.
//
// param[in] lAddress   Address to read from. 
// returns  Data at the given address. 
//--------//
//
DataType System::Read(AddressType lAddress)
{
    return mRam.Read(lAddress);
}

//--------//
// Write
//
// Writes data to memory at a given address.
//
// param[in] lAddress   Address to write to. 
// param[in] lData      Data to write. 
//--------//
//
void System::Write(AddressType lAddress, DataType lData)
{
    mRam.Write(lAddress, lData);
}

//--------//
// LoadMemory
//
// Loads RAM with a program, starting at the very beginning. This will
// make an exact copy of the data.
// This is likely a temporary function for testing the CPU,
// or at the very least will be heavily modified as more pieces are added.
//
// pre: No size checking is performed on the program or the offset. It is
//      assumed that the program will all fit inside memory.
//
// param[in] lProgram   The program to load, an array of uint8_t.
// param[in] lSize      Size of the array.
// param[in] lOffset    Offset in memory to start loading program to.
//--------//
//
void System::LoadMemory(char * lProgram, AddressType lSize, AddressType lOffset)
{
    uint8_t     lHexByteLow;
    uint8_t     lHexByteHigh;
    bool        lHighAcquired = false;

    for (AddressType lIndex = 0; lIndex < lSize; ++lIndex)
    {
        // Skip over invalid characters.
        if (!ValidHexCharacter(lProgram[lIndex], &lHexByteLow))
        {
            continue;
        }

        // We've already got the high hex byte, which means we just acquired the low byte.
        // Combine them into one byte and then write out to memory.
        if (lHighAcquired)
        {
            lHighAcquired = false;
            Write(lOffset++, ((lHexByteHigh << 4) | lHexByteLow));
            continue;
        }

        // If made it this far, then it is the high hex byte. Grab the low on next iteration.
        lHexByteHigh = lHexByteLow;
        lHighAcquired = true;
    }
}

//--------//
// DumpMemoryAsHex
//
// Dumps contents of memory to a file, converting raw memory to hex string.
//
// param[in] lFilename   File to write memory to.
//--------//
//
void System::DumpMemoryAsHex(const char * lFilename)
{
    static const char * lHexArray = "0123456789ABCDEF";
    size_t   lSize = mRam.GetSize() * 2;
    char     lBuffer[lSize];
    DataType lValue;
    File *   lFile;
    int      lStatus;

    // Open up a file.
    lStatus = ApiFileSystem::OpenFromExecDirectory(lFilename, "w+", &lFile);

    // Don't proceed if can't open.
    if (lStatus != File::SUCCESS)
    {
        printf("Couldn't open file!");
        return;
    }
    
    // Loop through all of memory and convert to hex.
    for (AddressType lIndex = 0; lIndex < mRam.GetSize(); ++lIndex)
    {
        lValue = Read(lIndex) & 0xFF;
        lBuffer[lIndex * 2] = lHexArray[lValue >> 4];
        lBuffer[lIndex * 2 + 1] = lHexArray[lValue & 0x0F];
    }

    // Write out to file.
    ApiFileSystem::Read(lBuffer, lSize, lFile);
}

//--------//
// DumpMemoryAsRaw
//
// Dumps raw contents of memory to a file.
//
// param[in] lFilename   File to write memory to.
//--------//
//
void System::DumpMemoryAsRaw(const char * lFilename)
{
    size_t   lSize = mRam.GetSize();
    char     lBuffer[lSize];
    File *   lFile;
    int      lStatus;

    // Open up a file.
    lStatus = ApiFileSystem::OpenFromExecDirectory(lFilename, "w+", &lFile);

    // Don't proceed if can't open.
    if (lStatus != File::SUCCESS)
    {
        printf("Couldn't open file!");
        return;
    }
    
    // Loop through all of memory and convert to hex.
    for (AddressType lIndex = 0; lIndex < lSize; ++lIndex)
    {
        lBuffer[lIndex] = static_cast<char>(Read(lIndex));
    }

    // Write out to file.
    ApiFileSystem::Write(lBuffer, lSize, lFile);
}
