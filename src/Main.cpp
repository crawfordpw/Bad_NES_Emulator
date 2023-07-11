//////////////////////////////////////////////////////////////////////////////////////////
//
// Main.cpp
//
// Entry point for the system.
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <System.hpp>
#include <File/StdFile.hpp>
#include <Errors/ApiErrors.hpp>
#include <unistd.h>

#ifdef _WIN32
    WindowsFileSystem * gFileSystem = new WindowsFileSystem();
#elif __linux__
    LinuxFileSystem * gFileSystem = new LinuxFileSystem();
#endif

#ifdef USE_LOGGER
#ifdef STDOUT_LOGGER
    StdLogger * gStdLogger = new StdLogger();
#endif
#endif

//--------//
// main
//
// Entry point of the program.
//--------//
//
int main(void)
{
    long int lFileSize;
    File *   lFile;
    int      lStatus;

#ifdef USE_LOGGER
#ifdef FILE_LOGGER
    FileLogger * lFileLogger = new FileLogger();
    lFileLogger->OpenLogFileFromExecDirectory("../Logs.txt");
#endif
    ApiLogger::Log("Program Started!\n");
#endif
    // Create the NES.
    System lNes;

    // Grab program from a file.
    const char * lProgramFile = "../Program.txt";
    lStatus = ApiFileSystem::OpenFromExecDirectory(lProgramFile, "r", &lFile);

    if (lStatus != ErrorCodes::SUCCESS)
    {
        gErrorManager.Post(lStatus, lProgramFile);
        return 0;
    }

    // Get file size.
    ApiFileSystem::SeekFromEnd(0, lFile);
    ApiFileSystem::Tell(&lFileSize, lFile);
    ApiFileSystem::SeekFromStart(0, lFile);

    // Read entire file.
    char * lObjectCode = new char [lFileSize];
    ApiFileSystem::Read(lObjectCode, lFileSize, lFile);
    ApiFileSystem::Close(lFile);

    // Load into memory.
    lNes.LoadMemory(lObjectCode, lFileSize, 0);
    delete [] lObjectCode;

    // Start the system.
    lNes.Start();

#ifdef USE_LOGGER
    ApiLogger::Log("Program End!\n");
#endif

    // Clean up any left over memory.
#ifdef USE_LOGGER
    ApiLogger::CleanupMemory();
#endif
    ApiFileSystem::CleanupMemory();

    return 0;
}
