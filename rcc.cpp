#include <stdio.h>
#include <string.h>

#include "rcc_dat.hpp"

struct rcc_cpu {
    u8 x=0,y=0;
    u16 pc = 0;
    
    u8 mem[1024];

    void readMem(const char* f) {
        memset(mem,0,1024);
        FILE *file;
        file = fopen(f, "rb");
        fread(mem, 1, 1024, file);
        fclose(file);
    }

    void execute() {
        u8& op = mem[pc];
        u16 n16 = (((u16)mem[pc+1])<<8) | (u16)mem[pc+2];
        u16 xy = (((u16)x)<<8) | (u16)y;

        bool jumped=false;
        switch (op)
        {
            case NOP:   break;
            case NAND:  x = ~(x&y);   break;
            case SWAP: {u8 z=x;x=y;y=z;} break;
            case SYSCALL:   break; // DOESN'T DO ANYTHING YET

            case LDn8X: x = mem[pc+1];  break;
            case LDn8Y: y = mem[pc+1];  break;
            case LDn16pX:   x = mem[n16];  break;
            case LDXn16p:   mem[n16] = x;  break;

            case ADD:   x+=y;   break;
            case SUB:   x-=y;   break;
            case INC:   x+=1;   break;
            case DEC:   x-=1;   break;

            case JMPn16:    pc = n16;   jumped = true; break;
            case JMPXZn16:  if(x==0) {pc = n16;  jumped = true;} break;
            case JMPXY: pc = xy;    jumped = true;   break;
            case JMPn16pZXY:    if(mem[n16]==0) {pc = xy;   jumped = true;}   break;
        }
        printf("%02X\n",op);
        if(!jumped)
            pc+=rcc_op_sizes[((op>>4)&0x0F)*4+(op&0x0F)];
    }

    void info() {
        printf("PC: %04X\nX: %02X, Y: %02X\n",pc,x,y);
    }
};

int main() {
    rcc_cpu cpu;
    cpu.readMem("prg.rcc");
    while(true) {
        cpu.execute();
        cpu.info();
        getchar();
    }
    return 0;
}