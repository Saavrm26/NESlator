#include "CPU6502.h"
#include "Bus.h"
#include <cstdint>

CPU6502::CPU6502() {

	using a = CPU6502;
	lookup  = {
        {"BRK", &a::BRK, &a::IMM, 7}, {"ORA", &a::ORA, &a::IZX, 6}, {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 3}, {"ORA", &a::ORA, &a::ZP0, 3},
        {"ASL", &a::ASL, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5}, {"PHP", &a::PHP, &a::IMP, 3},
        {"ORA", &a::ORA, &a::IMM, 2}, {"ASL", &a::ASL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ABS, 4}, {"ASL", &a::ASL, &a::ABS, 6},
        {"???", &a::XXX, &a::IMP, 6}, {"BPL", &a::BPL, &a::REL, 2}, {"ORA", &a::ORA, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4},
        {"ORA", &a::ORA, &a::ZPX, 4}, {"ASL", &a::ASL, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"CLC", &a::CLC, &a::IMP, 2}, {"ORA", &a::ORA, &a::ABY, 4}, {"???", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ABX, 4},
        {"ASL", &a::ASL, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7}, {"JSR", &a::JSR, &a::ABS, 6},
        {"AND", &a::AND, &a::IZX, 6}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"BIT", &a::BIT, &a::ZP0, 3}, {"AND", &a::AND, &a::ZP0, 3}, {"ROL", &a::ROL, &a::ZP0, 5},
        {"???", &a::XXX, &a::IMP, 5}, {"PLP", &a::PLP, &a::IMP, 4}, {"AND", &a::AND, &a::IMM, 2},
        {"ROL", &a::ROL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2}, {"BIT", &a::BIT, &a::ABS, 4},
        {"AND", &a::AND, &a::ABS, 4}, {"ROL", &a::ROL, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"BMI", &a::BMI, &a::REL, 2}, {"AND", &a::AND, &a::IZY, 5}, {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4}, {"AND", &a::AND, &a::ZPX, 4},
        {"ROL", &a::ROL, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6}, {"SEC", &a::SEC, &a::IMP, 2},
        {"AND", &a::AND, &a::ABY, 4}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4}, {"AND", &a::AND, &a::ABX, 4}, {"ROL", &a::ROL, &a::ABX, 7},
        {"???", &a::XXX, &a::IMP, 7}, {"RTI", &a::RTI, &a::IMP, 6}, {"EOR", &a::EOR, &a::IZX, 6},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 3},
        {"EOR", &a::EOR, &a::ZP0, 3}, {"LSR", &a::LSR, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
        {"PHA", &a::PHA, &a::IMP, 3}, {"EOR", &a::EOR, &a::IMM, 2}, {"LSR", &a::LSR, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 2}, {"JMP", &a::JMP, &a::ABS, 3}, {"EOR", &a::EOR, &a::ABS, 4},
        {"LSR", &a::LSR, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6}, {"BVC", &a::BVC, &a::REL, 2},
        {"EOR", &a::EOR, &a::IZY, 5}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4}, {"EOR", &a::EOR, &a::ZPX, 4}, {"LSR", &a::LSR, &a::ZPX, 6},
        {"???", &a::XXX, &a::IMP, 6}, {"CLI", &a::CLI, &a::IMP, 2}, {"EOR", &a::EOR, &a::ABY, 4},
        {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4},
        {"EOR", &a::EOR, &a::ABX, 4}, {"LSR", &a::LSR, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
        {"RTS", &a::RTS, &a::IMP, 6}, {"ADC", &a::ADC, &a::IZX, 6}, {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 3}, {"ADC", &a::ADC, &a::ZP0, 3},
        {"ROR", &a::ROR, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5}, {"PLA", &a::PLA, &a::IMP, 4},
        {"ADC", &a::ADC, &a::IMM, 2}, {"ROR", &a::ROR, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
        {"JMP", &a::JMP, &a::IND, 5}, {"ADC", &a::ADC, &a::ABS, 4}, {"ROR", &a::ROR, &a::ABS, 6},
        {"???", &a::XXX, &a::IMP, 6}, {"BVS", &a::BVS, &a::REL, 2}, {"ADC", &a::ADC, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4},
        {"ADC", &a::ADC, &a::ZPX, 4}, {"ROR", &a::ROR, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"SEI", &a::SEI, &a::IMP, 2}, {"ADC", &a::ADC, &a::ABY, 4}, {"???", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4}, {"ADC", &a::ADC, &a::ABX, 4},
        {"ROR", &a::ROR, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 2},
        {"STA", &a::STA, &a::IZX, 6}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 6},
        {"STY", &a::STY, &a::ZP0, 3}, {"STA", &a::STA, &a::ZP0, 3}, {"STX", &a::STX, &a::ZP0, 3},
        {"???", &a::XXX, &a::IMP, 3}, {"DEY", &a::DEY, &a::IMP, 2}, {"???", &a::NOP, &a::IMP, 2},
        {"TXA", &a::TXA, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2}, {"STY", &a::STY, &a::ABS, 4},
        {"STA", &a::STA, &a::ABS, 4}, {"STX", &a::STX, &a::ABS, 4}, {"???", &a::XXX, &a::IMP, 4},
        {"BCC", &a::BCC, &a::REL, 2}, {"STA", &a::STA, &a::IZY, 6}, {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 6}, {"STY", &a::STY, &a::ZPX, 4}, {"STA", &a::STA, &a::ZPX, 4},
        {"STX", &a::STX, &a::ZPY, 4}, {"???", &a::XXX, &a::IMP, 4}, {"TYA", &a::TYA, &a::IMP, 2},
        {"STA", &a::STA, &a::ABY, 5}, {"TXS", &a::TXS, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5},
        {"???", &a::NOP, &a::IMP, 5}, {"STA", &a::STA, &a::ABX, 5}, {"???", &a::XXX, &a::IMP, 5},
        {"???", &a::XXX, &a::IMP, 5}, {"LDY", &a::LDY, &a::IMM, 2}, {"LDA", &a::LDA, &a::IZX, 6},
        {"LDX", &a::LDX, &a::IMM, 2}, {"???", &a::XXX, &a::IMP, 6}, {"LDY", &a::LDY, &a::ZP0, 3},
        {"LDA", &a::LDA, &a::ZP0, 3}, {"LDX", &a::LDX, &a::ZP0, 3}, {"???", &a::XXX, &a::IMP, 3},
        {"TAY", &a::TAY, &a::IMP, 2}, {"LDA", &a::LDA, &a::IMM, 2}, {"TAX", &a::TAX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 2}, {"LDY", &a::LDY, &a::ABS, 4}, {"LDA", &a::LDA, &a::ABS, 4},
        {"LDX", &a::LDX, &a::ABS, 4}, {"???", &a::XXX, &a::IMP, 4}, {"BCS", &a::BCS, &a::REL, 2},
        {"LDA", &a::LDA, &a::IZY, 5}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5},
        {"LDY", &a::LDY, &a::ZPX, 4}, {"LDA", &a::LDA, &a::ZPX, 4}, {"LDX", &a::LDX, &a::ZPY, 4},
        {"???", &a::XXX, &a::IMP, 4}, {"CLV", &a::CLV, &a::IMP, 2}, {"LDA", &a::LDA, &a::ABY, 4},
        {"TSX", &a::TSX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 4}, {"LDY", &a::LDY, &a::ABX, 4},
        {"LDA", &a::LDA, &a::ABX, 4}, {"LDX", &a::LDX, &a::ABY, 4}, {"???", &a::XXX, &a::IMP, 4},
        {"CPY", &a::CPY, &a::IMM, 2}, {"CMP", &a::CMP, &a::IZX, 6}, {"???", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8}, {"CPY", &a::CPY, &a::ZP0, 3}, {"CMP", &a::CMP, &a::ZP0, 3},
        {"DEC", &a::DEC, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5}, {"INY", &a::INY, &a::IMP, 2},
        {"CMP", &a::CMP, &a::IMM, 2}, {"DEX", &a::DEX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
        {"CPY", &a::CPY, &a::ABS, 4}, {"CMP", &a::CMP, &a::ABS, 4}, {"DEC", &a::DEC, &a::ABS, 6},
        {"???", &a::XXX, &a::IMP, 6}, {"BNE", &a::BNE, &a::REL, 2}, {"CMP", &a::CMP, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4},
        {"CMP", &a::CMP, &a::ZPX, 4}, {"DEC", &a::DEC, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"CLD", &a::CLD, &a::IMP, 2}, {"CMP", &a::CMP, &a::ABY, 4}, {"NOP", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4}, {"CMP", &a::CMP, &a::ABX, 4},
        {"DEC", &a::DEC, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7}, {"CPX", &a::CPX, &a::IMM, 2},
        {"SBC", &a::SBC, &a::IZX, 6}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"CPX", &a::CPX, &a::ZP0, 3}, {"SBC", &a::SBC, &a::ZP0, 3}, {"INC", &a::INC, &a::ZP0, 5},
        {"???", &a::XXX, &a::IMP, 5}, {"INX", &a::INX, &a::IMP, 2}, {"SBC", &a::SBC, &a::IMM, 2},
        {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::SBC, &a::IMP, 2}, {"CPX", &a::CPX, &a::ABS, 4},
        {"SBC", &a::SBC, &a::ABS, 4}, {"INC", &a::INC, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"BEQ", &a::BEQ, &a::REL, 2}, {"SBC", &a::SBC, &a::IZY, 5}, {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4}, {"SBC", &a::SBC, &a::ZPX, 4},
        {"INC", &a::INC, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6}, {"SED", &a::SED, &a::IMP, 2},
        {"SBC", &a::SBC, &a::ABY, 4}, {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4}, {"SBC", &a::SBC, &a::ABX, 4}, {"INC", &a::INC, &a::ABX, 7},
        {"???", &a::XXX, &a::IMP, 7},
    };
}

CPU6502::~CPU6502() {}

uint8_t CPU6502::read(uint16_t addr) { return bus->read(addr, false); }

void CPU6502::write(uint16_t addr, uint8_t data) { return bus->write(addr, data); }

void CPU6502::clock() {
	if (cycles == 0) {
		opcode = read(pc);
		pc++;

		cycles = lookup[opcode].cycles;

		uint8_t extra_cycles1 = (this->*lookup[opcode].addrmode)();
		uint8_t extra_cycles2 = (this->*lookup[opcode].opearte)();

		cycles += (extra_cycles1 + extra_cycles2);
	}
	cycles--;
}

uint8_t CPU6502::GetFlag(FLAGS6502 f) { return (status & f) ? 1 : 0; }
void    CPU6502::SetFlag(FLAGS6502 f, bool v) {
    if (v) {
        status |= f;
    } else {
        status &= ~f;
    }
}

// Address mode: Implied
uint8_t CPU6502::IMP() {
	fetched = a;
	return 0;
}

// Address Mode : Immediate
uint8_t CPU6502::IMM() {
	addr_abs = pc++;
	return 0;
}

// Address Mode: Zero Page
uint8_t CPU6502::ZP0() {
	addr_abs = read(pc);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

// Address Mode: Zero Page with X Offset
uint8_t CPU6502::ZPX() {
	addr_abs = (read(pc) + x);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

// Address Mode: Zero Page with Y Offset
uint8_t CPU6502::ZPY() {
	addr_abs = (read(pc) + y);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

// Address Mode: Absolute
uint8_t CPU6502::ABS() {
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;
	addr_abs = (hi << 8) | lo;
	return 0;
}

// Address Mode : Absolute with x offset
uint8_t CPU6502::ABX() {
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;
	addr_abs = (hi << 8) | lo;
	addr_abs += x;
	if ((addr_abs & 0xFF00) != (hi << 8)) {
		return 1;
	} else {
		return 0;
	}
}

// Address Mode : Absolute with y offset
uint8_t CPU6502::ABY() {
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;
	addr_abs = (hi << 8) | lo;
	addr_abs += x;
	if ((addr_abs & 0xFF00) != (hi << 8)) {
		return 1;
	} else {
		return 0;
	}
}

// Address Mode : Indirect
uint8_t CPU6502::IND() {
	uint16_t ptr_lo = read(pc);
	pc++;
	uint16_t ptr_hi = read(pc);
	pc++;
	uint16_t ptr = (ptr_hi << 8) | ptr_lo;
	if (ptr_lo == 0xFF) {
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr);
	} else {
		addr_abs = (read(ptr + 1) << 8) | read(ptr);
	}
	return 0;
}

// Address Mode: Indirect with X offset
uint8_t CPU6502::IZX() {
	uint16_t t = read(pc);
	pc++;
	uint16_t lo = read(uint16_t(t + uint16_t(x)) & 0x00FF);
	uint16_t hi = read(uint16_t(t + uint16_t(x) + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	return 0;
}

// Address Mode: Indirect with Y offset
uint8_t CPU6502::IZY() {
	uint16_t t = read(pc);
	pc++;
	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (hi << 8)) {
		return 1;
	} else {
		return 0;
	}
}

uint8_t CPU6502::REL() {
	addr_rel = read(pc);
	pc++;
	if (addr_rel & 0x80) {
		addr_rel |= 0xFF00;
	}
	return 0;
}

uint8_t CPU6502::fetch() {
	if (lookup[opcode].addrmode != &CPU6502::IMP) {
		fetched = read(addr_abs);
	}
	return fetched;
}

// Instructions

// Addition with carry
// Function:    A = A + M + C
// Flags Out:   C, V, N, Z
uint8_t CPU6502::ADC() {
	fetch();

	temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);

	SetFlag(C, temp > 255);
	SetFlag(N, temp & 0x80);
	SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);
	SetFlag(Z, (temp & 0x00FF));

	a = temp & 0x00FF;
	return 1;
}

// Subtract with carry
// Function:    A = A - M - (1 - C) or A = A + ~M + C
// Flags Out:   C, V, N, Z
uint8_t CPU6502::SBC() {
	fetch();

	uint16_t value = ((uint16_t)fetched) ^ 0x00FF; // ones complement

	temp = (uint16_t)a + value + (uint16_t)GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, ((temp & 0x00FF) == 0));
	SetFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
	SetFlag(N, temp & 0x0080);
	a = temp & 0x00FF;
	return 1;
}

// Instruction: Bitwise Logic AND
// Function:    A = A & M
// Flags Out:   N, Z
uint8_t CPU6502::AND() {
	fetch();
	a = a & fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}

// Instruction: Arithmetic Shift Left
// Function:    A = C <- (A << 1) <- 0
// Flags Out:   N, Z, C
uint8_t CPU6502::ASL() {
	fetch();
	temp = (uint16_t)fetched << 1;
	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(N, temp & 0x80);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	if (lookup[opcode].addrmode == &CPU6502::IMP) {
		a = temp & 0x00FF;
	} else {
		write(addr_abs, temp);
	}
	return 0;
}

// Instruction: Branch if Carry Clear
// Function:    if(C == 0) pc = address
uint8_t CPU6502::BCC() {
	if (GetFlag(C) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Carry Set
// Function:    if(C == 1) pc = address
uint8_t CPU6502::BCS() {
	if (GetFlag(C) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Equal
// Function:    if(Z == 1) pc = address
uint8_t CPU6502::BEQ() {
	if (GetFlag(Z) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t CPU6502::BIT() {
	fetch();
	temp = a & fetched;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, fetched & (1 << 7));
	SetFlag(V, fetched & (1 << 6));
	return 0;
}

// Instruction: Branch if Negative
// Function:    if(N == 1) pc = address
uint8_t CPU6502::BMI() {
	if (GetFlag(N) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Not Equal
// Function:    if(Z == 0) pc = address
uint8_t CPU6502::BNE() {
	if (GetFlag(Z) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Positive
// Function:    if(N == 0) pc = address
uint8_t CPU6502::BPL() {
	if (GetFlag(N) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Break
// Function:    Program Sourced Interrupt
uint8_t CPU6502::BRK() {
	pc++;

	SetFlag(I, 1);
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	SetFlag(B, 1);
	write(0x0100 + stkp, status);
	stkp--;
	SetFlag(B, 0);

	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}

// Instruction: Branch if Overflow Clear
// Function:    if(V == 0) pc = address
uint8_t CPU6502::BVC() {
	if (GetFlag(V) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Overflow Set
// Function:    if(V == 1) pc = address
uint8_t CPU6502::BVS() {
	if (GetFlag(V) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Clear Carry Flag
// Function:    C = 0
uint8_t CPU6502::CLC() {
	SetFlag(C, false);
	return 0;
}

// Instruction: Clear Decimal Flag
// Function:    D = 0
uint8_t CPU6502::CLD() {
	SetFlag(D, false);
	return 0;
}

// Instruction: Disable Interrupts / Clear Interrupt Flag
// Function:    I = 0
uint8_t CPU6502::CLI() {
	SetFlag(I, false);
	return 0;
}

// Instruction: Clear Overflow Flag
// Function:    V = 0
uint8_t CPU6502::CLV() {
	SetFlag(V, false);
	return 0;
}

// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t CPU6502::TAX() {
	x = a;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t CPU6502::TAY() {
	y = a;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}

// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t CPU6502::TSX() {
	x = stkp;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t CPU6502::TXA() {
	a = x;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}

// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t CPU6502::TXS() {
	stkp = x;
	return 0;
}

// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t CPU6502::TYA() {
	a = y;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}

// This function captures illegal opcodes
uint8_t CPU6502::XXX() { return 0; }
