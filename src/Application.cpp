/////////////////////////////////////////////////////////////////////
//
// Application.cpp
//
// Implementation file for the application.
//
/////////////////////////////////////////////////////////////////////

#include <Application.hpp>
#include <Logger/ApiLogger.hpp>

//--------//
//
// Application
//
//--------//

//--------//
// Start
//
// Starts the application. Does some initialization and then enters
// the main processing loop.
//
// param[in]    lFilename   The nes rom to start running.
//--------//
//
void Application::Start(const char * lFilename)
{
    int lError;

    // If we are running cpu tests, don't continue afterwards.
    if (!mNes.CpuTest())
    {
        return;
    }

    // Load the cartridge with the rom.
    Cartridge lCartridge(lFilename);
    if (!lCartridge.IsValidImage())
    {
        CAPTURE_LOG("[!] Invalid ROM loaded into cartridge\n");
        return;
    }

    // Load cartridge into the system.
    mNes.InsertCartridge(&lCartridge);

    // Open the emulator window.
    mMainWindow = Window::Open();
    if (mMainWindow == NULL)
    {
        gErrorManager.Post(ErrorCodes::OUT_OF_MEMORY);
        return;
    }
    lError = mMainWindow->GetStatus();
    if (lError != ErrorCodes::SUCCESS)
    {
        gErrorManager.Post(lError);
        return;
    }

    // The main loop.
    Loop();
}

//--------//
// Loop
//
// The main procession loop of the application. This function does
// not return unless the user closes the application or something
// wrong has occured and the only resolution is to stop the program.
//--------//
//
void Application::Loop(void)
{
    while (!mMainWindow->ShouldClose() && mRunning)
    {
        mMainWindow->OnUpdate();
        //mNes.Clock();
    }
}
