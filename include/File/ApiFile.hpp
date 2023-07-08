//////////////////////////////////////////////////////////////////////////////////////////
//
// ApiFile.hpp
//
// Api file for generic file operations. This is what gets included when want to do file
// operations. In a file specific file, add the file system specific implementation.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef FILE_HPP
#define FILE_HPP

#include <stdio.h>
#include <stdint.h>

class FileSystem;
class ApiFileSystem;

//========//
// File
//
// Base class that every type of file should inherit from.
//========//
//
class File
{
    friend class ApiFileSystem;

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
// ApiFileSystem
//
// An Api to perform file operations agnostic to what the system is.
//========//
//
class ApiFileSystem
{
    public:

        ApiFileSystem(void)          = default;
        virtual ~ApiFileSystem(void) = default;

        static const char * GetCwd(void);
        static const char * GetExecDirectory(void);
        static int          GetStatus(File * lFile);
        static void         CleanupMemory();

        static int     Open(const char * lFilename, const char * lMode, File ** lFile);
        static int     OpenFromExecDirectory(const char * lFilename, const char * lMode, File ** lFile);
        static size_t  Read(void * lBuffer, size_t lCount, File * lFile);
        static size_t  Write(void * lBuffer, size_t lCount, File * lFile);
        static int     Close(File * lFile);
        static int     Seek(long int lOffset, File * lFile);
        static int     SeekFromStart(long int lOffset, File * lFile);
        static int     SeekFromEnd(long int lOffset, File * lFile);
        static int     Tell(long int * lPosition, File * lFile);

    protected:

        enum
        {
            MAX_FILENAME       = 250,
            EXEC_BUFFER_LENGTH = MAX_FILENAME * 2,
        };

        static FileSystem * cFileSystem;
        static char *       cCurrentDirectory;
        static char *       cExecDirectory;
        static char         cExecBuffer[];
};

//========//
// FileSystem
//
// Base class that ApiFileSystem will use to invoke file specific implementations.
//========//
//
class FileSystem : public ApiFileSystem
{
    friend class ApiFileSystem;

    public:

        FileSystem(void) {cFileSystem = this;}
        virtual ~FileSystem(void) = default;

    protected:

        virtual const char *  GetCwdFS(void)           = 0;
        virtual const char *  GetExecDirectoryFS(void) = 0;

        virtual int     OpenFile(const char * lFilename, const char * lMode, File ** lFile) = 0;
        virtual size_t  ReadFile(void * lBuffer, size_t lCount, File * lFile)  = 0;
        virtual size_t  WriteFile(void * lBuffer, size_t lCount, File * lFile) = 0;
        virtual int     CloseFile(File * lFile)                                = 0;
        virtual int     SeekFile(long int lOffset, File * lFile)               = 0;
        virtual int     SeekFromStartFile(long int lOffset, File * lFile)      = 0;
        virtual int     SeekFromEndFile(long int lOffset, File * lFile)        = 0;
        virtual int     TellFile(long int * lPosition, File * lFile)           = 0;
};

#endif
