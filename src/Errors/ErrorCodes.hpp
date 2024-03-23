//////////////////////////////////////////////////////////////////////////////////////////
//
// ApiErrors.hpp
//
// List of all error codes used in the system.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef ERROR_CODES_HPP
#define ERROR_CODES_HPP

//========//
// All error codes possible.
//
enum ErrorCodes
{
    SUCCESS,    // NO ERROR

    // GENERAL ERRORS
    INTERNAL_ERROR,         // Reserved for errors cases that shouldn't happen.

    // File errors.
    FILE_GENERAL_ERROR,
    FILE_COULD_NOT_OPEN,
    FILE_COULD_NOT_CLOSE,
    FILE_ALREADY_OPENED,
    FILE_ALREADY_CLOSED,
    FILE_READ_ERROR,
    FILE_WRITE_ERROR,
    FILE_SEEK_ERROR,
    FILE_TELL_ERROR,

    // NES specific file errors.
    INVALID_NES_FORMAT,
    MAPPER_NOT_SUPPORTED,

    // Memory errors.
    FAIL_TO_LOAD_MEMORY,
    OUT_OF_MEMORY,

    // Window errors
    WINDOW_FAIL_INIT,

    NUM_ERRORS
};

#endif
