//////////////////////////////////////////////////////////////////////////////////////////
//
// GlfwWindow.hpp
//
// File for GLFW windows.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP

#include <glfw/glfw3.h>
#include <Window/Window.hpp>
#include <Errors/ApiErrors.hpp>

//========//
// WindowData
//
// Data grabbed from the window properties.
//========//
//
struct WindowData
{
    int mHeight;
    int mWidth;
    int mPixelHeight;
    int mPixelWidth;
};

//========//
// GlfwWindow
//
// A GLFW specific window class.
//========//
//
class GlfwWindow : public Window
{
    public:

        GlfwWindow(const WindowProperties & lProperties) {Init(&lProperties);}
        virtual ~GlfwWindow(void) {Close();}

        virtual bool ShouldClose(void);
        virtual void OnUpdate(void);

    protected:

        void         Init(const WindowProperties * lProperties);
        virtual void Close(void);

        WindowData    mData;
        GLFWwindow *  mWindow;
        GLFWmonitor * mMonitor;
};

#endif
