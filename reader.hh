#pragma once

#include "defines.hh"

class EndianReader final: public std::ifstream {
    public:
    EndianReader(Endian endian = NATIVE): std::ifstream(), endian(endian) {}
    explicit EndianReader(const char* filename, std::ios::openmode mode = std::ios::in, 
    Endian endian = NATIVE) 
    : std::ifstream(filename, mode), endian(endian) {}
    explicit EndianReader(const std::string& filename, std::ios_base::openmode mode = ios_base::in,
    Endian endian = NATIVE) :
    std::ifstream(filename, mode), endian(endian) {}
    EndianReader(EndianReader&& rhs) : std::ifstream(std::move(rhs)) {}
    EndianReader(const EndianReader&) = delete;
    EndianReader& operator=(const EndianReader&) = delete;

    Endian endian;
    
    template <typename T>
    T ReadType() {
        T result{};
        char* ptr = (char*)&result;
        read(ptr, sizeof(T));
        if (endian != NATIVE)
            SwapVal(result);
        return result;
    }
};

class UniqueReader final: public std::unique_ptr<EndianReader, StreamDel> {
    private:
    using Base = std::unique_ptr<EndianReader, StreamDel>;
    public:
    constexpr UniqueReader() noexcept : Base() {}
    constexpr UniqueReader(std::nullptr_t np) noexcept : Base(np) {}
    explicit UniqueReader(pointer ptr) noexcept : Base(ptr) {}
    UniqueReader(UniqueReader&& rhs) noexcept : Base(std::move(rhs)) {}
    ~UniqueReader() {}
    
    explicit UniqueReader(const std::string& path, Endian endian = NATIVE,
    std::ios::openmode mode = EndianReader::binary) : UniqueReader(new EndianReader{path, mode, endian}) {}
};