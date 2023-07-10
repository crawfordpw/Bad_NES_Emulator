/////////////////////////////////////////////////////////////////////
//
// Logger.cpp
//
// Implementation file for logging.
//
/////////////////////////////////////////////////////////////////////

#include <Logger/ApiLogger.hpp>
#include <iostream>
#include <stdio.h>
#include <cstring>

//--------//
//
// ApiLogger
//
//--------//

Logger * ApiLogger::cLoggers[ApiLogger::MAX_NUM_LOGGERS];

//--------//
// ApiLogger
//
// Constructor.
//--------//
//
ApiLogger::ApiLogger(void)
{
    static int vInitialized = 0;

    if (vInitialized == 0)
    {
        for (int lIndex = 0; lIndex < MAX_NUM_LOGGERS; ++lIndex)
        {
            cLoggers[lIndex] = NULL;
        }
        ++vInitialized;
    }
}

//--------//
// RegisterLogger
//
// Registers a logger so it's CaptureLog message will get called
// along with othered registered loggers.
//
// param[in]    lLogger     Logger to register.
// returns  Logger id on succes, -1 if there was no room availble.
//--------//
//
int ApiLogger::RegisterLogger(Logger * lLogger)
{
    int lId = -1;

    for (int lIndex = 0; lIndex < MAX_NUM_LOGGERS; ++lIndex)
    {
        if (cLoggers[lIndex] == NULL)
        {
            cLoggers[lIndex] = lLogger;
            return lIndex;
        }
    }
    return lId;
}

//--------//
// UnregisterLogger
//
// Removed a logger from loggers array.
//
// param[in]    lId     Id of logger to remove.
//--------//
//
void ApiLogger::UnregisterLogger(int lId)
{
    if (lId < 0 || lId > MAX_NUM_LOGGERS)
    {
        return;
    }
    cLoggers[lId] = NULL;
}

//--------//
// Log
//
// Log a message, calling every registered loggers CaptureLog function.
//
// param[in]    lMessage    The message to log.
//--------//
//
void ApiLogger::Log(std::string * lMessage)
{
    for (int lIndex = 0; lIndex < MAX_NUM_LOGGERS; ++lIndex)
    {
        if (cLoggers[lIndex] != NULL)
        {
            cLoggers[lIndex]->CaptureLog(lMessage);
        }
    }
}

//--------//
// Log
//
// Log a message, calling every registered loggers CaptureLog function.
//
// param[in]    lMessage    The message to log.
//--------//
//
void ApiLogger::Log(const char * lMessage)
{
    size_t lLength = strlen(lMessage);
    Log(lMessage, lLength);
}

//--------//
// Log
//
// Log a message, calling every registered loggers CaptureLog function.
//
// param[in]    lMessage    The message to log.
// param[in]    lLength     Length of message.
//--------//
//
void ApiLogger::Log(const char * lMessage, size_t lLength)
{
    for (int lIndex = 0; lIndex < MAX_NUM_LOGGERS; ++lIndex)
    {
        if (cLoggers[lIndex] != NULL)
        {
            cLoggers[lIndex]->CaptureLog(lMessage, lLength);
        }
    }
}

//--------//
// CleanupMemory
//
// Removes all dynamic memory created by using the Api.
//--------//
//
void ApiLogger::CleanupMemory(void)
{
    for (int lIndex = 0; lIndex < MAX_NUM_LOGGERS; ++lIndex)
    {
        if (cLoggers[lIndex] != NULL)
        {
            delete cLoggers[lIndex];
            cLoggers[lIndex] = NULL;
        }
    }
}

//--------//
//
// ApiLogger
//
//--------//

//--------//
// Logger
//
// Constructor.
//
// param[in]    lRegister   Should it register with API.
//--------//
//
Logger::Logger(bool lRegister)
{
    if (lRegister)
    {
        mId = RegisterLogger(this);
    }
    else
    {
        mId = -1;
    }
}


//--------//
//
// StdLogger
//
//--------//

//--------//
// CaptureLog
//
// Prints message to console.
//
// param[in]    The message to log.
//--------//
//
void StdLogger::CaptureLog(std::string * lMessage)
{
    std::cout << *lMessage;
}

//--------//
// CaptureLog
//
// Prints message to console.
//
// param[in]    The message to log.
//--------//
//
void StdLogger::CaptureLog(const char * lMessage)
{
    printf("%s", lMessage);
}

//--------//
// CaptureLog
//
// Prints message to console.
//
// param[in]    The message to log.
//--------//
//
void StdLogger::CaptureLog(const char * lMessage, size_t lLength)
{
    printf("%s", lMessage);
}

#ifdef FILE_LOGGER
//--------//
//
// FileLogger
//
//--------//

//--------//
// FileLogger
//
// Constructor. Opens the file specified.
//
// param[in]    lFileName   The file to log messages to.
// param[in]    lRegister   Should it register with API.
//--------//
//
FileLogger::FileLogger(bool lRegister)
    : Logger(lRegister,) mFile(NULL)
{
}

//--------//
// ~FileLogger
//
// Destructor.
//--------//
//
FileLogger::~FileLogger(void)
{
    ApiFileSystem::Close(mFile);
}

//--------//
// OpenLogFile
//
// Opens the file specified for writing the messages to.
//
// param[in]    lFileName   The file to log messages to.
//--------//
//
int FileLogger::OpenLogFile(const char * lFileName)
{
    return ApiFileSystem::Open(lFileName, "w+", &mFile);
}

//--------//
// OpenLogFileFromExecDirectory
//
// Opens the file from the executable directory.
//
// param[in]    lFileName   The file to log messages to.
//--------//
//
int FileLogger::OpenLogFileFromExecDirectory(const char * lFileName)
{
    return ApiFileSystem::OpenFromExecDirectory(lFileName, "w+", &mFile);
}

//--------//
// CaptureLog
//
// Writes message to file
//
// param[in]    The message to log.
//--------//
//
void FileLogger::CaptureLog(std::string * lMessage)
{
    ApiFileSystem::Write(const_cast<char *>(lMessage->c_str()), lMessage->size(), mFile);
}

//--------//
// CaptureLog
//
// Writes message to file
//
// param[in]    The message to log.
//--------//
//
void FileLogger::CaptureLog(const char * lMessage)
{
    size_t lLength = strlen(lMessage);
    CaptureLog(lMessage, lLength);
}

//--------//
// CaptureLog
//
// Writes message to file
//
// param[in]    The message to log.
// param[in]    Size of message.
//--------//
//
void FileLogger::CaptureLog(const char * lMessage, size_t lLength)
{
    ApiFileSystem::Write(const_cast<char *>(lMessage), lLength, mFile);
}
#endif
