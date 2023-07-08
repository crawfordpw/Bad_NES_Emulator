/////////////////////////////////////////////////////////////////////
//
// File.cpp
//
// Implementation file for file operations.
//
/////////////////////////////////////////////////////////////////////

#include <File/ApiFile.hpp>

// Relative to where program is executing from.
const char * gTraceDumpLocation = "../";
const char * gLogLocation       = "../";
const char * gInputLocation     = "../";

//--------//
//
// ApiFileSystem
//
//--------//

FileSystem * ApiFileSystem::cFileSystem       = NULL;
char *       ApiFileSystem::cCurrentDirectory = NULL;
char *       ApiFileSystem::cExecDirectory    = NULL;

//--------//
// GetCwd
//
// Gets the current working directory from where the program
// was started from.
//
// returns  The current working directory.
//--------//
//
const char * ApiFileSystem::GetCwd(void)
{
    if (ApiFileSystem::cCurrentDirectory != NULL)
    {
        return reinterpret_cast<const char *>(ApiFileSystem::cCurrentDirectory);
    }
    return cFileSystem->GetCwdFS();
}

//--------//
// GetExecDirectory
//
// Gets the directory from where the program is executing.
//
// returns  The execution directory.
//--------//
//
const char * ApiFileSystem::GetExecDirectory(void)
{
    if (ApiFileSystem::cExecDirectory != NULL)
    {
        return ApiFileSystem::cExecDirectory;
    }
    return cFileSystem->GetExecDirectoryFS();
}

//--------//
// GetStatus
//
// Get the current status of the last operation performed by a given file.
//
// param[in]    lFile     The file to get status from.
// returns  Status on the last operation performed.
//--------//
//
int ApiFileSystem::GetStatus(File * lFile)
{
    if (lFile == NULL)
    {
        return File::FAILURE;
    }
    return lFile->GetStatus();
}

//--------//
// CleanupMemory
//
// Removes all dynamic memory created by using the Api.
//--------//
//
void ApiFileSystem::CleanupMemory(void)
{
    if (cCurrentDirectory != NULL)
    {
        delete [] cCurrentDirectory;
    }
    if (cExecDirectory != NULL)
    {
        delete [] cExecDirectory;
    }
    if (cFileSystem != NULL)
    {
        delete cFileSystem;
    }
}

//--------//
// Open
//
// Opens a file.
//
// param[in]  lFilename   Name of the file to open. 
// param[in]  lMode       Mode to open the file with, "r", "w", etc.
// param[out] lFile       The file structure created by open.
// returns  Status on the operation.
//--------//
//
int ApiFileSystem::Open(const char * lFilename, const char * lMode, File ** lFile)
{
    if (cFileSystem == NULL)
    {
        return File::FAILURE;
    }
    return cFileSystem->OpenFile(lFilename, lMode, lFile);
}

//--------//
// Read
//
// Reads a file.
//
// pre      lBuffer is expected to have enough space to hold
//          lCount bytes.
//
// param[in]    lBuffer     Location to read the bytes into.
// param[in]    lCount      Number of bytes to read.
// param[in]    lFile       The file structure to read from.
// returns  The number of bytes wrote to a file.
//          NOTE: Need to check status variable to know if succesful.
//--------//
//
size_t ApiFileSystem::Read(void * lBuffer, size_t lCount, File * lFile)
{
    if (cFileSystem == NULL)
    {
        if (lFile)
        {
            lFile->mStatus = File::FAILURE;
        }
        return 0;
    }
    return cFileSystem->ReadFile(lBuffer, lCount, lFile);
}

//--------//
// Write
//
// Writes to a file.
//
// pre      lBuffer is expected to hbe at least lCount bytes large.
//
// param[in]    lBuffer     Location to write the bytes from.
// param[in]    lCount      Number of bytes to write.
// param[in]    lFile       The file structure to write to.
// returns  The number of bytes wrote to a file.
//          NOTE: Need to check status variable to know if succesful.
//--------//
//
size_t ApiFileSystem::Write(void * lBuffer, size_t lCount, File * lFile)
{
    if (cFileSystem == NULL)
    {
        if (lFile)
        {
            lFile->mStatus = File::FAILURE;
        }
        return 0;
    }
    return cFileSystem->WriteFile(lBuffer, lCount, lFile);
}

//--------//
// Close
//
// Closes a file.
//
// param[in]    lFile       The file structure to close.
// returns  Status on the operation.
//--------//
//
int ApiFileSystem::Close(File * lFile)
{
    if (cFileSystem == NULL)
    {
        return File::FAILURE;
    }
    return cFileSystem->CloseFile(lFile);
}

//--------//
// Seek
//
// Sets the position indicator in file starting from the current position.
//
// param[in]    lOffset     Number of bytes to move file position.
// param[in]    lFile       The file structure to seek.
// returns  Status on the operation.
//--------//
//
int ApiFileSystem::Seek(long int lOffset, File * lFile)
{
    if (cFileSystem == NULL)
    {
        return File::FAILURE;
    }
    return cFileSystem->SeekFile(lOffset, lFile);
}

//--------//
// SeekFromStart
//
// Sets the position indicator in file starting from the start of the file.
//
// param[in]    lOffset     Number of bytes to move file position.
// param[in]    lFile       The file structure to seek.
// returns  Status on the operation.
//--------//
//
int ApiFileSystem::SeekFromStart(long int lOffset, File * lFile)
{
    if (cFileSystem == NULL)
    {
        return File::FAILURE;
    }
    return cFileSystem->SeekFromStartFile(lOffset, lFile);
}

//--------//
// SeekFromEnd
//
// Sets the position indicator in file starting from the end of the file.
//
// param[in]    lOffset     Number of bytes to move file position.
// param[in]    lFile       The file structure to seek.
// returns  Status on the operation.
//--------//
//
int ApiFileSystem::SeekFromEnd(long int lOffset, File * lFile)
{
    if (cFileSystem == NULL)
    {
        return File::FAILURE;
    }
    return cFileSystem->SeekFromEndFile(lOffset, lFile);
}

//--------//
// Tell
//
// Get the current position in the file.
//
// param[out]   lPosition   The current position.
// param[in]    lFile       The file structure to tell.
// returns  Status on the operation.
//--------//
//
int ApiFileSystem::Tell(long int * lPosition, File * lFile)
{
    if (cFileSystem == NULL)
    {
        return File::FAILURE;
    }
    return cFileSystem->TellFile(lPosition, lFile);
}
