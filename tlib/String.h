#pragma once

#include <tlib/stddef.h>

class String {
private:
    size_t size;
    char* data;


public: 
    String(const char* s);
};
