#pragma once

#include "defines.hh"

class EndianWriter final : public std::ofstream {
    public:
    EndianWriter(Endian endian = NATIVE) : std::ofstream(), endian(endian) {}
    explicit EndianWriter(const char* filename, std::ios::openmode mode = std::ios::out, 
    Endian endian = NATIVE) : std::ofstream(filename, mode), endian(endian) {}
    explicit EndianWriter(const std::string& filename, std::ios_base::openmode mode = ios_base::out,
    Endian endian = NATIVE) : std::ofstream(filename, mode), endian(endian) {}
    EndianWriter(EndianWriter&& rhs) : std::ofstream(std::move(rhs)) {}
    EndianWriter(const EndianWriter&) = delete;
    EndianWriter& operator=(const EndianWriter&) = delete;

    Endian endian;

    template <typename T>
    void WriteType(T val) {
        if (endian != NATIVE)
            SwapVal(val);
        std::array<u8, sizeof(T)> buffer = GetBytes(val);
        write((char*)buffer.data(), sizeof(T));
    }
};

class UniqueWriter final : public std::unique_ptr<EndianWriter, StreamDel> {
    private:
    using Base = std::unique_ptr<EndianWriter, StreamDel>;
    public:
    constexpr UniqueWriter() : Base() {}
    constexpr UniqueWriter(std::nullptr_t np) : Base(np) {}
    explicit UniqueWriter(pointer ptr) : Base(ptr) {}
    UniqueWriter(UniqueWriter&& rhs) : Base(std::move(rhs)) {}
    ~UniqueWriter() {}

    explicit UniqueWriter(const std::string& path, Endian endian = NATIVE,
    std::ios::openmode mode = EndianWriter::binary) : UniqueWriter(new EndianWriter{path, mode, endian}) {}
};