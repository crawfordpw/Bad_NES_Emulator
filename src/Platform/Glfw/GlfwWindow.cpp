/////////////////////////////////////////////////////////////////////
//
// Application.cpp
//
// Implementation file for GLFW windows.
//
/////////////////////////////////////////////////////////////////////

#include <new>
#include "GlfwWindow.hpp"

//--------//
//
// Window
//
//--------//

bool Window::cInitialized = false;

//--------//
// Open
//
// Creates a new GLFW window.
//
// param[in]    lProperties     Properties used to create the window with.
//--------//
//
Window * Window::Open(const WindowProperties & lProperties)
{
    return new(std::nothrow) GlfwWindow(lProperties);
}

//--------//
// ShutDown
//
// Terminates glfw initialization.
//--------//
//
void Window::ShutDown(void)
{
    cInitialized = false;
    glfwTerminate();
}

//--------//
//
// GlfwWindow
//
//--------//

//--------//
// Init
//
// Initializes GLFW if not done so already, and creates the window.
//
// param[in]    lProperties     Properties used to create the window with.
//--------//
//
void GlfwWindow::Init(const WindowProperties * lProperties)
{
    int lWindowWidth  = lProperties->mWidth * lProperties->mPixelWidth;
    int lWindowHeight = lProperties->mHeight * lProperties->mPixelHeight;

    // Copy the relevant window property data.
    mData.mWidth       = lProperties->mWidth;
    mData.mHeight      = lProperties->mHeight;
    mData.mPixelWidth  = lProperties->mPixelWidth;
    mData.mPixelHeight = lProperties->mPixelHeight;

    mStatus = ErrorCodes::WINDOW_FAIL_INIT;

    // Only initialize glfw one time on the first window creation.
    if (cInitialized == false)
    {
        if (!glfwInit())
        {
            return;
        }
        cInitialized = true;
    }

    // Hide the window while creating and positioning it.
    mMonitor = glfwGetPrimaryMonitor();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // Create the window.
    mWindow = glfwCreateWindow(lWindowWidth, lWindowHeight, lProperties->mTitle, NULL, NULL);
    if (mWindow == NULL)
    {
        return;
    }

    // Center the window if necessary.
    if (lProperties->mCenter)
    {
        int lMonitorX;
        int lMonitorY;
        const GLFWvidmode * lVideoMode = glfwGetVideoMode(mMonitor);
        glfwGetMonitorPos(mMonitor, &lMonitorX, &lMonitorY);
        glfwSetWindowPos(mWindow, lMonitorX + (lVideoMode->width - lWindowWidth) / 2, lMonitorY + (lVideoMode->height - lWindowHeight) / 2);
    }

    // Some glfw defaults.
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);
    glfwSetWindowShouldClose(mWindow, 0);
    glfwSetWindowUserPointer(mWindow, NULL);
    glfwDefaultWindowHints();
    glfwShowWindow(mWindow);

    // All is good.
    mStatus = ErrorCodes::SUCCESS;
}

//--------//
// ShouldClose
//
// Check to see if the window should close.
//--------//
//
bool GlfwWindow::ShouldClose(void)
{
    return glfwWindowShouldClose(mWindow);
}

//--------//
// OnUpdate
//
// Updates the GLFW window. Should be called every frame.
//--------//
//
void GlfwWindow::OnUpdate(void)
{
    glfwPollEvents();
    glfwSwapBuffers(mWindow);
}

//--------//
// Close
//
// Closes the window.
//--------//
//
void GlfwWindow::Close()
{
    glfwDestroyWindow(mWindow);
}
