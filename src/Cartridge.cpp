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
    mMapper(nullptr),
    mMirrorType(HORIZONTAL),
    mNes20Format(false),
    mPrgMirror(false),
    mChrRam(false),
    mValidImage(false)
{
#ifdef USE_LOGGER
    char lBuffer[ApiFileSystem::MAX_FILENAME * 2];
    snprintf(lBuffer, ApiFileSystem::MAX_FILENAME * 2, "[i] Opening file for cartridge, %s\n", lFilename);
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
    mPrgMemory.Resize(mHeader.mPrgBanks * DEFAULT_PRG_SIZE);

    // Load Program ROM.
    lStatus = mPrgMemory.LoadMemoryFromFile(lFile, mPrgMemory.GetSize());
    if (lStatus != ErrorCodes::SUCCESS)
    {
        mPrgMemory.Resize(0);
        gErrorManager.Post(lStatus);
        return;
    }

    // Prepare the Character ROM.
    // CHR cannot have a size 0, if the head indicates 0 then it's used as a RAM instead.
    if (mHeader.mChrBanks == 0)
    {
        mChrMemory.Resize(DEFAULT_CHR_SIZE);
        mChrRam = true;
    }
    else
    {
        mChrMemory.Resize(mHeader.mChrBanks * DEFAULT_CHR_SIZE);
    }

    // Load Character ROM.
    lStatus = mChrMemory.LoadMemoryFromFile(lFile, mChrMemory.GetSize());
    if (lStatus != ErrorCodes::SUCCESS)
    {
        mPrgMemory.Resize(0);
        mChrMemory.Resize(0);
        gErrorManager.Post(lStatus);
        return;
    }

    // Close the file.
    ApiFileSystem::Close(lFile);

    // Create the mapper.
    mMapper = MapperFactory(mMapperId, this);

    // Could not create a valid mapper.
    if (nullptr == mMapper)
    {
        mPrgMemory.Resize(0);
        mChrMemory.Resize(0);
        gErrorManager.Post(ErrorCodes::MAPPER_NOT_SUPPORTED);
        return;
    }

    // If we made it this far, then it was a valid file.
    mValidImage = true;

    CAPTURE_LOG("[i] Valid ROM file format\n");
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
// returns  Data at the given address. 
//--------//
//
DataType Cartridge::Read(AddressType lAddress)
{
    AddressType lMappedAddress;

    if (IsDisconnected())
    {
        return 0;
    }

    // If mapper doesn't exit, cartridge file is likely invalid. Open bus behavior is to
    //  return data last read.
    if (nullptr == mMapper)
    {
        return mSystem->mLastRead;
    }

    // If MapRead returns true, then read from program memory.
    if (mMapper->MapRead(lAddress, &lMappedAddress, &mSystem->mLastRead))
    {
        return mPrgMemory.Read(lMappedAddress);
    }

    // If made it this far, we are either reading from ram, which is the out parameter
    // of MapRead, or the cartridge doesn't care about the address being read.
    return mSystem->mLastRead;
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
    AddressType lMappedAddress;

    // If mapper doesn't exit, cartridge file is likely invalid and thus
    // this is a disconnected device.
    if (nullptr == mMapper)
    {
        return;
    }

    // If MapRead returns true, then write to program memory.
    if (mMapper->MapWrite(lAddress, &lMappedAddress, lData))
    {
        mPrgMemory.Write(lMappedAddress, lData);
    }
}
