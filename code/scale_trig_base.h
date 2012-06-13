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

#ifndef _SCALE_TRIG_BASE_
#define _SCALE_TRIG_BASE_

#include "scale_base.h"
#include "wx/wx.h"



class TrigBase : public Scale {
public:
	double locations [10000];
	void init_locations (double multiplier, bool tangent, int to);
	void init_dec_locations (double multiplier, bool tangent, int to);
	/* GJM -- Added _1 versions for S1 and T1 scales */
	void init_locations_1 (double multiplier, bool tangent, int to);
	void init_dec_locations_1 (double multiplier, bool tangent, int to);
	void init_small_locations (int to);
	void init_small_dec_locations (int to);
	void init_sine (double multiplier = 1.0);
	void init_tan (double multiplier = 1.0);
	void init_dec_sine (double multiplier = 1.0);
	void init_dec_tan (double multiplier = 1.0);
	/* GJM -- small sine and tangent inits for S1 / T1 scales */
	void init_small_sine (double multiplier = 1.0);
	void init_small_tan (double multiplier = 1.0);
	void init_small_dec_sine (double multiplier = 1.0);
	void init_small_dec_tan (double multiplier = 1.0);
	double drawing_footer (wxDC & dc, double * markings, int description, double x, double last_drawn, int to, bool next_extent);
	double draw_markings_for_1 (wxDC & dc, double * markings, int description, double x, double last_drawn, int from = 0, int to = 60);
	double draw_markings_for_5 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from = 0, int to = 300);
	double draw_markings_for_dec_5 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from = 0, int to = 500);
	double draw_markings_for_10 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from = 0, int to = 600);
	double draw_markings_for_dec_10 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from = 0, int to = 1000);
	int find_left_trig_index (double x);
	int find_right_trig_index (double x);
	int find_left_dec_trig_index (double x);
	int find_right_dec_trig_index (double x);
	TrigBase (int height);
};

#endif

