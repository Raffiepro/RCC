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
        to.put(std::stoi(arg,0,16));
    }
}

unsigned char arg_interp(std::string& arg) {
    if(tags.count(arg)>0) {
        return (unsigned char)tags[arg];
    } else {
        return (unsigned char)std::stoi(arg,0,16);
    }
}

void assemble(std::ifstream& f, std::ofstream& to) {
    std::string s;
    std::vector<std::string> t;

    while(std::getline(f, s)) {
        if(s.length()==0)   continue;
        
        tokenize(t, s);

        RCC_OP op = get_op(t);
        printf("%02X\n",op);

        if(op==NOP) {
            for(std::string& arg : t) {
                arg_interp(to, arg);
            }
        }
    }
}

int main() {
    std::ifstream f("asm.sl");
    std::ofstream to("compiled.sl");
    
    assemble(f, to);
    f.close();
    to.close();
}