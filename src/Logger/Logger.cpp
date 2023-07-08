/////////////////////////////////////////////////////////////////////
//
// Logger.cpp
//
// Implementation file for logging.
//
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <Logger/ApiLogger.hpp>

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
            lId = lIndex;
            cLoggers[lIndex] = lLogger;
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

#ifdef STDOUT_LOGGER
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
    std::cout << &lMessage << "\n";
}
#endif

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
//--------//
//
FileLogger::FileLogger()
    : mFile(NULL)
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
#endif
