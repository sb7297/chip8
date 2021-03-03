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

// Return second nibble in a 4 nibble number
static inline uint8_t snib(uint16_t instr) {
    return (0x0F & (instr >> 8));
}

// Return third nibble in a 4 nibble number
static inline uint8_t tnib(uint16_t instr) {
    return (0x00F & (instr >> 4));
}

static uint16_t get_next_instruction() {
    uint16_t ret = (memory[PC] << 8); PC++;
    ret += memory[PC]; PC++;
    return ret;
}

static void run_instruction(uint16_t instr) {
    int res;
    switch (instr >> 12) {
    case 0x0:
    if (instr & 0x000F == 0x0) {
        // CLS
        for (int i = 0; i < 64*32; i++) screen[i] = 0;
    } else if (instr & 0x000F == 0xE) {
        // RET
        PC = stack[SP]; SP--;
    }
    break;
    case 0x1:
    // JP
    PC = instr & 0x0FFF;
    break;
    case 0x2:
    // CALL
    SP++;
    stack[SP] = PC;
    PC = instr & 0x0FFF;
    break;
    case 0x3:
    // SE Vx, 0xnn
    if (V[snib(instr)] == (instr & 0x00FF)) PC += 2;
    else PC += 1;
    break;
    case 0x4:
    // SNE
    if (V[snib(instr)] != (instr & 0x00FF)) PC += 2;
    else PC += 1;
    break;
    case 0x5:
    // SE Vx, Vy
    if (V[snib(instr)] == V[tnib(instr)]) PC += 2;
    else PC += 1;
    break;
    case 0x6:
    // LD
    V[snib(instr)] = instr & 0x00FF;
    PC += 1;
    break;
    case 0x7:
    // ADD
    V[snib(instr)] = V[snib(instr)] + (0x00FF & instr);
    PC += 1;
    break;
    case 0x8:
    switch (instr & 0x000F) {
        case 0x0:
        // LD Vx, Vy
        V[snib(instr)] = V[tnib(instr)];
        PC += 1;
        break;
        case 0x1:
        // OR Vx, Vy
        V[snib(instr)] = V[snib(instr)] | V[tnib(instr)];
        PC += 1;
        break;
        case 0x2:
        // AND Vx, Vy
        V[snib(instr)] = V[snib(instr)] & V[tnib(instr)];
        PC += 1;
        break;
        case 0x3:
        // XOR Vx, Vy
        V[snib(instr)] = V[snib(instr)] ^ V[tnib(instr)];
        PC += 1;
        break;
        case 0x4:
        // ADD Vx, Vy
        res = V[snib(instr)] + V[tnib(instr)];
        if (res != ((uint8_t) res)) V[0xF] = 1;
        else V[0xF] = 0;
        V[snib(instr)] = (uint8_t) res;
        PC += 1;
        break;
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