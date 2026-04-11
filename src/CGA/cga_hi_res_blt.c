/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.2.0
 */
#include "cga_hi_res_blt.h"
#include "cga_constants.h"
#include "cga_lookup_table_y.h"

#define CGA_NO_SYNC

void cga_hi_res_screen_blt(const char* data) {
    __asm {
        .8086
        push    ds
        push    es
        push    bp
        pushf

#ifndef CGA_NO_SYNC
        mov     dx, CGA_STATUS_REG          ; CGA status port
WAIT0:  in      al, dx                      ; read status port
        test    al, 8                       ; in vertical retrace?
        jnz     WAIT0                       ; wait for it to end

WAIT1:  in      al, dx                      ; read status port
        test    al, 8                       ; vertical retrace started?
        jz      WAIT1                       ; wait for it to start
#endif

        cld                                 ; incremental MOVSW
        mov     di, CGA_VIDEO_RAM_SEGMENT
        mov     es, di
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

// 8x8 bitmap optimised
void cga_hi_res_blt8x8(cga_coord_t x, cga_coord_t y, const char* data) {
    __asm {
        .8086
        push    ds
        push    es
        pushf

        cld                                 ; incremental MOVS
        mov     di, CGA_VIDEO_RAM_SEGMENT   ; more 8086 quirks
        mov     es, di                      ; ES = VRAM segment
        mov     di, y                       ; DI = y
        shl     di, 1                       ; DI is a word offset
        mov     di, CGA_ROW_OFFSETS[di]     ; ES:DI -> VRAM
        mov     ax, x                       ; AX = x
        mov     bx, ax                      ; copy x
        test    bl, 7                       ; test lower 3 bits (x mod 8)
        jz      FAST                        ; byte aligned x coord

        shr     ax, 1                       ; calculate column byte x / 8
        shr     ax, 1                       ; 8086 limited to single step shifts
        shr     ax, 1                       ; BX is now *column* byte
        add     di, ax                      ; ES:DI -> VRAM (x,y)
        and     bx, 7                       ; get the shift count
        shl     bx, 1                       ; convert to word ptr

#ifndef CGA_NO_SYNC
        mov     dx, CGA_STATUS_REG          ; CGA status port
        in      al, dx                      ; read status port
        test    al, 8                       ; in vertical retrace?
        jnz     SHFT                        ; already in retrace - risk it (for performance)

SWAIT1: in      al, dx                      ; read status port
        test    al, 8                       ; vertical retrace started?
        jz      SWAIT1                      ; wait for it to start
#endif

SHFT:   mov     ax, CGA_HI_RES_MASKS[bx]    ; load the byte mask
        mov     cx, 1FB0h + 1               ; bank 0 stride
        mov     dx, 2000h - 1               ; bank 1 stride
        lds     si, data                    ; DS:SI -> data (safe now)
        test    di, 2000h                   ; starting bank?
        jnz     SBANK1

SBANK0:

SBANK1:
        mov     es:[di], ax
        //movsb
        jmp     END

FAST:   shr     ax, 1                       ; calculate column byte x / 8
        shr     ax, 1                       ; 8086 limited to single step shifts
        shr     ax, 1                       ; BX is now *column* byte
        add     di, ax                      ; ES:DI -> VRAM (x,y)
        lds     si, data                    ; DS:SI -> data (safe now)
        mov     cx, 1FB0h + 1               ; bank 0 stride

#ifndef CGA_NO_SYNC
        mov     dx, CGA_STATUS_REG          ; CGA status port
        in      al, dx                      ; read status port
        test    al, 8                       ; in vertical retrace?
        jnz     BLT                         ; already in retrace - risk it (for performance)

FWAIT1: in      al, dx                      ; read status port
        test    al, 8                       ; vertical retrace started?
        jz      FWAIT1                      ; wait for it to start
#endif

BLT:    mov     dx, 2000h - 1               ; bank 1 stride
        test    di, 2000h                   ; starting bank?
        jnz     FBANK1

FBANK0: movsb                               ; DS:SI -> ES:DI
        add     di, dx                      ; bank1 - stride
FBANK1: movsb                               ; DS:SI -> ES:DI
        sub     di, cx                      ; bank 0 stride
        movsb                               ; unrolled 8 loop...
        add     di, dx
        movsb
        sub     di, cx
        movsb
        add     di, dx
        movsb
        sub     di, cx
        movsb
        add     di, dx
        movsb
        sub     di, cx
END:
        popf
        pop     es
        pop     ds
    }
}

// general purpose
void cga_hi_res_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* data) {
    __asm {
        .8086
        push    ds
        push    es
        push    bp
        pushf

        cld                                 ; incremental MOVS
        mov     bx, y                       ; BX = y
        mov     cx, w                       ; CX = width
        shr     cx, 1                       ; calculate byte  width w / 8
        shr     cx, 1                       ; 8086 limited to single step shifts
        shr     cx, 1                       ; CX is now *byte* width
        mov     di, CGA_VIDEO_RAM_SEGMENT   ; more 8086 quirks
        mov     es, di                      ; ES = VRAM segment
        mov     di, bx                      ; DI = y
        shl     di, 1                       ; DI is a word offset
        mov     di, CGA_ROW_OFFSETS[di]     ; ES:DI -> VRAM
        mov     ax, x                       ; AX = x
        test    al, 7                       ; test lower 3 bits (x mod 8)
        jz      FAST                        ; byte aligned x coord

        shr     ax, 1                       ; calculate column byte x / 8
        shr     ax, 1                       ; 8086 limited to single step shifts
        shr     ax, 1                       ; AX is now *column* byte
        add     di, ax                      ; ES:DI -> VRAM (x,y)

#ifndef CGA_NO_SYNC
        mov     dx, CGA_STATUS_REG          ; CGA status port
        in      al, dx                      ; read status port
        test    al, 8                       ; in vertical retrace?
        jnz     SHFT                        ; already in retrace - risk it (for performance)

AWAIT1: in      al, dx                      ; read status port
        test    al, 8                       ; vertical retrace started?
        jz      AWAIT1                       ; wait for it to start
#endif

SHFT:   mov     dx, h                       ; DX = height
        mov     bx, CGA_BYTES_PER_ROW       ; 80 bytes per VRAM row
        sub     bx, cx                      ; 80 - *byte* width
        lds     si, data                    ; DS:SI -> data (safe now)


        jmp     END

FAST:   shr     ax, 1                       ; calculate column byte x / 8
        shr     ax, 1                       ; 8086 limited to single step shifts
        shr     ax, 1                       ; AX is now *column* byte
        add     di, ax                      ; ES:DI -> VRAM (x,y)

#ifndef CGA_NO_SYNC
        mov     dx, CGA_STATUS_REG          ; CGA status port
        in      al, dx                      ; read status port
        test    al, 8                       ; in vertical retrace?
        jnz     BLT                         ; already in retrace - risk it (for performance)

BWAIT1: in      al, dx                      ; read status port
        test    al, 8                       ; vertical retrace started?
        jz      BWAIT1                       ; wait for it to start
#endif

BLT:    mov     dx, h                       ; DX = height
        lds     si, data                    ; DS:SI -> data (safe now)
        mov     bp, 1FB0h                   ; bank 0 sub_stride - 80 next line
        add     bp, cx                      ; sub_stride + byte_width

        mov     bx, cx                      ; copy CX byte width
        mov     ax, 2000h                   ; bank 1 add_stride
        sub     ax, cx                      ; add_stride - byte width

        test    cx, 1                       ; even or odd byte width
        jz      EVEN                        ; word transfers
        test    di, 2000h
        jnz     OBANK1

OBANK0: mov     cx, bx                      ; set CX byte counter
        rep     movsb                       ; CX(DS:SI -> ES:DI)bytes
        mov     cx, bx                      ; restore CX
        dec     dx                          ; reduce row count
        jz      END                         ; no more rows
        add     di, ax                      ; bank1 - byte width

OBANK1: mov     cx, bx                      ; set CX byte counter
        rep     movsb                       ; CX(DS:SI -> ES:DI)bytes
        mov     cx, bx                      ; restore CX
        dec     dx                          ; reduce row count
        jz      END                         ; no more rows
        sub     di, bp                      ; bank 0

        jmp     OBANK0

EVEN:   shr     bx, 1
        test    di, 2000h
        jnz     EBANK1

EBANK0: mov     cx, bx                      ; set CX byte counter
        rep     movsw                       ; CX(DS:SI -> ES:DI)bytes
        dec     dx                          ; reduce row count
        jz      END                         ; no more rows
        add     di, ax                      ; bank1 - byte width

EBANK1: mov     cx, bx                      ; set CX byte counter
        rep     movsw                       ; CX(DS:SI -> ES:DI)bytes
        dec     dx                          ; reduce row count
        jz      END                         ; no more rows
        sub     di, bp                      ; bank 0

        jmp     EBANK0
END:
        popf
        pop     bp
        pop     es
        pop     ds
   }
}
