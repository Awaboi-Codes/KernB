#include "ata.h"
#include "ink.h"

unsigned char sector[512]; // global is fine

void readBlock(int lba) { // 512 byte blocks
    ata_read_sector(lba, sector);  // must be inside a function
}