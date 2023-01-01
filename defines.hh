#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include "types.h"

const bool islittleendian() {
    union {
        short int a;
        char b[2];
    } endiantest;

    endiantest.a = 0x1234;

    return endiantest.b[0] == 0x34;
}

enum class Endian { big, little };

const Endian NATIVE = islittleendian() ? Endian::little : Endian::big;

template <typename T>
void SwapVal(T& val) {
    union U
    {
        T raw;
        std::array<u8, sizeof(T)> buf;
    } src, dst;
    src.raw = val;
    std::reverse_copy(src.buf.begin(), src.buf.end(), dst.buf.begin());
    val = dst.raw;
}

struct StreamDel final {
    template <typename T>
    void operator()(std::basic_ifstream<T>* stream) {
        if (stream->is_open())
            stream->close();
        delete stream;
    }
    template <typename T>
    void operator()(std::basic_ofstream<T>* stream) {
        if (stream->is_open())
            stream->close();
        delete stream;
    }
};

template <typename T>
std::array<u8, sizeof(T)> GetBytes(T val) {
    union U
    {
        std::array<u8, sizeof(T)> ret;
        T raw;
    } buf;
    buf.raw = val;
    return buf.ret;
}