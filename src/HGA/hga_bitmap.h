#ifndef HGA_BITMAP_T
#define HGA_BITMAP_T

typedef struct {
    hga_coord_t width;          // mode 6 = 0..639, mode 4 = 0..319
    hga_coord_t height;         // modes 4 and 6 = 0..199
    hga_size_t data_size;       // limited to 64K max
    hhar* data;                 
} hga_bitmap_t;

hga_bitmap_t* hga_make_bmp(hga_bitmap_t* bmp, hga_coord_t width, hga_coord_t height);

#endif
