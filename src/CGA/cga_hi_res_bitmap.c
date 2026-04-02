#include "cga_hi_res_bitmap.h"

/*
FILE* fp = fopen("image.ppm", "rb");
fscanf(fp, "P6\n%d %d\n255\n", &width, &height);  // skip header
fgetc(fp); // consume newline

uint8_t* rgb_data = malloc(width * height * 3);
fread(rgb_data, 1, width * height * 3, fp);  // read all RGB triplets
fclose(fp);
 */
