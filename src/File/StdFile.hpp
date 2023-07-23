//////////////////////////////////////////////////////////////////////////////////////////
//
// StdFile.hpp
//
// Contains classes to manage files for c++ standard files.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef STD_FILE_HPP
#define STD_FILE_HPP

#include "ApiFile.hpp"

//========//
// StdFile
//
// Standard file.
//========//
//
class StdFile : public File
{
    public:

        StdFile() : mFileHandle(NULL) {}
        StdFile(const char * lFilename, const char * lMode);
        virtual ~StdFile(void) = default;

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

//========//
// StdFileSystem
//
// Standard FileSystem.
//========//
//
class StdFileSystem : public FileSystem
{
    public:

        StdFileSystem(void)              = default;
        virtual ~StdFileSystem(void)     = default;

    protected:
        virtual const char *  GetCwdFS(void)           override;
        virtual const char *  GetExecDirectoryFS(void) = 0;

        virtual int     OpenFile(const char * lFilename, const char * lMode, File ** lFile) override;
        virtual size_t  ReadFile(void * lBuffer, size_t lCount, File * lFile)   override;
        virtual size_t  WriteFile(void * lBuffer, size_t lCount, File * lFile)  override;
        virtual int     CloseFile(File * lFile)                                 override;
        virtual int     SeekFile(long int lOffset, File * lFile)                override;
        virtual int     SeekFromStartFile(long int lOffset, File * lFile)       override;
        virtual int     SeekFromEndFile(long int lOffset, File * lFile)         override;
        virtual int     TellFile(long int * lPosition, File * lFile)            override;
};

//========//
// WindowsFileSystem
//
// Windows Standard FileSystem.
//========//
//
#ifdef _WIN32
class WindowsFileSystem : public StdFileSystem
{
    public:

        WindowsFileSystem(void)              = default;
        virtual ~WindowsFileSystem(void)     = default;

    protected:
        virtual const char *  GetExecDirectoryFS(void) override;
};
#endif

//========//
// LinuxFileSystem
//
// Linux Standard FileSystem.
//========//
//
#ifdef __linux__
class LinuxFileSystem : public StdFileSystem
{
    public:

        LinuxFileSystem()              = default;
        virtual ~LinuxFileSystem(void) = default;

    protected:

        virtual const char *  GetExecDirectoryFS(void) override;
};
#endif

#endif
