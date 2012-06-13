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

#ifndef _SCALE_LOG_BASE_
#define _SCALE_LOG_BASE_

#include "wx/wx.h"
#include "scale_base.h"

class LogBase : public Scale {
public:
	double logs [1100];
	double pi_location;
	double e_location;
	double c_location;
	double c1_location;
	double degree_location;
	double m_location;
	double mm_location;
	double extension_index_shift;
	int left_extension_index;
	int right_extension_index;
	void prepare_extensions (double extension_multiplier, int maximum_index);
	void log_init (double multiplier = 1.0);
	void draw_left_log_base (wxDC & dc, double x, int index);
	void draw_right_log_base (wxDC & dc, double x, int index);
	void draw_both_sides_log_base (wxDC & dc, double x, int left_index, int right_index);
	void draw_log_base (wxDC & dc, double x);
	LogBase (int height);
};

#endif

