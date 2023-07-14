/////////////////////////////////////////////////////////////////////
//
// Errors.cpp
//
// Implementation file for errors.
//
/////////////////////////////////////////////////////////////////////

#include <Errors/ApiErrors.hpp>

//--------//
//
// ErrorManager
//
//--------//

ErrorManager gErrorManager;

//--------//
// ErrorManager
//
// Constructor.
//--------//
//
ErrorManager::ErrorManager()
  : StdLogger(false)
{
    // File errors.
    mErrorDefs[FILE_GENERAL_ERROR]   = {"[!] %hhu, File operation could not be performed", sizeof("[!] %hhu, File operation could not be performed"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_COULD_NOT_OPEN]  = {"[!] %hhu, Could not open file, %s", sizeof("[!] %hhu, Could not open file, %s"), ErrorDefinition::DYNAMIC};
    mErrorDefs[FILE_COULD_NOT_CLOSE] = {"[!] %hhu, Could not close file", sizeof("[!] %hhu, Could not close file"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_ALREADY_OPENED]  = {"[!] %hhu, Tried to open file that is already opened", sizeof("[!] %hhu, Tried to open file that is already opened"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_ALREADY_CLOSED]  = {"[!] %hhu, Tried to open file that is already closed", sizeof("[!] %hhu, Tried to open file that is already closed"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_READ_ERROR]      = {"[!] %hhu, Could not read from file", sizeof("[!] %hhu, Could not read from file"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_WRITE_ERROR]     = {"[!] %hhu, Could not write to file", sizeof("[!] %hhu, Could not write to file"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_SEEK_ERROR]      = {"[!] %hhu, Seek operation failed", sizeof("[!] %hhu, Seek operation failed"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_TELL_ERROR]      = {"[!] %hhu, Tell operation failed", sizeof("[!] %hhu, Tell operation failed"), ErrorDefinition::NORMAL};

    // NES specific file errors.
    mErrorDefs[INVALID_NES_FORMAT]   = {"[!] %hhu, File provided is not a properly NES formatted file", sizeof("[!] %hhu, File provided is not a properly NES formatted file"), ErrorDefinition::NORMAL};
    mErrorDefs[MAPPER_NOT_SUPPORTED] = {"[!] %hhu, Mapper required by game not supported", sizeof("[!] %hhu, Mapper required by game not supported"), ErrorDefinition::NORMAL};

    // Memory errors.
    mErrorDefs[FAIL_TO_LOAD_MEMORY]  = {"[!] %hhu, Could not load program memory", sizeof("[!] %hhu, Could not load program memory"), ErrorDefinition::NORMAL};
    mErrorDefs[OUT_OF_MEMORY]        = {"[!] %hhu, Ran out of memory", sizeof("[!] %hhu, Ran out of memory"), ErrorDefinition::NORMAL};
}

//--------//
// Post
//
// Posts an error to stdout.
//
// param[in]    lError  The error to post.
//--------//
//
void ErrorManager::Post(uint8_t lError)
{
    // Don't go out of bounds or post a no error.
    if (lError > ErrorCodes::NUM_ERRORS || lError == ErrorCodes::SUCCESS)
    {
        return;
    }

    // Dynamic errors shouldn't call this function, but in case it does,
    // add the extra information.
    if (mErrorDefs[lError].mType == ErrorDefinition::DYNAMIC)
    {
        Post(lError, "UNSPECIFIED", sizeof("UNSPECIFIED"));
    }
    else
    {
        size_t lSize = mErrorDefs[lError].mMessageSize;
        char lBuffer [lSize];
        snprintf(lBuffer, lSize, mErrorDefs[lError].mMessage, lError);
        Post(mErrorDefs[lError].mMessage, lSize);
    }
}

//--------//
// Post
//
// Posts an error to stdout. DYNAMIC errors only!
//
// param[in]    lError  The error to post.
// param[in]    lExtra  Extra information for dynamic errors.
//--------//
//
void ErrorManager::Post(uint8_t lError, const char * lExtra)
{
    Post(lError, lExtra, strlen(lExtra));
}

//--------//
// Post
//
// Posts an error to stdout. DYNAMIC errors only!
//
// param[in]    lError      The error to post.
// param[in]    lExtra      Extra information for dynamic errors.
// param[in]    lSizeExtra  Size of extra string.
//--------//
//
void ErrorManager::Post(uint8_t lError, const char * lExtra, size_t lSizeExtra)
{
    if (lError > ErrorCodes::NUM_ERRORS || lError == ErrorCodes::SUCCESS)
    {
        return;
    }

    if (mErrorDefs[lError].mType == ErrorDefinition::DYNAMIC)
    {
        size_t lSize = mErrorDefs[lError].mMessageSize + lSizeExtra + 1;
        char lBuffer[lSize];
        snprintf(lBuffer, lSize, mErrorDefs[lError].mMessage, lError, lExtra);
        Post(lBuffer, lSize);
    }
    else
    {
        Post(mErrorDefs[lError].mMessage, mErrorDefs[lError].mMessageSize);
    }
}

//--------//
// Post
//
// All Post roads lead here. The ifdefs are there so we don't post
// to stdout out twice in case there is already an stdout logger.
//
// param[in]    lError  The error to post.
// param[in]    lSize   Size of error string.
//--------//
//
void ErrorManager::Post(const char * lError, size_t lSize)
{
#if defined(USE_LOGGER) && !defined(STDOUT_LOGGER)
    CaptureLog(lError, lSize);
    ApiLogger::Log(lError, lSize);
#elif defined(USE_LOGGER) && defined(STDOUT_LOGGER)
    ApiLogger::Log(lError, lSize);
#else
    CaptureLog(lError, lSize);
#endif
}
