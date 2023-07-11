/////////////////////////////////////////////////////////////////////
//
// File.cpp
//
// Implementation file for file operations.
//
/////////////////////////////////////////////////////////////////////

#include <cstring>
#include <File/ApiFile.hpp>
#include <Errors/ErrorCodes.hpp>

//--------//
//
// ApiFileSystem
//
//--------//

FileSystem * ApiFileSystem::cFileSystem       = NULL;
char *       ApiFileSystem::cCurrentDirectory = NULL;
char *       ApiFileSystem::cExecDirectory    = NULL;
char         ApiFileSystem::cExecBuffer[EXEC_BUFFER_LENGTH];

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
    const char * lExecDirectory;

    memset(cExecBuffer, '\0', EXEC_BUFFER_LENGTH);

    if (ApiFileSystem::cExecDirectory != NULL)
    {
        lExecDirectory = cExecDirectory;
    }
    else
    {
        cFileSystem->GetExecDirectoryFS();

        if (cExecDirectory == NULL)
        {
            return cExecDirectory;
        }

        // Remove the executable name.
        for (int lIndex = MAX_FILENAME; lIndex > 0; --lIndex)
        {
            if (cExecDirectory[lIndex] == '/' || cExecDirectory[lIndex] == '\\')
            {
                cExecDirectory[lIndex + 1] = '\0';
                break;
            }
        }
        lExecDirectory = cExecDirectory;
    }
    strcpy(cExecBuffer, lExecDirectory);
    return lExecDirectory;
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
        return ErrorCodes::FILE_GENERAL_ERROR;
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
        cCurrentDirectory = NULL;
    }
    if (cExecDirectory != NULL)
    {
        delete [] cExecDirectory;
        cExecDirectory = NULL;
    }
    if (cFileSystem != NULL)
    {
        delete cFileSystem;
        cFileSystem = NULL;
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
        return ErrorCodes::FILE_GENERAL_ERROR;
    }
    return cFileSystem->OpenFile(lFilename, lMode, lFile);
}

//--------//
// OpenFromExecDirectory
//
// Opens a file from the executable directory.
//
// param[in]  lFilename   Name of the file to open. 
// param[in]  lMode       Mode to open the file with, "r", "w", etc.
// param[out] lFile       The file structure created by open.
// returns  Status on the operation.
//--------//
//
int ApiFileSystem::OpenFromExecDirectory(const char * lFilename, const char * lMode, File ** lFile)
{
    GetExecDirectory();
    if (cExecDirectory == NULL)
    {
        return ErrorCodes::FILE_GENERAL_ERROR;
    }
    strcat(cExecBuffer, lFilename);
    return Open(cExecBuffer, lMode, lFile);
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
            return ErrorCodes::FILE_GENERAL_ERROR;
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
            return ErrorCodes::FILE_GENERAL_ERROR;
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
        return ErrorCodes::FILE_GENERAL_ERROR;
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
        return ErrorCodes::FILE_GENERAL_ERROR;
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
        return ErrorCodes::FILE_GENERAL_ERROR;
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
        return ErrorCodes::FILE_GENERAL_ERROR;
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
        return ErrorCodes::FILE_GENERAL_ERROR;
    }
    return cFileSystem->TellFile(lPosition, lFile);
}
