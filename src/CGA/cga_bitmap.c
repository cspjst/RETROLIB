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
    bmp->data = NULL;
    return bmp;
}

cga_bitmap_t* cga_bmp_load(const char* file_path, mem_arena_t* arena, cga_bitmap_t* bmp) {
    errno = EINVAL;
    if(!file_path || !arena || !bmp) return NULL;
    FILE* f = fopen(file_path, "rb");
    if(!f) return NULL;

    if (fread(&bmp->depth, sizeof(bmp->depth), 1, f) != 1 ||
        fread(&bmp->width, sizeof(bmp->width), 1, f) != 1 ||
        fread(&bmp->height, sizeof(bmp->height), 1, f) != 1 ||
        fread(&bmp->palette, sizeof(bmp->palette), 1, f) != 1 ||
        fread(&bmp->size, sizeof(bmp->size), 1, f) != 1) return NULL;

    errno = ENOMEM;
    bmp->data = (char*)mem_arena_alloc(arena, bmp->size);
    if(!bmp->data) return NULL;
    if (fread(&bmp->data, 1, bmp->size, f) != bmp->size) return NULL;

    fclose(f);
    errno = 0;
    return bmp;
}

cga_bitmap_t* cga_bmp_save(const char* file_path, mem_arena_t* arena, cga_bitmap_t* bmp) {

}

void cga_bmp_dump(FILE* f, cga_bitmap_t* bmp) {
    if (!f || !bmp) return;

    fprintf(f, "CGA Bitmap:\n");
    fprintf(f, "  depth   = %hu\n", bmp->depth);
    fprintf(f, "  width   = %hu\n", bmp->width);
    fprintf(f, "  height  = %hu\n", bmp->height);
    fprintf(f, "  size    = %u\n", bmp->size);
    fprintf(f, "  palette = 0x%04X (%hu)\n", bmp->palette, bmp->palette);
    fprintf(f, "  data    = %p\n", (void*)bmp->data);

    if(bmp->data) {
        for(int i = 0; i < 16; ++i)
            fprintf(f, "%02X ", (unsigned char)bmp->data[i]);
        fprintf(f, "\n");
    }
}
