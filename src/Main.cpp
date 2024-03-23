//////////////////////////////////////////////////////////////////////////////////////////
//
// Main.cpp
//
// Entry point for the system.
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <Application.hpp>
#include <File/StdFile.hpp>
#include <unistd.h>

#ifdef _WIN32
    WindowsFileSystem * gFileSystem = new WindowsFileSystem();
#elif __linux__
    LinuxFileSystem * gFileSystem = new LinuxFileSystem();
#endif

#ifdef STDOUT_LOGGER
    StdLogger * gStdLogger = new StdLogger();
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

    // Start the application.
    Application lApp;
    lApp.Start("../tests/nestest.nes");

    // Clean up any left over memory.
    ApiFileSystem::CleanupMemory();
    Window::ShutDown();

#ifdef USE_LOGGER
    ApiLogger::Log("[i] Program End\n");
    ApiLogger::CleanupMemory();
#endif

    delete gFileSystem;

    return 0;
}
