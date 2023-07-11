/////////////////////////////////////////////////////////////////////
//
// StdFile.cpp
//
// Implementation file for c++ standard files.
//
/////////////////////////////////////////////////////////////////////

#include <File/StdFile.hpp>
#include <Errors/ApiErrors.hpp>

#ifdef _WIN32
    #include <direct.h>
    #include <windows.h>
    #define GetCurrentDir _getcwd
#elif __linux__
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

//--------//
//
// StdFile
//
//--------//

//--------//
// StdFile
//
// Constructor. Opens a file, setting mStatus in the process.
//
// param[in] lFilename   Name of the file to open. 
// param[in] lMode       Mode to open the file with, "r", "w", etc.
//--------//
//
StdFile::StdFile(const char * lFilename, const char * lMode)
  : mFileHandle(NULL)
{
    mStatus = Open(lFilename, lMode);
}

//--------//
// Open
//
// Opens a file.
//
// param[in] lFilename   Name of the file to open. 
// param[in] lMode       Mode to open the file with, "r", "w", etc.
// returns  Status on the operation.
//--------//
//
int StdFile::Open(const char * lFilename, const char * lMode)
{
    // Don't try opening another file if one is already opened.
    if (mFileHandle != NULL)
    {
        mStatus = ErrorCodes::FILE_ALREADY_OPENED;
        return mStatus;
    }

    // Open the file.
    mFileHandle = fopen(lFilename, lMode);

    // Open failed for some reason.
    if (mFileHandle == NULL)
    {
        mStatus = ErrorCodes::FILE_COULD_NOT_OPEN;
        return mStatus;
    }

    // Success!
    mStatus = ErrorCodes::SUCCESS;
    return mStatus;
}

//--------//
// Close
//
// Closes a file.
//
// returns  Status on the operation.
//--------//
//
int StdFile::Close()
{
    // Don't try closing a file if the file handle doesn't exist.
    if (mFileHandle == NULL)
    {
        mStatus = ErrorCodes::FILE_ALREADY_CLOSED;
        return mStatus;
    }

    // Close the file.
    mStatus = fclose(mFileHandle);
    mFileHandle = NULL;

    // Failed to close the file for some reason.
    if (mStatus != 0)
    {
        mStatus = ErrorCodes::FILE_COULD_NOT_CLOSE;
        return mStatus;
    }

    // Success!
    mStatus = ErrorCodes::SUCCESS;
    return mStatus;
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
// returns  The number of bytes wrote to a file.
//          NOTE: Need to check status variable to know if succesful.
//--------//
//
size_t StdFile::Read(void * lBuffer, size_t lCount)
{
    size_t lNumRead;

    // Don't try reading from a file if the file handle doesn't exist.
    if (mFileHandle == NULL)
    {
        mStatus = ErrorCodes::FILE_ALREADY_CLOSED;
        return mStatus;
    }

    // Don't have to do anything if we need to read 0 bytes.
    if (lCount == 0)
    {
        mStatus = ErrorCodes::SUCCESS;
        return mStatus;
    }

    // Don't want to read into a NULL buffer.
    if (lBuffer == NULL)
    {
        mStatus = ErrorCodes::FILE_GENERAL_ERROR;
        return mStatus;
    }

    // Read from the file.
    lNumRead = fread(lBuffer, sizeof(uint8_t), lCount, mFileHandle);

    // If the number of bytes read is different than requested, something may have gone wrong.
    if (lNumRead != lCount)
    {
        // It's only an error if end of file wasn't reached.
        if (feof(mFileHandle) != 0)
        {
            mStatus = ErrorCodes::FILE_READ_ERROR;
        }
    }

    // Return the number of bytes that were read.
    return lNumRead;
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
// returns  The number of bytes wrote to a file.
//          NOTE: Need to check status variable to know if succesful.
//--------//
//
size_t StdFile::Write(void * lBuffer, size_t lCount)
{
    size_t lNumWrote;

    // Don't try reading from a file if the file handle doesn't exist.
    if (mFileHandle == NULL)
    {
        mStatus = ErrorCodes::FILE_ALREADY_CLOSED;
        return mStatus;
    }

    // Don't have to do anything if we need to write 0 bytes.
    if (lCount == 0)
    {
        mStatus = ErrorCodes::SUCCESS;
        return mStatus;
    }

    // Don't want to write from a NULL buffer.
    if (lBuffer == NULL)
    {
        mStatus = ErrorCodes::FILE_GENERAL_ERROR;
        return mStatus;
    }

    // Read from the file.
    lNumWrote = fwrite(lBuffer, sizeof(uint8_t), lCount, mFileHandle);

    // If the number of bytes read is different than requested, something may have gone wrong.
    if (lNumWrote != lCount)
    {
        mStatus = ErrorCodes::SUCCESS;
    }

    // Return the number of bytes that were read.
    return lNumWrote;
}

//--------//
// Seek
//
// Sets the position indicator in file starting from the current position.
//
// param[in]    lOffset     Number of bytes to move file position.
// returns  Status on the operation.
//--------//
//
int StdFile::Seek(long int lOffset)
{
    return SeekHelper(lOffset, SEEK_CUR);
}

//--------//
// SeekFromStart
//
// Sets the position indicator in file starting from the start of the file.
//
// param[in]    lOffset     Number of bytes to move file position.
// returns  Status on the operation.
//--------//
//
int StdFile::SeekFromStart(long int lOffset)
{
    return SeekHelper(lOffset, SEEK_SET);
}

//--------//
// SeekFromEnd
//
// Sets the position indicator in file starting from the end of the file.
//
// param[in]    lOffset     Number of bytes to move file position.
// returns  Status on the operation.
//--------//
//
int StdFile::SeekFromEnd(long int lOffset)
{
    return SeekHelper(lOffset, SEEK_END);
}

//--------//
// SeekHelper
//
// Helper function for the seek functions. All seeks should call this,
// passing in where they want to start seeking from.
//
// Unsure yet if this should be part of the base class.
//
// param[in]    lOffset     Number of bytes to move file position.
// param[in]    lMode       Type of seek operation.
// returns  Status on the operation.
//--------//
//
int StdFile::SeekHelper(long int lOffset, int lMode)
{
    // Don't try seeking if the file handle doesn't exist.
    if (mFileHandle == NULL)
    {
        mStatus = ErrorCodes::FILE_ALREADY_CLOSED;
        return mStatus;
    }

    // Close the file.
    mStatus = fseek(mFileHandle, lOffset, lMode);

    // Failed to seek file for some reason.
    if (mStatus != 0)
    {
        mStatus = ErrorCodes::FILE_SEEK_ERROR;
        return mStatus;
    }

    // Success!
    mStatus = ErrorCodes::SUCCESS;
    return mStatus;
}

//--------//
// Tell
//
// Get the current position in the file.
//
// param[out]    lPosition     The current position.
// returns  Status on the operation.
//--------//
//
int StdFile::Tell(long int * lPosition)
{
    // Don't try telling if the file handle doesn't exist.
    if (mFileHandle == NULL)
    {
        mStatus = ErrorCodes::FILE_ALREADY_CLOSED;
        return mStatus;
    }

    // Perform the tell.
    *lPosition = ftell(mFileHandle);

    // Failed to tell the file for some reason.
    if (*lPosition == -1)
    {
        mStatus = ErrorCodes::FILE_TELL_ERROR;
        return mStatus;
    }

    // Success!
    mStatus = ErrorCodes::SUCCESS;
    return mStatus;
}

//--------//
//
// StdFileSystem
//
//--------//

//--------//
// OpenFile
//
// Opens a file. Creates a file using "new". Close will clean up the memory.
//
// param[in]  lFilename   Name of the file to open. 
// param[in]  lMode       Mode to open the file with, "r", "w", etc.
// param[out] lFile       The file structure created by open.
// returns  Status on the operation.
//--------//
//
int StdFileSystem::OpenFile(const char * lFilename, const char * lMode, File ** lFile)
{
    StdFile * lStdFile = new(std::nothrow) StdFile(lFilename, lMode);

    if (lStdFile == NULL)
    {
        gErrorManager.Post(ErrorCodes::OUT_OF_MEMORY);
        return ErrorCodes::FILE_GENERAL_ERROR;
    }

    *lFile = reinterpret_cast<File *>(lStdFile);
    return lStdFile->GetStatus();
}

//--------//
// CloseFile
//
// Closes a file. Cleans up memory from Open.
//
// param[in]    lFile       The file structure to close.
// returns  Status on the operation.
//--------//
//
int StdFileSystem::CloseFile(File * lFile)
{
    if (lFile == NULL)
    {
        return ErrorCodes::FILE_GENERAL_ERROR;
    }
    int lStatus = lFile->Close();
    delete reinterpret_cast<StdFile *>(lFile);
    return lStatus;
}

//--------//
// ReadFile
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
size_t StdFileSystem::ReadFile(void * lBuffer, size_t lCount, File * lFile)
{
    if (lFile == NULL)
    {
        return 0;
    }
    return lFile->Read(lBuffer, lCount);
}

//--------//
// WriteFile
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
size_t StdFileSystem::WriteFile(void * lBuffer, size_t lCount, File * lFile)
{
    if (lFile == NULL)
    {
        return 0;
    }
    return lFile->Write(lBuffer, lCount);
}

//--------//
// SeekFile
//
// Sets the position indicator in file starting from the current position.
//
// param[in]    lOffset     Number of bytes to move file position.
// param[in]    lFile       The file structure to seek.
// returns  Status on the operation.
//--------//
//
int StdFileSystem::SeekFile(long int lOffset, File * lFile)
{
    if (lFile == NULL)
    {
        return ErrorCodes::FILE_GENERAL_ERROR;
    }
    return lFile->Seek(lOffset);
}

//--------//
// SeekFromStartFile
//
// Sets the position indicator in file starting from the start of the file.
//
// param[in]    lOffset     Number of bytes to move file position.
// param[in]    lFile       The file structure to seek.
// returns  Status on the operation.
//--------//
//
int StdFileSystem::SeekFromStartFile(long int lOffset, File * lFile)
{
    if (lFile == NULL)
    {
        return ErrorCodes::FILE_GENERAL_ERROR;
    }
    return lFile->SeekFromStart(lOffset);
}

//--------//
// SeekFromEndFile
//
// Sets the position indicator in file starting from the end of the file.
//
// param[in]    lOffset     Number of bytes to move file position.
// param[in]    lFile       The file structure to seek.
// returns  Status on the operation.
//--------//
//
int StdFileSystem::SeekFromEndFile(long int lOffset, File * lFile)
{
    if (lFile == NULL)
    {
        return ErrorCodes::FILE_GENERAL_ERROR;
    }
    return lFile->SeekFromEnd(lOffset);
}

//--------//
// TellFile
//
// Get the current position in the file.
//
// param[out]   lPosition     The current position.
// param[in]    lFile         The file structure to tell.
// returns  Status on the operation.
//--------//
//
int StdFileSystem::TellFile(long int * lPosition, File * lFile)
{
    if (lFile == NULL)
    {
        return ErrorCodes::FILE_GENERAL_ERROR;
    }
    return lFile->Tell(lPosition);
}

//--------//
// GetCwdFS
//
// Get the current working directory.
//
// returns  The current working directory.
//--------//
//
const char * StdFileSystem::GetCwdFS(void)
{
    cCurrentDirectory = new(std::nothrow) char [MAX_FILENAME];
    if (cCurrentDirectory == NULL)
    {
        gErrorManager.Post(ErrorCodes::OUT_OF_MEMORY);
        return NULL;
    }

    if (!GetCurrentDir(cCurrentDirectory, MAX_FILENAME))
    {
        delete [] cCurrentDirectory;
        cCurrentDirectory = NULL;
        return cCurrentDirectory;
    }
    return reinterpret_cast<const char *>(cCurrentDirectory);
}

//--------//
//
// WindowsFileSystem
//
//--------//

#ifdef _WIN32
//--------//
// GetExecDirectoryFS
//
// Gets the directory from where the program is executing.
//
// returns  The execution directory.
//--------//
//
const char * WindowsFileSystem::GetExecDirectoryFS(void)
{
    cExecDirectory = new(std::nothrow) char[MAX_FILENAME];
    if (cExecDirectory == NULL)
    {
        gErrorManager.Post(ErrorCodes::OUT_OF_MEMORY);
        return NULL;
    }

    int lBytes = GetModuleFileName(NULL, cExecDirectory, MAX_FILENAME);

    if (lBytes == 0)
    {
        delete [] cExecDirectory;
        cExecDirectory = NULL;
    }
    return reinterpret_cast<const char *>(cExecDirectory);
}
#endif

//--------//
//
// LinuxFileSystem
//
//--------//

#ifdef __linux__
//--------//
// GetExecDirectoryFS
//
// Gets the directory from where the program is executing.
//
// returns  The execution directory.
//--------//
//
const char * LinuxFileSystem::GetExecDirectoryFS(void)
{
    cExecDirectory = new(std::nothrow) char[MAX_FILENAME];
    if (cExecDirectory == NULL)
    {
        gErrorManager.Post(ErrorCodes::OUT_OF_MEMORY);
        return NULL;
    }

    int lBytes = readlink("/proc/self/exe", cExecDirectory, MAX_FILENAME);

    if (lBytes <= 0)
    {
        delete [] cExecDirectory;
        cExecDirectory = NULL;
    }
    cExecDirectory[lBytes] = '\0';
    return reinterpret_cast<const char *>(cExecDirectory);
}
#endif
