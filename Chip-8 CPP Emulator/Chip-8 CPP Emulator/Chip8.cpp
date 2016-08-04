#include "Chip8.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

void Chip8::initialize() {
	pc		= 0x200;	//Program counter starts at 0x200
	opcode	= 0;		//Reset current opcode
	I		= 0;		//Reset index register
	sp		= 0;		//Reset stack pointer

	//Clear display
	std::fill_n(gfx, (64*32), 0);
	//Clear stack
	std::fill_n(stack, 16, 0);
	//Clear registers V0-VF
	std::fill_n(V, 16, 0);
	//Clear memory
	std::fill_n(memory, 4096, 0);

	loadFont();
	sound_timer = 0;
	delay_timer = 0;
}

void Chip8::loadRom(std::string path, unsigned pos = 0x200) {
	for (std::ifstream f(path.c_str(), std::ios::binary); f.good(); ) {
		memory[pos++ & 0xFFF] = f.get();
	}
}

void Chip8::loadFont() {
	for (int i = 0; i < sizeof(fontset); i++) {
		memory[0x50 + i] = fontset[i];
	}
}

void Chip8::cycle() {
	//Fetch opcode
	opcode = memory[pc] << 8 | memory[pc + 1];

	//Decode opcode
	switch (opcode & 0xF000) {
		case 0x0000:
			switch (opcode & 0x00FF) {
				case 0x00E0:

					break;
				case 0x00EE:
					sp--;
					pc = stack[sp];
					pc += 2;
					break;

				default:
					std::cout << printf("Unknown opcode: 0x%X\n", opcode);
					std::cin.get();
			}
			break;

		case 0x1000:
			pc = opcode & 0x0FFF;
			break;

		case 0x2000:
			stack[sp] = pc;
			++sp;
			pc = opcode & 0x0FFF;
			break;

		case 0x3000:
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
				pc += 4;
			}
			else {
				pc += 2;
			}
			break;

		case 0x6000:
			V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
			pc += 2;
			break;

		case 0x7000:
			V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
			pc += 2;
			break;

		case 0xA000:
			I = (opcode & 0x0FFF);
			pc += 2;
			break;

		case 0xD000: {
			unsigned short x = V[(opcode & 0x0F00) >> 8];
			unsigned short y = V[(opcode & 0x00F0) >> 4];
			unsigned short height = opcode & 0x000F;
			unsigned short pixel;

			V[0xF] = 0;
			for (int yline = 0; yline < height; yline++) {
				pixel = memory[I + yline];
				for (int xline = 0; xline < 8; xline++) {
					if ((pixel & (0x80 >> xline)) != 0) {
						if (gfx[(x + xline + ((y + yline) * 64))] == 1) {
							V[0xF] = 1;
						}
						gfx[x + xline + ((y + yline) * 64)] ^= 1;
					}
				}
			}
			drawflag = true;
			pc += 2;
		}
			break;

		case 0xF000:
			switch (opcode & 0x00FF) {
				case 0x0007:
					V[(opcode & 0x0F00) >> 8] = delay_timer;
					pc += 2;
				break;

				case 0x0015:
					delay_timer = (opcode & 0x0F00) >> 8;
					pc += 2;
				break;

				case 0x0029:
					I = (0x050 + V[(opcode & 0x0F00) >> 8]);
					pc += 2;
				break;

				case 0x0033:
					memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc += 2;
				break;

				case 0x0065:
					for (int i = 0; i < (opcode & 0x0F00) >> 8; i++) {
						V[i] = memory[I + i];
					}
					pc += 2;
					break;

				default:
					std::cout << printf("Unknown opcode: 0x%X\n", opcode);
					std::cin.get();
			}
			break;
		default:
			std::cout << printf("Unknown opcode: 0x%X\n", opcode);
			std::cin.get();
	}
}