/**
 * Screen dimensions likely in DOS games:
 CGA4		320 x 200
 CGA6		600 x 200
 HGA		720 x 348
 VGA		640 x 480
 SVGA		800 x 600
 XGA		1024 x 768
 */
#ifndef FXP_CONSTANTS_H
#define FXP_CONSTANTS_H

#define FXP_INTEGRAL_BITS		10
#define FXP_FRACTIONAL_BITS	    6
#define FXP_FRACTIONAL_MASK	    0x3F		// i.e. 0000000000.111111
#define FXP_MAX				    0x7FFF		// 0111111111.111111
#define FXP_MIN				    -0x8000		// 1000000000.000000
#define FXP_MAXINT			    511
#define FXP_MININT			    -512
#define FXP_INFINITY            32767;		// which in 10:6 bits layout is 511.984
#define FXP_NINFINITY           -32767;		// which in 10:6 bits layout is -512.000
#define FXP_FRACTIONAL_MIN	    0.015625	// which in 10:6 bits layout is 0000000000.000001

so then saturate can be

int16_t saturate(int32_t n) {
    if (n > FXP_INFINITY) return FXP_MAX;
    if (n < FXP_NINFINITY) return FXP_MIN;
    return (int16_t)(n);
}

#endif
