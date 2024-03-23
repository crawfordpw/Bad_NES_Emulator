/////////////////////////////////////////////////////////////////////
//
// Common.cpp
//
// Implementation file for common classes and functions.
//
/////////////////////////////////////////////////////////////////////

#include <Common.hpp>
#include <Errors/ApiErrors.hpp>

//--------//
//
// Device
//
//--------//

//--------//
// IsDisconnected
//
// Checks if device is disconnected from the system and posts an error (only once) if it is.
//
// returns  Whether the device is disconnected from the system.
//--------//
//
bool Device::IsDisconnected(void)
{
    if (nullptr == mSystem)
    {
        if (!mDisconnectedError)
        {
            // Only post the error once.
            gErrorManager.Post(ErrorCodes::INTERNAL_ERROR, "device is disconnected from system");
            mDisconnectedError = true;
        }
        return true;
    }
    return false;
}
