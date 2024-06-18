#pragma once

#include <string>   

class ReadHead
{
    const std::string& str;
    unsigned long pos;
public:
    ReadHead(const std::string& str) : str(str), pos(0) {}

    char peek() const { return str[pos]; }

    void advance() { ++pos; }
    void advance(unsigned long n) { pos += n; }
    void retreat() { --pos; }
    void retreat(unsigned long n) { pos -= n; }

    std::string read(unsigned long n) { return str.substr(pos, n); }
};