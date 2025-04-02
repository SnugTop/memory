// utils.c
#include <stdint.h>

uint8_t extract_offset(uint32_t address) {
    return address & 0xFF;
}

uint8_t extract_page_number(uint32_t address) {
    return (address >> 8) & 0xFF;
}

uint8_t extract_write_bit(uint32_t address) {
    return (address >> 16) & 0x1;
}