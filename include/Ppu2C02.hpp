//////////////////////////////////////////////////////////////////////////////////////////
//
// Ppu2C02.hpp
//
// Contains classes to manage the ppu.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef PPU_2C02_HPP
#define PPU_2C02_HPP

#include "Common.hpp"

//========//
// PpuRegister
//
// Class for ppu registers along with functions to manipulate its contents.
//========//
//
template <typename Width>
class PpuRegister
{
    public:
        PpuRegister(void) : mData(0) {}
        ~PpuRegister(void) = default;

        Width   Read(void)             {return mData;}
        void    Write(Width lData)     {mData = lData;}
        void    SetFlag(Width lFlag)   {mData |= lFlag;}
        void    ClearFlag(Width lFlag) {mData &= ~lFlag;}

    protected:
        Width mData;
};

//========//
// Ppu2C02
//
// Class for emulating the picture processing unit.
//========//
//
class Ppu2C02 : public Device
{
    public:

        Ppu2C02(void);
        virtual ~Ppu2C02(void);

        virtual DataType Read(AddressType lAddress) override;
        virtual void     Write(AddressType lAddress, DataType lData)        override;

    protected:

        //
        // REGISTERS
        //
        // Descriptions and information from https://www.nesdev.org/wiki/PPU_registers#Internal_registers
        //

        enum Registers
        {
            PPUCTRL = 0,        // PPU control register.
            PPUMASK,            // PPU mask register.
            PPUSTATUS,          // PPU status register.
            OAMADDR,            // OAM address port.
            OAMDATA,            // OAM data port.
            PPUSCROLL,          // PPU scrolling position register.
            PPUADDR,            // PPU address register.
            PPUDATA,            // PPU data register.

            NUM_REGISTERS
        };

        enum InternalRegisters
        {
            V = 0,      // During rendering, used for scroll position. Otherwise its current VRAM address.
            T,          // During rendering, specifies the starting coarse-x scroll for the next scanline and the starting y scroll for the screen.
                        //      Otherwise, holds the scroll or VRAM address before transferring it to v.
            X,          // The fine-x position of the current scroll, used during rendering alongside v.
            W,          // Toggles on each write to either PPUSCROLL or PPUADDR, indicating whether this is the first or second write.
                        //      Clears on reads of PPUSTATUS. Sometimes called the 'write latch' or 'write toggle'.

            NUM_INTERNAL_REGISTERS
        };

        enum PpuCtrlBits
        {
            BASE_NAMETBL    = BitMask(2),  // Base nametable address (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00).
            VRAM            = Bit(2),       // VRAM address increment per CPU read/write of PPUDATA (0: add 1, going across; 1: add 32, going down).
            SPRITE_PATTBL   = Bit(3),       // Sprite pattern table address for 8x8 sprites (0: $0000; 1: $1000; ignored in 8x16 mode).
            BACK_PATTBL     = Bit(4),       // Background pattern table address (0: $0000; 1: $1000).
            SPRITE_SIZE     = Bit(5),       // Sprite size (0: 8x8 pixels; 1: 8x16 pixels).
            PPU_MAS_SLAVE   = Bit(6),       // PPU master/slave select (0: read backdrop from EXT pins; 1: output color on EXT pins).
            NMI             = Bit(7)        // Generate NMI at start of vertical blanking interval (0: off; 1: on).
        };

        enum PpuMaskBits
        {
            GREYSCALE       = Bit(0),   // Greyscale (0: normal color, 1: produce a greyscale display).
            LEFT_BCKGRND    = Bit(1),   // 1: Show background in leftmost 8 pixels of screen, 0: Hide.
            LEFT_SPRITES    = Bit(2),   // 1: Show sprites in leftmost 8 pixels of screen, 0: Hide.
            SHOW_BCKGND     = Bit(3),   // Show background.
            SHOW_SPRITES    = Bit(4),   // Show sprites.
            RED             = Bit(5),   // Emphasize red (green on PAL/Dendy).
            GREEN           = Bit(6),   // Emphasize green (red on PAL/Dendy).
            BLUE            = Bit(7)    // Emphasize blue.
        };

        enum PpuStatusBits
        {
            OPEN_BUS        = BitMask(5),   // PPU open bus. Returns stale PPU bus contents.
            SPRITE_OFLOW    = Bit(5),       // Sprite overflow. The intent was for this flag to be set whenever more than eight sprites appear on a
                                            //      scanline, but a hardware bug causes the actual behavior to be more complicated and generate false
                                            //      positives as well as false negatives. This flag is set during sprite evaluation and cleared at dot 1
                                            //      (the second dot) of the pre-render line.
            SPRITE_0_HIT    = Bit(6),       // Sprite 0 Hit. Set when a nonzero pixel of sprite 0 overlaps a nonzero background pixel; cleared at dot
                                            //      1 of the pre-render line. Used for raster timing.
            VERTICAL_BLANK  = Bit(7)        // Vertical blank has started (0: not in vblank; 1: in vblank). Set at dot 1 of line 241 (the line *after* the
                                            //      post-render line); cleared after reading $2002 and at dot 1 of the pre-render line.
        };

        PpuRegister<uint8_t> mRegisters[NUM_REGISTERS]; 
        PpuRegister<uint8_t> mInternalRegisters[NUM_INTERNAL_REGISTERS];

        // Name tables are used to layout the background frame. It's dynamic, meaning it could change every frame.
        // The system only has room for 2 physical name tables (VRAM) that are each 1KB in size, however the NES supports
        // 4 logical name tables in total. The 3rd and 4th tables would be located on the cartridge. The index in mNameTable
        // selects either the 1st or 2nd nametable. Each nametable subdivides their memory into "tiles", representing an 8x8 group
        // of pixels. The address read from a specific mNameTable is used to index into a pattern table where the actual tiles are stored.
        // A name table is made of 32x30 tiles for 960 bytes. The remaning 64 bytes are used for the attribute table. This table is how we know which
        // palette (group of 4 colors) to use for a tile. To further complicate things, the screen is also subdived into "blocks", or a 2x2 grid of tiles.
        // The coordinates of the block (from top left of screen down to bottom right) is the index into the attribute table. Each byte represents a block
        // where every 2-bits in that byte is which palette that block is using.
        enum Nametable
        {
            NUM_NAME_TABLES = 2,
            NAME_TABLE_SIZE = 1024
        };
        MemoryRam mNameTable[NUM_NAME_TABLES];

        // Pattern tables contain the shape of tiles that make up backgrounds and sprites. The two pattern tables are used together
        // to index into a palette for a specific color, and are typically referred as "left" (first pattern table) and "right"
        // (second pattern table). Each tile takes up 16-bytes, 8 from the left and 8 right pattern tables. A pattern table is static memory.
        // Each bit is added from the two tables to index (0-3) into a specific palette, which is known from the attribute table.
        enum PatternTable
        {
            NUM_PATTERN_TABLES = 2,
            PATTERN_TABLE_SIZE = 4096
        };
        MemoryRom mPatternTable[NUM_PATTERN_TABLES];

        // Internal memory inside the PPU containing 64 sprites (4 bytes to describe information about each sprite).
        struct ObjectAttributeMemory
        {
            enum
            {
                // Amount of OAM memory.
                NUM_PRIMARY_SPRITES     = 64,
                NUM_SECONDARY_SPRITES   = 8,

                // For mTileIndex.
                BANK_TILE_BIT           = Bit(0),           // Bit for 8x16 sprites to see which bank of tiles to use.
                TILE_NUM_MASK           = BitMask(7,1),     // Remaining bits for tile number if sprite is 8x16.

                //For mAttribute.
                ATTR_PALETTE            = BitMask(2),       // Which palette to use for sprites, which is 4-7. (background gets 0-3).
                ATTR_UNIMPLEMENTED      = BitMask(3,2),     // Always readback as 0.
                ATTR_PRIO               = Bit(5),           // Priority (0: in front of background; 1: behind background).
                ATTR_FLIP_HORIZONAL     = Bit(6),           // Flip sprite horizontally.
                ATTR_FLIP_VERTICAL      = Bit(7),           // Flip sprite vertically.
            };

            ObjectAttributeMemory(void) : mYPos(0), mTileIndex(0), mAttribute(0), mXPos(0) {}

            uint8_t mYPos;          // Y position of the sprite. Delayed by one scanline, so must subtract 1 before writing here.
            uint8_t mTileIndex;     // Tile index number. For 8x8 sprites, it's the tile number withing the pattern table. selected bu bit 3 of PPUCTRL.
                                    //      For 8x16 sprite (bit 5 set of PPUCTRL), ignore selected pattern table and uses bit 9 from this number for
                                    //      which bank of tile it uses (0000 or 1000).
            uint8_t mAttribute;     // Contains various flags about the sprite indicated by which bits are set.
            uint8_t mXPos;          // Y position of the sprite.
        } mOam[ObjectAttributeMemory::NUM_PRIMARY_SPRITES];

        // In addition to the OAM, the PPU also contains an internal secondary OAM that is not directly accessible by the program. During each visible scanline,
        // this memory is cleared, and a linear search of the primary OAM is performed to find sprites within Y range of next scanline (the sprite evaluation phase https://www.nesdev.org/wiki/PPU_sprite_evaluation),
        // and copied to this memory. These are sprites to be rendered in the next scanline.
        ObjectAttributeMemory mSecondaryOam[ObjectAttributeMemory::NUM_SECONDARY_SPRITES];
};

#endif
