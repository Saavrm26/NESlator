#include "Bus.h"

Bus::Bus() { cpu.ConnectBus(this); }

Bus::~Bus() {}

void Bus::write(uint16_t addr, uint8_t data) {
	if (0x0000 <= addr && addr <= 0xFFFF)
		ram[addr] = data;
}
uint8_t Bus::read(uint16_t addr, bool bReadOnly) {
	if (0x0000 <= addr && addr <= 0xFFFF)
		return ram[addr];
	return 0x0000;
}
