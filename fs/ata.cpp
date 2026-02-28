// ata.cpp
#include "ata.h"

// Port I/O helpers
static inline void outb(unsigned short port, unsigned char val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline unsigned char inb(unsigned short port) {
    unsigned char val;
    asm volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}
static inline unsigned short inw(unsigned short port) {
    unsigned short val;
    asm volatile("inw %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static void ata_wait_ready() {
    // Wait until BSY clears and RDY sets
    while (inb(ATA_STATUS) & ATA_STATUS_BSY);
    while (!(inb(ATA_STATUS) & ATA_STATUS_RDY));
}

void ata_read_sector(unsigned int lba, unsigned char* buf) {
    ata_wait_ready();

    outb(ATA_DRIVE,        0xE0 | ((lba >> 24) & 0x0F)); // LBA mode, master drive
    outb(ATA_SECTOR_COUNT, 1);                            // read 1 sector
    outb(ATA_LBA_LOW,      lba & 0xFF);
    outb(ATA_LBA_MID,      (lba >> 8)  & 0xFF);
    outb(ATA_LBA_HIGH,     (lba >> 16) & 0xFF);
    outb(ATA_COMMAND,      0x20);                         // READ SECTORS command

    ata_wait_ready();

    // Check for error
    if (inb(ATA_STATUS) & ATA_STATUS_ERR) return;

    // Wait for DRQ (data ready to read)
    while (!(inb(ATA_STATUS) & ATA_STATUS_DRQ));

    // Read 256 words = 512 bytes = 1 sector
    unsigned short* ptr = (unsigned short*)buf;
    for (int i = 0; i < 256; i++) {
        ptr[i] = inw(ATA_DATA);
    }
}

void ata_write_sector(unsigned int lba, unsigned char* buf) {
    ata_wait_ready();

    outb(ATA_DRIVE,        0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_SECTOR_COUNT, 1);
    outb(ATA_LBA_LOW,      lba & 0xFF);
    outb(ATA_LBA_MID,      (lba >> 8)  & 0xFF);
    outb(ATA_LBA_HIGH,     (lba >> 16) & 0xFF);
    outb(ATA_COMMAND,      0x30);                         // WRITE SECTORS command

    ata_wait_ready();
    while (!(inb(ATA_STATUS) & ATA_STATUS_DRQ));

    unsigned short* ptr = (unsigned short*)buf;
    for (int i = 0; i < 256; i++) {
        // outw
        asm volatile("outw %0, %1" : : "a"(ptr[i]), "Nd"((unsigned short)ATA_DATA));
    }
}