#pragma once
#include <string>

class Chip8 {
	/**
	*	Variable declarations
	*/
	//The opcode is 2 bytes so a short is enough
private:
	unsigned short opcode;

	//Chip-8 has 4k memory
	/*
		Chip-8 Memory map
		0x000-0x1FF - Chip-8 interpreter (contains font set in emu)
		0x050-0x0A0 - Used for the build in 4x5 pixel font set (0-F)
		0x200-0xFFF - Program ROM and work RAM
	*/
	unsigned char memory[4096];

	//Chip-8 has 15 8bit registers named V0, V1 up to VE
	unsigned char V[16];

	//Chip-8 has a index register and a program counter. Value range: 0x000 - 0xFFF
	unsigned short I;
	unsigned short pc;

	//Chip-8 has 2 timer registers that count at 60hz. When set above 0 they will count down to 0
	unsigned char delay_timer;
	unsigned char sound_timer;

	//VAGUE!!!!!!!
	unsigned short stack[16]; //16 stacks... Why ???
	unsigned short sp; //STACK POINTER??
	
	//Chip-8 has a HEX based keypad 0x0 - 0xF
	unsigned char key[16];

	//Chip-8 fontset
	int fontset[5 * 16] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	void Chip8::loadFont();

public:
	void Chip8::initialize();
	void Chip8::loadRom(std::string path, unsigned pos);
	void Chip8::cycle();

	//Chip-8 has 62x32 pixels in state 1 or 0
	unsigned char gfx[64 * 32];

	//Let the renderer know when to draw.
	bool drawflag;
};


