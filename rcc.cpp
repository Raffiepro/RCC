#include <stdio.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;

enum RCC_OP : u8 {
    NOP = 0x00,
    NAND = 0x01,
    SWAP = 0x02,
    SYSCALL = 0x03,

    LDn8X = 0x10,
    LDn8Y = 0x11,
    LDn16pX = 0x12,
    LDXn16p = 0x13,

    ADD = 0x20,
    SUB = 0x21,
    INC = 0x22,
    DEC = 0x23,
    
    JMPn16 = 0x30,
    JMPXZn16 = 0x31,
    JMPXY = 0x32,
    JMPn16pZXY = 0x33
};
u8 rcc_op_sizes[] = {1,1,1,1,
                     2,2,3,3,
                     1,1,1,1,
                     3,3,1,3
                    };

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
        switch (op)
        {
            case NOP:   break;
            case NAND:  x=!(x&y);   break;
            case SWAP: {u8 z=x;x=y;y=z;} break;
            case SYSCALL:   break; // DOESN'T DO ANYTHING YET

            case LDn8X: x = mem[pc+1];  break;
            case LDn8Y: y = mem[pc+1];  break;
            case LDn16pX:   x = mem[*((u16*)&mem[pc+1])];  break;
            case LDXn16p:   mem[*((u16*)&mem[pc+1])] = x;  break;

            case ADD:   x+=y;   break;
            case SUB:   x-=y;   break;
            case INC:   x+=1;   break;
            case DEC:   x-=1;   break;

            case JMPn16:    pc=*((u16*)&mem[pc+1]); break;
            case JMPXZn16:  if(x==0) pc=*((u16*)&mem[pc+1]);    break;
            case JMPXY: pc=*((u16*)&x); break;
            case JMPn16pZXY:    if(mem[*((u16*)&mem[pc+1])]==0) pc=*((u16*)&x); break;
        }
        printf("%02X\n",op);
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