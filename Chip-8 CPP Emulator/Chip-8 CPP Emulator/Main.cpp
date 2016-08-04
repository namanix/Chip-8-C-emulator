#include "Chip8.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>


void draw(Chip8* cpu) {
	system("cls");
	int charCounter = 0;
	for (int i = 0; i < sizeof(cpu->gfx); i++) {
		if (cpu->gfx[i] < 1) {
			std::cout << " ";
		}
		else {
			std::cout << "#";
		}
		charCounter++;
		if (charCounter == 64) {
			std::cout << "\n";
			charCounter = 0;
		}
	}
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	cpu->drawflag = false;
}

int main(int argc, char **argv)
{
	Chip8* cpu = new Chip8();
	cpu->initialize();
	cpu->loadRom("Games/PONG2", 0x200);

	for (;;) {
		cpu->cycle();

		if (cpu->drawflag) {
			draw(cpu);
		}
	}
	
	return 0;
}

