#include "chip8.h"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <random>
const unsigned int FONTSET_SIZE = 80;

uint8_t fontset[FONTSET_SIZE] = 80;