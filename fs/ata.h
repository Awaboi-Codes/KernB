// ata.h
#pragma once

// ATA PIO mode — reads sectors directly from disk
// Primary bus ports
#define ATA_DATA        0x1F0
#define ATA_ERROR       0x1F1
#define ATA_SECTOR_COUNT 0x1F2
#define ATA_LBA_LOW     0x1F3
#define ATA_LBA_MID     0x1F4
#define ATA_LBA_HIGH    0x1F5
#define ATA_DRIVE       0x1F6
#define ATA_STATUS      0x1F7
#define ATA_COMMAND     0x1F7

#define ATA_STATUS_BSY  0x80  // busy
#define ATA_STATUS_RDY  0x40  // ready
#define ATA_STATUS_DRQ  0x08  // data request (ready to transfer)
#define ATA_STATUS_ERR  0x01  // error

void ata_read_sector(unsigned int lba, unsigned char* buf);
void ata_write_sector(unsigned int lba, unsigned char* buf);