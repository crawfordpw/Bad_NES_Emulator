//////////////////////////////////////////////////////////////////////////////////////////
//
// Main.cpp
//
// Entry point for the system.
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <System.hpp>
#include <File.hpp>
#include <unistd.h>

//--------//
// main
//
// Entry point of the program.
//--------//
//
int main(void)
{
    long int lFileSize;

    // Create the NES.
    System lNes;

    // Grab program from a file.
    UnixFile lFile("../Program.txt", "r");

    if (lFile.GetStatus() != File::SUCCESS)
    {
        printf("Couldn't open file!");
        return 0;
    }

    // Get file size.
    lFile.SeekFromEnd(0);
    lFile.Tell(&lFileSize);
    lFile.SeekFromStart(0);

    // Read entire file.
    char * lObjectCode = new char [lFileSize];
    lFile.Read(lObjectCode, lFileSize);

    // Load into memory.
    lNes.LoadMemory(lObjectCode, lFileSize, 0);
    delete [] lObjectCode;

    // Start the system.
    lNes.Start();

    return 0;
}
