//////////////////////////////////////////////////////////////////////////////////////////
//
// ApiErrors.hpp
//
// File that manages errors for the systems.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef API_ERRORS_HPP
#define API_ERRORS_HPP

#include <cstring>
#include <stdint.h>
#include <Logger/ApiLogger.hpp>

class ErrorManager;

//========//
// All error codes possible to register.
//
enum ErrorCodes
{
    // File errors.
    COULD_NOT_OPEN_FILE,
    READ_ERROR,
    WRITE_ERROR,

    // NES specific file errors.
    INVALID_NES_FORMAT,

    // Memory errors.
    FAIL_TO_LOAD_MEMORY,

    NUM_ERRORS
};

//========//
// ErrorDefinition
//
// Contains information about the error.
//========//
//
struct ErrorDefinition
{
    enum
    {
        NORMAL,
        DYNAMIC
    };

    const char * mMessage;
    size_t       mMessageSize;
    uint8_t      mType;
};

//========//
// ErrorLogger
//
// Manages errors posted in the system. A special type of Logger that is 
// always present and logs errorson the system. Unlike the regular std logger
// or a file logger.
//========//
//
class ErrorManager : public StdLogger
{
    public:

        ErrorManager(void);
        virtual ~ErrorManager(void) = default;

        void Post(uint8_t lError);
        void Post(uint8_t lError, const char * lExtra);
        void Post(uint8_t lError, const char * lExtra, size_t lSizeExtra);

    protected:

        void Post(const char * lError, size_t lSize);

        ErrorDefinition mErrorDefs[NUM_ERRORS];
};

extern ErrorManager gErrorManager;

#endif
