/////////////////////////////////////////////////////////////////////
//
// Cpu6502.cpp
//
// Implementation file for the cpu.
//
/////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <System.hpp>

#ifdef USE_LOGGER
#include <Logger/ApiLogger.hpp>
#endif

//--------//
//
// Cpu6502
//
//--------//

#ifdef TEST_CPU
char Cpu6502::cBuffer[Cpu6502::BUFFER_SIZE];
char Cpu6502::cFormatBuffer[Cpu6502::FORMAT_BUFFER_SIZE];
#endif

//--------//
// Cpu6502
//
// Constructor.
//--------//
//
Cpu6502::Cpu6502()
  :
    // Matrix based on http://www.oxyron.de/html/opcodes02.html
#ifdef USE_LOGGER
    mOpcodeMatrix
    {
        // Indexed Indirect = izx
        // Indirect Indexed = izy
        //                                 x0                                          x1                                                x2                                          x3                                                x4                                          x5                                          x6                                          x7                                          x8                                        x9                                          xA                                        xB                                          xC                                          xD                                          xE                                          xF
        /* 0x */    {&Cpu6502::BRK,&Cpu6502::Implied,7,"BRK"},  {&Cpu6502::ORA,&Cpu6502::IndexedIndirect,6,"ORA"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::SLO,&Cpu6502::IndexedIndirect,8,"SLO"},{&Cpu6502::NOP,&Cpu6502::ZeroPage,3,"NOP"}, {&Cpu6502::ORA,&Cpu6502::ZeroPage,3,"ORA"}, {&Cpu6502::ASL,&Cpu6502::ZeroPage,5,"ASL"}, {&Cpu6502::SLO,&Cpu6502::ZeroPage,5,"SLO"}, {&Cpu6502::PHP,&Cpu6502::Implied,3,"PHP"},{&Cpu6502::ORA,&Cpu6502::Immediate,2,"ORA"},{&Cpu6502::ASL,&Cpu6502::Implied,2,"ASL"},{&Cpu6502::ANC,&Cpu6502::Immediate,2,"ANC"},{&Cpu6502::NOP,&Cpu6502::Absolute,4,"NOP"}, {&Cpu6502::ORA,&Cpu6502::Absolute,4,"ORA"}, {&Cpu6502::ASL,&Cpu6502::Absolute,6,"ASL"}, {&Cpu6502::SLO,&Cpu6502::Absolute,6,"SLO"},
        /* 1x */    {&Cpu6502::BPL,&Cpu6502::Relative,2,"BPL"}, {&Cpu6502::ORA,&Cpu6502::IndirectIndexed,5,"ORA"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::SLO,&Cpu6502::IndirectIndexed,8,"SLO"},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4,"NOP"},{&Cpu6502::ORA,&Cpu6502::ZeroPageX,4,"ORA"},{&Cpu6502::ASL,&Cpu6502::ZeroPageX,6,"ASL"},{&Cpu6502::SLO,&Cpu6502::ZeroPageX,6,"SLO"},{&Cpu6502::CLC,&Cpu6502::Implied,2,"CLC"},{&Cpu6502::ORA,&Cpu6502::AbsoluteY,4,"ORA"},{&Cpu6502::NOP,&Cpu6502::Implied,2,"NOP"},{&Cpu6502::SLO,&Cpu6502::AbsoluteY,7,"SLO"},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4,"NOP"},{&Cpu6502::ORA,&Cpu6502::AbsoluteX,4,"ORA"},{&Cpu6502::ASL,&Cpu6502::AbsoluteX,7,"ASL"},{&Cpu6502::SLO,&Cpu6502::AbsoluteX,7,"SLO"},
        /* 2x */    {&Cpu6502::JSR,&Cpu6502::Absolute,6,"JSR"}, {&Cpu6502::AND,&Cpu6502::IndexedIndirect,6,"AND"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::RLA,&Cpu6502::IndexedIndirect,8,"RLA"},{&Cpu6502::BIT,&Cpu6502::ZeroPage,3,"BIT"}, {&Cpu6502::AND,&Cpu6502::ZeroPage,3,"AND"}, {&Cpu6502::ROL,&Cpu6502::ZeroPage,5,"ROL"}, {&Cpu6502::RLA,&Cpu6502::ZeroPage,5,"RLA"}, {&Cpu6502::PLP,&Cpu6502::Implied,4,"PLP"},{&Cpu6502::AND,&Cpu6502::Immediate,2,"AND"},{&Cpu6502::ROL,&Cpu6502::Implied,2,"ROL"},{&Cpu6502::ANC,&Cpu6502::Immediate,2,"ANC"},{&Cpu6502::BIT,&Cpu6502::Absolute,4,"BIT"}, {&Cpu6502::AND,&Cpu6502::Absolute,4,"AND"}, {&Cpu6502::ROL,&Cpu6502::Absolute,6,"ROL"}, {&Cpu6502::RLA,&Cpu6502::Absolute,6,"RLA"},
        /* 3x */    {&Cpu6502::BMI,&Cpu6502::Relative,2,"BMI"}, {&Cpu6502::AND,&Cpu6502::IndirectIndexed,5,"AND"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::RLA,&Cpu6502::IndirectIndexed,8,"RLA"},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4,"NOP"},{&Cpu6502::AND,&Cpu6502::ZeroPageX,4,"AND"},{&Cpu6502::ROL,&Cpu6502::ZeroPageX,6,"ROL"},{&Cpu6502::RLA,&Cpu6502::ZeroPageX,6,"RLA"},{&Cpu6502::SEC,&Cpu6502::Implied,2,"SEC"},{&Cpu6502::AND,&Cpu6502::AbsoluteY,4,"AND"},{&Cpu6502::NOP,&Cpu6502::Implied,2,"NOP"},{&Cpu6502::RLA,&Cpu6502::AbsoluteY,7,"RLA"},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4,"NOP"},{&Cpu6502::AND,&Cpu6502::AbsoluteX,4,"AND"},{&Cpu6502::ROL,&Cpu6502::AbsoluteX,7,"ROL"},{&Cpu6502::RLA,&Cpu6502::AbsoluteX,7,"RLA"},
        /* 4x */    {&Cpu6502::RTI,&Cpu6502::Implied,6,"RTI"},  {&Cpu6502::EOR,&Cpu6502::IndexedIndirect,6,"EOR"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::SRE,&Cpu6502::IndexedIndirect,8,"SRE"},{&Cpu6502::NOP,&Cpu6502::ZeroPage,3,"NOP"}, {&Cpu6502::EOR,&Cpu6502::ZeroPage,3,"EOR"}, {&Cpu6502::LSR,&Cpu6502::ZeroPage,5,"LSR"}, {&Cpu6502::SRE,&Cpu6502::ZeroPage,5,"SRE"}, {&Cpu6502::PHA,&Cpu6502::Implied,3,"PHA"},{&Cpu6502::EOR,&Cpu6502::Immediate,2,"EOR"},{&Cpu6502::LSR,&Cpu6502::Implied,2,"LSR"},{&Cpu6502::ASR,&Cpu6502::Immediate,2,"ASR"},{&Cpu6502::JMP,&Cpu6502::Absolute,3,"JMP"}, {&Cpu6502::EOR,&Cpu6502::Absolute,4,"EOR"}, {&Cpu6502::LSR,&Cpu6502::Absolute,6,"LSR"}, {&Cpu6502::SRE,&Cpu6502::Absolute,6,"SRE"},
        /* 5x */    {&Cpu6502::BVC,&Cpu6502::Relative,2,"BVC"}, {&Cpu6502::EOR,&Cpu6502::IndirectIndexed,5,"EOR"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::SRE,&Cpu6502::IndirectIndexed,8,"SRE"},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4,"NOP"},{&Cpu6502::EOR,&Cpu6502::ZeroPageX,4,"EOR"},{&Cpu6502::LSR,&Cpu6502::ZeroPageX,6,"LSR"},{&Cpu6502::SRE,&Cpu6502::ZeroPageX,6,"SRE"},{&Cpu6502::CLI,&Cpu6502::Implied,2,"CLI"},{&Cpu6502::EOR,&Cpu6502::AbsoluteY,4,"EOR"},{&Cpu6502::NOP,&Cpu6502::Implied,2,"NOP"},{&Cpu6502::SRE,&Cpu6502::AbsoluteY,7,"SRE"},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4,"NOP"},{&Cpu6502::EOR,&Cpu6502::AbsoluteX,4,"EOR"},{&Cpu6502::LSR,&Cpu6502::AbsoluteX,7,"LSR"},{&Cpu6502::SRE,&Cpu6502::AbsoluteX,7,"SRE"},
        /* 6x */    {&Cpu6502::RTS,&Cpu6502::Implied,6,"RTS"},  {&Cpu6502::ADC,&Cpu6502::IndexedIndirect,6,"ADC"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::RRA,&Cpu6502::IndexedIndirect,8,"RRA"},{&Cpu6502::NOP,&Cpu6502::ZeroPage,3,"NOP"}, {&Cpu6502::ADC,&Cpu6502::ZeroPage,3,"ADC"}, {&Cpu6502::ROR,&Cpu6502::ZeroPage,5,"ROR"}, {&Cpu6502::RRA,&Cpu6502::ZeroPage,5,"RRA"}, {&Cpu6502::PLA,&Cpu6502::Implied,4,"PLA"},{&Cpu6502::ADC,&Cpu6502::Immediate,2,"ADC"},{&Cpu6502::ROR,&Cpu6502::Implied,2,"ROR"},{&Cpu6502::ARR,&Cpu6502::Immediate,2,"ARR"},{&Cpu6502::JMP,&Cpu6502::Indirect,5,"JMP"}, {&Cpu6502::ADC,&Cpu6502::Absolute,4,"ADC"}, {&Cpu6502::ROR,&Cpu6502::Absolute,6,"ROR"}, {&Cpu6502::RRA,&Cpu6502::Absolute,6,"RRA"},
        /* 7x */    {&Cpu6502::BVS,&Cpu6502::Relative,2,"BVS"}, {&Cpu6502::ADC,&Cpu6502::IndirectIndexed,5,"ADC"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::RRA,&Cpu6502::IndirectIndexed,8,"RRA"},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4,"NOP"},{&Cpu6502::ADC,&Cpu6502::ZeroPageX,4,"ADC"},{&Cpu6502::ROR,&Cpu6502::ZeroPageX,6,"ROR"},{&Cpu6502::RRA,&Cpu6502::ZeroPageX,6,"RRA"},{&Cpu6502::SEI,&Cpu6502::Implied,2,"SEI"},{&Cpu6502::ADC,&Cpu6502::AbsoluteY,4,"ADC"},{&Cpu6502::NOP,&Cpu6502::Implied,2,"NOP"},{&Cpu6502::RRA,&Cpu6502::AbsoluteY,7,"RRA"},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4,"NOP"},{&Cpu6502::ADC,&Cpu6502::AbsoluteX,4,"ADC"},{&Cpu6502::ROR,&Cpu6502::AbsoluteX,7,"ROR"},{&Cpu6502::RRA,&Cpu6502::AbsoluteX,7,"RRA"},
        /* 8x */    {&Cpu6502::NOP,&Cpu6502::Immediate,2,"NOP"},{&Cpu6502::STA,&Cpu6502::IndexedIndirect,6,"STA"},{&Cpu6502::NOP,&Cpu6502::Immediate,2,"NOP"},{&Cpu6502::SAX,&Cpu6502::IndexedIndirect,6,"SAX"},{&Cpu6502::STY,&Cpu6502::ZeroPage,3,"STY"}, {&Cpu6502::STA,&Cpu6502::ZeroPage,3,"STA"}, {&Cpu6502::STX,&Cpu6502::ZeroPage,3,"STX"}, {&Cpu6502::SAX,&Cpu6502::ZeroPage,3,"SAX"}, {&Cpu6502::DEY,&Cpu6502::Implied,2,"DEY"},{&Cpu6502::NOP,&Cpu6502::Immediate,2,"NOP"},{&Cpu6502::TXA,&Cpu6502::Implied,2,"TXA"},{&Cpu6502::XAA,&Cpu6502::Immediate,2,"XAA"},{&Cpu6502::STY,&Cpu6502::Absolute,4,"STY"}, {&Cpu6502::STA,&Cpu6502::Absolute,4,"STA"}, {&Cpu6502::STX,&Cpu6502::Absolute,4,"STX"}, {&Cpu6502::SAX,&Cpu6502::Absolute,4,"SAX"},
        /* 9x */    {&Cpu6502::BCC,&Cpu6502::Relative,2,"BCC"}, {&Cpu6502::STA,&Cpu6502::IndirectIndexed,6,"STA"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::SHA,&Cpu6502::IndirectIndexed,6,"SHA"},{&Cpu6502::STY,&Cpu6502::ZeroPageX,4,"STY"},{&Cpu6502::STA,&Cpu6502::ZeroPageX,4,"STA"},{&Cpu6502::STX,&Cpu6502::ZeroPageY,4,"STX"},{&Cpu6502::SAX,&Cpu6502::ZeroPageY,4,"SAX"},{&Cpu6502::TYA,&Cpu6502::Implied,2,"TYA"},{&Cpu6502::STA,&Cpu6502::AbsoluteY,5,"STA"},{&Cpu6502::TXS,&Cpu6502::Implied,2,"TXS"},{&Cpu6502::SHS,&Cpu6502::AbsoluteY,5,"SHS"},{&Cpu6502::SHY,&Cpu6502::AbsoluteX,5,"SHY"},{&Cpu6502::STA,&Cpu6502::AbsoluteX,5,"STA"},{&Cpu6502::SHX,&Cpu6502::AbsoluteY,5,"SHX"},{&Cpu6502::SHA,&Cpu6502::AbsoluteY,5,"SHA"},
        /* Ax */    {&Cpu6502::LDY,&Cpu6502::Immediate,2,"LDY"},{&Cpu6502::LDA,&Cpu6502::IndexedIndirect,6,"LDA"},{&Cpu6502::LDX,&Cpu6502::Immediate,2,"LDX"},{&Cpu6502::LAX,&Cpu6502::IndexedIndirect,6,"LAX"},{&Cpu6502::LDY,&Cpu6502::ZeroPage,3,"LDY"}, {&Cpu6502::LDA,&Cpu6502::ZeroPage,3,"LDA"}, {&Cpu6502::LDX,&Cpu6502::ZeroPage,3,"LDX"}, {&Cpu6502::LAX,&Cpu6502::ZeroPage,3,"LAX"}, {&Cpu6502::TAY,&Cpu6502::Implied,2,"TAY"},{&Cpu6502::LDA,&Cpu6502::Immediate,2,"LDA"},{&Cpu6502::TAX,&Cpu6502::Implied,2,"TAX"},{&Cpu6502::LAX,&Cpu6502::Immediate,2,"LAX"},{&Cpu6502::LDY,&Cpu6502::Absolute,4,"LDY"}, {&Cpu6502::LDA,&Cpu6502::Absolute,4,"LDA"}, {&Cpu6502::LDX,&Cpu6502::Absolute,4,"LDX"}, {&Cpu6502::LAX,&Cpu6502::Absolute,4,"LAX"},
        /* Bx */    {&Cpu6502::BCS,&Cpu6502::Relative,2,"BCS"}, {&Cpu6502::LDA,&Cpu6502::IndirectIndexed,5,"LDA"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::LAX,&Cpu6502::IndirectIndexed,5,"LAX"},{&Cpu6502::LDY,&Cpu6502::ZeroPageX,4,"LDY"},{&Cpu6502::LDA,&Cpu6502::ZeroPageX,4,"LDA"},{&Cpu6502::LDX,&Cpu6502::ZeroPageY,4,"LDX"},{&Cpu6502::LAX,&Cpu6502::ZeroPageY,4,"LAX"},{&Cpu6502::CLV,&Cpu6502::Implied,2,"CLV"},{&Cpu6502::LDA,&Cpu6502::AbsoluteY,4,"LDA"},{&Cpu6502::TSX,&Cpu6502::Implied,2,"TSX"},{&Cpu6502::LAS,&Cpu6502::AbsoluteY,4,"LAS"},{&Cpu6502::LDY,&Cpu6502::AbsoluteX,4,"LDY"},{&Cpu6502::LDA,&Cpu6502::AbsoluteX,4,"LDA"},{&Cpu6502::LDX,&Cpu6502::AbsoluteY,4,"LDX"},{&Cpu6502::LAX,&Cpu6502::AbsoluteY,4,"LAX"},
        /* Cx */    {&Cpu6502::CPY,&Cpu6502::Immediate,2,"CPY"},{&Cpu6502::CMP,&Cpu6502::IndexedIndirect,6,"CMP"},{&Cpu6502::NOP,&Cpu6502::Immediate,2,"NOP"},{&Cpu6502::DCP,&Cpu6502::IndexedIndirect,8,"DCP"},{&Cpu6502::CPY,&Cpu6502::ZeroPage,3,"CPY"}, {&Cpu6502::CMP,&Cpu6502::ZeroPage,3,"CMP"}, {&Cpu6502::DEC,&Cpu6502::ZeroPage,5,"DEC"}, {&Cpu6502::DCP,&Cpu6502::ZeroPage,5,"DCP"}, {&Cpu6502::INY,&Cpu6502::Implied,2,"INY"},{&Cpu6502::CMP,&Cpu6502::Immediate,2,"CMP"},{&Cpu6502::DEX,&Cpu6502::Implied,2,"DEX"},{&Cpu6502::SBX,&Cpu6502::Immediate,2,"SBX"},{&Cpu6502::CPY,&Cpu6502::Absolute,4,"CPY"}, {&Cpu6502::CMP,&Cpu6502::Absolute,4,"CMP"}, {&Cpu6502::DEC,&Cpu6502::Absolute,6,"DEC"}, {&Cpu6502::DCP,&Cpu6502::Absolute,6,"DCP"},
        /* Dx */    {&Cpu6502::BNE,&Cpu6502::Relative,2,"BNE"}, {&Cpu6502::CMP,&Cpu6502::IndirectIndexed,5,"CMP"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::DCP,&Cpu6502::IndirectIndexed,8,"DCP"},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4,"NOP"},{&Cpu6502::CMP,&Cpu6502::ZeroPageX,4,"CMP"},{&Cpu6502::DEC,&Cpu6502::ZeroPageX,6,"DEC"},{&Cpu6502::DCP,&Cpu6502::ZeroPageX,6,"DCP"},{&Cpu6502::CLD,&Cpu6502::Implied,2,"CLD"},{&Cpu6502::CMP,&Cpu6502::AbsoluteY,4,"CMP"},{&Cpu6502::NOP,&Cpu6502::Implied,2,"NOP"},{&Cpu6502::DCP,&Cpu6502::AbsoluteY,7,"DCP"},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4,"NOP"},{&Cpu6502::CMP,&Cpu6502::AbsoluteX,4,"CMP"},{&Cpu6502::DEC,&Cpu6502::AbsoluteX,7,"DEC"},{&Cpu6502::DCP,&Cpu6502::AbsoluteX,7,"DCP"},
        /* Ex */    {&Cpu6502::CPX,&Cpu6502::Immediate,2,"CPX"},{&Cpu6502::SBC,&Cpu6502::IndexedIndirect,6,"SBC"},{&Cpu6502::NOP,&Cpu6502::Immediate,2,"NOP"},{&Cpu6502::ISB,&Cpu6502::IndexedIndirect,8,"ISB"},{&Cpu6502::CPX,&Cpu6502::ZeroPage,3,"CPX"}, {&Cpu6502::SBC,&Cpu6502::ZeroPage,3,"SBC"}, {&Cpu6502::INC,&Cpu6502::ZeroPage,5,"INC"}, {&Cpu6502::ISB,&Cpu6502::ZeroPage,5,"ISB"}, {&Cpu6502::INX,&Cpu6502::Implied,2,"INX"},{&Cpu6502::SBC,&Cpu6502::Immediate,2,"SBC"},{&Cpu6502::NOP,&Cpu6502::Implied,2,"NOP"},{&Cpu6502::SBC,&Cpu6502::Immediate,2,"SBC"},{&Cpu6502::CPX,&Cpu6502::Absolute,4,"CPX"}, {&Cpu6502::SBC,&Cpu6502::Absolute,4,"SBC"}, {&Cpu6502::INC,&Cpu6502::Absolute,6,"INC"}, {&Cpu6502::ISB,&Cpu6502::Absolute,6,"ISB"},
        /* Fx */    {&Cpu6502::BEQ,&Cpu6502::Relative,2,"BEQ"}, {&Cpu6502::SBC,&Cpu6502::IndirectIndexed,5,"SBC"},{&Cpu6502::JAM,&Cpu6502::Implied,0,"JAM"},  {&Cpu6502::ISB,&Cpu6502::IndirectIndexed,8,"ISB"},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4,"NOP"},{&Cpu6502::SBC,&Cpu6502::ZeroPageX,4,"SBC"},{&Cpu6502::INC,&Cpu6502::ZeroPageX,6,"INC"},{&Cpu6502::ISB,&Cpu6502::ZeroPageX,6,"ISB"},{&Cpu6502::SED,&Cpu6502::Implied,2,"SED"},{&Cpu6502::SBC,&Cpu6502::AbsoluteY,4,"SBC"},{&Cpu6502::NOP,&Cpu6502::Implied,2,"NOP"},{&Cpu6502::ISB,&Cpu6502::AbsoluteY,7,"ISB"},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4,"NOP"},{&Cpu6502::SBC,&Cpu6502::AbsoluteX,4,"SBC"},{&Cpu6502::INC,&Cpu6502::AbsoluteX,7,"INC"},{&Cpu6502::ISB,&Cpu6502::AbsoluteX,7,"ISB"}
    },
#else
    mOpcodeMatrix
    {
        // Indexed Indirect = izx
        // Indirect Indexed = izy
        //                             x0                                    x1                                          x2                                    x3                                          x4                                    x5                                   x6                                     x7                                    x8                                  x9                                    xA                                  xB                                    xC                                    xD                                    xE                                    xF
        /* 0x */    {&Cpu6502::BRK,&Cpu6502::Implied,7},  {&Cpu6502::ORA,&Cpu6502::IndexedIndirect,6},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::SLO,&Cpu6502::IndexedIndirect,8},{&Cpu6502::NOP,&Cpu6502::ZeroPage,3}, {&Cpu6502::ORA,&Cpu6502::ZeroPage,3},{&Cpu6502::ASL,&Cpu6502::ZeroPage,5},  {&Cpu6502::SLO,&Cpu6502::ZeroPage,5}, {&Cpu6502::PHP,&Cpu6502::Implied,3},{&Cpu6502::ORA,&Cpu6502::Immediate,2},{&Cpu6502::ASL,&Cpu6502::Implied,2},{&Cpu6502::ANC,&Cpu6502::Immediate,2},{&Cpu6502::NOP,&Cpu6502::Absolute,4}, {&Cpu6502::ORA,&Cpu6502::Absolute,4}, {&Cpu6502::ASL,&Cpu6502::Absolute,6}, {&Cpu6502::SLO,&Cpu6502::Absolute,6},
        /* 1x */    {&Cpu6502::BPL,&Cpu6502::Relative,2}, {&Cpu6502::ORA,&Cpu6502::IndirectIndexed,5},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::SLO,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::ORA,&Cpu6502::ZeroPageX,4},{&Cpu6502::ASL,&Cpu6502::ZeroPageX,6},{&Cpu6502::SLO,&Cpu6502::ZeroPageX,6},{&Cpu6502::CLC,&Cpu6502::Implied,2},{&Cpu6502::ORA,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::SLO,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::ORA,&Cpu6502::AbsoluteX,4},{&Cpu6502::ASL,&Cpu6502::AbsoluteX,7},{&Cpu6502::SLO,&Cpu6502::AbsoluteX,7},
        /* 2x */    {&Cpu6502::JSR,&Cpu6502::Absolute,6}, {&Cpu6502::AND,&Cpu6502::IndexedIndirect,6},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::RLA,&Cpu6502::IndexedIndirect,8},{&Cpu6502::BIT,&Cpu6502::ZeroPage,3}, {&Cpu6502::AND,&Cpu6502::ZeroPage,3},{&Cpu6502::ROL,&Cpu6502::ZeroPage,5},  {&Cpu6502::RLA,&Cpu6502::ZeroPage,5}, {&Cpu6502::PLP,&Cpu6502::Implied,4},{&Cpu6502::AND,&Cpu6502::Immediate,2},{&Cpu6502::ROL,&Cpu6502::Implied,2},{&Cpu6502::ANC,&Cpu6502::Immediate,2},{&Cpu6502::BIT,&Cpu6502::Absolute,4}, {&Cpu6502::AND,&Cpu6502::Absolute,4}, {&Cpu6502::ROL,&Cpu6502::Absolute,6}, {&Cpu6502::RLA,&Cpu6502::Absolute,6},
        /* 3x */    {&Cpu6502::BMI,&Cpu6502::Relative,2}, {&Cpu6502::AND,&Cpu6502::IndirectIndexed,5},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::RLA,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::AND,&Cpu6502::ZeroPageX,4},{&Cpu6502::ROL,&Cpu6502::ZeroPageX,6},{&Cpu6502::RLA,&Cpu6502::ZeroPageX,6},{&Cpu6502::SEC,&Cpu6502::Implied,2},{&Cpu6502::AND,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::RLA,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::AND,&Cpu6502::AbsoluteX,4},{&Cpu6502::ROL,&Cpu6502::AbsoluteX,7},{&Cpu6502::RLA,&Cpu6502::AbsoluteX,7},
        /* 4x */    {&Cpu6502::RTI,&Cpu6502::Implied,6},  {&Cpu6502::EOR,&Cpu6502::IndexedIndirect,6},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::SRE,&Cpu6502::IndexedIndirect,8},{&Cpu6502::NOP,&Cpu6502::ZeroPage,3}, {&Cpu6502::EOR,&Cpu6502::ZeroPage,3},{&Cpu6502::LSR,&Cpu6502::ZeroPage,5},  {&Cpu6502::SRE,&Cpu6502::ZeroPage,5}, {&Cpu6502::PHA,&Cpu6502::Implied,3},{&Cpu6502::EOR,&Cpu6502::Immediate,2},{&Cpu6502::LSR,&Cpu6502::Implied,2},{&Cpu6502::ASR,&Cpu6502::Immediate,2},{&Cpu6502::JMP,&Cpu6502::Absolute,3}, {&Cpu6502::EOR,&Cpu6502::Absolute,4}, {&Cpu6502::LSR,&Cpu6502::Absolute,6}, {&Cpu6502::SRE,&Cpu6502::Absolute,6},
        /* 5x */    {&Cpu6502::BVC,&Cpu6502::Relative,2}, {&Cpu6502::EOR,&Cpu6502::IndirectIndexed,5},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::SRE,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::EOR,&Cpu6502::ZeroPageX,4},{&Cpu6502::LSR,&Cpu6502::ZeroPageX,6},{&Cpu6502::SRE,&Cpu6502::ZeroPageX,6},{&Cpu6502::CLI,&Cpu6502::Implied,2},{&Cpu6502::EOR,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::SRE,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::EOR,&Cpu6502::AbsoluteX,4},{&Cpu6502::LSR,&Cpu6502::AbsoluteX,7},{&Cpu6502::SRE,&Cpu6502::AbsoluteX,7},
        /* 6x */    {&Cpu6502::RTS,&Cpu6502::Implied,6},  {&Cpu6502::ADC,&Cpu6502::IndexedIndirect,6},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::RRA,&Cpu6502::IndexedIndirect,8},{&Cpu6502::NOP,&Cpu6502::ZeroPage,3}, {&Cpu6502::ADC,&Cpu6502::ZeroPage,3},{&Cpu6502::ROR,&Cpu6502::ZeroPage,5},  {&Cpu6502::RRA,&Cpu6502::ZeroPage,5}, {&Cpu6502::PLA,&Cpu6502::Implied,4},{&Cpu6502::ADC,&Cpu6502::Immediate,2},{&Cpu6502::ROR,&Cpu6502::Implied,2},{&Cpu6502::ARR,&Cpu6502::Immediate,2},{&Cpu6502::JMP,&Cpu6502::Indirect,5}, {&Cpu6502::ADC,&Cpu6502::Absolute,4}, {&Cpu6502::ROR,&Cpu6502::Absolute,6}, {&Cpu6502::RRA,&Cpu6502::Absolute,6},
        /* 7x */    {&Cpu6502::BVS,&Cpu6502::Relative,2}, {&Cpu6502::ADC,&Cpu6502::IndirectIndexed,5},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::RRA,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::ADC,&Cpu6502::ZeroPageX,4},{&Cpu6502::ROR,&Cpu6502::ZeroPageX,6},{&Cpu6502::RRA,&Cpu6502::ZeroPageX,6},{&Cpu6502::SEI,&Cpu6502::Implied,2},{&Cpu6502::ADC,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::RRA,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::ADC,&Cpu6502::AbsoluteX,4},{&Cpu6502::ROR,&Cpu6502::AbsoluteX,7},{&Cpu6502::RRA,&Cpu6502::AbsoluteX,7},
        /* 8x */    {&Cpu6502::NOP,&Cpu6502::Immediate,2},{&Cpu6502::STA,&Cpu6502::IndexedIndirect,6},{&Cpu6502::NOP,&Cpu6502::Immediate,2},{&Cpu6502::SAX,&Cpu6502::IndexedIndirect,6},{&Cpu6502::STY,&Cpu6502::ZeroPage,3}, {&Cpu6502::STA,&Cpu6502::ZeroPage,3},{&Cpu6502::STX,&Cpu6502::ZeroPage,3},  {&Cpu6502::SAX,&Cpu6502::ZeroPage,3}, {&Cpu6502::DEY,&Cpu6502::Implied,2},{&Cpu6502::NOP,&Cpu6502::Immediate,2},{&Cpu6502::TXA,&Cpu6502::Implied,2},{&Cpu6502::XAA,&Cpu6502::Immediate,2},{&Cpu6502::STY,&Cpu6502::Absolute,4}, {&Cpu6502::STA,&Cpu6502::Absolute,4}, {&Cpu6502::STX,&Cpu6502::Absolute,4}, {&Cpu6502::SAX,&Cpu6502::Absolute,4},
        /* 9x */    {&Cpu6502::BCC,&Cpu6502::Relative,2}, {&Cpu6502::STA,&Cpu6502::IndirectIndexed,6},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::SHA,&Cpu6502::IndirectIndexed,6},{&Cpu6502::STY,&Cpu6502::ZeroPageX,4},{&Cpu6502::STA,&Cpu6502::ZeroPageX,4},{&Cpu6502::STX,&Cpu6502::ZeroPageY,4},{&Cpu6502::SAX,&Cpu6502::ZeroPageY,4},{&Cpu6502::TYA,&Cpu6502::Implied,2},{&Cpu6502::STA,&Cpu6502::AbsoluteY,5},{&Cpu6502::TXS,&Cpu6502::Implied,2},{&Cpu6502::SHS,&Cpu6502::AbsoluteY,5},{&Cpu6502::SHY,&Cpu6502::AbsoluteX,5},{&Cpu6502::STA,&Cpu6502::AbsoluteX,5},{&Cpu6502::SHX,&Cpu6502::AbsoluteY,5},{&Cpu6502::SHA,&Cpu6502::AbsoluteY,5},
        /* Ax */    {&Cpu6502::LDY,&Cpu6502::Immediate,2},{&Cpu6502::LDA,&Cpu6502::IndexedIndirect,6},{&Cpu6502::LDX,&Cpu6502::Immediate,2},{&Cpu6502::LAX,&Cpu6502::IndexedIndirect,6},{&Cpu6502::LDY,&Cpu6502::ZeroPage,3}, {&Cpu6502::LDA,&Cpu6502::ZeroPage,3},{&Cpu6502::LDX,&Cpu6502::ZeroPage,3},  {&Cpu6502::LAX,&Cpu6502::ZeroPage,3}, {&Cpu6502::TAY,&Cpu6502::Implied,2},{&Cpu6502::LDA,&Cpu6502::Immediate,2},{&Cpu6502::TAX,&Cpu6502::Implied,2},{&Cpu6502::LAX,&Cpu6502::Immediate,2},{&Cpu6502::LDY,&Cpu6502::Absolute,4}, {&Cpu6502::LDA,&Cpu6502::Absolute,4}, {&Cpu6502::LDX,&Cpu6502::Absolute,4}, {&Cpu6502::LAX,&Cpu6502::Absolute,4},
        /* Bx */    {&Cpu6502::BCS,&Cpu6502::Relative,2}, {&Cpu6502::LDA,&Cpu6502::IndirectIndexed,5},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::LAX,&Cpu6502::IndirectIndexed,5},{&Cpu6502::LDY,&Cpu6502::ZeroPageX,4},{&Cpu6502::LDA,&Cpu6502::ZeroPageX,4},{&Cpu6502::LDX,&Cpu6502::ZeroPageY,4},{&Cpu6502::LAX,&Cpu6502::ZeroPageY,4},{&Cpu6502::CLV,&Cpu6502::Implied,2},{&Cpu6502::LDA,&Cpu6502::AbsoluteY,4},{&Cpu6502::TSX,&Cpu6502::Implied,2},{&Cpu6502::LAS,&Cpu6502::AbsoluteY,4},{&Cpu6502::LDY,&Cpu6502::AbsoluteX,4},{&Cpu6502::LDA,&Cpu6502::AbsoluteX,4},{&Cpu6502::LDX,&Cpu6502::AbsoluteY,4},{&Cpu6502::LAX,&Cpu6502::AbsoluteY,4},
        /* Cx */    {&Cpu6502::CPY,&Cpu6502::Immediate,2},{&Cpu6502::CMP,&Cpu6502::IndexedIndirect,6},{&Cpu6502::NOP,&Cpu6502::Immediate,2},{&Cpu6502::DCP,&Cpu6502::IndexedIndirect,8},{&Cpu6502::CPY,&Cpu6502::ZeroPage,3}, {&Cpu6502::CMP,&Cpu6502::ZeroPage,3},{&Cpu6502::DEC,&Cpu6502::ZeroPage,5},  {&Cpu6502::DCP,&Cpu6502::ZeroPage,5}, {&Cpu6502::INY,&Cpu6502::Implied,2},{&Cpu6502::CMP,&Cpu6502::Immediate,2},{&Cpu6502::DEX,&Cpu6502::Implied,2},{&Cpu6502::SBX,&Cpu6502::Immediate,2},{&Cpu6502::CPY,&Cpu6502::Absolute,4}, {&Cpu6502::CMP,&Cpu6502::Absolute,4}, {&Cpu6502::DEC,&Cpu6502::Absolute,6}, {&Cpu6502::DCP,&Cpu6502::Absolute,6},
        /* Dx */    {&Cpu6502::BNE,&Cpu6502::Relative,2}, {&Cpu6502::CMP,&Cpu6502::IndirectIndexed,5},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::DCP,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::CMP,&Cpu6502::ZeroPageX,4},{&Cpu6502::DEC,&Cpu6502::ZeroPageX,6},{&Cpu6502::DCP,&Cpu6502::ZeroPageX,6},{&Cpu6502::CLD,&Cpu6502::Implied,2},{&Cpu6502::CMP,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::DCP,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::CMP,&Cpu6502::AbsoluteX,4},{&Cpu6502::DEC,&Cpu6502::AbsoluteX,7},{&Cpu6502::DCP,&Cpu6502::AbsoluteX,7},
        /* Ex */    {&Cpu6502::CPX,&Cpu6502::Immediate,2},{&Cpu6502::SBC,&Cpu6502::IndexedIndirect,6},{&Cpu6502::NOP,&Cpu6502::Immediate,2},{&Cpu6502::ISB,&Cpu6502::IndexedIndirect,8},{&Cpu6502::CPX,&Cpu6502::ZeroPage,3}, {&Cpu6502::SBC,&Cpu6502::ZeroPage,3},{&Cpu6502::INC,&Cpu6502::ZeroPage,5},  {&Cpu6502::ISB,&Cpu6502::ZeroPage,5}, {&Cpu6502::INX,&Cpu6502::Implied,2},{&Cpu6502::SBC,&Cpu6502::Immediate,2},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::SBC,&Cpu6502::Immediate,2},{&Cpu6502::CPX,&Cpu6502::Absolute,4}, {&Cpu6502::SBC,&Cpu6502::Absolute,4}, {&Cpu6502::INC,&Cpu6502::Absolute,6}, {&Cpu6502::ISB,&Cpu6502::Absolute,6},
        /* Fx */    {&Cpu6502::BEQ,&Cpu6502::Relative,2}, {&Cpu6502::SBC,&Cpu6502::IndirectIndexed,5},{&Cpu6502::JAM,&Cpu6502::Implied,0},  {&Cpu6502::ISB,&Cpu6502::IndirectIndexed,8},{&Cpu6502::NOP,&Cpu6502::ZeroPageX,4},{&Cpu6502::SBC,&Cpu6502::ZeroPageX,4},{&Cpu6502::INC,&Cpu6502::ZeroPageX,6},{&Cpu6502::ISB,&Cpu6502::ZeroPageX,6},{&Cpu6502::SED,&Cpu6502::Implied,2},{&Cpu6502::SBC,&Cpu6502::AbsoluteY,4},{&Cpu6502::NOP,&Cpu6502::Implied,2},{&Cpu6502::ISB,&Cpu6502::AbsoluteY,7},{&Cpu6502::NOP,&Cpu6502::AbsoluteX,4},{&Cpu6502::SBC,&Cpu6502::AbsoluteX,4},{&Cpu6502::INC,&Cpu6502::AbsoluteX,7},{&Cpu6502::ISB,&Cpu6502::AbsoluteX,7}
    },
#endif
    mHalted(false),
    mInterruptVectors
    {
        {0xFFFA, 0xFFFB},
        {0xFFFC, 0xFFFD},
        {0xFFFE, 0xFFFF},
        {0xFFFE, 0xFFFF},
    }
{
#if defined(TEST_CPU)
    mTotalCycles = 0;
    mFunctor     = NULL;
#endif
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

//--------//
// Read
//
// Reads data from the system.
//
// param[in] lAddress   Address to read from.
// param[in] lLastRead  If some devices are not connected, this variable
//                      simulates "open bus behavior". Where a read of
//                      a disconnected device results in the last value read.
//                      Ignore what's passed in since the cpu stores this value
//                      already internally.
// returns  Data at the given address. 
//--------//
//
DataType Cpu6502::Read(AddressType lAddress, DataType lLastRead)
{
    if (mSystem == NULL)
    {
        return mFetchedData;
    }
    return mSystem->Read(lAddress, mFetchedData);
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
void Cpu6502::Write(AddressType lAddress, DataType lData)
{
    if (mSystem)
    {
        mSystem->Write(lAddress, lData);
    }
}

//--------//
// StepClock
//
// Performs the fetch-decode-execute cycle for instructions. All
// operations execute fully on the "first" clock cycle of that
// instruction. If an instruction is in progress (indicated by
// the mCyclesLeft variable), then just decrement the counter and
// do nothing else.
//--------//
//
void Cpu6502::StepClock()
{
    uint8_t lAddCycleAddress;
    uint8_t lAddCycleInstruction;

    // Don't do anything if halted.
    if (mHalted)
    {
        return;
    }

    // No instruction is in progress, so perform fetch-decode-execute.
    if (mCyclesLeft == 0)
    {
#if defined(TEST_CPU)
        // Convert instruction to something more human readable.
        Disassemble(mRegisters.mPc);

        // Add contents of registers to disassembled instruction.
        snprintf(cFormatBuffer, Cpu6502::FORMAT_BUFFER_SIZE, "A:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:%u\n",
                mRegisters.mAcc, mRegisters.mX, mRegisters.mY, mRegisters.mStatus,mRegisters.mSp, mTotalCycles);
        strncat(cBuffer, cFormatBuffer, Cpu6502::BUFFER_SIZE);

        // Call appropriate functor to handle the trace.
        if (mFunctor)
        {
            mFunctor->Execute();
        }
#ifdef DUMP_STACK
        std::string lString = cBuffer + "\n" + DumpStack() + "\n";
        ApiLogger::Log(&lString);
#endif
#endif

        // Grab next instruction and increment program counter.
        FetchOpcode();

        // Figure out where the data is going to be.
        lAddCycleAddress     = (this->*mOpcodeMatrix[mOpcode].mAddressMode)();

        // Execute the instruction.
        lAddCycleInstruction = (this->*mOpcodeMatrix[mOpcode].mInstruction)();

        // If both the address mode and instruction indicates that an extra cycle needs to be added, add it here.
        if (lAddCycleAddress == Cpu6502::ADD_CLOCK_CYCLE && lAddCycleInstruction == Cpu6502::ADD_CLOCK_CYCLE)
        {
            mCyclesLeft += 1;
        }

        // Calculate how much cycles this instruction takes. 
        mCyclesLeft += mOpcodeMatrix[mOpcode].mCycles;
    }

    // Decrement the cycles counter, as one cycle has now elapsed.
    --mCyclesLeft;

#ifdef TEST_CPU
    ++mTotalCycles;
#endif
}

//--------//
// NMI
//
// Non-maskable interrupt. Does exactly what an IRQ does just reading from
// a different interrupt vector. Can happen even when interrupts are disabled.
//--------//
//
void Cpu6502::NMI()
{
    // Don't do anything if halted.
    if (mHalted)
    {
        return;
    }

    // NMI still interrupts, even if interrupts are disabled.
    // https://www.nesdev.org/wiki/Status_flags#I:_Interrupt_Disable

    // Store the return address onto the stack, high byte then low byte.
    PushStack((mRegisters.mPc >> 8) & 0x00FF);
    PushStack(mRegisters.mPc & 0x00FF);

    // Make sure break flag is clear.
    ClearFlag(Flags::B);
    SetFlag(Flags::U);                  // Unused flag should always be high.
    PushStack(mRegisters.mStatus);

    // Disable interrupts.
    SetFlag(Flags::I);

    // Finally, jump to the address from the interrupt vector.
    mRegisters.mPc = (Read(mInterruptVectors[NMI_VECTOR].mLowByte) | (Read(mInterruptVectors[NMI_VECTOR].mHighByte) << 8));

    // Hardware takes 7 clock cycles to do this.
    mCyclesLeft = 7;
}

//--------//
// IRQ
//
// Interrupt request. Store the current program counter onto stack
// along with the status register. Can only be done if interrupts are
// not disabled.
//--------//
//
void Cpu6502::IRQ()
{
    // Don't do anything if halted.
    if (mHalted)
    {
        return;
    }

    // If interrupts are enabled.
    if (GetFlag(Flags::I) == 0)
    {
        // Store the return address onto the stack, high byte then low byte.
        PushStack((mRegisters.mPc >> 8) & 0x00FF);
        PushStack(mRegisters.mPc & 0x00FF);

        // Make sure break flag is clear.
        ClearFlag(Flags::B);
        SetFlag(Flags::U);                  // Unused flag should always be high.
        PushStack(mRegisters.mStatus);

        // Disable interrupts.
        SetFlag(Flags::I);

        // Finally, jump to the address from the interrupt vector.
        mRegisters.mPc = (Read(mInterruptVectors[IRQ_VECTOR].mLowByte) | (Read(mInterruptVectors[IRQ_VECTOR].mHighByte) << 8));

        // Hardware takes 7 clock cycles to do this.
        mCyclesLeft = 7;
    }
}

//--------//
// Reset
//
// Resets the CPU back to a state as if was just turned on.
//--------//
//
void Cpu6502::Reset()
{
    // Reset is the only thing that will reset this flag.
    mHalted = false;

    // Reset registers and internals.
    mOpcode             = 0x1A;                 // NOP - Implied. Souldn't matter since PC will jump to some other Opcode.
    mFetchedData        = 0x00;
    mAddress            = 0x0000;
    mRelativeAddress    = 0x0000;
    mRegisters.mAcc     = 0x00;
    mRegisters.mX       = 0x00;
    mRegisters.mY       = 0x00;
    mRegisters.mStatus  = 0x00 | Flags::U | Flags::I;   // Unused flag should always be high.
                                                        // https://github.com/OneLoneCoder/olcNES/issues/34

    // Believe this is what the hardware does. The hardware does "fake" reads of those registers and
    // discards the results, decrementing the SP. It should end up at 0xFD after this.
    // https://www.pagetable.com/?p=410
    mRegisters.mSp      = 0x00;
    --mRegisters.mSp;
    --mRegisters.mSp;
    --mRegisters.mSp;

    // Grab program counter from known interrupt vector.
    mRegisters.mPc = (Read(mInterruptVectors[RESET_VECTOR].mLowByte) | (Read(mInterruptVectors[RESET_VECTOR].mHighByte) << 8));

    // Hardware takes 7 clock cycles to do this.
    mCyclesLeft = 7;
}

//--------//
// SetOrClearFlag
//
// Sets or clears a flag given a certain condition.
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
    mOpcode = Read(mRegisters.mPc++);
}

//--------//
// FetchData
//
// Grabs data from an address set by one of the addressing modes.
//--------//
//
void Cpu6502::FetchData()
{
    // For immediate addressing, the data immediately follows the opcode.
    // mAddress already contains it, so just return.
    if (mOpcodeMatrix[mOpcode].mAddressMode == &Cpu6502::Immediate)
    {
        mFetchedData = mAddress;
    }
    mFetchedData = Read(mAddress);
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
    Write(cStartOfStack | mRegisters.mSp, lData);
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
    ++mRegisters.mSp;
    DataType lData = Read(cStartOfStack | mRegisters.mSp);
    return lData;
}

//--------//
// ADDRESSING MODES
//

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
    mAddress = mRegisters.mPc++;
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
    mAddress = Read(mRegisters.mPc++);
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
    mAddress = Read(mRegisters.mPc++) + mRegisters.mX;
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
    mAddress = Read(mRegisters.mPc++) + mRegisters.mY;
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
    mRelativeAddress = Read(mRegisters.mPc++);
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
    uint16_t lLowByte  = Read(mRegisters.mPc++);
    uint16_t lHighByte = Read(mRegisters.mPc++);
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
    uint16_t lLowByte  = Read(mRegisters.mPc++);
    uint16_t lHighByte = Read(mRegisters.mPc++);
    mAddress = (lHighByte << 8) | lLowByte;
    mAddress += mRegisters.mX;

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
    uint16_t lLowByte  = Read(mRegisters.mPc++);
    uint16_t lHighByte = Read(mRegisters.mPc++);
    mAddress = (lHighByte << 8) | lLowByte;
    mAddress += mRegisters.mY;

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
    DataType    lLowByte  = Read(mRegisters.mPc++);
    DataType    lHighByte = Read(mRegisters.mPc++);
    AddressType lIndirect = (lHighByte << 8) | lLowByte;

    // Now grab the actual address using the contents of the previous address.
    // NOTE: Hardware bug alert! If the low byte and high byte of the indirect
    //       address crossed page boundaries, the 6502 wrapped back around to the same page.
    //       http://www.oxyron.de/html/opcodes02.html
    if (lLowByte & 0x00FF)
    {
        lLowByte  = Read(lIndirect);
        lHighByte = Read(lIndirect & 0xFF00);
    }
    else // This is how it was supposed to work in all cases.
    {
        lLowByte  = Read(lIndirect);
        lHighByte = Read(lIndirect + 1);
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
    AddressType lIndirect = (Read(mRegisters.mPc++) + mRegisters.mX) & 0x00FF;
    AddressType lLowByte  = Read(lIndirect & 0x00FF) ;        // Hardware bug! The 6502 did not cross page boundaries here.
    AddressType lHighByte = Read((lIndirect + 1) & 0x00FF);  // Hardware bug! The 6502 did not cross page boundaries here.
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
    AddressType lIndirect = Read(mRegisters.mPc++);
    AddressType lLowByte  = Read(lIndirect & 0x00FF);
    AddressType lHighByte = Read((lIndirect + 1) & 0x00FF);
    mAddress  = (lHighByte << 8) | lLowByte;
    mAddress += mRegisters.mY;

    if ((mAddress & 0xFF00) != (lHighByte << 8))
    {
        return ADD_CLOCK_CYCLE;
    }
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// INSTRUCTIONS

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
    FetchData();
    uint16_t lResult = mRegisters.mAcc + mFetchedData + GetFlag(Flags::C);

    // Set the Carry flag based on if the result is more than the maximum value of an 8-bit number.
    SetOrClearFlag(Flags::C, lResult > 0x00FF);

    // Set the Overflow flag if the sign bit has changed from result exceeding +127 or -128.
    // https://forums.nesdev.org/viewtopic.php?t=6331
    SetOrClearFlag(Flags::V, ((mRegisters.mAcc ^ lResult) & (mFetchedData ^ lResult)) & 0x0080);

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
    FetchData();
    mRegisters.mAcc = mRegisters.mAcc & mFetchedData;

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
    FetchData();
    uint16_t lData = mFetchedData << 1;

    // Set the Carry flag if bit 8 is set.
    SetOrClearFlag(Flags::C, lData > 0x00FF);

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lData & 0x00FF) == 0);

    // Set the Negative flag if bit 7 is set.
    SetOrClearFlag(Flags::N, lData & Bit(7));

    // Only store result to accumulator if address mode was Implied.
    if (mOpcodeMatrix[mOpcode].mAddressMode == &Cpu6502::Implied)
    {
        mRegisters.mAcc = lData & 0x00FF;
    }
    else
    {
        Write(mAddress, lData & 0x00FF);
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
    FetchData();

    // Set the Negative flag if the MSB is set at memory location.
    SetOrClearFlag(Flags::N, mFetchedData & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (mRegisters.mAcc & mFetchedData) == 0);

    // Set the Overflow flag if bit 6 is set at memory location.
    SetOrClearFlag(Flags::V, mFetchedData & Bit(6));

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

    // Store the return address onto the stack, high byte then low byte.
    PushStack((mRegisters.mPc >> 8) & 0x00FF);
    PushStack(mRegisters.mPc & 0x00FF);

    // Mark the break in status register, then store it onto the stack.
    SetFlag(Flags::B);
    SetFlag(Flags::U);                  // Unused flag should always be high.
    PushStack(mRegisters.mStatus);
    ClearFlag(Flags::B);

    // Disable interrupts.
    // Should this happen before status is pushed onto stack?
    SetFlag(Flags::I);

    // Finally, jump to the address from the interrupt vector.
    mRegisters.mPc = (Read(mInterruptVectors[BRK_VECTOR].mLowByte) | (Read(mInterruptVectors[BRK_VECTOR].mHighByte) << 8));

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
    FetchData();
    uint16_t lResult = mRegisters.mAcc - mFetchedData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Carry flag if the data in memory is <= than accumulator.
    SetOrClearFlag(Flags::C, mFetchedData <= mRegisters.mAcc);

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
    FetchData();
    uint16_t lResult = mRegisters.mX - mFetchedData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Carry flag if the X register is >= than the data in memory.
    SetOrClearFlag(Flags::C, mRegisters.mX >= mFetchedData);

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
    FetchData();
    uint16_t lResult = mRegisters.mY - mFetchedData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Carry flag if the X register is >= than the data in memory.
    SetOrClearFlag(Flags::C, mRegisters.mY >= mFetchedData);

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
    FetchData();
    uint16_t lResult = mFetchedData - 1;

    // Write result back to memory.
    Write(mAddress, lResult & 0x00FF);

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
    FetchData();
    mRegisters.mAcc = mFetchedData ^ mRegisters.mAcc;

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mAcc == 0);

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
    FetchData();
    uint16_t lResult = mFetchedData + 1;

    // Write result back to memory.
    Write(mAddress, lResult & 0x00FF);

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
    FetchData();
    mRegisters.mAcc = mFetchedData;

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
    FetchData();
    mRegisters.mX = mFetchedData;

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
    FetchData();
    mRegisters.mY = mFetchedData;

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
    // Grab the data and perform the right shift.
    FetchData();
    uint16_t lData = mFetchedData >> 1;

    // Set the Carry flag if bit 0 is set.
    SetOrClearFlag(Flags::C, mFetchedData & Bit(0));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lData & 0x00FF) == 0);

    // N flag is always reset.
    ClearFlag(Flags::N);

    // Only store result to accumulator if address mode was Implied.
    if (mOpcodeMatrix[mOpcode].mAddressMode == &Cpu6502::Implied)
    {
        mRegisters.mAcc = lData & 0x00FF;
    }
    else
    {
        Write(mAddress, lData & 0x00FF);
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
    FetchData();
    mRegisters.mAcc |= mFetchedData;

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

    // Ignore break flag.
    ClearFlag(Flags::B);

    // Unused flag should always be high.
    // https://github.com/OneLoneCoder/olcNES/issues/34
    SetFlag(Flags::U);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// ROL
//
// Rotate Left. Shifts either memory or accumlator left one bit, with the carry flag stored at bit 0.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ROL()
{
    // Perform rotate left operation.
    FetchData();
    uint16_t lResult = (mFetchedData << 1) | GetFlag(Flags::C);

    // Set the Carry flag if bit 8 is set.
    SetOrClearFlag(Flags::C, lResult & 0xFF00);

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if bit 7 is set.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Only store result to accumulator if address mode was Implied.
    if (mOpcodeMatrix[mOpcode].mAddressMode == &Cpu6502::Implied)
    {
        mRegisters.mAcc = lResult & 0x00FF;
    }
    else
    {
        Write(mAddress, lResult & 0x00FF);
    }
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// ROR
//
// Rotate Right. Shifts either memory or accumlator right one bit, with the carry flag stored at bit 7.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ROR()
{
    // Perform rotate right operation.
    FetchData();
    uint16_t lResult = (mFetchedData >> 1) | (GetFlag(Flags::C) << 7);

    // Set the Carry flag if bit 0 is set.
    SetOrClearFlag(Flags::C, mFetchedData & Bit(0));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if bit 7 is set.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Only store result to accumulator if address mode was Implied.
    if (mOpcodeMatrix[mOpcode].mAddressMode == &Cpu6502::Implied)
    {
        mRegisters.mAcc = lResult & 0x00FF;
    }
    else
    {
        Write(mAddress, lResult & 0x00FF);
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
    mRegisters.mPc  = PopStack();
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
    FetchData();
    uint16_t lInverted = mFetchedData ^ 0x00FF;
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
    Write(mAddress, mRegisters.mAcc);
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
    Write(mAddress, mRegisters.mX);
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
    Write(mAddress, mRegisters.mY);
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
// Alias for JAM. Halts the processor.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::KIL()
{
    mHalted = true;
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// ILLEGAL INSTRUCTIONS

//--------//
// ANC
//
// AND Memory with Accumulator then Move Negative Flag to Carry Flag.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ANC()
{
    // Perform AND.
    FetchData();
    mRegisters.mAcc &= mFetchedData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mRegisters.mAcc == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    // Set the Carry flag if the data in memory is <= than accumulator.
    SetOrClearFlag(Flags::C, mRegisters.mAcc & Bit(7));

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// ARR
//
// AND Accumulator then Rotate Right.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ARR()
{
    // Perform AND operation.
    FetchData();
    mRegisters.mAcc &= mFetchedData;

    // Rotate Right
    mRegisters.mAcc = (mRegisters.mAcc >> 1) | (GetFlag(Flags::C) << 7);

    // Set the Carry flag based if Bit 6 of result is set.
    SetOrClearFlag(Flags::C, mRegisters.mAcc  & Bit(6));

    // Set if Bit 6 is different than Bit 5.
    SetOrClearFlag(Flags::V, ((mRegisters.mAcc & Bit(6)) ^ (mRegisters.mAcc & Bit(5))));

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (mRegisters.mAcc & 0x00FF) == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// ASR
//
// AND then Logical Shift Right.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ASR()
{
    // Perform AND the right shift.
    FetchData();
    mRegisters.mAcc = (mRegisters.mAcc & mFetchedData) >> 1;

    // Set the Carry flag based if Bit 7 of fetched data was set.
    SetOrClearFlag(Flags::C, mFetchedData  & Bit(7));

    // Negative flag is always reset.
    ClearFlag(Flags::N);

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (mRegisters.mAcc & 0x00FF) == 0);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// DCP
//
// Decrements the contents of a memory address and does a CMP.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::DCP()
{
    // Perform decrement.
    FetchData();
    Write(mAddress, --mFetchedData);

    uint16_t lResult = mRegisters.mAcc - mFetchedData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Carry flag if the data in memory is <= than accumulator.
    SetOrClearFlag(Flags::C, mFetchedData <= mRegisters.mAcc);

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// ISB
//
// Increments the contents of a memory address and does a SBC from accumulator (Also known as ISC or INS).
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::ISB()
{
    // Perform increment.
    FetchData();
    Write(mAddress, ++mFetchedData);

    uint16_t lInverted = mFetchedData ^ 0x00FF;
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

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// JAM
//
// Stop execution of the processor. Ignores everything, except for reset. Even interrupts.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::JAM()
{
    mHalted = true;
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// LAS
//
// AND Memory with Stack Pointer.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::LAS()
{
    // AND operation.
    FetchData();
    DataType lData = mRegisters.mSp & mFetchedData;

    // Result gets stored in these 3 registers.
    mRegisters.mAcc = lData;
    mRegisters.mX   = lData;
    mRegisters.mSp  = lData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, lData == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, lData & Bit(7));

    return ADD_CLOCK_CYCLE;
}

//--------//
// LAX
//
// LDA and LDX operation.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::LAX()
{
    // Load accumulator with memory.
    FetchData();
    mRegisters.mAcc = mFetchedData;
    mRegisters.mX   = mFetchedData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, mFetchedData == 0);

    // Set the Negative flag if the result is negative.
    SetOrClearFlag(Flags::N, mFetchedData & Bit(7));

    return ADD_CLOCK_CYCLE;
}

//--------//
// RLA
//
// Rotate Left then AND with Accumulator.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::RLA()
{
    // Perform rotate left operation.
    FetchData();
    uint16_t lResult = (mFetchedData << 1) | GetFlag(Flags::C);

    // Set the Carry flag if bit 8 is set.
    SetOrClearFlag(Flags::C, lResult & 0xFF00);

    // Write out to memory.
    Write(mAddress, lResult & 0x00FF);

    // Perform AND operation.
    mRegisters.mAcc &= lResult;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Set the Negative flag if bit 7 is set.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// RRA
//
// Rotate Right and Add Memory to Accumuatlor
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::RRA()
{
    // Perform rotate right operation.
    FetchData();
    uint16_t lResult = (mFetchedData >> 1) | (GetFlag(Flags::C) << 7);

    // Write result back to memory.
    Write(mAddress, lResult & 0x00FF);

    // Perform Add with carry flag, which is set if bit 0 was shifted out.
    lResult += mRegisters.mAcc + (mFetchedData & Bit(0));

    // Set the Carry flag based on if the result is more than the maximum value of an 8-bit number.
    SetOrClearFlag(Flags::C, lResult > 0x00FF);

    // Set the Overflow flag if the sign bit has changed from result exceeding +127 or -128.
    SetOrClearFlag(Flags::V, ((mRegisters.mAcc ^ lResult) & (lResult ^ lResult)) & 0x0080);

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Make sure we are just storing the low byte from the result.
    mRegisters.mAcc = lResult & 0x00FF;

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// SAX
//
// Accumulator AND X register, stored in memory.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SAX()
{
    // Load memory with the AND operation.
    Write(mAddress, mRegisters.mAcc & mRegisters.mX);
    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// SBX
//
// Subtract Memory from Accumulator AND Index Register X.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SBX()
{
    // Perform AND then Subtraction.
    FetchData();
    uint16_t lInverted = (mFetchedData & mRegisters.mX) ^ 0x00FF;
    uint16_t lResult   = mRegisters.mAcc + lInverted + GetFlag(Flags::C);

    // Carry if >= 0, otherwise it's a borrow.
    SetOrClearFlag(Flags::C, lResult >= 0);

    // Set the Negative flag if the MSB is set.
    SetOrClearFlag(Flags::N, lResult & Bit(7));

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (lResult & 0x00FF) == 0);

    // Make sure we are just storing the low byte from the result.
    mRegisters.mX = lResult & 0x00FF;

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// SHA
//
// Store Accumulator AND Index Register X AND Value.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SHA()
{
    // AND the accumulator and X register.
    DataType lData = mRegisters.mAcc & mRegisters.mX;

    // Get the address for the third operand. It's the address without the Y register offset.
    AddressType lOperand = (Read(mRegisters.mPc - 2) | (Read(mRegisters.mPc - 1) << 8));

    // In this addressing mode, only the High Byte is considered.
    if (mOpcodeMatrix[mOpcode].mAddressMode == &Cpu6502::IndirectIndexed)
    {
        lOperand = lOperand & 0xFF00;
    }

    // In either addressing mode, add one to the operation and perform the final AND.
    Write(mAddress, lData & (lOperand + 1));

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// SHX
//
// Store Index Register X AND Value.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SHX()
{
    // Get the address for the third operand, without the Y register offset. Only the
    // High Byte is considered.
    AddressType lOperand = (Read(mRegisters.mPc - 1) << 8) & 0xFF00;

    // Perform final AND and store result.
    Write(mAddress, mRegisters.mX & (lOperand + 1));

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// SHY
//
// Store Index Register Y AND Value.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SHY()
{
    // Get the address for the third operand, without the X register offset. Only the
    // High Byte is considered.
    AddressType lOperand = (Read(mRegisters.mPc - 1) << 8) & 0xFF00;

    // Perform final AND and store result.
    Write(mAddress, mRegisters.mY & (lOperand + 1));

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// SHS
//
// Transfer Accumulator AND Index Register X to Stack Pointer then Store Stack Pointer AND HighByte in Memory.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SHS()
{
    // AND the accumulator and X register.
    mRegisters.mSp = mRegisters.mAcc & mRegisters.mX;

    // Get the address for the third operand, without the Y register offset. Only the
    // High Byte is considered.
    AddressType lOperand = (Read(mRegisters.mPc - 1) << 8) & 0xFF00;

    // Perform final AND and store result.
    Write(mAddress, mRegisters.mSp & (lOperand + 1));

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// SLO
//
// Performs ASL of a memory locations and ORs result with accumulator.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SLO()
{
    // Perform the left shift.
    FetchData();
    uint16_t lData = mFetchedData << 1;

    // Write back to memory.
    Write(mAddress, lData & 0x00FF);

    // Set the Carry flag if bit 8 is set.
    SetOrClearFlag(Flags::C, lData > 0x00FF);

    // Now do the OR.
    mRegisters.mAcc |= lData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (mRegisters.mAcc & 0x00FF) == 0);

    // Set the Negative flag if bit 7 is set.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// SRE
//
// Logical shift right then XOR memory with accumulator.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::SRE()
{
    // Perform the right shift.
    FetchData();

    // Set the Carry flag if bit 0 is set.
    SetOrClearFlag(Flags::C, mFetchedData & Bit(0));

    uint16_t lData = mFetchedData >> 1;

    // Write back to memory.
    Write(mAddress, lData & 0x00FF);

    // Now do the XOR.
    mRegisters.mAcc ^= lData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (mRegisters.mAcc & 0x00FF) == 0);

    // Set the Negative flag if bit 7 is set.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// XAA
//
// Non-deterministic Operation of Accumulator, Index Register X, Memory, and Bus Contents.
//
// returns  Flag to coordinate with the address mode to determine if an additional clock cycle is needed.
//--------//
//
uint8_t Cpu6502::XAA()
{
    // Perform operation. The accumulator gets OR'd with some "magic", usually one of 4 values.
    // Chose one arbitraily as it depends on the microprocessor itself, external factors, and contents
    // of the data bus. This is a highly unstable instruction.
    FetchData();
    mRegisters.mAcc = (mRegisters.mAcc | 0x00) & mRegisters.mX & mFetchedData;

    // Set the Zero flag if the result is zero.
    SetOrClearFlag(Flags::Z, (mRegisters.mAcc & 0x00FF) == 0);

    // Set the Negative flag if bit 7 is set.
    SetOrClearFlag(Flags::N, mRegisters.mAcc & Bit(7));

    return DONT_ADD_CLOCK_CYCLE;
}

//--------//
// MISCELLANEOUS

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

#ifdef TEST_CPU
//--------//
// Disassemble
//
// Encodes the instruction back to the assembly.
//
// param[in]  lAddress       Address of instruction.
// returns  Dissassembled instruction, in an easier format to read.
//--------//
//
char * Cpu6502::Disassemble(AddressType lAddress)
{
    DataType    lLowByte;
    DataType    lHighByte;
    char *      lLocation    = cBuffer;
    AddressType lPc          = lAddress;

    DataType    lOpCode      = Read(lAddress++);
    Instruction lInstruction = mOpcodeMatrix[lOpCode];

    // Get the address. This is pretty ugly, but should only be used during debugging.
    // Essentially it's printing out the assembly with some additional addressing mode information
    // in a format that exactly matches the nestest.log file.
    if (lInstruction.mAddressMode == &Cpu6502::Implied)
    {
        if (lInstruction.mInstruction == &Cpu6502::LSR || lInstruction.mInstruction == &Cpu6502::ASL ||
            lInstruction.mInstruction == &Cpu6502::ROL || lInstruction.mInstruction == &Cpu6502::ROR)
        {
            lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                        "%04X  %02X        %s %01X                           ", lPc, lOpCode, lInstruction.mName, 10);
        }
        else
        {
            lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                                  "%04X  %02X        %s                             ", lPc, lOpCode, lInstruction.mName);
        }
    }
    else if (lInstruction.mAddressMode == &Cpu6502::Immediate)
    {
        lLowByte   = Read(lAddress++);
        lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                              "%04X  %02X %02X     %s #$%02X                        ", lPc, lOpCode, lLowByte, lInstruction.mName, lLowByte);
    }
    else if (lInstruction.mAddressMode == &Cpu6502::ZeroPage)
    {
        lLowByte   = Read(lAddress++);
        lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                              "%04X  %02X %02X     %s $%02X = %02X                    ", lPc, lOpCode, lLowByte, lInstruction.mName, lLowByte, Read(lLowByte));
    }
    else if (lInstruction.mAddressMode == &Cpu6502::ZeroPageX)
    {
        lLowByte       = Read(lAddress++);
        DataType lData = lLowByte + mRegisters.mX;
        lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                              "%04X  %02X %02X     %s $%02X,X @ %02X = %02X             ", lPc, lOpCode, lLowByte, lInstruction.mName, lLowByte, lData, Read(lData));
    }
    else if (lInstruction.mAddressMode == &Cpu6502::ZeroPageY)
    {
        lLowByte       = Read(lAddress++);
        DataType lData = lLowByte + mRegisters.mY;
        lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                              "%04X  %02X %02X     %s $%02X,Y @ %02X = %02X             ", lPc, lOpCode, lLowByte, lInstruction.mName, lLowByte, lData, Read(lData));
    }
    else if (lInstruction.mAddressMode == &Cpu6502::Relative)
    {
        lLowByte              = Read(lAddress++);
        AddressType lRelative = lLowByte;
        if (lLowByte & Bit(7))
        {
            lRelative |= 0xFF00;
        }
        lRelative += lAddress;

        lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                              "%04X  %02X %02X     %s $%04X                       ", lPc, lOpCode, lLowByte, lInstruction.mName, lRelative);
    }
    else if (lInstruction.mAddressMode == &Cpu6502::Absolute)
    {
        lLowByte   = Read(lAddress++);
        lHighByte  = Read(lAddress);
        if (lInstruction.mInstruction != &Cpu6502::JMP && lInstruction.mInstruction != &Cpu6502::JSR)
        {
            lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                "%04X  %02X %02X %02X  %s $%04X = %02X                  ", lPc, lOpCode, lLowByte, lHighByte, lInstruction.mName, static_cast<AddressType>(((lHighByte << 8) | lLowByte)), Read(static_cast<AddressType>(((lHighByte << 8) | lLowByte))));
        }
        else
        {
            lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                "%04X  %02X %02X %02X  %s $%04X                       ", lPc, lOpCode, lLowByte, lHighByte, lInstruction.mName, static_cast<AddressType>(((lHighByte << 8) | lLowByte)));
        }
    }
    else if (lInstruction.mAddressMode == &Cpu6502::AbsoluteX)
    {
        lLowByte          = Read(lAddress++);
        lHighByte         = Read(lAddress++);
        AddressType lTemp = (lHighByte << 8) | lLowByte;
        lAddress          = lTemp + mRegisters.mX;

        lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                        "%04X  %02X %02X %02X  %s $%04X,X @ %04X = %02X         ", lPc, lOpCode, lLowByte, lHighByte, lInstruction.mName, lTemp, lAddress, Read(lAddress));
    }
    else if (lInstruction.mAddressMode == &Cpu6502::AbsoluteY)
    {
        lLowByte          = Read(lAddress++);
        lHighByte         = Read(lAddress++);
        AddressType lTemp = (lHighByte << 8) | lLowByte;
        lAddress          = lTemp + mRegisters.mY;

        lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                        "%04X  %02X %02X %02X  %s $%04X,Y @ %04X = %02X         ", lPc, lOpCode, lLowByte, lHighByte, lInstruction.mName, lTemp, lAddress, Read(lAddress));
    }
    else if (lInstruction.mAddressMode == &Cpu6502::Indirect)
    {
        AddressType lIndirect;
        lLowByte              = Read(lAddress++);
        lHighByte             = Read(lAddress);
        AddressType lTemp     = (lHighByte << 8) | lLowByte;

        if (lLowByte & 0x00FF)
        {
            lIndirect = (Read(lTemp & 0xFF00) << 8) | Read(lTemp);
        }
        else
        {
            lIndirect = (Read(lTemp + 1) << 8) | Read(lTemp);
        }

        lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                        "%04X  %02X %02X %02X  %s ($%04X) = %04X              ", lPc, lOpCode, lLowByte, lHighByte, lInstruction.mName, lTemp, lIndirect);
    }
    else if (lInstruction.mAddressMode == &Cpu6502::IndexedIndirect)
    {
        lLowByte              = Read(lAddress++);
        AddressType lTemp     = (lLowByte + mRegisters.mX) & 0xFF;
        AddressType lIndirect = ((Read((lTemp + 1) & 0x00FF)) << 8) | (Read(lTemp));

        lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                        "%04X  %02X %02X     %s ($%02X,X) @ %02X = %04X = %02X    ", lPc, lOpCode, lLowByte, lInstruction.mName, lLowByte, lTemp, lIndirect, Read(lIndirect));
    }
    else if (lInstruction.mAddressMode == &Cpu6502::IndirectIndexed)
    {
        lLowByte              = Read(lAddress++);
        AddressType lTemp     = Read(lLowByte & 0x00FF) | (Read((lLowByte + 1) & 0x00FF) << 8);
        AddressType lIndirect = lTemp + mRegisters.mY;

        lLocation += snprintf(lLocation, sizeof(cBuffer) - (lLocation - cBuffer),
                        "%04X  %02X %02X     %s ($%02X),Y = %04X @ %04X = %02X  ", lPc, lOpCode, lLowByte, lInstruction.mName, lLowByte, lTemp, lIndirect, Read(lIndirect));
    }

    return cBuffer;
}

//--------//
// DumpStack
//
// Dumps the entire contents of what is currently on the stack.
// This is pretty inefficient and should only be used for debugging.
//
// returns  String containing the stack.
//--------//
//
std::string Cpu6502::DumpStack(void)
{
    DataType    lValue;
    static char vBuffer[810];

    char *      lLocation   = vBuffer;
    uint16_t    lNumColumns = 32;
    uint16_t    lNumRows    = cStackSize / lNumColumns;

    lLocation += sprintf(lLocation, "\t\t\t");

    for (AddressType lRow = 0, lIndex = 0; lRow < lNumRows; ++lRow)
    {
        for (AddressType lColumn = 0; lColumn < lNumColumns; ++lColumn, ++lIndex)
        {
            lValue = Read(cStartOfStack + lIndex);
            lLocation += sprintf(lLocation, "%02X ", lValue);
        }
        lLocation += sprintf(lLocation, "\n\t\t\t");
    }

    return vBuffer;
}
#endif
