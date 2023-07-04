//////////////////////////////////////////////////////////////////////////////////////////
//
// File.hpp
//
// Contains classes to manage files, regarless of filesystem or OS used. For now, just
// support Unix.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef FILE_HPP
#define FILE_HPP

#include <stdio.h>

//========//
// File
//
// Base class that every type of file should inherit from.
//========//
//
class File
{
    public:

        enum Status
        {
            SUCCESS                 =  0,
            FAILURE                 = -1,
            FILE_ALREADY_OPENED     = -2,
            FILE_ALREADY_CLOSED     = -3
        };

        File(void)          = default;
        virtual ~File(void) = default;

        virtual int     Open(const char * lFilename, const char * lMode) = 0;
        virtual size_t  Read(void * lBuffer, size_t lCount)  = 0;
        virtual size_t  Write(void * lBuffer, size_t lCount) = 0;
        virtual int     Close(void)                          = 0;
        virtual int     Seek(long int lOffset)               = 0;
        virtual int     SeekFromStart(long int lOffset)      = 0;
        virtual int     SeekFromEnd(long int lOffset)        = 0;
        virtual int     Tell(long int * lPosition)           = 0;

        int GetStatus(void) {return mStatus;}

    protected:

        int mStatus;
};

//========//
// UnixFile
//
// Unix specific implementation for file operations.
//========//
//
class UnixFile : public File
{
    public:

        UnixFile() : mFileHandle(NULL) {}
        UnixFile(const char * lFilename, const char * lMode);
        virtual ~UnixFile(void);

        virtual int     Open(const char * lFilename, const char * lMode) override;
        virtual size_t  Read(void * lBuffer, size_t lCount)  override;
        virtual size_t  Write(void * lBuffer, size_t lCount) override;
        virtual int     Close(void)                          override;
        virtual int     Seek(long int lOffset)               override;
        virtual int     SeekFromStart(long int lOffset)      override;
        virtual int     SeekFromEnd(long int lOffset)        override;
        virtual int     Tell(long int * lPosition)           override;

    protected:

        FILE * mFileHandle;

        int SeekHelper(long int lOffset, int lMode);
};

#endif
