//check_bin.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logical address in hex, e.g. 0x4214>\n", argv[0]);
        return 1;
    }

    uint32_t logical_addr = (uint32_t)strtol(argv[1], NULL, 16);

    // Extract page number and offset
    uint8_t page = (logical_addr >> 8) & 0xFF;
    uint8_t offset = logical_addr & 0xFF;

    // Open BACKING_STORE.bin
    FILE *bs = fopen("BACKING_STORE.bin", "rb");
    if (!bs) {
        perror("Error opening BACKING_STORE.bin");
        return 1;
    }

    // Seek and read
    fseek(bs, logical_addr, SEEK_SET);
    signed char value;
    fread(&value, sizeof(signed char), 1, bs);
    fclose(bs);

    // Clean output without "physical"
    printf("Logical: 0x%04X | Page: 0x%02X | Offset: 0x%02X | File Offset: %u | Value: %d\n",
           logical_addr, page, offset, logical_addr, value);

    return 0;
}
