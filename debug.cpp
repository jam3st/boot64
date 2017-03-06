#include "types.hpp"

namespace Gx {

    constexpr uint16_t debugPort = 0x03f8;

    uint8_t getByte(uint64_t const val, size_t const index) {
        union {
            uint8_t asBytes[sizeof(uint64_t)];
            uint64_t asUint64;
        } value;
        value.asUint64 = val;
        return value.asBytes[index];
    }

    uint64_t readSp() {
        uint64_t value;
        __asm__ volatile("movq %%rsp, %[value]" : [value] "=a"(value));
        return value;
    }

    uint64_t readAx() {
        uint64_t value;
        __asm__ volatile("movq %%rax, %[value]" : [value] "=a"(value));
        return value;
    }

    uint64_t readBx() {
        uint64_t value;
        __asm__ volatile("movq %%rbx, %[value]" : [value] "=a"(value));
        return value;
    }

    uint64_t readDi() {
        uint64_t value;
        __asm__ volatile("movq %%rdi, %[value]" : [value] "=a"(value));
        return value;
    }

    uint64_t readSi() {
        uint64_t value;
        __asm__ volatile("movq %%rsi, %[value]" : [value] "=a"(value));
        return value;
    }

    uint8_t in8(uint16_t const port) {
        uint8_t value;
        __asm__ volatile("inb %[port], %[value]" : [value] "=a"(value) : [port] "d"(port));
        return value;
    }

    void out8(uint16_t port, uint8_t value) {
        __asm__ volatile("outb %[value], %[port]":: [port] "d"(port), [value] "a"(value));
    }

    void dumpByte(uint8_t const what) {
        while ((in8(debugPort + 5) & 0x20) == 0) {
        }
        out8(debugPort, what);

    }

    uint64_t getBits(uint64_t const val, size_t const start, size_t const end) {
        return (val >> start) & ((1 << (end - start)) - 1);
    }

    uint8_t toPrintHex(uint64_t const val) {
        if (val < 10) {
            return val + '0';
        } else {
            return val - 10 + 'a';
        }
    }

    void dump64(uint64_t const value) {
        for (ssize_t i = 64; i > 0; i = i - 4) {
            dumpByte(toPrintHex(getBits(value, i - 4, i - 1)));
        }
    }

    void dump32(uint32_t const value) {
        for (ssize_t i = 32; i > 0; i = i - 4) {
            dumpByte(toPrintHex(getBits(value, i - 4, i - 1)));
        }
    }


    void dumpString(uint8_t const str[]) {
        uint8_t const *start = str;
        while (*start != '\0') {
            dumpByte(*start);
            ++start;
        }
    }
}


struct BootInfo {
    Gx::uint32_t flags;
    Gx::uint8_t dontCare[40];
    Gx::uint32_t mmapLength;
    Gx::uint32_t mmapPtr;
    Gx::uint8_t careLess[34];
}  __attribute__((packed));

struct MmapEntry {
    Gx::uint32_t size;
    Gx::uint64_t addr;
    Gx::uint64_t len;
    enum class usage : Gx::uint32_t {
        available = 1,
        reserved = 2
    } type;
} __attribute__((packed));



extern "C"
__attribute((visibility("default"))) __attribute__ ((used)) __attribute__((noreturn))
void bootStart(BootInfo const* const bootInfo) {
    Gx::uint8_t avail[] = "avail: ";
    Gx::uint8_t noavl[] = "noavl: ";
    Gx::uint8_t sep[] = "    ";
    Gx::uint8_t newLine[] = "\r\n";

    if((bootInfo->flags & 0x40ul) != 0) {
        MmapEntry const* const mmapEndAddr = reinterpret_cast<MmapEntry* const>(bootInfo->mmapPtr + bootInfo->mmapLength);
        for(MmapEntry const* mmap = reinterpret_cast<MmapEntry* const>(bootInfo->mmapPtr); mmap < mmapEndAddr; ++mmap) {
            if(mmap->type == MmapEntry::usage::available && mmap->size == (sizeof(*mmap) - sizeof(mmap->size))) {
                Gx::dump64(mmap->addr);
                Gx::dumpString(sep);
                Gx::dump64(mmap->addr + mmap->len);
                Gx::dumpString(newLine);
            }
        }
    }
    Gx::dumpString(newLine);
    for(;;) {
    }
}
