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
{
    mErrorDefs[COULD_NOT_OPEN_FILE] = {"[!] Could not open file, %s", sizeof("[!] Could not open file, %s"), ErrorDefinition::DYNAMIC};
    mErrorDefs[READ_ERROR]          = {"[!] Could not read from file", sizeof("[!] Could not read from file"), ErrorDefinition::NORMAL};
    mErrorDefs[WRITE_ERROR]         = {"[!] Could not write to file", sizeof("[!] Could not write to file"), ErrorDefinition::NORMAL};
    mErrorDefs[INVALID_NES_FORMAT]  = {"[!] File provided is not a properly NES formatted file", sizeof("[!] File provided is not an NES formatted file"), ErrorDefinition::NORMAL};
    mErrorDefs[FAIL_TO_LOAD_MEMORY] = {"[!] Could not load program memory", sizeof("[!] Could not load program memory"), ErrorDefinition::NORMAL};
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
    // Don't go out of bounds.
    if (lError > NUM_ERRORS)
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
        Post(mErrorDefs[lError].mMessage, mErrorDefs[lError].mMessageSize);
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
    if (lError > NUM_ERRORS)
    {
        return;
    }

    size_t lSize = mErrorDefs[lError].mMessageSize + lSizeExtra + 1;
    char lBuffer[lSize];
    snprintf(lBuffer, lSize, mErrorDefs[lError].mMessage, lExtra);
    Post(lBuffer, lSize);
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
