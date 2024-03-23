/////////////////////////////////////////////////////////////////////
//
// CMemory.cpp
//
// Implementation file for memory.
//
/////////////////////////////////////////////////////////////////////

#include <Memory.hpp>

//--------//
//
// Memory
//
//--------//

//--------//
// LoadMemoryFromFile
//
// Loads allocated memory with current position of file pointer.
//
// @pre     Assumes lMemory is pre-allocated and has room.
//
// param[in]    lFile    File to load
// param[in]    lSize    How much to load.
// param[out]   lMemory  Where to load the memory to.
// returns  Status of the memory load, SUCCESS or error code.
//--------//
//
int Memory::LoadMemoryFromFile(File * lFile, size_t lSize, uint8_t * lMemory)
{
    if (mSize < lSize)
    {
        return ErrorCodes::FAIL_TO_LOAD_MEMORY;
    }
    ApiFileSystem::Read(lMemory, lSize, lFile);
    return lFile->GetStatus();
}

//--------//
//
// MemoryRom
//
//--------//

//--------//
// LoadMemoryFromFile
//
// Loads allocated memory with current position of file pointer.
//
// param[in]    lFile   File to load
// param[in]    lSize   How much to load.
// returns  Status of the memory load, SUCCESS or error code.
//--------//
//
int MemoryRom::LoadMemoryFromFile(File * lFile, size_t lSize)
{
    return Memory::LoadMemoryFromFile(lFile, lSize, mMemory);
}
