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
// MemoryMapped
//
//--------//

//--------//
// LoadMemoryFromFile
//
// Loads allocated memory with current position of file pointer.
//
// param[in]    lFile   File to load
// param[in]    lSize   How much to load.
//--------//
//
int MemoryMapped::LoadMemoryFromFile(File * lFile, size_t lSize)
{
    if (mSize < lSize)
    {
        return FAILURE;
    }
    ApiFileSystem::Read(mMemory, lSize, lFile);
    return lFile->GetStatus();
}
