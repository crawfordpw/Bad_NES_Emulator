//////////////////////////////////////////////////////////////////////////////////////////
//
// Application.hpp
//
// File for the entire application.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "System.hpp"
#include "Window/Window.hpp"

//========//
// Application
//
// The main apoplication that control everything in the program.
//========//
//
class Application
{
    public:

        Application(void) : mRunning(true) {}
        ~Application(void)                 {if (mMainWindow) {delete mMainWindow;}}

        void Start(const char * lFilename);

    protected:

        void Loop(void);

        Window * mMainWindow;
        System   mNes;
        bool     mRunning;
};

#endif
