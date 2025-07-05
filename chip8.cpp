#include "chip8.h"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <random>
const unsigned int FONTSET_SIZE = 80;
const unsigned int STARTING_ROM = 0x200;

uint8_t fontset[FONTSET_SIZE] =
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

void Chip8::LoadROM(char const* filename){

	std::ifstream file(filename,std::ios::binary | std::ios::ate);

	if (file.is_open()){
		std::streampos size = file.tellg();
		char* buffer = new char[size];
		file.seekg(0,std::ios::beg);
		file.read(buffer,size);
		file.close();

		for(long i = 0; i < size; ++i){
			memory[STARTING_ROM + i] = buffer[i];
		}
		delete[] buffer;
	}
}

Chip8::Chip8(){
	pc = STARTING_ROM;
	index = 0;
	sp = 0;
	delayTimer = 0;
	soundTimer = 0;
	opcode = 0;

	std::memset(memory,0,sizeof(memory));
	std::memset(registers, 0 , sizeof(registers));
	std::memset(stack, 0, sizeof(stack));
	std::memset(keypad, 0, sizeof(keypad));
    std::memset(video, 0, sizeof(video));
}

void Chip8::OP_00E0(){
	// Clear the display
	std::memset(video,0,sizeof(video));
}

void Chip8::OP_00EE(){
	sp--;
	pc = stack[sp];
}

void Chip8::OP_1nnn(){
	u_int16_t nnn = opcode & 0x0FFFu;
	pc = nnn;
}
void Chip8::OP_2nnn(){
	u_int16_t nnn = opcode & 0x0FFFu;
	stack[sp] = pc;
	++sp;
	pc = nnn;
}
void Chip8::OP_3xkk(){
	u_int8_t kk = opcode & 0x00FFu;
	u_int8_t Vx = (opcode & 0x0F00u) >> 8u;
	if(registers[Vx] == kk) pc+=2;
}
void Chip8::OP_4xkk(){
	u_int8_t kk = opcode & 0x00FFu;
	u_int8_t Vx = (opcode &0x0F00u) >> 8u;
	if(registers[Vx] != kk) pc+=2;
}