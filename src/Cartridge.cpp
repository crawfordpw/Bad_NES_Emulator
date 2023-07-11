/////////////////////////////////////////////////////////////////////
//
// Cartridge.cpp
//
// Implementation file for a cartridge.
//
/////////////////////////////////////////////////////////////////////

#include <System.hpp>
#include <File/ApiFile.hpp>
#include <Errors/ApiErrors.hpp>

#ifdef USE_LOGGER
#include <Logger/ApiLogger.hpp>
#endif

//--------//
//
// Cartridge
//
//--------//

//--------//
// Cartridge
//
// Constructor. Opens up the NES file and configures itself based on header information
// that the file format provides about the game.
//
// param[in]    lFilename   Location to NES game.
//--------//
//
Cartridge::Cartridge(const char * lFilename)
  : mAddressStart(System::CARTRIDGE_START),
    mAddressEnd(mAddressStart + System::CARTRIDGE_SIZE),
    mMapperId(0),
    mMapper(NULL),
    mMirrorType(HORIZONTAL),
    mNes20Format(false),
    mPrgMirror(false),
    mChrRam(false),
    mValidImage(false)
{
#ifdef USE_LOGGER
    char lBuffer[ApiFileSystem::MAX_FILENAME * 1.5];
    snprintf(lBuffer, ApiFileSystem::MAX_FILENAME * 1.5, "[i] Opening file for cartridge, %s\n", lProgramFile);
    ApiLogger::Log(lBuffer);
#endif

    int    lStatus;
    File * lFile;
    size_t lBytes;

    // Attempt to open the provided file.
    lStatus = ApiFileSystem::Open(lFilename, "rb", &lFile);
    if (lStatus != ErrorCodes::SUCCESS)
    {
        gErrorManager.Post(lStatus, lFilename);
        return;
    }

    // Read in the header information.
    lBytes = ApiFileSystem::Read(&mHeader, sizeof(Header), lFile);
    if (lBytes != sizeof(Header))
    {
        gErrorManager.Post(lFile->GetStatus());
        return;
    }

    // Check to see if its a valid NES file.
    if (mHeader.mId[0] != 'N' || mHeader.mId[1] != 'E' || mHeader.mId[2] != 'S' || mHeader.mId[3] != 0x1A)
    {
        gErrorManager.Post(ErrorCodes::INVALID_NES_FORMAT);
        return;
    }

    // Is this an NES 2.0 format?
    if (GetNes20() == Flags7Bits::NES_20_ID)
    {
        mNes20Format = true;
    }

    // Following https://www.nesdev.org/wiki/INES, suggested a good rule of thumb
    // is to either refuse this case or mask off the upper nibble of the mapper id.
    if (!mNes20Format)
    {
        for (int lIndex = 1; lIndex < 5; ++lIndex)
        {
            if (mHeader.mUnused[lIndex] != 0x00)
            {
                gErrorManager.Post(ErrorCodes::INVALID_NES_FORMAT);
                return;
            }
        }
    }

    // Grab information from the header.
    mMirrorType = GetMirroring();
    mMapperId   = GetHighNibbleMapId() | (GetLowNibbleMapId() >> 4);

    // Not sure what to do if there is a trainer yet. For now, just skip past it.
    if (GetTrainer())
    {
        ApiFileSystem::Seek(TRAINER_SIZE, lFile);
    }

    // Prepare the Program ROM.
    if (mHeader.mPrgBanks == 1)
    {
        mPrgMirror = true;
    }
    mPrgRom.Resize(mHeader.mPrgBanks * DEFAULT_PRG_SIZE);

    // Load Program ROM.
    lStatus = mPrgRom.LoadMemoryFromFile(lFile, mPrgRom.GetSize());
    if (lStatus != ErrorCodes::SUCCESS)
    {
        mPrgRom.Resize(0);
        gErrorManager.Post(lStatus);
        return;
    }

    // Prepare the Character ROM.
    // CHR cannot have a size 0, if the head indicates 0 then it's used as a RAM instead.
    if (mHeader.mChrBanks == 0)
    {
        mChrRom.Resize(DEFAULT_CHR_SIZE);
        mChrRam = true;
    }
    else
    {
        mChrRom.Resize(mHeader.mChrBanks * DEFAULT_CHR_SIZE);
    }

    // Load Character ROM.
    lStatus = mChrRom.LoadMemoryFromFile(lFile, mChrRom.GetSize());
    if (lStatus != ErrorCodes::SUCCESS)
    {
        mPrgRom.Resize(0);
        mChrRom.Resize(0);
        gErrorManager.Post(lStatus);
        return;
    }

    // Close the file.
    ApiFileSystem::Close(lFile);

    // Create the mapper.
    mMapper = MapperFactory(mMapperId, this);

    // Could not create a valid mapper.
    if (mMapper == NULL)
    {
        mPrgRom.Resize(0);
        mChrRom.Resize(0);
        gErrorManager.Post(ErrorCodes::MAPPER_NOT_SUPPORTED);
        return;
    }

    // If we made it this far, then it was a valid file.
    mValidImage = true;

#ifdef USE_LOGGER
    ApiLogger::Log("[i] Valid file format!");
#endif
}

//--------//
// ~Cartridge
//
// Destructor.
//--------//
//
Cartridge::~Cartridge()
{
    if (mMapper)
    {
        delete mMapper;
    }
}

//--------//
// Read
//
// Reads data from the system.
//
// param[in] lAddress   Address to read from.
// param[in] lLastRead  If some devices are not connected, this variable
//                      simulates "open bus behavior". Where a read of
//                      a disconnected device results in the last value read.
// returns  Data at the given address. 
//--------//
//
DataType Cartridge::Read(AddressType lAddress, DataType lLastRead)
{
    return lLastRead;
}

//--------//
// Write
//
// Writes data to memory on the system.
//
// param[in] lAddress   Address to write to. 
// param[in] lData      Data to write. 
//--------//
//
void Cartridge::Write(AddressType lAddress, DataType lData)
{
    return;
}
