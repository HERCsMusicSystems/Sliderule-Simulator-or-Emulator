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

#include "setup_file_reader.h"
#include "scale_log_base.h"
#include "sliderule.h"

class Log_dk : public LogBase {
public:
	double factor;
	double reciprocal_factor;
	virtual double getLocation (double x) {return 0.5 + log10 (x) * reciprocal_factor;}
	virtual double getValue (double x) {x -= 0.5; return pow (10.0, x * factor);}
	virtual void scaleInit (void) {faceUp (); log_init (reciprocal_factor);}
	void draw_dk_base (wxDC & dc, double x, wxString marker = _T ("1")) {
		double location;
		if (absolute (logs [900] - logs [800]) < minimum_step) {
			if (absolute (logs [900] - logs [400]) > minimum_step) {
				location = x + logs [400];
				draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			}
		} else {
			location = x + logs [100]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [200]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [300]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [400]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y1);
			location = x + logs [500]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [600]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [700]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [800]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
		}
		location = x + logs [900]; draw_index_location (dc, location, marker);
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		int marker_shift = (int) (factor * -0.5);
		draw_index_location (dc, x, wxString :: Format (_T ("%+i"), marker_shift++));
		for (int ind = 0; ind < (int) factor; ind++)
			draw_dk_base (dc, x + (double) ind * reciprocal_factor * (double) scale_length, wxString :: Format (_T ("%+i"), marker_shift++));
	}
	Log_dk (int factor, int height) : LogBase (height) {
		if (factor < 0) factor = 1;
		factor <<= 1;
		this -> factor = (double) factor;
		reciprocal_factor = 1.0 / this -> factor;
	}
};

class Log_dk_down : public Log_dk {
public:
	virtual void scaleInit (void) {faceDown (); log_init (reciprocal_factor);}
	Log_dk_down (int factor, int height) : Log_dk (factor, height) {}
};

bool check_decimal_tracking_scales (SetupFileReader & fr, Sliderule * slide_rule) {
	if (fr . id ("scale_CK")) {
		if (! fr . get_int ()) return false;
		int factor = fr . int_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new Log_dk (factor, fr . int_symbol));
	}
	if (fr . id ("scale_DK")) {
		if (! fr . get_int ()) return false;
		int factor = fr . int_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new Log_dk_down (factor, fr . int_symbol));
	}
	if (fr . id ("scale_Cdk")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new Log_dk (5, fr . int_symbol));}
	if (fr . id ("scale_Ddk")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new Log_dk_down (5, fr . int_symbol));}
	if (fr . id ("scale_Adk")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new Log_dk (10, fr . int_symbol));}
	if (fr . id ("scale_Bdk")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new Log_dk_down (10, fr . int_symbol));}
	if (fr . id ("scale_Kdk")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new Log_dk (15, fr . int_symbol));}
	if (fr . id ("scale_Jdk")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new Log_dk_down (15, fr . int_symbol));}
	return true;
}

