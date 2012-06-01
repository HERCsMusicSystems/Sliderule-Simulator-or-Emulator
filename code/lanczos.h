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

#ifndef _LANCZOS_
#define _LANCZOS_

#include "wx/wx.h"

struct colour {unsigned char red, green, blue, alpha;};

struct lanczos_colour {
	unsigned char rm2, gm2, bm2, am2;
	unsigned char rm1, gm1, bm1, am1;
	unsigned char r0, g0, b0, a0;
	unsigned char r1, g1, b1, a1;
	unsigned char r2, g2, b2, a2;
	unsigned char r3, g3, b3, a3;
};

struct lanczos_packed_colour {unsigned long int cm2, cm1, c0, c1, c2, c3;};

extern void draw_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2);
extern void draw_line (wxDC & dc, lanczos_colour * left_colour_table, lanczos_colour * right_colour_table, double x, int y1, int y2);
extern void initialise_lanczos_colours (lanczos_colour * colours, double r1, double g1, double b1, double r2, double g2, double b2);
extern void draw_thin_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2);

#endif

