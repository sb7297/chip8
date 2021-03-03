#include <stdio.h>
#include <stdint.h>

int main() {
    chip8_init();
    
    FILE* fp = fopen("test_opcode.ch8", "rb");
    uint8_t buf[0xDFF] = {0};
    fread(buf, sizeof(buf), 1, fp);
}