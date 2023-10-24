#ifndef FTHELPER_H
#define FTHELPER_H
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

struct Worked
{
        char            dxcall[16];
        char            mode[8];
#ifdef __WXMSW__
        unsigned long   freq;
#else
        uint64_t        freq;
#endif
};
#endif
