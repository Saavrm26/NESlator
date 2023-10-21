#pragma once
#include "CPU6502.h"
#include <array>
#include <cstdint>

class Bus {
  public:
	Bus();
	~Bus();

  public:
	void    write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr, bool bReadOnly = false);

  public: // devices
	CPU6502                        cpu;
	std::array<uint8_t, 64 * 1024> ram;
};
