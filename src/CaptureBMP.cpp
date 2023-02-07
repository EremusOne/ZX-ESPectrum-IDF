#include "CaptureBMP.h"
#include "Video.h"
#include "FileUtils.h"
#include "stdio.h"

void CaptureBMP::capture(const char* filename)
{
    if (0 == filename || 0 == *filename) {
        printf("CaptureBMP: null/empty filename\n");
        return;
    }

    int w = VIDEO::vga.xres;
    int h = VIDEO::vga.yres;

    int count = w / 4;
    uint32_t *linebuf = new uint32_t[count];
    if (NULL == linebuf) {
        printf("CaptureBMP: unable to allocate line buffer\n");
        return;
    }

    std::string fullfn = FileUtils::MountPoint + filename;

    FILE* pf = fopen(fullfn.c_str(), "wb");
    if (NULL == pf) {
        delete[] linebuf;
        printf("CaptureBMP: unable to open file %s for writing\n", fullfn.c_str());
        return;
    }

    printf("CaptureBMP: capturing %d x %d to %s...\n", w, h, fullfn.c_str());

    for (int y = 0; y < h; y++) {
        uint32_t* src = (uint32_t*)VIDEO::vga.backBuffer[y];
        uint32_t* dst = linebuf;
        for (int i = 0; i < count; i++) {
            uint32_t srcval = *src++;
            uint32_t dstval = 0;
            dstval |= ((srcval & 0xFFFF0000) >> 16);
            dstval |= ((srcval & 0x0000FFFF) << 16);
            *dst++ = dstval;
        }
        fwrite(dst, sizeof(uint32_t), count, pf);
    }

    fclose(pf);
    delete[] linebuf;

    printf("CaptureBMP: done\n");
}