///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2010 Robert P. Wolf                       //
//                                                                               //
// Permission is hereby granted, free of charge, to any person obtaining a copy  //
// of this software and associated documentation files (the "Software"), to deal //
// in the Software without restriction, including without limitation the rights  //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     //
// copies of the Software, and to permit persons to whom the Software is         //
// furnished to do so, subject to the following conditions:                      //
//                                                                               //
// The above copyright notice and this permission notice shall be included in    //
// all copies or substantial portions of the Software.                           //
//                                                                               //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     //
// THE SOFTWARE.                                                                 //
///////////////////////////////////////////////////////////////////////////////////

#include "lanczos.h"

static double sinc (double x) {if (x == 0.0) return 1.0; x *= 3.14159265358979323846; return sin (x) / x;}

static double lanczos3 (double x) {
	if (x < -3.0) return 0.0;
	if (x > 3.0) return 0.0;
	return sinc (x) * sinc (x / 3.0);
}

#define CLBANG(c, c1, c2, lx)\
	colour = (int) (0.4999 + c2 * lx + c1 - c1 * lx);\
	colours [ind] . c = (unsigned char) (colour < 0 ? 0 : (colour > 255 ? 255 : colour));
void initialise_lanczos_colours (lanczos_colour * colours, double r1, double g1, double b1, double r2, double g2, double b2) {
	for (int ind = 0; ind < 256; ind++) {
		double shift = (double) ind / 256.0;
		double lm2 = lanczos3 (-2.0 - shift);
		double lm1 = lanczos3 (-1.0 - shift);
		double l0 = lanczos3 (-shift);
		double l1 = lanczos3 (1.0 - shift);
		double l2 = lanczos3 (2.0 - shift);
		double l3 = lanczos3 (3.0 - shift);
		int colour;
		CLBANG(rm2, r1, r2, lm2); CLBANG(gm2, g1, g2, lm2); CLBANG(bm2, b1, b2, lm2); CLBANG(am2, 0, 255, lm2); //colours [ind] . am2 = lm2;
		CLBANG(rm1, r1, r2, lm1); CLBANG(gm1, g1, g2, lm1); CLBANG(bm1, b1, b2, lm1); CLBANG(am1, 0, 255, lm1); //colours [ind] . am1 = lm1;
		CLBANG(r0, r1, r2, l0); CLBANG(g0, g1, g2, l0); CLBANG(b0, b1, b2, l0); CLBANG(a0, 0, 255, l0); //colours [ind] . a0 = l0;
		CLBANG(r1, r1, r2, l1); CLBANG(g1, g1, g2, l1); CLBANG(b1, b1, b2, l1); CLBANG(a1, 0, 255, l1); //colours [ind] . a1 = l1;
		CLBANG(r2, r1, r2, l2); CLBANG(g2, g1, g2, l2); CLBANG(b2, b1, b2, l2); CLBANG(a2, 0, 255, l2); //colours [ind] . a2 = l2;
		CLBANG(r3, r1, r2, l3); CLBANG(g3, g1, g2, l3); CLBANG(b3, b1, b2, l3); CLBANG(a3, 0, 255, l3); //colours [ind] . a3 = l3;
	}
}

#ifdef USE_PACKED_COLOURS
#define RKBANG(c)\
	dc . SetPen (wxPen (wxColour (lpc -> c)));\
	dc . DrawLine (base_x, y1, base_x, y2); base_x++;
void draw_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	colour_table += (int) (256.0 * (x - (double) base_x));
	lanczos_packed_colour * lpc = (lanczos_packed_colour *) colour_table;
	base_x -= 2;
	if (base_x < 0) return;
	RKBANG(cm2);
	RKBANG(cm1);
	RKBANG(c0);
	RKBANG(c1);
	RKBANG(c2);
	RKBANG(c3);
}
void draw_line (wxDC & dc, lanczos_colour * left_colour_table, lanczos_colour * right_colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	int shift = (int) (256.0 * (x - (double) base_x));
	left_colour_table += shift;
	lanczos_packed_colour * lpc = (lanczos_packed_colour *) left_colour_table;
	base_x -= 2;
	if (base_x < 0) return;
	RKBANG(cm2);
	RKBANG(cm1);
	RKBANG(c0);
	right_colour_table += shift;
	lpc = (lanczos_packed_colour *) right_colour_table;
	RKBANG(c1);
	RKBANG(c2);
	RKBANG(c3);
}
void draw_thin_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	int offset = (int) (256.0 * (x - (double) base_x));
	colour_table += offset;
	lanczos_packed_colour * lpc = (lanczos_packed_colour *) colour_table;
//	base_x -= 2;
	if (base_x < 0) return;
//	RKBANG(rm2, gm2, bm2);
//	RKBANG(rm1, gm1, bm1);
	RKBANG(c0);
	if (offset == 0) return;
	RKBANG(c1);
//	RKBANG(r2, g2, b2);
//	RKBANG(r3, g3, b3);
}
#else
#define RKBANG(r, g, b)\
	dc . SetPen (wxPen (wxColour (colour_table -> r, colour_table -> g, colour_table -> b)));\
	dc . DrawLine (base_x, y1, base_x, y2); base_x++;
void draw_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	colour_table += (int) (256.0 * (x - (double) base_x));
	base_x -= 2;
	if (base_x < 0) return;
	RKBANG(rm2, gm2, bm2);
	RKBANG(rm1, gm1, bm1);
	RKBANG(r0, g0, b0);
	RKBANG(r1, g1, b1);
	RKBANG(r2, g2, b2);
	RKBANG(r3, g3, b3);
}
void draw_line (wxDC & dc, lanczos_colour * left_colour_table, lanczos_colour * right_colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	int shift = (int) (256.0 * (x - (double) base_x));
	lanczos_colour * colour_table = left_colour_table + shift;
	base_x -= 2;
	if (base_x < 0) return;
	RKBANG(rm2, gm2, bm2);
	RKBANG(rm1, gm1, bm1);
	RKBANG(r0, g0, b0);
	colour_table = right_colour_table + shift;
	RKBANG(r1, g1, b1);
	RKBANG(r2, g2, b2);
	RKBANG(r3, g3, b3);
}
void draw_thin_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	int offset = (int) (256.0 * (x - (double) base_x));
	colour_table += offset;
//	base_x -= 2;
	if (base_x < 0) return;
//	RKBANG(rm2, gm2, bm2);
//	RKBANG(rm1, gm1, bm1);
	RKBANG(r0, g0, b0);
	if (offset == 0) return;
	RKBANG(r1, g1, b1);
//	RKBANG(r2, g2, b2);
//	RKBANG(r3, g3, b3);
}
#endif

/*static void blend (wxColour * c, double r, double g, double b, double a, double am) {
	r = r * a + (double) c -> Red () * am;
	g = g * a + (double) c -> Green () * am;
	b = b * a + (double) c -> Blue () * am;
	if (r < 0.0) r = 0.0; if (r > 255.0) r = 255.0;
	if (g < 0.0) g = 0.0; if (g > 255.0) g = 255.0;
	if (b < 0.0) b = 0.0; if (b > 255.0) b = 255.0;
	c -> Set ((unsigned char) r, (unsigned char) g, (unsigned char) b);
}

void draw_alpha_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	colour_table += (int) (256.0 * (x - (double) base_x));
	base_x -= 2;
	if (base_x < 0) return;
	for (int ind = y1; ind <= y2; ind++) {
		wxColour c;
		dc . GetPixel (wxPoint (base_x + 0, ind), & c);
		blend (& c, colour_table -> rm2, colour_table -> gm2, colour_table -> bm2, colour_table -> am2, 1.0 - colour_table -> am2);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 0, ind);
		//
		double am1 = colour_table -> am1;
		dc . GetPixel (wxPoint (base_x + 1, ind), & c);
		blend (& c, colour_table -> rm1, colour_table -> gm1, colour_table -> bm1, colour_table -> am1, 1.0 - colour_table -> am1);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 1, ind);
		//
		dc . GetPixel (wxPoint (base_x + 2, ind), & c);
		blend (& c, colour_table -> r0, colour_table -> g0, colour_table -> b0, colour_table -> a0, 1.0 - colour_table -> a0);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 2, ind);
		//
		dc . GetPixel (wxPoint (base_x + 3, ind), & c);
		blend (& c, colour_table -> r1, colour_table -> g1, colour_table -> b1, colour_table -> a1, 1.0 - colour_table -> a1);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 3, ind);
		//
		dc . GetPixel (wxPoint (base_x + 4, ind), & c);
		blend (& c, colour_table -> r2, colour_table -> g2, colour_table -> b2, colour_table -> a2, 1.0 - colour_table -> a2);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 4, ind);
		//
		dc . GetPixel (wxPoint (base_x + 5, ind), & c);
		blend (& c, colour_table -> r3, colour_table -> g3, colour_table -> b3, colour_table -> a3, 1.0 - colour_table -> a3);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 5, ind);
		//
	}
}*/

