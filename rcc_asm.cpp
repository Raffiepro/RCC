#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string.h>

#include "rcc_dat.hpp"

std::unordered_map<std::string, size_t> tags;

void tokenize(std::vector<std::string>& t, std::string& s) {
    t.clear();
    std::string token;
    for(char& c : s) {
        if(c==' ')  {
            if (!token.empty()) {
                t.push_back(token);
                token.clear();
            }
        } else {
            token.push_back(c);
        }
    }
    if (!token.empty()) {
        t.push_back(token); // Push last token
    }
}

void arg_interp(std::ofstream& to, std::string& arg) {
    if(tags.count(arg)>0) {
        to.put((unsigned char)tags[arg]);
    } else {
        bool hex = false;
        if(arg[0]=='$') {
            arg.erase(arg.begin());
            hex=true;
        }
        printf("ARG: %s\n",arg.c_str());
        if(hex) to.put(std::stoi(arg,0,16));
        else to.put(std::stoi(arg,0,10));
    }
}
void arg_interp16(std::ofstream& to, std::string& arg) {
    if(arg[0]=='[') arg.erase(arg.begin());
    if(arg[arg.length()-1]==']') arg.erase(arg.end()-1);
    
    u16 arg16;
    if(tags.count(arg)>0)
        arg16 = tags[arg];
    else {
        bool hex = false;
        if(arg[0]=='$') {
            arg.erase(arg.begin());
            hex=true;
        }

        printf("ARG16: %s\n",arg.c_str());

        if(hex) arg16 = std::stoi(arg,0,16);
        else arg16 = std::stoi(arg,0,10);
    }
    
    to.put(arg16&0xFF);
    to.put((arg16>>8)&0xFF);
}

void assemble(std::ifstream& f, std::ofstream& to) {
    std::string s;
    std::vector<std::string> t;

    u16 pc=0;
    while(std::getline(f, s)) {
        if(s.length()==0)   continue;
        if(s[0]==':') {
            tags[s] = pc;
            std::cout<<"Added tag "<<s<<" as PC: "<<pc<<'\n';
            continue;
        }
        tokenize(t, s);
        RCC_OP op = get_op(t);
        printf("%02X\n",op);

        pc+=rcc_op_sizes[((op>>4)&0x0F)*4+(op&0x0F)];
    }

    f.clear();
    f.seekg(0, std::ios::beg);

    while(std::getline(f, s)) {
        if(s.length()==0)   continue;
        if(s[0]==':') continue;
        
        tokenize(t, s);
        RCC_OP op = get_op(t);
        
        if(op!=NOP) to.put(op);
        switch(op) {
            case LDn8X: arg_interp(to, t[1]); break;
            case LDn8Y: arg_interp(to, t[1]); break;
            case LDn16pX:   arg_interp16(to, t[1]); break;
            case LDXn16p:   arg_interp16(to, t[1]); break;

            case JMPn16:      arg_interp16(to, t[1]); break;
            case JMPXZn16:    arg_interp16(to, t[1]); break;
            case JMPn16pZXY:  arg_interp16(to, t[1]); break;
        }

        /*if(op==NOP) {
            for(std::string& arg : t) {
                arg_interp(to, arg);
            }
        }*/
    }
}

int main() {
    std::ifstream f("asm.sl");
    std::ofstream to("compiled.sl");
    
    assemble(f, to);
    f.close();
    to.close();
}