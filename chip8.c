#include <stdint.h>
#include <stdbool.h>

// Remember that a non-explicitly-initialized static global is zero-initialized.
static uint8_t memory[0xFFF];

// Registers
static uint8_t V[0xF];
static uint16_t I;

// Special registers
// delay
static uint8_t D;
// sound
static uint8_t S;

// Pseudo-registers
static uint16_t PC;
static uint8_t SP;

static uint16_t stack[0xF];

static uint16_t screen[64*32];

static bool keypad[0xF];

void chip8_init() {
    PC = 0x200;
}

static uint16_t get_next_instruction() {
    uint16_t ret = (memory[PC] << 8); PC++;
    ret += memory[PC]; PC++;
    return ret;
}

static void run_instruction(uint16_t instruction) {
    switch (instruction >> 12) {
    case 0x0:
    if (instruction & 0x000F == 0x0) {
        // CLS
        for (int i = 0; i < 64*32; i++) screen[i] = 0;
    } else if (instruction & 0x000F == 0xE) {
        // RET
        PC = stack[SP]; SP--;
    }
    break;

    }
}

void chip8_do_cycle() {
    // fetch instruction
    uint16_t instruction = get_next_instruction();
    // execute instruction
    run_instruction(instruction);
}