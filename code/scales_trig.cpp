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
#include "sliderule.h"
#include "scale_trig_base.h"

#define _PI       3.14159265358979323846

class TrigSUp : public TrigBase {
public:
	int left_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 1.0 + log10 (sin (x * _PI / 180.0));}
	virtual double getValue (double x) {return dec_correction (asin (pow (10.0, x - 1.0)) * 180.0 / _PI);}
	void init_this (void) {init_sine (); left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 300, 600);
		for (int ind = 10; ind < 70; ind += 10) location = draw_markings_for_10 (dc, & locations [ind * 60], ind, x, location, false);
		double distance = absolute (locations [5400] - locations [4800]);
		wxString text90 = _T ("90");
		wxSize extent = dc . GetTextExtent (text90);
		if (distance < minimum_step) {draw_index_location (dc, x + locations [5400], text90); return;}
		location = draw_markings_for_10 (dc, & locations [4200], 70, x, location, distance < (double) extent . x);
		if (distance < (double) extent . x) draw_index_location (dc, x + locations [5400], text90);
		else location = draw_markings_for_10 (dc, & locations [4800], 80, x, location, false);
	}
	TrigSUp (int height) : TrigBase (height) {left_index = 0;}
};

class TrigSDown : public TrigSUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSDown (int height) : TrigSUp (height) {}
};

/* GJM -- High accuracy small degree S scales (replaces ST/SRT scale) */
class TrigS1Up : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 2.0 + log10 (sin (x * _PI / 180.0));}
	virtual double getValue (double x) {return dec_correction (asin (pow (10.0, x - 2.0)) * 180.0 / _PI);}
	void init_this (void) {
		init_small_sine ();
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 30, right_index != 6000 ? right_index : 360);
		if (left_index <= 30) draw_text_marker (dc, _T ("30'"), x + locations [30]);
		draw_text_marker (dc, _T ("1:30'"), x + locations [90]);
	}
	TrigS1Up (int height) : TrigBase (height) {left_index = 5; right_index = 600;}
};

class TrigS1Down : public TrigS1Up {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigS1Down (int height) : TrigS1Up (height) {}
};

class TrigSAUp : public TrigBase {
public:
	int left_index;
	double factor;
	double shift;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 1.0 + log10 (sin (x * _PI / 180.0)) / factor - shift;}
	virtual double getValue (double x) {x += shift; return dec_correction (asin (pow (10.0, x * factor - factor)) * 180.0 / _PI);}
	void init_this (void) {init_sine (1.0 / factor); left_index = find_left_trig_index (0.0 - (left_extension - shift) * (double) scale_length);}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		x -= shift * (double) scale_length;
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 30, 600);
		for (int ind = 10; ind < 70; ind += 10) location = draw_markings_for_10 (dc, & locations [ind * 60], ind, x, location, false);
		double distance = absolute (locations [5400] - locations [4800]);
		wxString text90 = _T ("90");
		wxSize extent = dc . GetTextExtent (text90);
		if (distance < minimum_step) {draw_index_location (dc, x + locations [5400], text90); return;}
		location = draw_markings_for_10 (dc, & locations [4200], 70, x, location, distance < (double) extent . x);
		if (distance < (double) extent . x) draw_index_location (dc, x + locations [5400], text90);
		else location = draw_markings_for_10 (dc, & locations [4800], 80, x, location, false);
	}
	TrigSAUp (int height) : TrigBase (height) {this -> factor = 2.0; left_index = 0; this -> shift = 0.0;}
	TrigSAUp (int factor, int height) : TrigBase (height) {if (factor < 1) factor = 1; this -> factor = (double) factor; left_index = 0; this -> shift = 0.5;}
};

class TrigSADown : public TrigSAUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSADown (int height) : TrigSAUp (height) {}
	TrigSADown (int factor, int height) : TrigSAUp (factor, height) {}
};

class TrigTUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 1.0 + log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return dec_correction (atan (pow (10.0, x - 1.0)) * 180.0 / _PI);}
	void init_this (void) {
		init_tan ();
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 300, 600);
		for (int ind = 10; ind < 40; ind += 10) location = draw_markings_for_10 (dc, & locations [ind * 60], ind, x, location, false);
		if (right_index > 2400) location = draw_markings_for_10 (dc, & locations [2400], 40, x, location, false, 0, right_index < 3000 ? right_index - 2400 : 600);
		if (right_index > 3000) location = draw_markings_for_10 (dc, & locations [3000], 50, x, location, false, 0, right_index < 3600 ? right_index - 3000 : 600);
		if (right_index > 3600) location = draw_markings_for_10 (dc, & locations [3600], 60, x, location, false, 0, right_index < 4200 ? right_index - 3600 : 600);
		if (right_index > 4200) location = draw_markings_for_10 (dc, & locations [4200], 70, x, location, false, 0, right_index < 4800 ? right_index - 4200 : 600);
		if (right_index > 4800) location = draw_markings_for_10 (dc, & locations [4800], 80, x, location, false, 0, right_index < 5400 ? right_index - 4800 : 600);
	}
	TrigTUp (int height) : TrigBase (height) {left_index = 0; right_index = 2700;}
};

class TrigTDown : public TrigTUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigTDown (int height) : TrigTUp (height) {}
};

/* GJM -- High accuracy small degree T scales (replaces ST/SRT scale) */
class TrigT1Up : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 2.0 + log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return dec_correction (atan (pow (10.0, x - 2.0)) * 180.0 / _PI);}
	void init_this (void) {
		init_small_tan ();
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 30, right_index != 6000 ? right_index : 360);
		if (left_index <= 30) draw_text_marker (dc, _T ("30'"), x + locations [30]);
		draw_text_marker (dc, _T ("1:30'"), x + locations [90]);
	}
	TrigT1Up (int height) : TrigBase (height) {left_index = 5; right_index = 600;}
};

class TrigT1Down : public TrigT1Up {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigT1Down (int height) : TrigT1Up (height) {}
};

class TrigTAUp : public TrigBase {
public:
	int left_index, right_index;
	double fraction, shift;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 1.0 + log10 (tan (x * _PI / 180.0)) / fraction - shift;}
	virtual double getValue (double x) {x += shift; return dec_correction (atan (pow (10.0, x * fraction - fraction)) * 180.0 / _PI);}
	void init_this (void) {
		init_tan (1.0 / fraction);
		left_index = find_left_trig_index (0.0 - (left_extension - shift) * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension + shift));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		x -= shift * (double) scale_length;
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 300, 600);
		for (int ind = 10; ind < 40; ind += 10) location = draw_markings_for_10 (dc, & locations [ind * 60], ind, x, location, false);
		if (right_index > 2400) location = draw_markings_for_10 (dc, & locations [2400], 40, x, location, false, 0, right_index < 3000 ? right_index - 2400 : 600);
		if (right_index > 3000) location = draw_markings_for_10 (dc, & locations [3000], 50, x, location, false, 0, right_index < 3600 ? right_index - 3000 : 600);
		if (right_index > 3600) location = draw_markings_for_10 (dc, & locations [3600], 60, x, location, false, 0, right_index < 4200 ? right_index - 3600 : 600);
		if (right_index > 4200) location = draw_markings_for_10 (dc, & locations [4200], 70, x, location, false, 0, right_index < 4800 ? right_index - 4200 : 600);
		if (right_index > 4800) location = draw_markings_for_10 (dc, & locations [4800], 80, x, location, false, 0, right_index < 5400 ? right_index - 4800 : 600);
	}
	TrigTAUp (int height) : TrigBase (height) {fraction = 2.0; shift = 0.0; left_index = 0; right_index = 2700;}
	TrigTAUp (int fraction, int height) : TrigBase (height) {
		if (fraction < 1) fraction = 1;
		this -> fraction = (double) fraction;
		shift = 0.5;
		left_index = 0; right_index = 2700;
	}
};

class TrigTADown : public TrigTAUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigTADown (int height) : TrigTAUp (height) {}
	TrigTADown (int fraction, int height) : TrigTAUp (fraction, height) {}
};

class TrigT2Up : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return dec_correction (atan (pow (10.0, x)) * 180.0 / _PI);}
	void init_this (void) {
		init_tan ();
		left_index = find_left_trig_index ((double) scale_length * (1.0 - left_extension)); //(0.0 - (double) left_extension * ((double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (2.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		x -= (double) scale_length;
		double location = locations [1200];
//		draw_index_location (dc, x + locations [2700], _T ("45"));
		if (left_index <= 1800) location = draw_markings_for_10 (dc, & locations [1200], 20, x, location, false, left_index <= 1200 ? 0 : left_index - 1200, 600);
		if (left_index <= 2400) location = draw_markings_for_10 (dc, & locations [1800], 30, x, location, false, left_index <= 1800 ? 0 : left_index - 1800, 600);
		if (left_index <= 3000) location = draw_markings_for_10 (dc, & locations [2400], 40, x, location, false, left_index <= 2400 ? 0 : left_index - 2400, 600);
		location = draw_markings_for_10 (dc, & locations [3000], 50, x, location, false);
		location = draw_markings_for_10 (dc, & locations [3600], 60, x, location, false);
		location = draw_markings_for_10 (dc, & locations [4200], 70, x, location, false);
		if (right_index > 4800) location = draw_markings_for_10 (dc, & locations [4800], 80, x, location, false, 0, right_index < 5400 ? right_index - 4800 : 600);
	}
	TrigT2Up (int height) : TrigBase (height) {left_index = 0; right_index = 6000;}
};

class TrigT2Down : public TrigT2Up {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigT2Down (int height) : TrigT2Up (height) {}
};

class TrigSTUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return log10 (x * _PI / 1.8);}
	virtual double getValue (double x) {return dec_correction (pow (10.0, x) * 1.8 / _PI);}
	void init_this (void) {
		init_small_locations (600);
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 30, right_index != 6000 ? right_index : 360);
		if (left_index <= 30) draw_text_marker (dc, _T ("30'"), x + locations [30]);
		draw_text_marker (dc, _T ("1:30'"), x + locations [90]);
	}
	TrigSTUp (int height) : TrigBase (height) {left_index = 5; right_index = 600;}
};

class TrigSTDown : public TrigSTUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSTDown (int height) : TrigSTUp (height) {}
};

class ScaleSQRTSINEUp : public TrigSUp {
public:
	virtual double getLocation (double x) {x = dec_recorrection (x); x = sin (x * _PI / 180.0) * 10.0; return x * x / 100.0;}
	virtual double getValue (double x) {x *= 100.0; x = sqrt (x); return dec_correction (asin (x * 0.1) * 180.0 / _PI);}
	void init_sqrt_sine_locations (void) {
		int index = 0;
		for (int degree = 0; degree <= 90; degree++) {
			double angle = (double) degree / 180.0;
			for (int minute = 0; minute < 60; minute++) {
				double sub_angle = (double) minute / 10800.0;
				double value = _PI * (angle + sub_angle);
				value = sin (value) * 10.0;
				value *= value / 100.0;
				value *= (double) scale_length;
				locations [index++] = value;
			}
		}
		locations [index++] = (double) scale_length;
		left_index = 0;
	}
	virtual void scaleInit (void) {faceUp (); init_sqrt_sine_locations ();}
	virtual void draw (wxDC & dc, double x) {TrigSUp :: draw (dc, x); draw_index_location (dc, x, _T ("0"));}
	ScaleSQRTSINEUp (int height) : TrigSUp (height) {}
};

class ScaleSQRTSINEDown : public ScaleSQRTSINEUp {
public:
	virtual void scaleInit (void) {faceDown (); init_sqrt_sine_locations ();}
	ScaleSQRTSINEDown (int height) : ScaleSQRTSINEUp (height) {}
};

bool check_trig_scales (SetupFileReader & fr, Sliderule * slide_rule) {
	if (fr . id ("scale_S")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSUp (fr . int_symbol));}
	if (fr . id ("scale_S_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSDown (fr . int_symbol));}
/* GJM -- New high accuracy S1 scales */
	if (fr . id ("scale_S1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigS1Up (fr . int_symbol));}
	if (fr . id ("scale_S1_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigS1Down (fr . int_symbol));}
	if (fr . id ("scale_SA")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSAUp (fr . int_symbol));}
	if (fr . id ("scale_SA_down") || fr . id ("scale_SB")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSADown (fr . int_symbol));}
	if (fr . id ("scale_T")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTUp (fr . int_symbol));}
	if (fr . id ("scale_T_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTDown (fr . int_symbol));}
/* GJM -- New high accuracy T1 scales */
	if (fr . id ("scale_T1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigT1Up (fr . int_symbol));}
	if (fr . id ("scale_T1_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigT1Down (fr . int_symbol));}

	if (fr . id ("scale_TA")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTAUp (fr . int_symbol));}
	if (fr . id ("scale_TB")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTADown (fr . int_symbol));}
	if (fr . id ("scale_T2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigT2Up (fr . int_symbol));}
	if (fr . id ("scale_T2_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigT2Down (fr . int_symbol));}
	if (fr . id ("scale_ST")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSTUp (fr . int_symbol));}
	if (fr . id ("scale_ST_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSTDown (fr . int_symbol));}

	if (fr . id ("scale_SCK")) {
		if (! fr . get_int ()) return false;
		int factor = fr . int_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new TrigSAUp (factor, fr . int_symbol));
	}
	if (fr . id ("scale_SDK")) {
		if (! fr . get_int ()) return false;
		int factor = fr . int_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new TrigSADown (factor, fr . int_symbol));
	}
	if (fr . id ("scale_TCK")) {
		if (! fr . get_int ()) return false;
		int factor = fr . int_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new TrigTAUp (factor, fr . int_symbol));
	}
	if (fr . id ("scale_TDK")) {
		if (! fr . get_int ()) return false;
		int factor = fr . int_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new TrigTADown (factor, fr . int_symbol));
	}

	if (fr . id ("scale_SQRT_S")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSQRTSINEUp (fr . int_symbol));}
	if (fr . id ("scale_SQRT_S_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSQRTSINEDown (fr . int_symbol));}
	return true;
}

