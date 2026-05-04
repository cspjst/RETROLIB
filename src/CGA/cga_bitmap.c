/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.5
 */
#include "cga_bitmap.h"

#include <errno.h>

#include "cga_types.h"

cga_bitmap_t* cga_make_bmp(cga_bitmap_t* bmp, cga_colour_depth_t depth, cga_coord_t width, cga_coord_t height, unsigned int pal) {
    if(!bmp || width == 0 || height == 0) return NULL;
    bmp->depth  = depth;
    bmp->width  = width;
    bmp->height = height;
    bmp->size = ((bmp->width * bmp->depth + 7) >> 3) * bmp->height;
    bmp->blocks = 0;
    for(int i = 0; i < 8; ++i) bmp->data[i] = NULL;
    return bmp;
}

cga_bitmap_t* cga_bmp_load(const char* file_path, mem_arena_t* arena) {
    errno = EINVAL;
    if(!file_path || !arena) return NULL;
    // allocate a new bitmap
    cga_bitmap_t* bmp = (cga_bitmap_t*)mem_arena_alloc(arena, sizeof(cga_bitmap_t));
    if(!bmp) return NULL;                   // arena sets errno to ENOMEM
    FILE* f = fopen(file_path, "rb");
    if(!f) return NULL;                     // fopen sets errno
     // read header fields...
    if( fread(&bmp->depth,   sizeof(bmp->depth),   1, f) != 1 ||
        fread(&bmp->width,   sizeof(bmp->width),   1, f) != 1 ||
        fread(&bmp->height,  sizeof(bmp->height),  1, f) != 1 ||
        fread(&bmp->palette, sizeof(bmp->palette), 1, f) != 1 ||
        fread(&bmp->size,    sizeof(bmp->size),    1, f) != 1 ||
        fread(&bmp->blocks,  sizeof(bmp->blocks),  1, f) != 1
    ) goto fail;
    // allocate data buffers and read bitmap raw data blocks
    for(int i = 0; i < bmp->blocks; ++i) {
        bmp->data[i] = (char*)mem_arena_alloc(arena, bmp->size);
        if(!bmp->data[i]) goto fail;            // arena sets errno to ENOMEM
        if(fread(bmp->data[i], 1, bmp->size, f) != bmp->size) goto fail;
    }
    // NULLify any unused data block pointers
    for(int i = bmp->blocks; i < 8; ++i) bmp->data[i] = NULL;
    fclose(f);
    errno = 0;
    return bmp;
fail:
    fclose(f);
    if(errno == 0) errno = EIO;
    return NULL;
}

dos_memsize_t cga_bmp_save(const char* file_path, const cga_bitmap_t* bmp) {
    errno = EINVAL;
    if(!file_path || !bmp) return 0;
    FILE* f = fopen(file_path, "wb");
    if(!f) return 0;
    // calculate header file size
    dos_memsize_t size = sizeof(bmp->depth) +
                         sizeof(bmp->width) +
                         sizeof(bmp->height) +
                         sizeof(bmp->palette) +
                         sizeof(bmp->size) +
                         sizeof(bmp->blocks);
    // write header to file...
    if( fwrite(&bmp->depth, sizeof(bmp->depth), 1, f) != 1      ||
        fwrite(&bmp->width, sizeof(bmp->width), 1, f) != 1      ||
        fwrite(&bmp->height, sizeof(bmp->height), 1, f) != 1    ||
        fwrite(&bmp->palette, sizeof(bmp->palette), 1, f) != 1  ||
        fwrite(&bmp->size, sizeof(bmp->size), 1, f) != 1        ||
        fwrite(&bmp->blocks, sizeof(bmp->blocks), 1, f) != 1
    ) goto fail;
    // write bitmap raw data to file
    for(int i = 0; i < bmp->blocks; ++i) {
        if (fwrite(bmp->data[i], 1, bmp->size, f) != bmp->size) goto fail;
    }
    fclose(f);
    errno = 0;
    return size + bmp->size;
fail:
    fclose(f);
    remove(file_path);          // clean up partial file
    return 0;
}

void cga_bmp_dump(FILE* f, cga_bitmap_t* bmp) {
    if (!f || !bmp) return;

    fprintf(f, "CGA Bitmap:\n");
    fprintf(f, "  depth   = %hu\n", bmp->depth);
    fprintf(f, "  width   = %hu\n", bmp->width);
    fprintf(f, "  height  = %hu\n", bmp->height);
    fprintf(f, "  palette = 0x%04X (%hu)\n", bmp->palette, bmp->palette);
    fprintf(f, "  size    = %u\n", bmp->size);
    fprintf(f, "  blocks  = %u\n", bmp->blocks);
    fprintf(f, "data[0]%p ",(void*)bmp->data[0]);
    fprintf(f, "[1]%p ",(void*)bmp->data[1]);
    fprintf(f, "[2]%p ",(void*)bmp->data[2]);
    fprintf(f, "[3]%p\n",(void*)bmp->data[3]);
    fprintf(f, "    [4]%p ",(void*)bmp->data[4]);
    fprintf(f, "[5]%p ",(void*)bmp->data[5]);
    fprintf(f, "[6]%p ",(void*)bmp->data[6]);
    fprintf(f, "[7]%p\n",(void*)bmp->data[7]);
    for(int i = 0; i < 16 >> bmp->depth; ++i) {
        if(bmp->data[i]) {
            for(int j = 0; j < 16; ++j)
                fprintf(f, "%02X ", (unsigned char)bmp->data[i][j]);
            fprintf(f, "\n");
        }
    }
}
