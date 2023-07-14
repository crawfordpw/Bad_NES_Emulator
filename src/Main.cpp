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
#ifdef USE_LOGGER
#ifdef FILE_LOGGER
    FileLogger * lFileLogger = new FileLogger();
    lFileLogger->OpenLogFileFromExecDirectory("../Logs.txt");
#endif
    ApiLogger::Log("[i] Program Started\n");
#endif

    // Create the NES.
    System lNes;

#ifdef TEST_CPU
    ApiLogger::Log("[i] Starting cpu tests...\n");
    lNes.CpuTest();
#else
    // Start the system.
    lNes.Start();
#endif

    // Clean up any left over memory.
#ifdef USE_LOGGER
    ApiLogger::Log("[i] Program End\n");
    ApiLogger::CleanupMemory();
#endif
    ApiFileSystem::CleanupMemory();

    return 0;
}
