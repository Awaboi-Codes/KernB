#include "framebuffer.h"
#include "memory.h"
#include "../fs/ink.h"

// ============================================================
//  Multiboot2 structs
// ============================================================
struct mb2_tag {
    unsigned int type;
    unsigned int size;
};

struct mb2_tag_framebuffer {
    unsigned int  type;
    unsigned int  size;
    unsigned long long addr;
    unsigned int  pitch;
    unsigned int  width;
    unsigned int  height;
    unsigned char bpp;
    unsigned char fb_type;
    unsigned short reserved;
};

extern "C" void kernel_main(unsigned int magic, unsigned int mb_info_addr) {
    (void)magic;

    // Walk multiboot2 tags
    unsigned char* tags = (unsigned char*)(mb_info_addr + 8);
    while (true) {
        mb2_tag* tag = (mb2_tag*)tags;
        if (tag->type == 0) break;
        if (tag->type == 8) {
            mb2_tag_framebuffer* fbt = (mb2_tag_framebuffer*)tag;
            fb.addr   = (unsigned int*)((unsigned int)fbt->addr);
            fb.width  = fbt->width;
            fb.height = fbt->height;
            fb.pitch  = fbt->pitch;
            fb.bpp    = fbt->bpp;
        }
        tags += (tag->size + 7) & ~7;
    }

    if (!fb.addr) {
        // Fallback to VGA text if no framebuffer
        unsigned short* vga = (unsigned short*)0xB8000;
        const char* msg = "NO FRAMEBUFFER! Enable it in GRUB.";
        for (int i = 0; msg[i]; i++) vga[i] = 0x0F00 | msg[i];
        while (true) asm volatile("hlt");
    }

    readBlock(0);
    fbsys::draw_string(5, 5, reinterpret_cast<const char*>(sector), COLOR_WHITE);

    while (true) asm volatile("hlt");
}