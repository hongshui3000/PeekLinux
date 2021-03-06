#include "lgui.h"
#include "Debug.h"
#include "Color.h"

#include "p_malloc.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.141592654f
#endif

#define LGUI_WIDTH 320
#define LGUI_HEIGHT 240
static unsigned short *lgui_buffer = 0;

#define CLIP_STACK_SIZE 5
static Rectangle clip_stack[CLIP_STACK_SIZE];
static short clip_index = 0;
#define CLIP clip_stack[clip_index]

static int translate_x = 0, translate_y = 0;

#define SCALE_PIXEL(scale, r, g, b) ((( ((unsigned short)(r))*scale) & 0xF800) | \
	(((((unsigned short)(g))*scale) >> 5) & 0x7E0) | \
	(((((unsigned short)(b))*scale) >> 11) & 0x1F))

#if 0
#define PIXEL_MODULATE_ALPHA(pixel, srcpixel, alpha, result) \
	(result) = ( (( (((unsigned short)(pixel & 0xF800)) * (unsigned short)(alpha))>>8)&0xF800) + \
    (( (((unsigned short)(srcpixel & 0xF800)) * (256-(unsigned short)(alpha)))>>8)&0xF800) ) | \
	( (( (((unsigned short)(pixel & 0x7E0)) * (unsigned short)(alpha))>>8)&0x7E0) + \
    (( (((unsigned short)(srcpixel & 0x7E0)) * (256-(unsigned short)(alpha)))>>8)&0x7E0) ) | \
	( (( (((unsigned short)(pixel & 0x1F)) * (unsigned short)(alpha))>>8)&0x1F) + \
    (( (((unsigned short)(srcpixel & 0x1F)) * (256-(unsigned short)(alpha)))>>8)&0x1F));
#else
#define PIXEL_MODULATE_ALPHA(__pixel, __srcpixel, __alpha, __result) \
{ \
	unsigned int __s = (__pixel), __d = (__srcpixel); \
	__s = (__s | __s << 16) & 0x07e0f81f; \
	__d = (__d | __d << 16) & 0x07e0f81f; \
	__d += (__s - __d) * ((__alpha) >> 3) >> 5; \
	__d &= 0x07e0f81f; \
	(__result) = (unsigned short)(__d | __d >> 16); \
}
#endif

#define FONT_HEADER_SIZE (6*4)

static void initD(void );

void lgui_attach(void *buf)
{
	static int aacircleInit = 0;
	lgui_buffer = (unsigned short *)buf;
    
	EMO_ASSERT(buf != NULL, "lgui attach to NULL buffer")

	if (!aacircleInit) {
		initD();
		aacircleInit = 1;
		lgui_clip_identity();
	}
}

void lgui_clear(unsigned char pixel)
{
	memset(lgui_buffer, pixel, LGUI_WIDTH*LGUI_HEIGHT*2);
}

#if 0
#define GRADIENT_STEP(res, sr, er, val, idx) res = (unsigned short)(((sr)<<7) * (((val-1)-idx)<<7)/((val-1)<<7)); \
		res += 127+(unsigned short)(((er)<<7) * (idx<<7)/((val-1)<<7)); \
		res >>= 7;
#else
#define GRADIENT_STEP(res, sr, er, val, idx) res = (unsigned short)(((sr) * ((val)-idx))/((val))); \
		res += 1+(unsigned short)(((er) * (idx))/((val))); 
#endif
void lgui_vertical_gradient(
	unsigned char start_red, unsigned char start_green, unsigned char start_blue,
	unsigned char start_alpha,
	unsigned char end_red, unsigned char end_green, unsigned char end_blue,
	unsigned char end_alpha,
	int startx, int starty, int width, int height)
{
	unsigned short red;
	unsigned short green;
	unsigned short blue;
	unsigned short alpha;
	int line, ypos, i;
	unsigned short *buf, pixel, ipixel, srcpixel;
	Rectangle rect;
	int cline, ccol, cwidth, cheight;

	startx += translate_x;
	starty += translate_y;

	rect.x = startx;
	rect.y = starty;
	rect.width = width;
	rect.height = height;
	if (lgui_clip_rect(&rect, &ccol, &cline, &cwidth, &cheight) == 0)
		return;

	lgui_set_dirty();

	ypos = starty;
	ypos += cline;
	for (line = cline; line < cheight; ++line) {
		if (ypos < 0 || ypos >= LGUI_HEIGHT) {
			++ypos;
			continue;
		}
		GRADIENT_STEP(red, start_red, end_red, height, line)
		GRADIENT_STEP(green, start_green, end_green, height, line)
		GRADIENT_STEP(blue, start_blue, end_blue, height, line)
		GRADIENT_STEP(alpha, start_alpha, end_alpha, height, line)

		pixel = RGB_TO_565(red, green, blue);
		ipixel = pixel;

		buf = lgui_buffer + startx + ypos*LGUI_WIDTH;

		buf += ccol;
		if (alpha < 255) {
			for (i = ccol; i < cwidth; ++i) {
				srcpixel = *buf;
				PIXEL_MODULATE_ALPHA(ipixel, srcpixel, alpha, pixel)
				*buf = pixel;
				++buf;
			}
		} else {
			for (i = ccol; i < cwidth; ++i) {
				*buf = pixel;
				++buf;
			}
		}
		++ypos;
	}
}

void lgui_vertical_gradientG(Gradient *g,
	int startx, int starty, int width, int height)
{
	int hstart, hend;
	int stop, maxStop;
	Color cstart, cend;

	EMO_ASSERT(g != NULL, "lgui vgradient missing gradient")

	hstart = height;
	cstart = gradient_getStop(g, 0, &hstart);
	hend = height;
	cend = gradient_getStop(g, 1, &hend);
	stop = 1;
	maxStop = gradient_stops(g);
	do {
		if (hstart != hend)
#if 1
			lgui_vertical_gradient(cstart.rgba.red, cstart.rgba.green, cstart.rgba.blue,
					cstart.rgba.alpha,
					cend.rgba.red, cend.rgba.green, cend.rgba.blue, cend.rgba.alpha,
					startx, starty+hstart, width, hend-hstart);
#else
			lgui_vertical_gradient(0xFF, 0xFF, 0xFF,
					0xFF, 0xFF, 0xFF,
					startx, starty+hstart, width, hend-hstart);
#endif
		++stop;
		cstart = cend;
		hstart = hend;
		hend = height;
		if (stop < maxStop)
			cend = gradient_getStop(g, stop, &hend);
	} while(stop < maxStop);
}

void lgui_hline(int x, int y, int len, int width,
		unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	lgui_vline(x, y, width, len, red, green, blue, alpha);
}

void lgui_vline(int x, int y, int len, int width,
		unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	int line, ypos, i;
	unsigned short *buf, pixel, srcpixel, ipixel;
	Rectangle rect;
	int cline, ccol, cwidth, cheight;

	x += translate_x;
	y += translate_y;

	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = len;
	if (lgui_clip_rect(&rect, &ccol, &cline, &cwidth, &cheight) == 0)
		return;

	lgui_set_dirty();

	ypos = y;
	pixel = RGB_TO_565(red, green, blue);
	ipixel = pixel;
	ypos += cline;
	for (line = cline; line < cheight; ++line) {
		if (ypos < 0 || ypos >= LGUI_HEIGHT) {
			++ypos;
			continue;
		}

		buf = lgui_buffer + x + ypos*LGUI_WIDTH;

		buf += ccol;
		if (alpha < 255) {
			for (i = ccol; i < cwidth; ++i) {
				srcpixel = *buf;
				PIXEL_MODULATE_ALPHA(ipixel, srcpixel, alpha, pixel)
				*buf = pixel;
				++buf;
			}
		} else {
			for (i = ccol; i < cwidth; ++i) {
				*buf = pixel;
				++buf;
			}
		}

		++ypos;
	}
}

void lgui_box(int x, int y, int width, int height, int linewidth,
			  unsigned char red, unsigned char green, unsigned char blue,
			  unsigned char alpha, unsigned int borderFlags)
{
	if (borderFlags & LGUI_TOP)
		lgui_hline(x, y, width, linewidth, red, green, blue, alpha);
	if (borderFlags & LGUI_BOTTOM)
		lgui_hline(x, y+height-linewidth, width, linewidth, red, green, blue, alpha);
	if (borderFlags & LGUI_LEFT)
		lgui_vline(x, y, height, linewidth, red, green, blue, alpha);
	if (borderFlags & LGUI_RIGHT)
		lgui_vline(x+width-linewidth, y, height, linewidth, red, green, blue, alpha);
}

// FIXME - needs clipping and alpha - unusable at the moment
void lgui_luminence_alpha_blitC(int destx, int desty, int imgx, int imgy, int imgdx, int imgdy,
							   int imgwidth, int imgheight, unsigned char *img,
							   unsigned char red, unsigned char green, unsigned char blue)
{
	unsigned short *buf;
	unsigned char *imgbuf;
	unsigned short pixel, srcpixel;
	unsigned char scale;
	int line, col, ypos, imgypos;
	
	ypos = desty;
	imgypos = imgy;
	for (line = 0; line < imgdy; ++line) {
		if (ypos < 0 || ypos >= LGUI_HEIGHT) {
			++ypos;
			++imgypos;
			continue;
		}
		buf = lgui_buffer + destx + ypos*LGUI_WIDTH;
		imgbuf = img + imgx + imgypos*imgwidth;

		for (col = 0; col < imgdx; ++col) {
			scale = (*imgbuf);
			if (scale > 0) {
				srcpixel = *buf;
				pixel = RGB_TO_565(red, green, blue);
				PIXEL_MODULATE_ALPHA(pixel, srcpixel, scale, *buf)
			}
			++buf;
			++imgbuf;
		}
		++ypos;
		++imgypos;
	}
}

void lgui_luminence_A4_blitC(int destx, int desty, int imgx, int imgy,
		int imgdx, int imgdy, int imgwidth, int imgheight, unsigned char *img,
		Color c, int mode, int isItalic)
{
	unsigned short *buf;
	unsigned char *imgbuf;
	unsigned short pixel, srcpixel;
	unsigned char scale, scaleorig;
	int line, col, ypos, imgypos;
	int cline, cwidth, ccol, cheight;
	Rectangle rect;
	
	destx += translate_x;
	desty += translate_y;

	ypos = desty;
	imgypos = imgy;
	
	rect.x = destx;
	rect.y = desty;
	rect.width = imgdx;
	rect.height = imgdy;
	if (lgui_clip_rect(&rect, &ccol, &cline, &cwidth, &cheight) == 0)
		return;

	lgui_set_dirty();

	pixel = RGB_TO_565(c.rgba.red, c.rgba.green, c.rgba.blue);
	ypos += cline;
	imgypos += cline;
	for (line = cline; line < cheight; ++line) {
		if (ypos < 0 || ypos >= LGUI_HEIGHT) {
			++ypos;
			++imgypos;
			continue;
		}
		buf = lgui_buffer + destx + ypos*LGUI_WIDTH;
		imgbuf = img + (imgx >> 1) + imgypos*((imgwidth+1)>>1);

		buf += ((ccol+1) >> 1) << 1;
		if (isItalic)
			buf += (cheight-line) >> 2;
		imgbuf += (ccol+1) >> 1;
		for (col = (ccol+1) >> 1; col < (cwidth+1) >> 1; ++col) {
			scaleorig = (*imgbuf);
			scale = scaleorig & 0xF0;
			if (mode == LGUI_MODE_ALPHA) {	
				if (scale > 0) {
					srcpixel = *buf;
					PIXEL_MODULATE_ALPHA(pixel, srcpixel, scale, *buf)
				}
			} else if (mode == LGUI_MODE_STENCIL) {
				if (scale > 0) {
					pixel = SCALE_PIXEL(scale, c.rgba.red, c.rgba.green, c.rgba.blue);
					*buf = pixel;
				}
			} else { /* REPLACE */
				pixel = SCALE_PIXEL(scale, c.rgba.red, c.rgba.green, c.rgba.blue);
				*buf = pixel;					
			}
			++buf;
			scale = (scaleorig << 4) & 0xF0;
			if (mode == LGUI_MODE_ALPHA) {
				if (scale > 0) {
					srcpixel = *buf;
					PIXEL_MODULATE_ALPHA(pixel, srcpixel, scale, *buf)
				}
			} else if (mode == LGUI_MODE_STENCIL) {
				if (scale > 0) {
					pixel = SCALE_PIXEL(scale, c.rgba.red, c.rgba.green, c.rgba.blue);
					*buf = pixel;
				}
			} else { /* REPLACE */
				pixel = SCALE_PIXEL(scale, c.rgba.red, c.rgba.green, c.rgba.blue);
				*buf = pixel;
			}
			
			++buf;
			++imgbuf;
		}
		++ypos;
		++imgypos;
	}
}

void lgui_blitRGB565(int destx, int desty, int imgx, int imgy,
        int imgwidth, int imgheight, int rowstride,
		unsigned char *img, int isStencil, unsigned char alpha)
{
    unsigned short *buf;
    unsigned char *imgbuf;
    unsigned short opixel, pixel, srcpixel, imgpixel;
    int line, col, ypos, imgypos;
	int cline, cwidth, ccol, cheight;
	Rectangle rect;
	
	destx += translate_x;
	desty += translate_y;

	ypos = desty;
	imgypos = imgy;
	
	rect.x = destx;
	rect.y = desty;
	rect.width = imgwidth;
	rect.height = imgheight;
	if (lgui_clip_rect(&rect, &ccol, &cline, &cwidth, &cheight) == 0)
		return;

	lgui_set_dirty();

	ypos += cline;
	imgypos += cline;
    for (line = cline; line < cheight; ++line) {
        if (ypos < 0 || ypos >= LGUI_HEIGHT) {
            ++ypos;
            ++imgypos;
            continue;
        }
        buf = lgui_buffer + destx + ypos*LGUI_WIDTH;
        imgbuf = img + imgx + imgypos*rowstride*2;

		buf += ccol;
		imgbuf += ccol << 1;
		if (alpha < 255) {
			for (col = ccol; col < cwidth; ++col) {
					pixel = *((unsigned short *)(imgbuf));
					opixel = pixel;
					srcpixel = *buf;
					imgpixel = pixel;
					PIXEL_MODULATE_ALPHA(imgpixel, srcpixel, alpha, opixel)
					if (isStencil) {
						if (pixel > 0)
							*buf = opixel;
					} else {
						*buf = opixel;
					}
				++buf;
				imgbuf +=2;
			}
		} else {
			for (col = ccol; col < cwidth; ++col) {
					pixel = *((unsigned short *)(imgbuf));
					opixel = pixel;
					if (isStencil) {
						if (pixel > 0)
							*buf = opixel;
					} else {
						*buf = opixel;
					}
				++buf;
				imgbuf +=2;
			}
		}
        ++ypos;
        ++imgypos;
    }
}

void lgui_blitRGB565A8(int destx, int desty, int imgx, int imgy,
        int imgwidth, int imgheight, int rowstride,
		unsigned char *img, unsigned char alpha)
{
    unsigned short *buf;
    unsigned char *imgbuf, scale;
    unsigned short pixel, srcpixel;
    int line, col, ypos, imgypos;
	int cline, cwidth, ccol, cheight;
	Rectangle rect;
	
	destx += translate_x;
	desty += translate_y;

	ypos = desty;
	imgypos = imgy;
	
	rect.x = destx;
	rect.y = desty;
	rect.width = imgwidth;
	rect.height = imgheight;
	if (lgui_clip_rect(&rect, &ccol, &cline, &cwidth, &cheight) == 0)
		return;

	lgui_set_dirty();

	ypos += cline;
	imgypos += cline;
    for (line = cline; line < cheight; ++line) {
        if (ypos < 0 || ypos >= LGUI_HEIGHT) {
            ++ypos;
            ++imgypos;
            continue;
        }
        buf = lgui_buffer + destx + ypos*LGUI_WIDTH;
        imgbuf = img + (imgx + imgypos*rowstride)*3;

		buf += ccol;
		imgbuf += ccol *3;
		if (alpha < 255) {
			for (col = ccol; col < cwidth; ++col) {
				pixel = *(imgbuf);
				pixel |= *(imgbuf+1) << 8;
				scale = *((unsigned char *)(imgbuf+2));
				scale = (unsigned char)((int)scale*((int)alpha<<7)/(0xFF<<7));
				srcpixel = *buf;
				PIXEL_MODULATE_ALPHA(pixel, srcpixel, scale, *buf)
				/* *buf = pixel;*/
				++buf;
				imgbuf +=3;
			}
		} else {
			for (col = ccol; col < cwidth; ++col) {
				pixel = *(imgbuf);
				pixel |= *(imgbuf+1) << 8;
				scale = *((unsigned char *)(imgbuf+2));
				srcpixel = *buf;
				PIXEL_MODULATE_ALPHA(pixel, srcpixel, scale, *buf)
				/* *buf = pixel;*/
				++buf;
				imgbuf +=3;
			}
		}
        ++ypos;
        ++imgypos;
    }
}

// FIXME - needs clipping
void lgui_black_alpha_blitRGB(int destx, int desty, int imgx, int imgy,
	 	int imgwidth, int imgheight, unsigned char *img)
{
	unsigned short *buf;
	unsigned char *imgbuf;
	unsigned short pixel;
	int line, col, ypos, imgypos;
	
	destx += translate_x;
	desty += translate_y;

	ypos = desty;
	imgypos = imgy;
	for (line = 0; line < imgheight; ++line) {
		if (ypos < 0 || ypos >= LGUI_HEIGHT) {
			++ypos;
			++imgypos;
			continue;
		}
		buf = lgui_buffer + destx + ypos*LGUI_WIDTH;
		imgbuf = img + imgx + imgypos*imgwidth*3;

		for (col = 0; col < imgwidth; ++col) {
				pixel = RGB_TO_565(imgbuf[0], imgbuf[1], imgbuf[2]);
				if (pixel > 0)
					*buf = pixel;
			++buf;
			imgbuf +=3;
		}
		++ypos;
		++imgypos;
	}

}

// FIXME - needs clipping
void lgui_alpha_blitRGBA(int destx, int desty, int imgx, int imgy,
	 	int imgwidth, int imgheight, unsigned char *img)
{
	unsigned short *buf;
	unsigned char *imgbuf, scale;
	unsigned short pixel, srcpixel;
	int line, col, ypos, imgypos;
	
	destx += translate_x;
	desty += translate_y;

	ypos = desty;
	imgypos = imgy;
	for (line = 0; line < imgheight; ++line) {
		if (ypos < 0 || ypos >= LGUI_HEIGHT) {
			++ypos;
			++imgypos;
			continue;
		}
		buf = lgui_buffer + destx + ypos*LGUI_WIDTH;
		imgbuf = img + imgx + imgypos*imgwidth*4;

		for (col = 0; col < imgwidth; ++col) {
				scale = imgbuf[3];
				srcpixel = *buf;
				pixel = RGB_TO_565(imgbuf[0], imgbuf[1], imgbuf[2]);
				PIXEL_MODULATE_ALPHA(pixel, srcpixel, scale, *buf)
			++buf;
			imgbuf +=4;
		}
		++ypos;
		++imgypos;
	}

}

/* Wu Citcle implementation */
static unsigned char I=255;           /* Intensity. */
static int A=1;             /*Accuracy determained by argument of the root function D.*/

/*  
 * R < 10^A
 * Memory = 4 * 10^(2A)
 * For example:
 *   for memory 40K (small machines), screens not more than about 100*100,
 *   for memory 4Meg, screens about 1000*1000.
 */

/*
 *  Auxiliary:
 */
static unsigned char D[100];             /* Precalcuated reversed fractions of roots.*/
static int POWERES_OF_TEN[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

 static void initD(){
    int p = POWERES_OF_TEN[A*2];
	int i, f;
    /* D = new int[p]; */
    for(i=0; i<p; i++ ){  
        f = (       (int)(I*sqrt((double)i))       )%I;
        D[i] =  (unsigned char)(f == 0 ? 0 : I-f);
    }
    D[0] = 0;  /* Secure from floating point errors on poor platforms. */
}

static int putpixelX = 0;
static int putpixelY = 0;
static void putpixelAlpha(int x, int y, unsigned short pixel,
					 unsigned char intensity)
{
	unsigned short srcpixel;
	unsigned short *buf;

	x += translate_x;
	y += translate_y;

	x+=putpixelX;
	y+=putpixelY;

	if (x < CLIP.x || x > CLIP.x + CLIP.width ||
			y < CLIP.y || y > CLIP.y + CLIP.height) {
		return;
	}

    if (y < 0 || y > LGUI_HEIGHT)
        return;

	lgui_set_dirty();

	buf = lgui_buffer+x+y*LGUI_WIDTH;
	srcpixel = *buf;
	PIXEL_MODULATE_ALPHA(pixel, srcpixel, intensity, *buf)
}

     /*Input: uses precalculated variables D and I.*/
 void drawArch( int R, unsigned char red, unsigned char green, unsigned char blue , int arcs) {
    int y=0;
    int x=R;
    int d=0;
	unsigned char dnew;
	unsigned short pixel;

	pixel = RGB_TO_565(red, green, blue);
    while( y<x ) {
        dnew = D[R*R-y*y];
        if( dnew < d ) x--;
		/* lower half or circle */
		if (arcs & LGUI_CORNER_BOTTOMRIGHT) {
		/* / upper */
        putpixelAlpha(x-1,y,   pixel, dnew);
        putpixelAlpha(x,  y,   pixel, (I-dnew));
		/* / lower */
        putpixelAlpha(y, x-1,   pixel, dnew);
        putpixelAlpha(y,   x,   pixel, (I-dnew));
		}

		if (arcs & LGUI_CORNER_BOTTOMLEFT) {
		/* \ upper */
        putpixelAlpha(-(x-1),y, pixel, dnew);
        putpixelAlpha(-x,  y,   pixel, (I-dnew));
		/* \ lower */
		putpixelAlpha(-y, x-1,  pixel, dnew);
        putpixelAlpha(-y,   x,  pixel, (I-dnew));
		}

		/* upper half or circle */
		if (arcs & LGUI_CORNER_TOPRIGHT) {
		/* \ upper */
		putpixelAlpha(x-1,-y,   pixel, dnew);
        putpixelAlpha(x,  -y,   pixel, (I-dnew));
		/* \ lower */
        putpixelAlpha(y, -(x-1),   pixel, dnew);
        putpixelAlpha(y,   -x,   pixel, (I-dnew));
		}

		if (arcs & LGUI_CORNER_TOPLEFT) {
		/* / upper */
        putpixelAlpha(-(x-1),-y,   pixel, dnew);
        putpixelAlpha(-x,  -y,   pixel, (I-dnew));
		/* / lower */
		putpixelAlpha(-y, -(x-1),   pixel, dnew);
        putpixelAlpha(-y,   -x,   pixel, (I-dnew));
		}

        y++;
        d = dnew;
    }    
}

 void lgui_aacircle(int x, int y, int rad, int arcs, unsigned char red, unsigned char green, unsigned char blue)
 {
	putpixelX = x;
	putpixelY = y;
	drawArch(rad, red, green, blue, arcs);
 }

 void lgui_roundedbox_line(int x, int y, int width, int height, int radius,
						  unsigned char red, unsigned char green, unsigned char blue,
						  unsigned int borderFlags, unsigned int borderCornerFlags)
 {
	int radAdd;
	if (borderCornerFlags & LGUI_CORNER_TOPLEFT)
		lgui_aacircle(x+radius, y+radius, radius, LGUI_CORNER_TOPLEFT, red, green, blue);
	if (borderCornerFlags & LGUI_CORNER_TOPRIGHT)
		lgui_aacircle(x+width-radius, y+radius, radius, LGUI_CORNER_TOPRIGHT, red, green, blue);
	if (borderCornerFlags & LGUI_CORNER_BOTTOMLEFT)
		lgui_aacircle(x+radius, y+height-radius, radius, LGUI_CORNER_BOTTOMLEFT, red, green, blue);
	if (borderCornerFlags & LGUI_CORNER_BOTTOMRIGHT)
		lgui_aacircle(x+width-radius, y+height-radius, radius, LGUI_CORNER_BOTTOMRIGHT, red, green, blue);

	if (borderFlags & LGUI_TOP) {
		radAdd = 0;
		if (borderCornerFlags & LGUI_CORNER_TOPLEFT)
			radAdd += radius;
		if (borderCornerFlags & LGUI_CORNER_TOPRIGHT)
			radAdd += radius;
		lgui_hline(x+ ((borderCornerFlags & LGUI_CORNER_TOPLEFT) ? radius : 0),
			y, width-radAdd, 1, red, green, blue, 0xFF);
	}

	if (borderFlags & LGUI_BOTTOM) {
		radAdd = 0;
		if (borderCornerFlags & LGUI_CORNER_BOTTOMLEFT)
			radAdd += radius;
		if (borderCornerFlags & LGUI_CORNER_BOTTOMRIGHT)
			radAdd += radius;
		lgui_hline(x+ ((borderCornerFlags & LGUI_CORNER_BOTTOMLEFT) ? radius : 0),
			y+height, width-radAdd, 1, red, green, blue, 0xFF);
	}

	if (borderFlags & LGUI_LEFT) {
		radAdd = 0;
		if (borderCornerFlags & LGUI_CORNER_TOPLEFT)
			radAdd += radius;
		if (borderCornerFlags & LGUI_CORNER_BOTTOMLEFT)
			radAdd += radius;
		lgui_vline(x,
			y+((borderCornerFlags & LGUI_CORNER_TOPLEFT) ? radius : 0),
			height-radAdd+1, 1, red, green, blue, 0xFF);
	}

	if (borderFlags & LGUI_RIGHT) {
		radAdd = 0;
		if (borderCornerFlags & LGUI_CORNER_TOPRIGHT)
			radAdd += radius;
		if (borderCornerFlags & LGUI_CORNER_BOTTOMRIGHT)
			radAdd += radius;
		lgui_vline(x+width,
			y+((borderCornerFlags & LGUI_CORNER_TOPRIGHT) ? radius : 0),
			height-radAdd+1, 1, red, green, blue, 0xFF);
	}
 }

 void lgui_roundedbox_fill(int x, int y, int width, int height, int radius,
						  unsigned char red, unsigned char green, unsigned char blue,
						  unsigned char alpha, unsigned int cornerFlags)
 {
	int radAdd;
	if (cornerFlags & LGUI_CORNER_TOPLEFT)
		lgui_circlefill(x+radius, y+radius, radius, red, green, blue, alpha);
	if (cornerFlags & LGUI_CORNER_TOPRIGHT)
		lgui_circlefill(x+width-radius, y+radius, radius, red, green, blue, alpha);
	if (cornerFlags & LGUI_CORNER_BOTTOMLEFT)
		lgui_circlefill(x+radius, y+height-radius, radius, red, green, blue, alpha);
	if (cornerFlags & LGUI_CORNER_BOTTOMRIGHT)
		lgui_circlefill(x+width-radius, y+height-radius, radius, red, green, blue, alpha);

	radAdd = 0;
	if (cornerFlags & LGUI_CORNER_TOPLEFT)
		radAdd += radius;
	if (cornerFlags & LGUI_CORNER_TOPRIGHT)
		radAdd += radius;
	lgui_hline(x+((cornerFlags & LGUI_CORNER_TOPLEFT) ? radius : 0), y,
		width-radAdd, radius, red, green, blue, alpha);
	
	lgui_hline(x, y+radius, width, height-radius*2, red, green, blue, alpha);

	radAdd = 0;
	if (cornerFlags & LGUI_CORNER_BOTTOMLEFT)
		radAdd += radius;
	if (cornerFlags & LGUI_CORNER_BOTTOMRIGHT)
		radAdd += radius;
	lgui_hline(x+((cornerFlags & LGUI_CORNER_BOTTOMLEFT) ? radius : 0),
		y+height-radius, width-radAdd, radius, red, green, blue, alpha);
 }

void lgui_rbox_gradient(Gradient *g, int x, int y, int width, int height, int radius,
						unsigned int cornerFlags)
{
	int hstart, hend;
	int stop, maxStop;
	Color cstart, cend;
	float cr;
	Rectangle rect;
	unsigned int red;
	unsigned int green;
	unsigned int blue;
	int line, ypos, i;
	unsigned short *buf, pixel;
	int iheight, iwidth, imod, imodl, imodr;
	int cline, cheight;
	int startx, cstartx, cendx;
	int istart;

	EMO_ASSERT(g != NULL, "lgui rbox missing gradient")

	x += translate_x;
	y += translate_y;

	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	hstart = height;
	cstart = gradient_getStop(g, 0, &hstart);
	hend = height;
	cend = gradient_getStop(g, 1, &hend);
	stop = 1;
	maxStop = gradient_stops(g);
	
	if (!lgui_clip_rect(&rect, &cstartx, &cline, &cendx, &cheight))
		return;

	lgui_set_dirty();

	do {
		if (hstart != hend) {
			/*lgui_vertical_gradient(cstart.rgba.red, cstart.rgba.green, cstart.rgba.blue,
					cend.rgba.red, cend.rgba.green, cend.rgba.blue,
					startx, starty+hstart, width, hend-hstart);*/
			startx = x;
			ypos = y+hstart;
			iheight = hend-hstart;
			
			rect.y = ypos;
			rect.height = iheight;
			if (lgui_clip_rect(&rect, &cstartx, &cline, &cendx, &cheight) == 0) {
				goto skip_gradient_step;
			}
			ypos += cline;
			for (line = cline; line < cheight; ++line) {
				if (ypos < 0 || ypos >= LGUI_HEIGHT) {
					++ypos;
					continue;
				}
				GRADIENT_STEP(red, cstart.rgba.red, cend.rgba.red, iheight, line)
				GRADIENT_STEP(green, cstart.rgba.green, cend.rgba.green, iheight, line)
				GRADIENT_STEP(blue, cstart.rgba.blue, cend.rgba.blue, iheight, line)

				pixel = RGB_TO_565(red, green, blue);

				imodl = 0;
				imodr = 0;
				if (ypos-y < radius) {
					imod = radius-(ypos-y);
					cr = (float)cos((float)imod/(float)(radius)*M_PI/2.0f);
					imod = radius - (int)(cr*(float)radius);
					if (cornerFlags & LGUI_CORNER_TOPLEFT)
						imodl = imod;
					if (cornerFlags & LGUI_CORNER_TOPRIGHT)
						imodr = imod;
				} else if (ypos-y > height-radius) {
					imod = (height+y)-ypos;
					cr = (float)cos((radius-imod)/(float)radius*M_PI/2.0f);
					imod = radius - (int)(cr*(float)radius);
					if (cornerFlags & LGUI_CORNER_BOTTOMLEFT)
						imodl = imod;
					if (cornerFlags & LGUI_CORNER_BOTTOMRIGHT)
						imodr = imod;
				} else {
					imod = 0;
				}

				buf = lgui_buffer + startx + ypos*LGUI_WIDTH;
				buf += imodl;
				istart = 0;

				if (cstartx > imodl) {
					buf += cstartx-imodl;
					istart = cstartx;
				} else {
					istart = imodl;
				}

				if (width-imodr+1 > cendx) {
					iwidth = cendx;
				} else {
					iwidth = width - imodr + 1;
				}

				for (i = istart; i < iwidth; ++i) {
					*buf = pixel;
					++buf;
				}
				++ypos;
			}
		}
skip_gradient_step:
		++stop;
		cstart = cend;
		hstart = hend;
		hend = height;
		if (stop < maxStop)
			cend = gradient_getStop(g, stop, &hend);
	} while(stop < maxStop);
}

void lgui_circlefill(int xc, int yc, int r,
		  unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	short x = 0;
	short y = r-1;
	short d = 3 - 2*r;
	short diagonalInc = 10 - 4*r;
	short rightInc = 6;

	while (x <= y) {
		lgui_hline(xc-x, yc+y, (x<<1)+1, 1, red, green, blue, alpha);
		lgui_hline(xc-x, yc-y, (x<<1)+1, 1, red, green, blue, alpha);
		lgui_hline(xc-y, yc+x, (y<<1)+1, 1, red, green, blue, alpha);
		lgui_hline(xc-y, yc-x, (y<<1)+1, 1, red, green, blue, alpha);

		if (d >=  0) {
			d += diagonalInc;
			diagonalInc += 8;
			y -= 1;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		x += 1;
	}
}

void lgui_draw_font(int x, int y, int maxw, int maxh, const char *utf8, Font *f, Color c,
		int isBold)
{
	const char *p;
	unsigned int val;
	int adv;
	int xadvance, yadvance, width, height, baselinedy;
	unsigned char *data;
	int fontHeight;
	
	if (utf8 == NULL || f == NULL)
		return;

	fontHeight = font_getHeight(f);
	maxw += x;
	maxh += y;

	p = utf8;
	while (*p != 0) {
		val = UTF8toUTF32(p, &adv);
		data = (unsigned char *)font_getGlyph(f, val, isBold, A4, &width, &height,
				&xadvance, &yadvance, &baselinedy);
		if (!(x + xadvance + translate_x < CLIP.x ||
				x + translate_x > CLIP.x + CLIP.width ||
				y + fontHeight + translate_y < CLIP.y ||
				y + translate_y > CLIP.y + CLIP.height)) {
			if (data == NULL)
				emo_printf("lgui_draw_font() Glyph missing: %s", utf8);
			else
				lgui_luminence_A4_blitC(x, y+fontHeight-baselinedy, 0, 0, width, height,
						width, height, data, c, LGUI_MODE_ALPHA, 0);
		}
		x += xadvance;
		y += yadvance;
		if (x > maxw || y > maxh)
			break;
		p += adv;
	}
}

struct EscapeSequenceState_t {
	int isBold;
	int isUnderline;
	int isItalic;
	Color basecolor;
	Color currentColor;
	int useBackground;
	Color backgroundColor;
};
typedef struct EscapeSequenceState_t EscapeSequenceState;

/* assumes p[0] is '<' */
static int lgui_process_escape_seq(const char *p, EscapeSequenceState *state)
{
	EMO_ASSERT_INT(p != NULL, 0, "lgui process escape seq on NULL string")
	EMO_ASSERT_INT(state != NULL, 0, "lgui process escape seq missing state")

	if (p[1] == 'b' && p[2] == '>') {
		state->isBold = 1;
		return 3;
	} else if (p[1] == '/' && p[2] == 'b' && p[3] == '>') {
		state->isBold = 0;
		return 4;
	} else if (p[1] == 'u' && p[2] == '>') {
		state->isUnderline = 1;
		return 3;
	} else if (p[1] == '/' && p[2] == 'u' && p[3] == '>') {
		state->isUnderline = 0;
		return 4;
	} else if (p[1] == 'i' && p[2] == '>') {
		state->isItalic = 1;
		return 3;
	} else if (p[1] == '/' && p[2] == 'i' && p[3] == '>') {
		state->isItalic = 0;
		return 4;
	} else if (p[1] == 'c' && p[10] == '>') {
		state->currentColor.value = strtoul(p+2, NULL, 16);
		//sscanf(p+2, "%x", &state->currentColor.value);
		return 11;
	} else if (p[1] == '/' && p[2] == 'c' && p[3] == '>') {
		state->currentColor.value = state->basecolor.value;
		return 4;
	} else if (p[1] == 'g' && p[10] == '>') {
		state->backgroundColor.value = strtoul(p+2, NULL, 16);
		//sscanf(p+2, "%x", &state->backgroundColor.value);
		state->useBackground = 1;
		return 11;
	} else if (p[1] == '/' && p[2] == 'g' && p[3] == '>') {
		state->useBackground = 0;
		return 4;
	}
	return 0;
}

static int lgui_text_line_complex(const char *p, int x, Font *f, EscapeSequenceState *statein)
{
	int idx = 0, lastStart = 0, adv;
	EscapeSequenceState state;
	unsigned int val;
	int width, height, xadvance, yadvance, baselinedy;

	EMO_ASSERT_INT(p != NULL, 0, "lgui line complex on NULL string")
	
	if (statein)
		state = *statein;
	else
		memset(&state, 0, sizeof(EscapeSequenceState));
	while (*p != 0) {
another_escape:
		if (statein && *p == '<') {
			adv = lgui_process_escape_seq(p, &state);
			if (adv > 0) {
				p += adv;
				idx += adv;
				goto another_escape;
			}
		}
		if (*p == 10 || *p == 13) {
			return idx+1;
		}
		val = UTF8toUTF32(p, &adv);
		p += adv;
		idx += adv;
		font_getGlyph(f, val, state.isBold, A4, &width, &height,
				&xadvance, &yadvance, &baselinedy);
		x += xadvance;
		if (x+xadvance > CLIP.x + CLIP.width) {
			return lastStart;
		}
		if (p[-1] == ' ')
			lastStart = idx;
	}
	return idx;
}

void lgui_complex_draw_font(int _x, int y, int maxw, int maxh, const char *utf8,
		Font *f, Color c, int cursorindex, int startindex, int *windowPercent,
		int *windowOffset, int useEscapeSeqs, Rectangle *cursorBox)
{
	const char *p, *startp;
	unsigned int val;
	int adv, i, pos, x, endcount, innercount;
	int xadvance, yadvance, width, height, baselinedy;
	unsigned char *data;
	int fontHeight;
	EscapeSequenceState state;

	if (utf8 == NULL || f == NULL)
		return;

	fontHeight = font_getHeight(f);
	x = _x;
	maxw += x;
	maxh += y;
	
	state.isBold = 0;
	state.isItalic = 0;
	state.isUnderline = 0;
	state.basecolor = c;
	state.currentColor = c;
	state.useBackground = 0;

	innercount = 0;
	p = utf8;
	for (i = 0; i < startindex; ++i) {
		if (*p == 0)
			return;
more_escape_skip:
		if (useEscapeSeqs && *p == '<') {
			adv = lgui_process_escape_seq(p, &state);
			if (adv > 0) {
				p += adv;
				goto more_escape_skip;
			}
		}
		UTF8toUTF32(p, &adv);
		p += adv;
	}
next_line:
	startp = p;
	pos = lgui_text_line_complex(p, x, f, &state);
	while (*p != 0 && p-startp < pos) {
more_escape:
		if (useEscapeSeqs && *p == '<') {
			adv = lgui_process_escape_seq(p, &state);
			if (adv > 0) {
				p += adv;
				goto more_escape;
			}
		}
		val = UTF8toUTF32(p, &adv);
		data = (unsigned char *)font_getGlyph(f, val, state.isBold, A4, &width, &height,
				&xadvance, &yadvance, &baselinedy);
		if (!(x + xadvance + translate_x < CLIP.x ||
				x + translate_x > CLIP.x + CLIP.width ||
				y + fontHeight + translate_y < CLIP.y ||
				y + translate_y > CLIP.y + CLIP.height)) {
			if (data == NULL) {
				/*emo_printf(" Glyph missing" NL);*/
			} else {
				++innercount;
				if (state.useBackground)
					lgui_hline(x, y+1, xadvance, fontHeight+2,
							state.backgroundColor.rgba.red, state.backgroundColor.rgba.green,
							state.backgroundColor.rgba.blue, state.backgroundColor.rgba.alpha);
				lgui_luminence_A4_blitC(x, y+fontHeight-baselinedy, 0, 0, width, height,
						width, height, data, state.currentColor, LGUI_MODE_ALPHA, state.isItalic);
				if (state.isUnderline)
					lgui_hline(x, y+fontHeight+1, xadvance, 1,
							state.currentColor.rgba.red, state.currentColor.rgba.green,
							state.currentColor.rgba.blue, state.currentColor.rgba.alpha);
			}
			if (i == cursorindex && cursorBox != NULL) {
				cursorBox->x = x;
				cursorBox->y = y+2;
				cursorBox->width = 2;
				cursorBox->height = fontHeight;
			}
		}
		++i;
		x += xadvance;
		y += yadvance;
		if (x > maxw)
			break;
		p += adv;
	}
	if (*p != 0) {
		x = _x;
		y += fontHeight;
		if (y+fontHeight+2 <= maxh)
			goto next_line;
	}
	endcount = 0;
	while (*p != 0) {
		++endcount;
more_escape_end:
		if (useEscapeSeqs && *p == '<') {
			adv = lgui_process_escape_seq(p, &state);
			if (adv > 0) {
				p += adv;
				goto more_escape_end;
			}
		}
		UTF8toUTF32(p, &adv);
		p += adv;
	}
	if (i == cursorindex && cursorBox != NULL) {
		cursorBox->x = x;
		cursorBox->y = y+2;
		cursorBox->width = 2;
		cursorBox->height = fontHeight;
	}

	if (innercount + endcount + startindex == 0) {
		*windowPercent = 100;
		*windowOffset = 0;
	} else {
		*windowPercent = innercount*100 / (innercount + endcount + startindex);
		*windowOffset = startindex*100 / (innercount + endcount + startindex);
	}
}

void lgui_measure_font(const char *utf8, Font *f, int isBold, IPoint *output)
{
	const char *p;
	unsigned int val;
	int adv;
	int xadvance, yadvance, width, height, baselinedy;
	unsigned char *data;
	int fontHeight;
	
	if (f == NULL || utf8 == NULL) {
		output->x = 0;
		output->y = 0;
		return;
	}

	fontHeight = font_getHeight(f);
	output->x = 0;
	output->y = fontHeight+1;

	p = utf8;
	while (*p != 0) {
		val = UTF8toUTF32(p, &adv);
		data = (unsigned char *)font_getGlyph(f, val, isBold, A4, &width,
                &height, &xadvance, &yadvance, &baselinedy);
		if (data == NULL) {
			emo_printf("lgui_measure_font() Glyph missing: %s" NL, utf8);
		} else {
			output->x += xadvance;
		}
		p += adv;
	}
}

void lgui_measure_font_complex(const char *utf8, Font *f, IPoint *output)
{
	const char *p;
	int adv;
	int fontHeight;
	EscapeSequenceState state;
	
	if (f == NULL || utf8 == NULL) {
		output->x = 0;
		output->y = 0;
		return;
	}

	fontHeight = font_getHeight(f);
	output->y = 0;
	
	lgui_clip_push();
	CLIP.x = 0;
	CLIP.width = output->x;

	p = utf8;
	while (*p != 0) {
		adv = lgui_text_line_complex(p, 0, f, &state);
		p += adv;
		if (adv == 0)
			break;
		output->y += fontHeight+1;
	}

	if (output->y == 0)
		output->y = fontHeight+1;

	lgui_clip_pop();
}

void lgui_clip_set(Rectangle *rect)
{
	EMO_ASSERT(rect != NULL, "lgui clip set missing rect")

	if (rect->y < 0)
		emo_printf("" NL);

	CLIP.x = rect->x;
	CLIP.y = rect->y;
	CLIP.width = rect->width;
	CLIP.height = rect->height;
}

void lgui_clip_union(Rectangle *rect)
{
	int yh, xh, nyh, nxh;

	EMO_ASSERT(rect != NULL, "lgui clip union missing rect")

	if (rect->y < 0)
		emo_printf("" NL);

	yh = CLIP.y + CLIP.height;
	xh = CLIP.x + CLIP.width;

	nyh = rect->y + rect->height;
	nxh = rect->x + rect->width;

	if (rect->x < CLIP.x) CLIP.x = rect->x;
	if (rect->y < CLIP.y) CLIP.y = rect->y;
	if (nxh > xh)
		CLIP.width = nxh - CLIP.x;
	else
		CLIP.width = xh - CLIP.x;
	if (nyh > yh)
		CLIP.height = nyh - CLIP.y;
	else
		CLIP.height = yh - CLIP.y;
}

void lgui_clip_and(Rectangle *rect)
{
	int yh, xh, nyh, nxh;

	EMO_ASSERT(rect != NULL, "lgui clip add missing rect")

	if (rect->y < 0)
		emo_printf("" NL);

	yh = CLIP.y + CLIP.height;
	xh = CLIP.x + CLIP.width;

	nyh = rect->y + rect->height;
	nxh = rect->x + rect->width;

	if (rect->x > CLIP.x) CLIP.x = rect->x;
	if (rect->y > CLIP.y) CLIP.y = rect->y;
	if (nxh < xh)
		CLIP.width = nxh - CLIP.x;
	else
		CLIP.width = xh - CLIP.x;
	if (nyh < yh)
		CLIP.height = nyh - CLIP.y;
	else
		CLIP.height = yh - CLIP.y;
}

void lgui_clip_identity(void)
{
	clip_index = 0;
	CLIP.x = 0;
	CLIP.y = 0;
	CLIP.width = LGUI_WIDTH;
	CLIP.height = LGUI_HEIGHT;
}

void lgui_clip_push(void)
{
	if (clip_index >= CLIP_STACK_SIZE-1) {
		emo_printf("Attempted to push clip stack and overflow\n");
		return;
	}
	++clip_index;
	CLIP.x = clip_stack[clip_index-1].x;
	CLIP.y = clip_stack[clip_index-1].y;
	CLIP.width = clip_stack[clip_index-1].width;
	CLIP.height = clip_stack[clip_index-1].height;
}

void lgui_clip_pop(void)
{
	--clip_index;
	if (clip_index < 0) {
		emo_printf("Attempted to pop lgui clip stack below 0\n");
		clip_index = 0;
	}
}

int lgui_clip_rect(Rectangle *rect, int *lowx, int *lowy,
		int *highx, int *highy)
{
	int val;

	EMO_ASSERT_INT(rect != NULL, 0, "lgui clip rect missing rect")
	EMO_ASSERT_INT(lowx != NULL, 0, "lgui clip rect missing lowx")
	EMO_ASSERT_INT(lowy != NULL, 0, "lgui clip rect missing lowy")
	EMO_ASSERT_INT(highx != NULL, 0, "lgui clip rect missing highx")
	EMO_ASSERT_INT(highy != NULL, 0, "lgui clip rect missing highy")

	/* Y */

	/* assume y inside clip region, possible partial draw */
	*lowy = 0;
	
	/* y beyond clip region, no-draw */
	if (rect->y > CLIP.y + CLIP.height)
		return 0;

	/* y prior to clip region, possible partial draw */
	if (rect->y < CLIP.y)
		*lowy = CLIP.y - rect->y;

	/* if our start is beyond our finish, return */
	if (*lowy > rect->height)
		return 0;

	/* Y high */
	val = rect->y + rect->height;

	/* assume y+height inside clip, possible partial draw */
	*highy = rect->height;

	/* upper prior to start of clip region, no-draw */
	if (val < CLIP.y)
		return 0;

	/* upper post clip region, possible partial draw */
	if (val > CLIP.y + CLIP.height)
		*highy = CLIP.y + CLIP.height - rect->y;

	/* zero size or negative size region, no-draw */
	if (*highy <= 0)
		return 0;

	/* X */

	/* assume x inside clip region, possible partial draw */
	*lowx = 0;
	
	/* x beyond clip region, no-draw */
	if (rect->x > CLIP.x + CLIP.width)
		return 0;

	/* x prior to clip region, possible bartial draw */
	if (rect->x < CLIP.x)
		*lowx = CLIP.x - rect->x;

	/* if our start is beyond our finish, return */
	if (*lowx > rect->width)
		return 0;

	/* X high */
	val = rect->x + rect->width;

	/* assume x+width inside clip, possible partial draw */
	*highx = rect->width;

	/* upper prior to start of clip region, no-draw */
	if (val < CLIP.x)
		return 0;

	/* upper post clip region, possible partial draw */
	if (val > CLIP.x + CLIP.width)
		*highx = CLIP.x + CLIP.width - rect->x;

	/* zero size or negative size region, no-draw */
	if (*highx <= 0)
		return 0;

	return 1;
}

void lgui_translate(int x, int y)
{
	translate_x += x;
	translate_y += y;
}

void lgui_get_translate(int *x, int *y)
{
	*x = translate_x;
	*y = translate_y;
}

#define REDRAW_REGION_COUNT 30
static int region_index = 0;
static Rectangle region_blit[REDRAW_REGION_COUNT];
#define REGION (region_blit[region_index])

void lgui_push_region(void)
{
	REGION.x = CLIP.x;
	REGION.y = CLIP.y;
	REGION.width = CLIP.width;
	REGION.height = CLIP.height;
	++region_index;
}

int lgui_index_count(void)
{
	return region_index;
}

Rectangle *lgui_get_region(int index)
{
	return &region_blit[index];
}

static int lguiDirty = 0;

void lgui_blit_done(void)
{
	region_index = 0;
	lguiDirty = 0;
}

void lgui_set_dirty(void)
{
	lguiDirty = 1;
}

int lgui_is_dirty(void)
{
	return lguiDirty;
}
