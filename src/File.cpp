/////////////////////////////////////////////////////////////////////
//
// File.cpp
//
// Implementation file for file operations.
//
/////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <File.hpp>

//--------//
//
// UnixFile
//
//--------//

//--------//
// UnixFile
//
// Constructor. Opens a file, setting mStatus in the process.
//
// param[in] lFilename   Name of the file to open. 
// param[in] lMode       Mode to open the file with, "r", "w", etc.
//--------//
//
UnixFile::UnixFile(const char * lFilename, const char * lMode)
{
    mStatus = Open(lFilename, lMode);
}

//--------//
// ~UnixFile
//
// Destructor. Closes the file handle
//--------//
//
UnixFile::~UnixFile(void)
{
    Close();
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
int UnixFile::Open(const char * lFilename, const char * lMode)
{
    // Don't try opening another file if one is already opened.
    if (mFileHandle != NULL)
    {
        mStatus = File::FILE_ALREADY_OPENED;
        return mStatus;
    }

    // Open the file.
    mFileHandle = fopen(lFilename, lMode);

    // Open failed for some reason.
    if (mFileHandle == NULL)
    {
        mStatus = File::FAILURE;
        return mStatus;
    }

    // Success!
    mStatus = File::SUCCESS;
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
int UnixFile::Close()
{
    // Don't try closing a file if the file handle doesn't exist.
    if (mFileHandle == NULL)
    {
        mStatus = File::FILE_ALREADY_CLOSED;
        return mStatus;
    }

    // Close the file.
    mStatus = fclose(mFileHandle);

    // Failed to close the file for some reason.
    if (mStatus != 0)
    {
        mStatus = File::FAILURE;
        return mStatus;
    }

    // Success!
    mStatus = File::SUCCESS;
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
size_t UnixFile::Read(void * lBuffer, size_t lCount)
{
    size_t lNumRead;

    // Don't try reading from a file if the file handle doesn't exist.
    if (mFileHandle == NULL)
    {
        mStatus = File::FILE_ALREADY_CLOSED;
        return mStatus;
    }

    // Don't have to do anything if we need to read 0 bytes.
    if (lCount == 0)
    {
        mStatus = File::SUCCESS;
        return mStatus;
    }

    // Don't want to read into a NULL buffer.
    if (lBuffer == NULL)
    {
        mStatus = File::FAILURE;
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
            mStatus = File::FAILURE;
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
size_t UnixFile::Write(void * lBuffer, size_t lCount)
{
    size_t lNumWrote;

    // Don't try reading from a file if the file handle doesn't exist.
    if (mFileHandle == NULL)
    {
        mStatus = File::FILE_ALREADY_CLOSED;
        return mStatus;
    }

    // Don't have to do anything if we need to write 0 bytes.
    if (lCount == 0)
    {
        mStatus = File::SUCCESS;
        return mStatus;
    }

    // Don't want to write from a NULL buffer.
    if (lBuffer == NULL)
    {
        mStatus = File::FAILURE;
        return mStatus;
    }

    // Read from the file.
    lNumWrote = fwrite(lBuffer, sizeof(uint8_t), lCount, mFileHandle);

    // If the number of bytes read is different than requested, something may have gone wrong.
    if (lNumWrote != lCount)
    {
        mStatus = File::FAILURE;
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
int UnixFile::Seek(long int lOffset)
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
int UnixFile::SeekFromStart(long int lOffset)
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
int UnixFile::SeekFromEnd(long int lOffset)
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
int UnixFile::SeekHelper(long int lOffset, int lMode)
{
    // Don't try seeking if the file handle doesn't exist.
    if (mFileHandle == NULL)
    {
        mStatus = File::FILE_ALREADY_CLOSED;
        return mStatus;
    }

    // Close the file.
    mStatus = fseek(mFileHandle, lOffset, lMode);

    // Failed to seek file for some reason.
    if (mStatus != 0)
    {
        mStatus = File::FAILURE;
        return mStatus;
    }

    // Success!
    mStatus = File::SUCCESS;
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
int UnixFile::Tell(long int * lPosition)
{
    // Don't try telling if the file handle doesn't exist.
    if (mFileHandle == NULL)
    {
        mStatus = File::FILE_ALREADY_CLOSED;
        return mStatus;
    }

    // Perform the tell.
    *lPosition = ftell(mFileHandle);

    // Failed to tell the file for some reason.
    if (*lPosition == -1)
    {
        mStatus = File::FAILURE;
        return mStatus;
    }

    // Success!
    mStatus = File::SUCCESS;
    return mStatus;
}
