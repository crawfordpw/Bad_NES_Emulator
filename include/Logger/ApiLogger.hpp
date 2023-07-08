/////////////////////////////////////////////////////////////////////
//
// ApiLogger.hpp
//
// Api file for agnostic logging.
//
/////////////////////////////////////////////////////////////////////

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <Common.hpp>

#ifdef FILE_LOGGER
#include <File/ApiFile.hpp>
#endif

class Logger;

//========//
// ApiLogger
//
// An Api class that files outside of this one should use to log.
//========//
//
class ApiLogger
{
    public:

        enum
        {
            MAX_NUM_LOGGERS = 2
        };

        ApiLogger(void);
        virtual ~ApiLogger(void) = default;

        static void Log(std::string * lMessage);
        static void CleanupMemory();

    protected:

        int  RegisterLogger(Logger * lLogger);
        void UnregisterLogger(int lId);

        static Logger * cLoggers[];
};

//========//
// Logger
//
// Base logger class that every logger should inherit from.
//========//
//
class Logger : public ApiLogger
{
    friend class ApiLogger;

    public:

        Logger(void)          {mId = RegisterLogger(this);}
        virtual ~Logger(void) {UnregisterLogger(mId);}

    protected:

        virtual void CaptureLog(std::string * lMessage) = 0;

        int mId;
};

#ifdef STDOUT_LOGGER
//========//
// StdLogger
//
// Log to standard out.
//========//
//
class StdLogger : public Logger
{
    public:

        StdLogger(void)          = default;
        virtual ~StdLogger(void) = default;

    protected:

        virtual void CaptureLog(std::string * lMessage);
};
#endif

#ifdef FILE_LOGGER
//========//
// FileLogger
//
// Log to a file.
//========//
//
class FileLogger : public Logger
{
    public:

        FileLogger();
        virtual ~FileLogger(void);

        int OpenLogFile(const char * lFileName);

    protected:

        virtual void CaptureLog(std::string * lMessage);

        const char * mFileName;
        File *       mFile;
};
#endif

#endif
