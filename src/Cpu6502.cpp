/////////////////////////////////////////////////////////////////////
//
// Cpu6502.cpp
//
// Implementation file for the cpu.
//
/////////////////////////////////////////////////////////////////////

#include <stdio.h> // temporary for printing.
#include <Cpu6502.hpp>
#include <System.hpp>

//--------//
// Cpu6502
//
// Constructor.
//--------//
//
Cpu6502::Cpu6502()
  : // Matrix based on http://www.oxyron.de/html/opcodes02.html
    mOpcodeMatrix
    {
        // Indexed Indirect = izx
        // Indirect Indexed = izy
        //                          x0                                  x1                                          x2                                      x3                                          x4                                  x5                                      x6                                  x7                                      x8                                  x9                                  xA                                  xB                                      xC                                  xD                                  xE                                      xF
        /* 0x */    {&Cpu6502::BRK,&Cpu6502::Implied,7},{&Cpu6502::ORA,&Cpu6502::IndexedIndirect,6},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndexedIndirect,8},{&Cpu6502::NOP,&Cpu6502::ZeroPage,3},{&Cpu6502::ORA,&Cpu6502::ZeroPage,3},{&Cpu6502::ASL,&Cpu6502::ZeroPage,5},{&Cpu6502::KIL,&Cpu6502::ZeroPage,5},{&Cpu6502::PHP,&Cpu6502::Implied,3},{&Cpu6502::ORA,&Cpu6502::Immediate,2},{&Cpu6502::ASL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::Immediate,2},{&Cpu6502::NOP,&Cpu6502::Absolute,4},{&Cpu6502::ORA,&Cpu6502::Absolute,4},{&Cpu6502::ASL,&Cpu6502::Absolute,6},{&Cpu6502::KIL,&Cpu6502::Absolute,6},
        /* 1x */    {&Cpu6502::BPL,&Cpu6502::Relative,2},{&Cpu6502::ORA,&Cpu6502::IndirectIndexed,5},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::ORA,&Cpu6502::ZeroPageX,4},{&Cpu6502::ASL,&Cpu6502::ZeroPageX,6},{&Cpu6502::KIL,&Cpu6502::ZeroPageX,6},{&Cpu6502::CLC,&Cpu6502::Implied,2},{&Cpu6502::ORA,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::ORA,&Cpu6502::AbsoluteX,4},{&Cpu6502::ASL,&Cpu6502::AbsoluteX,7},{&Cpu6502::KIL,&Cpu6502::AbsoluteX,7},
        /* 2x */    {&Cpu6502::JSR,&Cpu6502::Absolute,6},{&Cpu6502::AND,&Cpu6502::IndexedIndirect,6},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndexedIndirect,8},{&Cpu6502::BIT,&Cpu6502::ZeroPage,3},{&Cpu6502::AND,&Cpu6502::ZeroPage,3},{&Cpu6502::ROL,&Cpu6502::ZeroPage,5},{&Cpu6502::KIL,&Cpu6502::ZeroPage,5},{&Cpu6502::PLP,&Cpu6502::Implied,4},{&Cpu6502::AND,&Cpu6502::Immediate,2},{&Cpu6502::ROL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::Immediate,2},{&Cpu6502::BIT,&Cpu6502::Absolute,4},{&Cpu6502::AND,&Cpu6502::Absolute,4},{&Cpu6502::ROL,&Cpu6502::Absolute,6},{&Cpu6502::KIL,&Cpu6502::Absolute,6},
        /* 3x */    {&Cpu6502::BMI,&Cpu6502::Relative,2},{&Cpu6502::AND,&Cpu6502::IndirectIndexed,5},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::AND,&Cpu6502::ZeroPageX,4},{&Cpu6502::ROL,&Cpu6502::ZeroPageX,6},{&Cpu6502::KIL,&Cpu6502::ZeroPageX,6},{&Cpu6502::SEC,&Cpu6502::Implied,2},{&Cpu6502::AND,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::AND,&Cpu6502::AbsoluteX,4},{&Cpu6502::ROL,&Cpu6502::AbsoluteX,7},{&Cpu6502::KIL,&Cpu6502::AbsoluteX,7},
        /* 4x */    {&Cpu6502::RTI,&Cpu6502::Implied,6},{&Cpu6502::EOR,&Cpu6502::IndexedIndirect,6},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndexedIndirect,8},{&Cpu6502::NOP,&Cpu6502::ZeroPage,3},{&Cpu6502::EOR,&Cpu6502::ZeroPage,3},{&Cpu6502::LSR,&Cpu6502::ZeroPage,5},{&Cpu6502::KIL,&Cpu6502::ZeroPage,5},{&Cpu6502::PHA,&Cpu6502::Implied,5},{&Cpu6502::EOR,&Cpu6502::Immediate,2},{&Cpu6502::LSR,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::Immediate,2},{&Cpu6502::JMP,&Cpu6502::Absolute,3},{&Cpu6502::EOR,&Cpu6502::Absolute,4},{&Cpu6502::LSR,&Cpu6502::Absolute,6},{&Cpu6502::KIL,&Cpu6502::Absolute,6},
        /* 5x */    {&Cpu6502::BVC,&Cpu6502::Relative,2},{&Cpu6502::EOR,&Cpu6502::IndirectIndexed,5},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::EOR,&Cpu6502::ZeroPageX,4},{&Cpu6502::LSR,&Cpu6502::ZeroPageX,6},{&Cpu6502::KIL,&Cpu6502::ZeroPageX,6},{&Cpu6502::CLI,&Cpu6502::Implied,2},{&Cpu6502::EOR,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::EOR,&Cpu6502::AbsoluteX,4},{&Cpu6502::LSR,&Cpu6502::AbsoluteX,7},{&Cpu6502::KIL,&Cpu6502::AbsoluteX,7},
        /* 6x */    {&Cpu6502::RTS,&Cpu6502::Implied,6},{&Cpu6502::ADC,&Cpu6502::IndexedIndirect,6},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndexedIndirect,8},{&Cpu6502::NOP,&Cpu6502::ZeroPage,3},{&Cpu6502::ADC,&Cpu6502::ZeroPage,3},{&Cpu6502::ROR,&Cpu6502::ZeroPage,5},{&Cpu6502::KIL,&Cpu6502::ZeroPage,5},{&Cpu6502::PLA,&Cpu6502::Implied,4},{&Cpu6502::ADC,&Cpu6502::Immediate,2},{&Cpu6502::ROR,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::Immediate,2},{&Cpu6502::JMP,&Cpu6502::Indirect,5},{&Cpu6502::ADC,&Cpu6502::Absolute,4},{&Cpu6502::ROR,&Cpu6502::Absolute,6},{&Cpu6502::KIL,&Cpu6502::Absolute,6},
        /* 7x */    {&Cpu6502::BVS,&Cpu6502::Relative,2},{&Cpu6502::ADC,&Cpu6502::IndirectIndexed,5},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::ADC,&Cpu6502::ZeroPageX,4},{&Cpu6502::ROR,&Cpu6502::ZeroPageX,6},{&Cpu6502::KIL,&Cpu6502::ZeroPageX,6},{&Cpu6502::SEI,&Cpu6502::Implied,2},{&Cpu6502::ADC,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::ADC,&Cpu6502::AbsoluteX,4},{&Cpu6502::ROR,&Cpu6502::AbsoluteX,7},{&Cpu6502::KIL,&Cpu6502::AbsoluteX,7},
        /* 8x */    {&Cpu6502::NOP,&Cpu6502::Immediate,2},{&Cpu6502::STA,&Cpu6502::IndexedIndirect,6},{&Cpu6502::NOP,&Cpu6502::Immediate,2},{&Cpu6502::KIL,&Cpu6502::IndexedIndirect,6},{&Cpu6502::STY,&Cpu6502::ZeroPage,3},{&Cpu6502::STA,&Cpu6502::ZeroPage,3},{&Cpu6502::STX,&Cpu6502::ZeroPage,3},{&Cpu6502::KIL,&Cpu6502::ZeroPage,3},{&Cpu6502::DEY,&Cpu6502::Implied,2},{&Cpu6502::NOP,&Cpu6502::Immediate,2},{&Cpu6502::TXA,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::Immediate,2},{&Cpu6502::STY,&Cpu6502::Absolute,4},{&Cpu6502::STA,&Cpu6502::Absolute,4},{&Cpu6502::STX,&Cpu6502::Absolute,4},{&Cpu6502::KIL,&Cpu6502::Absolute,4},
        /* 9x */    {&Cpu6502::BCC,&Cpu6502::Relative,2},{&Cpu6502::STA,&Cpu6502::IndirectIndexed,6},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndirectIndexed,6},{&Cpu6502::STY,&Cpu6502::ZeroPageX,4},{&Cpu6502::STA,&Cpu6502::ZeroPageX,4},{&Cpu6502::STX,&Cpu6502::ZeroPageY,4},{&Cpu6502::KIL,&Cpu6502::ZeroPageY,4},{&Cpu6502::TYA,&Cpu6502::Implied,2},{&Cpu6502::STA,&Cpu6502::AbsoluteY,5},{&Cpu6502::TXS,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::AbsoluteY,5},{&Cpu6502::KIL,&Cpu6502::AbsoluteX,5},{&Cpu6502::STA,&Cpu6502::AbsoluteX,5},{&Cpu6502::KIL,&Cpu6502::AbsoluteY,5},{&Cpu6502::KIL,&Cpu6502::AbsoluteY,5},
        /* Ax */    {&Cpu6502::LDY,&Cpu6502::Immediate,2},{&Cpu6502::LDA,&Cpu6502::IndexedIndirect,6},{&Cpu6502::LDX,&Cpu6502::Immediate,2},{&Cpu6502::KIL,&Cpu6502::IndexedIndirect,6},{&Cpu6502::LDY,&Cpu6502::ZeroPage,3},{&Cpu6502::LDA,&Cpu6502::ZeroPage,3},{&Cpu6502::LDX,&Cpu6502::ZeroPage,3},{&Cpu6502::KIL,&Cpu6502::ZeroPage,3},{&Cpu6502::TAY,&Cpu6502::Implied,2},{&Cpu6502::LDA,&Cpu6502::Immediate,2},{&Cpu6502::TAX,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::Immediate,2},{&Cpu6502::LDY,&Cpu6502::Absolute,4},{&Cpu6502::LDA,&Cpu6502::Absolute,4},{&Cpu6502::LDX,&Cpu6502::Absolute,4},{&Cpu6502::KIL,&Cpu6502::Absolute,4},
        /* Bx */    {&Cpu6502::BCS,&Cpu6502::Relative,2},{&Cpu6502::LDA,&Cpu6502::IndirectIndexed,5},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndirectIndexed,5},{&Cpu6502::LDY,&Cpu6502::ZeroPageX,4},{&Cpu6502::LDA,&Cpu6502::ZeroPageX,4},{&Cpu6502::LDX,&Cpu6502::ZeroPageY,4},{&Cpu6502::KIL,&Cpu6502::ZeroPageY,4},{&Cpu6502::CLV,&Cpu6502::Implied,2},{&Cpu6502::LDA,&Cpu6502::AbsoluteY,4},{&Cpu6502::TSX,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::AbsoluteY,4},{&Cpu6502::LDY,&Cpu6502::AbsoluteX,4},{&Cpu6502::LDA,&Cpu6502::AbsoluteX,4},{&Cpu6502::LDX,&Cpu6502::AbsoluteY,4},{&Cpu6502::KIL,&Cpu6502::AbsoluteY,4},
        /* Cx */    {&Cpu6502::CPY,&Cpu6502::Immediate,2},{&Cpu6502::CMP,&Cpu6502::IndexedIndirect,6},{&Cpu6502::NOP,&Cpu6502::Immediate,2},{&Cpu6502::KIL,&Cpu6502::IndexedIndirect,8},{&Cpu6502::CPY,&Cpu6502::ZeroPage,3},{&Cpu6502::CMP,&Cpu6502::ZeroPage,3},{&Cpu6502::DEC,&Cpu6502::ZeroPage,5},{&Cpu6502::KIL,&Cpu6502::ZeroPage,5},{&Cpu6502::INY,&Cpu6502::Implied,2},{&Cpu6502::CMP,&Cpu6502::Immediate,2},{&Cpu6502::DEX,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::Immediate,2},{&Cpu6502::CPY,&Cpu6502::Absolute,4},{&Cpu6502::CMP,&Cpu6502::Absolute,4},{&Cpu6502::DEC,&Cpu6502::Absolute,4},{&Cpu6502::KIL,&Cpu6502::Absolute,4},
        /* Dx */    {&Cpu6502::BNE,&Cpu6502::Relative,2},{&Cpu6502::CMP,&Cpu6502::IndirectIndexed,5},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::CMP,&Cpu6502::ZeroPageX,4},{&Cpu6502::DEC,&Cpu6502::ZeroPageX,6},{&Cpu6502::KIL,&Cpu6502::ZeroPageX,6},{&Cpu6502::CLD,&Cpu6502::Implied,2},{&Cpu6502::CMP,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::CMP,&Cpu6502::AbsoluteX,4},{&Cpu6502::DEC,&Cpu6502::AbsoluteX,7},{&Cpu6502::KIL,&Cpu6502::AbsoluteX,7},
        /* Ex */    {&Cpu6502::CPX,&Cpu6502::Immediate,2},{&Cpu6502::SBC,&Cpu6502::IndexedIndirect,6},{&Cpu6502::NOP,&Cpu6502::Immediate,2},{&Cpu6502::KIL,&Cpu6502::IndexedIndirect,8},{&Cpu6502::CPX,&Cpu6502::ZeroPage,3},{&Cpu6502::SBC,&Cpu6502::ZeroPage,3},{&Cpu6502::INC,&Cpu6502::ZeroPage,5},{&Cpu6502::KIL,&Cpu6502::ZeroPage,5},{&Cpu6502::INX,&Cpu6502::Implied,2},{&Cpu6502::SBC,&Cpu6502::Immediate,2},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::SBC,&Cpu6502::Immediate,2},{&Cpu6502::CPX,&Cpu6502::Absolute,4},{&Cpu6502::SBC,&Cpu6502::Absolute,4},{&Cpu6502::INC,&Cpu6502::Absolute,6},{&Cpu6502::KIL,&Cpu6502::Absolute,6},
        /* Fx */    {&Cpu6502::BEQ,&Cpu6502::Relative,2},{&Cpu6502::SBC,&Cpu6502::IndirectIndexed,5},{&Cpu6502::KIL,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::SBC,&Cpu6502::ZeroPageX,4},{&Cpu6502::INC,&Cpu6502::ZeroPageX,6},{&Cpu6502::KIL,&Cpu6502::ZeroPageX,6},{&Cpu6502::SED,&Cpu6502::Implied,2},{&Cpu6502::SBC,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::KIL,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::SBC,&Cpu6502::AbsoluteX,4},{&Cpu6502::INC,&Cpu6502::AbsoluteX,7},{&Cpu6502::KIL,&Cpu6502::AbsoluteX,7}
    },
    mInterruptVectors
    {
        {0xFFFA, 0xFFFB},
        {0xFFFC, 0xFFFD},
        {0xFFFE, 0xFFFF},
        {0xFFFE, 0xFFFF},
    }
{
}

//--------//
// ~Cpu6502
//
// Destructor.
//--------//
//
Cpu6502::~Cpu6502()
{
}

// TODO - Remove
void Cpu6502::Test(void)
{
    
}

//--------//
// Reset
//
// Resets the CPU back to a state as if was just turned on.
//--------//
//
void Cpu6502::Reset()
{
    mOpcode             = 0x1A;                 // NOP - Implied
    mAddress            = 0x0000;
    mRelativeAddress    = 0x0000;
    mRegisters.mPc      = 0x0000;
    mRegisters.mAcc     = 0x00;
    mRegisters.mX       = 0x00;
    mRegisters.mY       = 0x00;
    mRegisters.mStatus  = 0x00 | Flags::U;      // Unused flag should always be high.
                                                // https://github.com/OneLoneCoder/olcNES/issues/34

    mRegisters.mSp      = 0x00;
    PushStack((mRegisters.mPc >> 8) & 0x00FF);
    PushStack(mRegisters.mPc & 0x00FF);
    PushStack(mRegisters.mStatus);
}

//--------//
// SetOrClearFlag
//
// Sets or clears a given a certain condition.
//
// param[in]    lFlag         The flag to set or clear.
// param[in]    lCondition    Condition to determine to either set or clear a flag.
//--------//
//
void Cpu6502::SetOrClearFlag(Flags lFlag, bool lCondition)
{
    if (lCondition)
    {
        SetFlag(lFlag);
    }
    else
    {
        ClearFlag(lFlag);
    }
}

//--------//
// FetchOpcode
//
// Grabs the next opcode and increments the program counter.
//--------//
//
void Cpu6502::FetchOpcode()
{
    mOpcode = mSystem->Read(mRegisters.mPc++);
}

//--------//
// FetchData
//
// Grabs data from an address set by one of the addressing modes.
//--------//
//
DataType Cpu6502::FetchData()
{
    return mSystem->Read(mAddress);
}

//--------//
// PushStack
//
// Pushes data onto the stack, wrapping arround in the the event of overflow.
//
// param[in]    lData   Data to push onto the stack.
//--------//
//
void Cpu6502::PushStack(uint8_t lData)
{
    mSystem->Write(mStartOfStack | mRegisters.mSp, lData);
    --mRegisters.mSp;
}

//--------//
// PopStack
//
// Pops data off the stack, wrapping arround in the the event of underflow.
//
// returns  Data whereever the stack pointer was pointing to.
//--------//
//
DataType Cpu6502::PopStack()
{
    uint8_t lData = mSystem->Read(mStartOfStack | mRegisters.mSp);
    ++mRegisters.mSp;
    return lData;
}

//--------//
//
// ADDRESSING MODES
//
//--------/

//--------//
// Implied
//
// There is no addressing mode, the instruction operates on specific areas of memory.
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::Implied()
{
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// Immediate
//
// Expects the next byte to be used as a literal value, just increment the program counter.
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::Immediate()
{
    mAddress = mSystem->Read(mRegisters.mPc++);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// ZeroPage
//
// Only is capable of addressing the first 258 bytes of memory.
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ZeroPage()
{
    mAddress = mSystem->Read(mRegisters.mPc++);
    mAddress &= 0x00FF;
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// ZeroPageX
//
// Same as ZeroPage but adds the contents of the X register as an offset.
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ZeroPageX()
{
    mAddress = mSystem->Read(mRegisters.mPc++) + mRegisters.mX;
    mAddress &= 0x00FF; // Hardware bug! The 6502 did not cross page boundaries here.
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// ZeroPageY
//
// Same as ZeroPage but adds the contents of the Y register as an offset.
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ZeroPageY()
{
    mAddress = mSystem->Read(mRegisters.mPc++) + mRegisters.mY;
    mAddress &= 0x00FF; // Hardware bug! The 6502 did not cross page boundaries here.
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// Relative
//
// Used for conditional branching to provide an offset to the program counter.
// The offset needs to be between -128 and 127, so may need to sign extend if 
// the low byte has the MSB set since we are storing addresses as a 16-bit value
// instead of an 8-bit value.
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::Relative()
{
    mRelativeAddress = mSystem->Read(mRegisters.mPc++);
    if (mRelativeAddress & Bit(7))
    {
        mRelativeAddress |= 0xFF00;
    }
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// Absolute
//
// A full 16-bit address. Two bytes following an opcode are used.
// The low byte comes first, then the high byte
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::Absolute()
{
    uint16_t lLowByte = mSystem->Read(mRegisters.mPc++);
    uint16_t lHighByte = mSystem->Read(mRegisters.mPc++);
    mAddress = (lHighByte << 8) | lLowByte;
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// AbsoluteX
//
// Same as Aboluste Addressing but adds the contents of the X register.
// Since this may cross a page boundary, an additional clock cycle may
// incur.
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::AbsoluteX()
{
    uint16_t lLowByte = mSystem->Read(mRegisters.mPc++);
    uint16_t lHighByte = mSystem->Read(mRegisters.mPc++);
    mAddress = (lHighByte << 8) | lLowByte;
    mAddress += + mRegisters.mX;

    // Adding the X register to the address may result in crossing
    // a page boundary, which adds 1 additional clock cycle.
    // We can check this by seeing if the high byte of the address is
    // different than what we initially read since the high byte of any
    // address represents the page number (low byte indicates the byte within
    // that page).
    if ((mAddress & 0xFF00) != (lHighByte << 8))
    {
        return ADD_CLOCK_CYCLE;
    }
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// AbsoluteY
//
// Same as Aboluste Addressing but adds the contents of the Y register.
// Since this may cross a page boundary, an additional clock cycle may
// incur.
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::AbsoluteY()
{
    uint16_t lLowByte = mSystem->Read(mRegisters.mPc++);
    uint16_t lHighByte = mSystem->Read(mRegisters.mPc++);
    mAddress = (lHighByte << 8) | lLowByte;
    mAddress += + mRegisters.mY;

    // Adding the Y register to the address may result in crossing
    // a page boundary, which adds 1 additional clock cycle.
    // We can check this by seeing if the high byte of the address is
    // different than what we initially read since the high byte of any
    // address represents the page number (low byte indicates the byte within
    // that page).
    if ((mAddress & 0xFF00) != (lHighByte << 8))
    {
        return ADD_CLOCK_CYCLE;
    }
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// Indirect
//
// Use the next two bytes of the instruction to get an "indirect" address.
// The contents of this "indirect" address is used to get the actual address.
// This function also emulates the hardware bug present on the actual cpu.
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::Indirect()
{
    // Grab the address for the address.
    uint16_t lLowByte  = mSystem->Read(mRegisters.mPc++);
    uint16_t lHighByte = mSystem->Read(mRegisters.mPc++);
    uint16_t lIndrect  = (lHighByte << 8) | lLowByte;

    // Now grab the actual address using the contents of the previous address.
    // NOTE: Hardware bug alert! If the low byte and high byte of the indirect
    //       address crossed page boundaries, the 6502 wrapped back around to the same page.
    //       http://www.oxyron.de/html/opcodes02.html
    if (lLowByte & 0x00FF)
    {
        lLowByte = mSystem->Read(lIndrect);
        lHighByte = mSystem->Read(lIndrect & 0xFF00);
    }
    else // This is how it was supposed to work in all cases.
    {
        lLowByte = mSystem->Read(lIndrect++);
        lHighByte = mSystem->Read(lIndrect);
    }
    mAddress = (lHighByte << 8) | lLowByte;

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// IndexedIndirect
//
// This is like ZeroPageX, but the address after adding the contents of the X register
// is used to get the actual address. The lookup address will never overflow to the next
// page, and will always stay within or wrap around in the zero-page.
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::IndexedIndirect()
{
    uint16_t lIndirect = mSystem->Read(mRegisters.mPc++) + mRegisters.mX;
    uint16_t lLowByte = mSystem->Read(lIndirect) & 0x00FF;      // Hardware bug! The 6502 did not cross page boundaries here.
    uint16_t lHighByte = mSystem->Read(lIndirect + 1) & 0x00FF; // Hardware bug! The 6502 did not cross page boundaries here.
    mAddress = (lHighByte << 8) | lLowByte;
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// IndexedIndirect
//
// This is like ZeroPageY, but the address after adding the contents of the Y register
// is used to get the actual address. The lookup address may cross page boundaries, in
// which case the instruction will take an extra clock cycle.
//
// returns  Flag to coordinate with the instruction to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::IndirectIndexed()
{
    uint16_t lIndirect = mSystem->Read(mRegisters.mPc++) + mRegisters.mY;
    uint16_t lLowByte = mSystem->Read(lIndirect);
    uint16_t lHighByte = mSystem->Read(lIndirect + 1);
    mAddress = (lHighByte << 8) | lLowByte;

    if ((mAddress & 0xFF00) != (lHighByte << 8))
    {
        return ADD_CLOCK_CYCLE;
    }
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
//
// INSTRUCTIONS
//
//--------/

//--------//
// ADC
//
// Add memory to accumulator with carry, setting appropriate status flags.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ADC()
{
    // Perform the add operation.
    uint8_t  lData = FetchData();
    uint16_t lResult = mRegisters.mAcc + lData + GetFlag(Flags::C);

    // Set the Carry flag based on if the result is more than the maximum value of an 8-bit number.
    SetOrClearFlag(Flags::C, lResult > 0x00FF);

    // Set the Overflow flag if the sign bit has changed from result exceeding +127 or -128.
    // https://forums.nesdev.org/viewtopic.php?t=6331
    SetOrClearFlag(Flags::V, ((mRegisters.mAcc ^ lResult) & (lData ^ lResult)) & 0x0080);

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Make sure we are just storing the low byte from the result.
    mRegisters.mAcc = lResult & 0x00FF;

    // This instruction could add an additional clock cycle, depending on the address mode.
    return ADD_CLOCK_CYCLE;
}

//--------//
// AND
//
// Bitwise AND with the accumulator and memory, storing back into accumulator.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::AND()
{
    // Perform the and operation.
    mRegisters.mAcc = mRegisters.mAcc & FetchData();

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mAcc == 0);

    // This instruction could add an additional clock cycle, depending on the address mode.
    return ADD_CLOCK_CYCLE;
}

//--------//
// ASL
//
// Arithmetic shift left. Store back to accumulator only if address mode is implied. Otherwise,
// store it back at the memory address.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ASL()
{
    // Perform the left shift.
    uint16_t lData = FetchData() << 1;

    // Set the Carry flag if bit 8 is set.
    SetOrClearFlag(Flags::C, lData > 0x00FF);

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lData & 0x00FF) == 0);

    // Set the Negative flag if bit 7 is set.
    SetOrClearFlag(Flags::C, lData & Bit(7));

    // Only store result to accumulator if address mode was Implied.
    if (mOpcodeMatrix[mOpcode].mAddressMode == &Implied)
    {
        mRegisters.mAcc = lData & 0x00FF;
    }
    else
    {
        mSystem->Write(mAddress, lData & 0x00FF);
    }
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// BCC
//
// Branch on Carry Clear. Add the relative offset to the program counter if carry flag is clear.
// Branch if (C == 0).
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::BCC()
{
    return BranchHelper(GetFlag(Flags::C) == FLAG_NOT_SET);
}

//--------//
// BCS
//
// Branch on Carry Set. Add the relative offset to the program counter if carry flag is set.
// Branch if (C == 1).
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::BCS()
{
    return BranchHelper(GetFlag(Flags::C) == FLAG_SET);
}

//--------//
// BEQ
//
// Branch on Result Zero. Add the relative offset to the program counter if zero flag is set.
// Branch if (Z == 1).
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::BEQ()
{
    return BranchHelper(GetFlag(Flags::Z) == FLAG_SET);
}

//--------//
// BIT
//
// Test Bits in Memory with Accumulator.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::BIT()
{
    // Perform the and operation.
    uint8_t lData = FetchData();

    // Set the Negative flag if the MSB is set at memory location.
    SetOrClearFlag(Flags::N, lData & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (mRegisters.mAcc & lData) == 0);

    // Set the Overflow flag if bit 6 is set at memory location.
    SetOrClearFlag(Flags::V, lData & Bit(6));

    // This instruction could add an additional clock cycle, depending on the address mode.
    return ADD_CLOCK_CYCLE;
}

//--------//
// BMI
//
// Branch on Result Minus. Add the relative offset to the program counter if negative flag is set.
// Branch if (N == 1).
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::BMI()
{
    return BranchHelper(GetFlag(Flags::N) == FLAG_SET);
}

//--------//
// BNE
//
// Branch on Result Not Zero. Add the relative offset to the program counter if Zero flag is not set.
// Branch if (Z == 0).
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::BNE()
{
    return BranchHelper(GetFlag(Flags::Z) == FLAG_NOT_SET);
}

//--------//
// BPL
//
// Branch on Result Plus. Add the relative offset to the program counter if Negative flag is not set.
// Branch if (N == 0).
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::BPL()
{
    return BranchHelper(GetFlag(Flags::N) == FLAG_NOT_SET);
}

//--------//
// BRK
//
// Break Command. This is a program generated interrupt.
// Sequence found here - https://www.pagetable.com/?p=410
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::BRK()
{
    // Break is actually a 2-byte opcode, with the second byte
    // being padding. Skip over it.
    ++mRegisters.mPc;

    // Disable interrupts.
    SetFlag(Flags::I);

    // Store the return address onto the stack, high byte then low byte.
    PushStack((mRegisters.mPc >> 8) & 0x00FF);
    PushStack(mRegisters.mPc & 0x00FF);

    // Mark the break in status register, then store it onto the stack.
    SetFlag(Flags::B);
    PushStack(mRegisters.mStatus);
    ClearFlag(Flags::B);

    // Finally, jump to the address from the interrupt vector.
    mRegisters.mPc = (mSystem->Read(mInterruptVectors[BRK_VECTOR].mLowByte) | (mSystem->Read(mInterruptVectors[BRK_VECTOR].mHighByte) << 8));

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// BVC
//
// Branch on Overflow Clear. Add the relative offset to the program counter if Overflow flag is not set.
// Branch if (V == 0).
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::BVC()
{
    return BranchHelper(GetFlag(Flags::V) == FLAG_NOT_SET);
}

//--------//
// BVS
//
// Branch on Overflow Set. Add the relative offset to the program counter if Overflow flag is set.
// Branch if (V == 1).
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::BVS()
{
    return BranchHelper(GetFlag(Flags::V) == FLAG_SET);
}

//--------//
// CLC
//
// Clear Carry Flag.
// C = 0.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::CLC()
{
    ClearFlag(Flags::C);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// CLD
//
// Clear Decimal Mode.
// D = 0.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::CLD()
{
    ClearFlag(Flags::D);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// CLI
//
// Clear Interrupt Disable.
// I = 0.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::CLI()
{
    ClearFlag(Flags::I);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// CLV
//
// Clear Overflow Flag.
// V = 0.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::CLV()
{
    ClearFlag(Flags::V);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// CMP
//
// Compare Memory and Accumulator.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::CMP()
{
    // Perform subtraction.
    DataType lData   = FetchData();
    uint16_t lResult = mRegisters.mAcc - lData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Carry flag if the data in memory is <= than accumulator.
    SetOrClearFlag(Flags::C, lData <= mRegisters.mAcc);

    return ADD_CLOCK_CYCLE;
}

//--------//
// CPX
//
// Compare Index Register X To Memory.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::CPX()
{
    // Perform subtraction.
    DataType lData   = FetchData();
    uint16_t lResult = mRegisters.mX - lData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Carry flag if the X register is >= than the data in memory.
    SetOrClearFlag(Flags::C, mRegisters.mX >= lData);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// CPY
//
// Compare Index Register Y To Memory.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::CPY()
{
    // Perform subtraction.
    DataType lData   = FetchData();
    uint16_t lResult = mRegisters.mY - lData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Carry flag if the X register is >= than the data in memory.
    SetOrClearFlag(Flags::C, mRegisters.mY >= lData);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// DEC
//
// Decrement Memory By One.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::DEC()
{
    // Perform decrement.
    uint16_t lResult = FetchData() - 1;

    // Write result back to memory.
    mSystem->Write(mAddress, lResult & 0x00FF);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// DEX
//
// Decrement Index Register X By One.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::DEX()
{
    // Perform decrement.
    --mRegisters.mX;

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, mRegisters.mX & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mX == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// DEY
//
// Decrement Index Register Y By One.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::DEY()
{
    // Perform subtraction.
    --mRegisters.mY;

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, mRegisters.mY & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mY == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// EOR
//
// Exclusive Or Memory with Accumulator.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::EOR()
{
    // Perform decrement.
    DataType lResult = FetchData() ^ mRegisters.mAcc;

    // Write result back to memory.
    mSystem->Write(mAddress, lResult);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, lResult == 0);

    return ADD_CLOCK_CYCLE;
}

//--------//
// INC
//
// Increment Memory By One.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::INC()
{
    // Perform increment.
    uint16_t lResult = FetchData() + 1;

    // Write result back to memory.
    mSystem->Write(mAddress, lResult & 0x00FF);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// INX
//
// Increment Index Register X By One.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::INX()
{
    // Perform increment.
    ++mRegisters.mX;

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, mRegisters.mX & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mX == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// INY
//
// Increment Index Register Y By One.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::INY()
{
    // Perform increment.
    ++mRegisters.mY;

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, mRegisters.mY & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mY == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// JMP
//
// Jump Indirect. Jump to an address location.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::JMP()
{
    mRegisters.mPc = mAddress;
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// JSR
//
// Jump To Subroutine. Transfers control to subroutine location, leaving a return pointer on the stack.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::JSR()
{
    // The RTS instruction will increment the program counter by one, so
    // we need the PC to point to just before the next instruction. Since
    // the PC is already pointing to next instruction, decrement by one.
    --mRegisters.mPc;

    // Push program counter onto stack.
    PushStack((mRegisters.mPc >> 8) & 0x00FF);
    PushStack(mRegisters.mPc & 0x00FF);

    // Jump to subroutine.
    mRegisters.mPc = mAddress;

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// LDA
//
// Load Accumulator with Memory.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::LDA()
{
    // Load accumulator with memory.
    DataType lData = FetchData();
    mRegisters.mAcc = lData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mAcc == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    return ADD_CLOCK_CYCLE;
}

//--------//
// LDX
//
// Load Index Register X from Memory.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::LDX()
{
    // Load accumulator with memory.
    DataType lData = FetchData();
    mRegisters.mX = lData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mX == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, mRegisters.mX & Bit(7));

    return ADD_CLOCK_CYCLE;
}

//--------//
// LDY
//
// Load Index Register Y from Memory.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::LDY()
{
    // Load accumulator with memory.
    DataType lData = FetchData();
    mRegisters.mY = lData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mY == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, mRegisters.mY & Bit(7));

    return ADD_CLOCK_CYCLE;
}

//--------//
// LSR
//
// Logical shift right.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::LSR()
{
    // Grab the data.
    uint16_t lData = FetchData();

    // Set the Carry flag if bit 8 is set.
    SetOrClearFlag(Flags::C, lData & Bit(0));

    // Perform the right shift.
    lData = lData >> 1;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lData & 0x00FF) == 0);

    // Set the Negative flag if bit 7 is set.
    SetOrClearFlag(Flags::C, lData & Bit(7));

    // Only store result to accumulator if address mode was Implied.
    if (mOpcodeMatrix[mOpcode].mAddressMode == &Implied)
    {
        mRegisters.mAcc = lData & 0x00FF;
    }
    else
    {
        mSystem->Write(mAddress, lData & 0x00FF);
    }
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// NOP
//
// No Operation
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::NOP()
{
    return ADD_CLOCK_CYCLE;
}

//--------//
// ORA
//
// OR memory with Accumulator.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ORA()
{
    // Perform the and operation.
    mRegisters.mAcc = mRegisters.mAcc | FetchData();

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mAcc == 0);

    // This instruction could add an additional clock cycle, depending on the address mode.
    return ADD_CLOCK_CYCLE;
}

//--------//
// PHA
//
// Push Accumulator on Stack.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::PHA()
{
    PushStack(mRegisters.mAcc);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// PHP
//
// Push Processor Status on Stack.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::PHP()
{
    // According to link below, the status register is pushed to stack
    // with both the break and unused flags set. But doesn't set these in the microprocessor.
    // https://www.masswerk.at/6502/6502_instruction_set.html#ASL
    PushStack(mRegisters.mStatus | Flags::B | Flags::U);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// PLA
//
// Pull Accumulator from Stack.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::PLA()
{
    // Store value from stack to accumulator.
    mRegisters.mAcc = PopStack();

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mAcc == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// PLP
//
// Pull Processor Status from Stack.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::PLP()
{
    // Pop value from stack and store to status register.
    mRegisters.mStatus = PopStack();

    // Unused flag should always be high.
    // https://github.com/OneLoneCoder/olcNES/issues/34
    SetFlag(Flags::U);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// ROL
//
// Rotate Left. Shifts either memory or accumlator left one bit, with the carry flag store at bit 0.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ROL()
{
    // Perform rotate left operation.
    uint16_t lResult = (FetchData() << 1) | GetFlag(Flags::C);

    // Set the Carry flag if bit 8 is set.
    SetOrClearFlag(Flags::C, lResult & 0xFF00);

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if bit 7 is set.
    SetOrClearFlag(Flags::C, lResult & Bit(7));

    // Only store result to accumulator if address mode was Implied.
    if (mOpcodeMatrix[mOpcode].mAddressMode == &Implied)
    {
        mRegisters.mAcc = lResult & 0x00FF;
    }
    else
    {
        mSystem->Write(mAddress, lResult & 0x00FF);
    }
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// ROR
//
// Rotate Right. Shifts either memory or accumlator right one bit, with the carry flag store at bit 7.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ROR()
{
    // Perform rotate right operation.
    DataType lData   = FetchData();
    uint16_t lResult = (lData >> 1) | (GetFlag(Flags::C) << 7);

    // Set the Carry flag if bit 1 is set.
    SetOrClearFlag(Flags::C, lData & 0x01);

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if bit 7 is set.
    SetOrClearFlag(Flags::C, lResult & Bit(7));

    // Only store result to accumulator if address mode was Implied.
    if (mOpcodeMatrix[mOpcode].mAddressMode == &Implied)
    {
        mRegisters.mAcc = lData & 0x00FF;
    }
    else
    {
        mSystem->Write(mAddress, lData & 0x00FF);
    }
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// RTI
//
// Return from Interrupt.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::RTI()
{
    // Pop value from stack and store to status register.
    mRegisters.mStatus = PopStack();

    // Ignore break flag.
    ClearFlag(Flags::B);

    // Unused flag should always be high.
    // https://github.com/OneLoneCoder/olcNES/issues/34
    SetFlag(Flags::U);

    // Grab the program counter from the stack, low byte first then high byte.
    mRegisters.mPc = PopStack();
    mRegisters.mPc |= (PopStack() << 8);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// RTS
//
// Return from Subroutine.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::RTS()
{
    // Grab the program counter from the stack, low byte first then high byte.
    mRegisters.mPc = PopStack();
    mRegisters.mPc |= (PopStack() << 8);

    // JSR decremented program counter before pushing to stack, now we
    // increment it to have the PC point to next instruction.
    ++mRegisters.mPc;

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// SBC
//
// Subtract memory from accumulator with Borrow, setting appropriate status flags.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SBC()
{
    uint16_t lInverted = FetchData() ^ 0x00FF;
    uint16_t lResult   = mRegisters.mAcc + lInverted + GetFlag(Flags::C);

    // Set the Carry flag based on if the result is more than the maximum value of an 8-bit number.
    SetOrClearFlag(Flags::C, lResult > 0x00FF);

    // Set the Overflow flag if the sign bit has changed from result exceeding +127 or -128.
    // https://forums.nesdev.org/viewtopic.php?t=6331
    SetOrClearFlag(Flags::V, ((mRegisters.mAcc ^ lResult) & (lInverted ^ lResult)) & 0x0080);

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Make sure we are just storing the low byte from the result.
    mRegisters.mAcc = lResult & 0x00FF;

    return ADD_CLOCK_CYCLE;
}

//--------//
// SEC
//
// Set Carry Flag.
// C = 1.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SEC()
{
    SetFlag(Flags::C);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// SED
//
// Set Decimal Mode.
// D = 1.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SED()
{
    SetFlag(Flags::D);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// SEI
//
// Set Interrupt Disable.
// I = 1.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SEI()
{
    SetFlag(Flags::I);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// STA
//
// Store Accumulator in Memory.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::STA()
{
    mSystem->Write(mAddress, mRegisters.mAcc);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// STX
//
// Store Index Register X in Memory.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::STX()
{
    mSystem->Write(mAddress, mRegisters.mX);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// STY
//
// Store Index Register Y in Memory.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::STY()
{
    mSystem->Write(mAddress, mRegisters.mY);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// TAX
//
// Transfer Accumulator to Index Register X.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::TAX()
{
    // Perform transfer.
    mRegisters.mX = mRegisters.mAcc;

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, mRegisters.mX & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mX == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// TAY
//
// Transfer Accumulator to Index Register Y.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::TAY()
{
    // Perform transfer.
    mRegisters.mY = mRegisters.mAcc;

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, mRegisters.mY & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mY == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// TSX
//
// Transfer Stack Pointer to Index Register X.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::TSX()
{
    // Perform transfer.
    mRegisters.mX = mRegisters.mSp;

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, mRegisters.mX & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mX == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// TXA
//
// Transfer Index Register X to Accumulator.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::TXA()
{
    // Perform transfer.
    mRegisters.mAcc = mRegisters.mX;

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mAcc == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// TXA
//
// Transfer Index Register X to Stack Pointer.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::TXS()
{
    // Perform transfer.
    mRegisters.mSp = mRegisters.mX;
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// TXA
//
// Transfer Index Register Y to Accumulator.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::TYA()
{
    // Perform transfer.
    mRegisters.mAcc = mRegisters.mY;

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mAcc == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// KIL
//
// Illegal opcodes.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::KIL()
{
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// BranchHelper
//
// Helper function for branch instructions. Most branch instructions are the same execpt for
// what determines the branch.
//
// param[in]  lBranch   Should we branch or not.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::BranchHelper(bool lBranch)
{
    if (lBranch)
    {
        // Branching adds an additional clock cycle.
        ++mCyclesLeft;
        mAddress = mRegisters.mPc + mRelativeAddress;

        // Crossing a page boundary also adds an additional clock cycle.
        if ((mAddress & 0xFF00) != (mRegisters.mPc & 0xFF00))
        {
            ++mCyclesLeft;
        }
        mRegisters.mPc = mAddress;
    }
    return DONT_ADD_CLOCK_CYCLE;
}
