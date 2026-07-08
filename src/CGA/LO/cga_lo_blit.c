#include "cga_lo_blit.h"

#include "../cga_constants.h"
#include "../cga_lookup_table_y.h"  // IWYU pragma: keep

void cga_lo_screen_blit(const char* data) {
    __asm {
        .8086
        push    ds
        push    es
        push    bp
        pushf

        cld                                 ; incremental MOVSW
        mov     di, CGA_VIDEO_RAM_SEGMENT   ; load even VRAM segment address
        mov     es, di                      ; ES:DI -> VRAM
        xor     di, di                      ; 0 is an even number so bank 0
        mov     ax, 1FB0h                   ; 2000h - 80 byte row increment & because add reg, reg is 1 cycle faster than xor reg,imm on 8086
        mov     bx, CGA_WORDS_PER_ROW       ; 40 words per row
        mov     dx, CGA_ROWS_PER_BANK       ; 100 rows/bank
        lds     si, data                    ; DS:SI -> linear source
        mov     bp, CGA_BYTES_PER_BANK      ; 2000h row decrement

ROWS:   mov     cx, bx                      ; load REP count
        rep     movsw                       ; copy 40 word row to bank 0
        add     di, ax                      ; start of bank 1
        mov     cx, bx                      ; load REP count
        rep     movsw                       ; copy 40 word row to bank 1
        sub     di, bp                      ; start of bank 0
        dec     dx                          ; next 2 rows
        jnz     ROWS                        ; until done

        popf
        pop     bp
        pop     es
        pop     ds
    }
}

void cga_lo_blit(cga_coord_t x, cga_coord_t y, cga_bitmap_t* bmp) {
    cga_size_t noff, nseg;
    noff = nseg = 0;
    _asm {
        .8086
        push    ds
        push    es
        pushf

        cld                                 ; inc mode
        // setup ES:BX to point to the bitmap struct
        les     bx, bmp                     ; ES:BX = bmp*
        mov     ax, x                       ; AX = x
        mov     di, ax                      ; DI = x
        // select bitmap data block
        and     ax, 3                       ; AX = x mod 4 ie which 0..3 bitmap data block
        shl     ax, 1                       ; convert AX to double word offset
        shl     ax, 1                       ; 8086 single shifts only, MUL 4
        // setup width, height and DS:SI ptr to the correct bitmap data
        mov     cx, es:[bx + BMP_WIDTH]     ; CX = bitmap pixel width
        shr     cx, 1                       ; DIV 4 bits per byte
        shr     cx, 1                       ; CX = bitmap byte width
        mov     dx, es:[bx + BMP_HEIGHT]    ; DX = bitmap pixel height
        add     bx, ax                      ; select bitmap address
        mov     si, es:[bx + BMP_DATA]      ; SI = offset of data block
        mov     ax, es:[bx + BMP_DATA + 2]  ; AX = segment of data block

        // setup x (DI as above), y and ES:DI ptr to VRAM
        shr     di, 1                       ; DI = x div 4
        shr     di, 1                       ; DI = VRAM byte column
        mov     bx, y                       ; BX = y
        shl     bx, 1                       ; turn y into a word table index
        add     di, CGA_ROW_OFFSETS[bx]     ; DI = byte column + row offset
        mov     bx, CGA_VIDEO_RAM_SEGMENT
        mov     es, bx                      ; ES:DI = VRAM*
        mov     ds, ax                      ; DS:SI = data*

        test    di, CGA_BANK_BIT            ; starting bank?
        jnz     BANK1

BANK0:  mov     bx, cx                      ; BX = copy byte width
        test    cx, 1                       ; odd byte width?
        je      EVEN0
        movsb                               ; paste 1 byte
        dec     cx                          ; width - 1
EVEN0:  shr     cx, 1                       ; CX DIV 2 = word count
        rep movsw                           ; paste remainder
        add     di, 2000h                   ; bank 1
        sub     di, bx                      ; - byte width
        mov     cx, bx                      ; restore count byte width
        dec     dx                          ; height--
        jz      END                         ; height == 0

BANK1:  mov     bx, cx                      ; BX = copy byte width
        test    cx, 1                       ; odd byte width?
        je      EVEN1
        movsb                               ; paste 1 byte
        dec     cx                          ; width - 1
EVEN1:  shr     cx, 1                       ; CX DIV 2 = word count
        rep movsw                           ; paste remainder
        sub     di, 1FB0h                   ; bank 0 next line
        sub     di, bx                      ; - byte width
        mov     cx, bx                      ; restore count byte width
        dec     dx                          ; height--
        jz      END                         ; height == 0

        jmp     BANK0

END:    popf
        pop     es
        pop     ds
    }
    //printf("nseg = %X noff = %X\n", nseg, noff);
}

void cga_lo_mask_blit(cga_coord_t x, cga_coord_t y, cga_bitmap_t* mask, cga_bitmap_t* bmp) {
    // use BP:AX as holding registers for DS:SI for mask and bmp use xchg to swap between mask and bmp
}
