//////////////////////////////////////////////////////////////////////////////////////////
//
// Window.hpp
//
// Class repesenting a generic window the application will use.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef WINDOW_HPP
#define WINDOW_HPP

//========//
// WindowProperties
//
// Properties to use when creating a window.
//========//
//
struct WindowProperties
{
    int          mHeight;
    int          mWidth;
    int          mPixelHeight;
    int          mPixelWidth;
    bool         mCenter;
    const char * mTitle;

    WindowProperties(unsigned int lHeight = 480, unsigned int lWidth = 780, unsigned int lPixelHeight = 2,
                     unsigned int lPixelWidth = 2, bool lCenter = true, const char * lTitle = "Bad NES")
      : mHeight(lHeight), mWidth(lWidth), mPixelHeight(lPixelHeight), mPixelWidth(lPixelWidth), mCenter(lCenter),
        mTitle(lTitle)
    {
    }
};

//========//
// Window
//
// Platform agnostic window class. All interactions with the windowing subsystem should
// be done through this. Platform specific windowing should inherit from this class
// and implement the virtual methods. It intenionally does not have a contructor.
//
// Windows are created by calling the static Open method. Only one of these should
// be defined per build. The method should return an platform dependent window.
//
// Some windowing subsystems need initializing before using it. This should happen
// one time during Open. Shutdown is used to clean up this initialization if neccessary
// and should be called at program end (or when finished with creating and using windows).
//========//
//
class Window
{
    public:

        virtual ~Window(void) = default;

        static Window * Open(const WindowProperties & lProperties = WindowProperties());
        static void     ShutDown(void);

        virtual bool    ShouldClose(void) = 0;
        virtual void    OnUpdate(void)    = 0;

        int             GetStatus(void) {return mStatus;}

    protected:

        virtual void    Close(void) = 0;

        static bool  cInitialized;
        int          mStatus;
};

#endif
