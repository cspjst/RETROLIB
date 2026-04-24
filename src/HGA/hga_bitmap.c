/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */

#include "hga_bitmap_t" 

hga_bitmap_t* hga_make_bmp(hga_bitmap_t* bmp, hga_coord_t width, hga_coord_t height) {
    if(!bmp || width == 0 || height == 0) return NULL;
    bmp->width  = width;
    bmp->height = height;
    bmp->size = ((bmp->width * bmp->depth + 7) >> 3) * bmp->height;
    bmp->data = NULL;
    return bmp;
}
