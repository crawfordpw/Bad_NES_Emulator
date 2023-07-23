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
    mErrorDefs[FILE_GENERAL_ERROR]   = {"[!] %d, File operation could not be performed\n", sizeof("[!] %d, File operation could not be performed\n"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_COULD_NOT_OPEN]  = {"[!] %d, Could not open file, %s\n", sizeof("[!] %d, Could not open file, %s\n"), ErrorDefinition::DYNAMIC};
    mErrorDefs[FILE_COULD_NOT_CLOSE] = {"[!] %d, Could not close file\n", sizeof("[!] %d, Could not close file\n"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_ALREADY_OPENED]  = {"[!] %d, Tried to open file that is already opened\n", sizeof("[!] %d, Tried to open file that is already opened\n"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_ALREADY_CLOSED]  = {"[!] %d, Tried to open file that is already closed\n", sizeof("[!] %d, Tried to open file that is already closed\n"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_READ_ERROR]      = {"[!] %d, Could not read all requested data from file\n", sizeof("[!] %d, Could not read from file\n"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_WRITE_ERROR]     = {"[!] %d, Could not write to file\n", sizeof("[!] %d, Could not write to file\n"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_SEEK_ERROR]      = {"[!] %d, Seek operation failed\n", sizeof("[!] %d, Seek operation failed\n"), ErrorDefinition::NORMAL};
    mErrorDefs[FILE_TELL_ERROR]      = {"[!] %d, Tell operation failed\n", sizeof("[!] %d, Tell operation failed\n"), ErrorDefinition::NORMAL};

    // NES specific file errors.
    mErrorDefs[INVALID_NES_FORMAT]   = {"[!] %d, File provided is not a properly NES formatted file\n", sizeof("[!] %d, File provided is not a properly NES formatted file\n"), ErrorDefinition::NORMAL};
    mErrorDefs[MAPPER_NOT_SUPPORTED] = {"[!] %d, Mapper required by game not supported\n", sizeof("[!] %d, Mapper required by game not supported\n"), ErrorDefinition::NORMAL};

    // Memory errors.
    mErrorDefs[FAIL_TO_LOAD_MEMORY]  = {"[!] %d, Could not load program memory\n", sizeof("[!] %d, Could not load program memory\n"), ErrorDefinition::NORMAL};
    mErrorDefs[OUT_OF_MEMORY]        = {"[!] %d, Ran out of memory\n", sizeof("[!] %d, Ran out of memory\n"), ErrorDefinition::NORMAL};

    // Window errors.
    mErrorDefs[WINDOW_FAIL_INIT]  = {"[!] %d, Failed to initialize window\n", sizeof("[!] %d, Failed to initialize window\n"), ErrorDefinition::NORMAL};
}

//--------//
// Post
//
// Posts an error to stdout.
//
// param[in]    lError  The error to post.
//--------//
//
void ErrorManager::Post(int lError)
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
        char lBuffer[lSize];
        snprintf(lBuffer, lSize, mErrorDefs[lError].mMessage, lError);
        Post(lBuffer, lSize);
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
void ErrorManager::Post(int lError, const char * lExtra)
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
void ErrorManager::Post(int lError, const char * lExtra, size_t lSizeExtra)
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
