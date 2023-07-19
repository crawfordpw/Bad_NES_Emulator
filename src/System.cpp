/////////////////////////////////////////////////////////////////////
//
// System.cpp
//
// Implementation file for the system.
//
/////////////////////////////////////////////////////////////////////

#include <System.hpp>
#include <File/ApiFile.hpp>
#include <Errors/ApiErrors.hpp>

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
System::System(void)
  : mRam(RAM_SIZE), mCartridge(NULL)
{
    mCpu.Connect(this);
    mRam.Connect(this);
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
// InsertCartridge
//
// Connects the cartidge to the system.
//
// param[in]    lCartridge  The cartridge insert.
//--------//
//
void System::InsertCartridge(Cartridge * lCartridge)
{
    if (!lCartridge->IsValidImage())
    {
        return;
    }
    mCartridge = lCartridge;
    mCartridge->Connect(this);
}

//--------//
// RemoveCartridge
//
// Disconnect the cartidge from the system.
//--------//
//
void System::RemoveCartridge(void)
{
    if (mCartridge)
    {
        mCartridge->Disconnect();
        mCartridge = NULL;
    }
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
// param[in] lLastRead  Attempts to replicate open bus behavior.
// returns  Data at the given address. 
//--------//
//
DataType System::Read(AddressType lAddress, DataType lLastRead)
{
    DataType lData = lLastRead;

    // The cartridge has passive access to all reads within the system.
    // even if it's not within its address range.
    if (mCartridge)
    {
        lData = mCartridge->Read(lAddress, lLastRead);
    }

    // 2KB is mirrrored across 8KB.
    if (lAddress >= System::RAM_START && lAddress <= System::RAM_RANGE)
    {
        lData = mRam.Read(lAddress & (RAM_SIZE - 1), lLastRead);
    }
    else if (lAddress >= System::PPU_REGISTER_START && lAddress <= PPU_REGISTER_RANGE)
    {
        // Add when ppu is added.
    }

    return lData;
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
    // The cartridge has passive access to all writes within the system.
    // even if it's not within its address range.
    if (mCartridge)
    {
        mCartridge->Write(lAddress, lData);
    }

    // 2KB is mirrrored across 8KB.
    if (lAddress >= System::RAM_START && lAddress <= System::RAM_RANGE)
    {
        mRam.Write(lAddress & (RAM_SIZE - 1), lData);
    }
    else if (lAddress >= System::PPU_REGISTER_START && lAddress <= PPU_REGISTER_RANGE)
    {
        // Add when ppu is added.
    }
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
    static const char * vHexArray = "0123456789ABCDEF";
    size_t   lSize = mRam.GetSize() * 2;
    char     lBuffer[lSize];
    DataType lValue;
    File *   lFile;
    int      lStatus;

    // Open up a file.
    lStatus = ApiFileSystem::OpenFromExecDirectory(lFilename, "w+", &lFile);

    // Don't proceed if can't open.
    if (lStatus != ErrorCodes::SUCCESS)
    {
        gErrorManager.Post(lStatus);
        return;
    }
    
    // Loop through all of memory and convert to hex.
    for (AddressType lIndex = 0; lIndex < mRam.GetSize(); ++lIndex)
    {
        lValue = Read(lIndex, 0x00) & 0xFF;
        lBuffer[lIndex * 2] = vHexArray[lValue >> 4];
        lBuffer[lIndex * 2 + 1] = vHexArray[lValue & 0x0F];
    }

    // Write out to file.
    ApiFileSystem::Write(lBuffer, lSize, lFile);
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
    if (lStatus != ErrorCodes::SUCCESS)
    {
        gErrorManager.Post(lStatus);
        return;
    }
    
    // Loop through all of memory and convert to hex.
    for (AddressType lIndex = 0; lIndex < lSize; ++lIndex)
    {
        lBuffer[lIndex] = static_cast<char>(Read(lIndex, 0x00));
    }

    // Write out to file.
    ApiFileSystem::Write(lBuffer, lSize, lFile);
}

#ifdef TEST_CPU
//--------//
// CpuTest
//
// Tests the cpu by running test located at ./test/nestest.nes from
// project source directoy.
//--------//
//
void System::CpuTest(void)
{
    // Grab the test file.
    char lFilename[ApiFileSystem::MAX_FILENAME * 2];

    const char * lExecDirectory = ApiFileSystem::GetExecDirectory();
    if (lExecDirectory == NULL)
    {
        gErrorManager.Post(ErrorCodes::FILE_GENERAL_ERROR);
    }
    sprintf(lFilename, "%s%s", lExecDirectory, "../tests/nestest.nes");

    // Load the cartridge.
    Cartridge lCartridge(lFilename);

    if (!lCartridge.IsValidImage())
    {
        ApiLogger::Log("[!] Invalid ROM loaded into cartridge\n");
        return;
    }

    // Load the rom.
    InsertCartridge(&lCartridge);

    // Setup system needed for test rom to work properly.
    lCartridge.Write(0xFFFC, 0x00);
    lCartridge.Write(0xFFFD, 0xC0);
    mCpu.PushStack(0x00);
    mCpu.PushStack(0x08);
    ++mCpu.mRegisters.mSp;
    ++mCpu.mRegisters.mSp;

    // Get cpu into a known good state.
    mCpu.Reset();

    // Setup callback after each cpu instruction.
    TestNesFunctor lTest(&mCpu, true);
    if (lTest.mStopExecution == true)
    {
        return;
    }
    mCpu.mFunctor = &lTest;

    ApiLogger::Log("[i] Nestest started");

    // Start executing test rom.
    while (lTest.mStopExecution == false)
    {
        mCpu.StepClock();
    }

    // Final cleanup.
    RemoveCartridge();
}

//--------//
//
// TestNesFunctor
//
//--------//

char TestNesFunctor::cLineBuffer[Cpu6502::BUFFER_SIZE];
char TestNesFunctor::cErrorBuffer[TestNesFunctor::ERROR_BUFFER_SIZE];

//--------//
// TestNesFunctor
//
// Constructor. Load everything needed for Execute.
//--------//
//
TestNesFunctor::TestNesFunctor(Cpu6502 * lCpu, bool lStopAtFirstFail)
  :  mStopExecution(true), mCpu(lCpu), mTrace(NULL), mStopAtFirstFail(lStopAtFirstFail), mCurrentPosition(0), mLineNum(1)
{
    int    lStatus;
    File * lLog;

    // Open up a known good nes log to comapare to.
    ApiFileSystem::OpenFromExecDirectory("../tests/nestest.log", "r", &lLog);
    lStatus = ApiFileSystem::GetStatus(lLog);
    if (lStatus != ErrorCodes::SUCCESS)
    {
        gErrorManager.Post(lStatus, "../tests/nestest.log");
        return;
    }

    // Open up a new file for a trace of the cpu.
    ApiFileSystem::OpenFromExecDirectory("../cpu_trace.txt", "w+", &mTrace);
    lStatus = ApiFileSystem::GetStatus(mTrace);
    if (lStatus != ErrorCodes::SUCCESS)
    {
        gErrorManager.Post(lStatus, "../cpu_trace.txt");
        return;
    }

    // Get the size of the log.
    ApiFileSystem::SeekFromEnd(0, lLog);
    ApiFileSystem::Tell(&mFileSize, lLog);
    ApiFileSystem::SeekFromStart(0, lLog);

    // Create memory to read log file into.
    mLogFile = new(std::nothrow) char[mFileSize];
    if (mLogFile == NULL)
    {
        gErrorManager.Post(ErrorCodes::OUT_OF_MEMORY);
        return;
    }

    // Store log in memory.
    ApiFileSystem::Read(mLogFile, mFileSize, lLog);
    lStatus = ApiFileSystem::GetStatus(lLog);
    if (lStatus != ErrorCodes::SUCCESS)
    {
        gErrorManager.Post(lStatus);
        return;
    }

    // All went well.
    mStopExecution = false;
}

//--------//
// ~TestNesFunctor
//
// Destructor.
//--------//
//
TestNesFunctor::~TestNesFunctor(void)
{
    ApiFileSystem::Close(mTrace);

    if (mLogFile)
    {
        delete [] mLogFile;
    }
}

//--------//
// TestNesFunctor
//
// Execute.
//--------//
//
void TestNesFunctor::Execute(void)
{
    // Write trace out to file.
    ApiFileSystem::Write(mCpu->cBuffer, strlen(mCpu->cBuffer), mTrace);

    // Copy one line of the log file.
    int lIndex = 0;
    while (mLogFile[mCurrentPosition] != '\n')
    {
        cLineBuffer[lIndex++] = mLogFile[mCurrentPosition++];
    }

    // Grab the newline as well as null terminate the buffer.
    ++mCurrentPosition;
    cLineBuffer[lIndex++] = '\n';
    cLineBuffer[lIndex]   = '\0';

    // Check if the lines are equal between the log and cpu trace.
    if (strcmp(cLineBuffer, mCpu->cBuffer) != 0)
    {
        snprintf(cErrorBuffer, TestNesFunctor::ERROR_BUFFER_SIZE, "\n[---] Mismatched trace, line %d!\n[---] Log file:  %s[---] Cpu trace: %s", mLineNum, cLineBuffer, mCpu->cBuffer);
        ApiLogger::Log(cErrorBuffer);

        if (mStopAtFirstFail)
        {
            mStopExecution = true;
        }
    }

    // If this far, then the test passed.
    if (mLineNum == cLastLine)
    {
        mStopExecution = true;
        ApiLogger::Log("\n[+] NesTest has passed!\n", sizeof("\n[+] NesTest has passed!\n"));
    }

    ++mLineNum;
}
#endif
