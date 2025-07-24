#pragma once
#include <vector>
#include <string>

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

RCC_OP get_op(std::vector<std::string>& t) {
    if(t[0]=="nop")    return NOP;
    if(t[0]=="nand")    return NAND;
    if(t[0]=="swap")    return SWAP;
    if(t[0]=="syscall")    return SYSCALL;

    if(t[0]=="ld") {
        if(t[2]=="y")   return LDn8Y;
        if(t[1]=="x")   return LDXn16p;
        if(t[1][0]=='[' && t[1][t[1].length()-1]==']')   return LDn16pX;
        return LDn8X;
    }

    if(t[0]=="add") return ADD;
    if(t[0]=="sub") return SUB;
    if(t[0]=="inc") return INC;
    if(t[0]=="dec") return DEC;

    if(t[0]=="jmp") {
        if(t.size()==2) {
            if(t[1][0]=='x')    return JMPXY;
            else return JMPn16;
        } else {
            if(t[1][0]=='x')    return JMPXZn16;
            else    return JMPn16pZXY;
        }
    }
    return NOP;
}