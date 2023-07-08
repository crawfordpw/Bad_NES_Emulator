//////////////////////////////////////////////////////////////////////////////////////////
//
// Cpu6502.hpp
//
// Contains classes to manage the cpu.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef CPU_6502_HPP
#define CPU_6502_HPP

#include <vector>
#include <unordered_map>
#include "Common.hpp"

//========//
// Cpu6502
//
// Class to emulate the 6502 cpu.
//========//
//
class Cpu6502 : public Device
{
    public:

        enum InterruptVectorIndex
        {
            NMI_VECTOR       = 0,
            RESET_VECTOR,
            IRQ_VECTOR,
            BRK_VECTOR,
            NUM_VECTORS
        };

        struct InterruptVector
        {
            uint16_t mLowByte;
            uint16_t mHighByte;
        };

        Cpu6502();
        ~Cpu6502();

        void    Reset();
        void    StepClock();
        uint8_t GetCyclesLeft() {return mCyclesLeft;}

        const InterruptVector  mInterruptVectors[NUM_VECTORS];

    private:

        enum ExtraCycleFlags
        {
            DONT_ADD_CLOCK_CYCLE = 0,
            ADD_CLOCK_CYCLE      = 1,
        };

        enum Flags
        {
            C = Bit(0),   // Carry
            Z = Bit(1),   // Zero
            I = Bit(2),   // Interrupt Disable
            D = Bit(3),   // Decimal (not used for NES)
            B = Bit(4),   // Break
            U = Bit(5),   // unused
            V = Bit(6),   // Overflow
            N = Bit(7),   // Negative

            FLAG_NOT_SET = 0,
            FLAG_SET     = 1
        };

        void NMI();
        void IRQ();

        uint8_t  GetFlag(Flags lFlag)   {return (mRegisters.mStatus & lFlag) ? FLAG_SET : FLAG_NOT_SET;}
        void     SetFlag(Flags lFlag)   {mRegisters.mStatus |= lFlag;}
        void     ClearFlag(Flags lFlag) {mRegisters.mStatus &= ~lFlag;}
        void     SetOrClearFlag(Flags lFlag, bool lCondition);

        void     FetchOpcode();
        DataType FetchData();
        void     PushStack(uint8_t lData);
        DataType PopStack();

        // Addressing modes.
        // These are used as sort of "helper" functions to know the address
        // of the data required by an instruction.
        uint8_t Implied();                uint8_t Immediate(); 
        uint8_t ZeroPage();               uint8_t ZeroPageX(); 
        uint8_t ZeroPageY();              uint8_t Relative();  
        uint8_t Absolute();               uint8_t AbsoluteX(); 
        uint8_t AbsoluteY();              uint8_t Indirect(); 
        uint8_t IndexedIndirect();        uint8_t IndirectIndexed();
        
        // Instructions.
        // https://www.pagetable.com/c64ref/6502/?tab=2#ADC
        uint8_t ADC();    uint8_t AND();    uint8_t ASL();    uint8_t BCC();
        uint8_t BCS();    uint8_t BEQ();    uint8_t BIT();    uint8_t BMI();
        uint8_t BNE();    uint8_t BPL();    uint8_t BRK();    uint8_t BVC();
        uint8_t BVS();    uint8_t CLC();    uint8_t CLD();    uint8_t CLI();
        uint8_t CLV();    uint8_t CMP();    uint8_t CPX();    uint8_t CPY();
        uint8_t DEC();    uint8_t DEX();    uint8_t DEY();    uint8_t EOR();
        uint8_t INC();    uint8_t INX();    uint8_t INY();    uint8_t JMP();
        uint8_t JSR();    uint8_t LDA();    uint8_t LDX();    uint8_t LDY();
        uint8_t LSR();    uint8_t NOP();    uint8_t ORA();    uint8_t PHA();
        uint8_t PHP();    uint8_t PLA();    uint8_t PLP();    uint8_t ROL();
        uint8_t ROR();    uint8_t RTI();    uint8_t RTS();    uint8_t SBC();
        uint8_t SEC();    uint8_t SED();    uint8_t SEI();    uint8_t STA();
        uint8_t STX();    uint8_t STY();    uint8_t TAX();    uint8_t TAY();
        uint8_t TSX();    uint8_t TXA();    uint8_t TXS();    uint8_t TYA();
        uint8_t KIL();

        // Branch instruction helper function.
        uint8_t BranchHelper(bool lBranch);

#ifdef USE_LOGGER
        struct Instruction 
        {
            uint8_t      (Cpu6502::*mInstruction) (void) = nullptr;
            uint8_t      (Cpu6502::*mAddressMode) (void) = nullptr;
            uint8_t      mCycles                         = 0;
            const char * mName;
        };
#else
        struct Instruction 
        {
            uint8_t      (Cpu6502::*mInstruction) (void) = nullptr;
            uint8_t      (Cpu6502::*mAddressMode) (void) = nullptr;
            uint8_t      mCycles                         = 0;
        };
#endif

        struct Registers
        {
            uint8_t   mSp;          // Stack pointer.
            uint8_t   mAcc;         // Accumulator.
            uint8_t   mX;           // Index register X.
            uint8_t   mY;           // Index register y.
            uint8_t   mStatus;      // Status register.
            uint16_t  mPc;          // Program counter.
        };

        const std::vector<Instruction> mOpcodeMatrix;
        DataType                 mOpcode;               // Opcode of current instruction being executed.
        AddressType              mAddress;              // Address used for the current instruction.
        AddressType              mRelativeAddress;      // Address offset used for branch instructions.
        uint8_t                  mCyclesLeft;           // Remaining clock cycles current instruction has.
        Registers                mRegisters;            // All registers the cpu has.
        inline static constexpr uint16_t       mStartOfStack = 0x0100;
        inline static constexpr uint8_t        mStackSize    = 0xFF;

        private:

            const char * Disassemble(Instruction * lInstruction);

};

#endif