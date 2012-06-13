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

class TrigSDecUp : public TrigBase {
public:
	int left_index;
	virtual double getLocation (double x) {return 1.0 + log10 (sin (x * _PI / 180.0));}
	virtual double getValue (double x) {return asin (pow (10.0, x - 1.0)) * 180.0 / _PI;}
	void init_this (void) {init_dec_sine (); left_index = find_left_dec_trig_index (0.0 - left_extension * (double) scale_length);}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 500, 1000);
		for (int ind = 10; ind < 70; ind += 10) location = draw_markings_for_dec_10 (dc, & locations [ind * 100], ind, x, location, false);
		double distance = absolute (locations [9000] - locations [8000]);
		wxString text90 = _T ("90");
		wxSize extent = dc . GetTextExtent (text90);
		if (distance < minimum_step) {draw_index_location (dc, x + locations [9000], text90); return;}
		location = draw_markings_for_dec_10 (dc, & locations [7000], 70, x, location, distance < (double) extent . x);
		if (distance < (double) extent . x) draw_index_location (dc, x + locations [9000], text90);
		else location = draw_markings_for_dec_10 (dc, & locations [8000], 80, x, location, false);
	}
	TrigSDecUp (int height) : TrigBase (height) {left_index = 0;}
};

class TrigSDecDown : public TrigSDecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSDecDown (int height) : TrigSDecUp (height) {}
};

/* GJM -- High accuracy small degree S decimal scales (replaces STDec scale) */
class TrigS1DecUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {return 2.0 + log10 (sin (x * _PI / 180.0));}
	virtual double getValue (double x) {return asin (pow (10.0, x - 2.0)) * 180.0 / _PI;}
	void init_this (void) {
		init_small_dec_sine ();
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 50, right_index != 10000 ? right_index : 600);
		if (left_index <= 50) draw_text_marker (dc, _T ("0.5"), x + locations [50]);
		draw_text_marker (dc, _T ("1.5"), x + locations [150]);
	}
	TrigS1DecUp (int height) : TrigBase (height) {left_index = 5; right_index = 1000;}
};

class TrigS1DecDown : public TrigS1DecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigS1DecDown (int height) : TrigS1DecUp (height) {}
};

class TrigSADecUp : public TrigBase {
public:
	int left_index;
	double factor;
	double shift;
	virtual double getLocation (double x) {return 1.0 + log10 (sin (x * _PI / 180.0)) / factor - shift;}
	virtual double getValue (double x) {x += shift; return asin (pow (10.0, x * factor - factor)) * 180.0 / _PI;}
	void init_this (void) {init_dec_sine (1.0 / factor); left_index = find_left_trig_index (0.0 - (left_extension - shift) * (double) scale_length);}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		x -= shift * (double) scale_length;
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 50, 1000);
		for (int ind = 10; ind < 70; ind += 10) location = draw_markings_for_dec_10 (dc, & locations [ind * 100], ind, x, location, false);
		double distance = absolute (locations [9000] - locations [8000]);
		wxString text90 = _T ("90");
		wxSize extent = dc . GetTextExtent (text90);
		if (distance < minimum_step) {draw_index_location (dc, x + locations [9000], text90); return;}
		location = draw_markings_for_dec_10 (dc, & locations [7000], 70, x, location, distance < (double) extent . x);
		if (distance < (double) extent . x) draw_index_location (dc, x + locations [9000], text90);
		else location = draw_markings_for_dec_10 (dc, & locations [8000], 80, x, location, false);
	}
	TrigSADecUp (int height) : TrigBase (height) {this -> factor = 2.0; left_index = 0; this -> shift = 0.0;}
	TrigSADecUp (int factor, int height) : TrigBase (height) {if (factor < 1) factor = 1; this -> factor = (double) factor; left_index = 0; this -> shift = 0.5;}
};

class TrigSADecDown : public TrigSADecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSADecDown (int height) : TrigSADecUp (height) {}
	TrigSADecDown (int factor, int height) : TrigSADecUp (factor, height) {}
};

class TrigTDecUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return atan (pow (10.0, x - 1.0)) * 180.0 / _PI;}
	void init_this (void) {
		init_dec_tan ();
		left_index = find_left_dec_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_dec_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 500, 1000);
		for (int ind = 10; ind < 40; ind += 10) location = draw_markings_for_dec_10 (dc, & locations [ind * 100], ind, x, location, false);
		if (right_index > 4000) location = draw_markings_for_dec_10 (dc, & locations [4000], 40, x, location, false, 0, right_index < 5000 ? right_index - 4000 : 1000);
		if (right_index > 5000) location = draw_markings_for_dec_10 (dc, & locations [5000], 50, x, location, false, 0, right_index < 6000 ? right_index - 5000 : 1000);
		if (right_index > 6000) location = draw_markings_for_dec_10 (dc, & locations [6000], 60, x, location, false, 0, right_index < 7000 ? right_index - 6000 : 1900);
		if (right_index > 7000) location = draw_markings_for_dec_10 (dc, & locations [7000], 70, x, location, false, 0, right_index < 8000 ? right_index - 7000 : 1000);
		if (right_index > 8000) location = draw_markings_for_dec_10 (dc, & locations [8000], 80, x, location, false, 0, right_index < 9000 ? right_index - 8000 : 1000);
	}
	TrigTDecUp (int height) : TrigBase (height) {left_index = 0; right_index = 4500;}
};

class TrigTDecDown : public TrigTDecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigTDecDown (int height) : TrigTDecUp (height) {}
};

/* GJM -- High accuracy small degree T decimal scales (replaces STDec scale) */
class TrigT1DecUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {return 2.0 + log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return atan (pow (10.0, x - 2.0)) * 180.0 / _PI;}
	void init_this (void) {
		init_small_dec_tan ();
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 50, right_index != 10000 ? right_index : 600);
		if (left_index <= 50) draw_text_marker (dc, _T ("0.5"), x + locations [50]);
		draw_text_marker (dc, _T ("1.5"), x + locations [150]);
	}
	TrigT1DecUp (int height) : TrigBase (height) {left_index = 5; right_index = 1000;}
};

class TrigT1DecDown : public TrigT1DecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigT1DecDown (int height) : TrigT1DecUp (height) {}
};

class TrigTADecUp : public TrigBase {
public:
	int left_index, right_index;
	double fraction, shift;
	virtual double getLocation (double x) {return 1.0 + log10 (tan (x * _PI / 180.0)) / fraction - shift;}
	virtual double getValue (double x) {x += shift; return atan (pow (10.0, x * fraction - fraction)) * 180.0 / _PI;}
	void init_this (void) {
		init_dec_tan (1.0 / fraction);
		left_index = find_left_dec_trig_index (0.0 - (left_extension - shift) * (double) scale_length);
		right_index = find_right_dec_trig_index ((double) scale_length * (1.0 + right_extension + shift));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		x -= shift * (double) scale_length;
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 500, 1000);
		for (int ind = 10; ind < 40; ind += 10) location = draw_markings_for_dec_10 (dc, & locations [ind * 100], ind, x, location, false);
		if (right_index > 4000) location = draw_markings_for_dec_10 (dc, & locations [4000], 40, x, location, false, 0, right_index < 5000 ? right_index - 4000 : 1000);
		if (right_index > 5000) location = draw_markings_for_dec_10 (dc, & locations [5000], 50, x, location, false, 0, right_index < 6000 ? right_index - 5000 : 1000);
		if (right_index > 6000) location = draw_markings_for_dec_10 (dc, & locations [6000], 60, x, location, false, 0, right_index < 7000 ? right_index - 6000 : 1000);
		if (right_index > 7000) location = draw_markings_for_dec_10 (dc, & locations [7000], 70, x, location, false, 0, right_index < 8000 ? right_index - 7000 : 1000);
		if (right_index > 8000) location = draw_markings_for_dec_10 (dc, & locations [8000], 80, x, location, false, 0, right_index < 9000 ? right_index - 8000 : 900);
	}
	TrigTADecUp (int height) : TrigBase (height) {fraction = 2.0; shift = 0.0; left_index = 0; right_index = 4500;}
	TrigTADecUp (int fraction, int height) : TrigBase (height) {
		if (fraction < 1) fraction = 1;
		this -> fraction = (double) fraction;
		shift = 0.5;
		left_index = 0; right_index = 4500;
	}
};

class TrigTADecDown : public TrigTADecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigTADecDown (int height) : TrigTADecUp (height) {}
	TrigTADecDown (int fraction, int height) : TrigTADecUp (fraction, height) {}
};

class TrigT2DecUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {return log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return atan (pow (10.0, x)) * 180.0 / _PI;}
	void init_this (void) {
		init_dec_tan ();
		left_index = find_left_dec_trig_index ((double) scale_length * (1.0 - left_extension)); //(0.0 - (double) left_extension * ((double) scale_length);
		right_index = find_right_dec_trig_index ((double) scale_length * (2.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		x -= (double) scale_length;
		double location = locations [2000];
//		draw_index_location (dc, x + locations [2700], _T ("45"));
		if (left_index <= 3000) location = draw_markings_for_dec_10 (dc, & locations [2000], 20, x, location, false, left_index <= 2000 ? 0 : left_index - 2000, 1000);
		if (left_index <= 4000) location = draw_markings_for_dec_10 (dc, & locations [3000], 30, x, location, false, left_index <= 3000 ? 0 : left_index - 3000, 1000);
		if (left_index <= 5000) location = draw_markings_for_dec_10 (dc, & locations [4000], 40, x, location, false, left_index <= 4000 ? 0 : left_index - 4000, 1000);
		location = draw_markings_for_dec_10 (dc, & locations [5000], 50, x, location, false);
		location = draw_markings_for_dec_10 (dc, & locations [6000], 60, x, location, false);
		location = draw_markings_for_dec_10 (dc, & locations [7000], 70, x, location, false);
		if (right_index > 8000) location = draw_markings_for_dec_10 (dc, & locations [8000], 80, x, location, false, 0, right_index < 9000 ? right_index - 8000 : 1000);
	}
	TrigT2DecUp (int height) : TrigBase (height) {left_index = 0; right_index = 10000;}
};

class TrigT2DecDown : public TrigT2DecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigT2DecDown (int height) : TrigT2DecUp (height) {}
};

class TrigSTDecUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {return log10 (x * _PI / 1.8);}
	virtual double getValue (double x) {return pow (10.0, x) * 1.8 / _PI;}
	void init_this (void) {
		init_small_dec_locations (1000);
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 50, right_index != 10000 ? right_index : 600);
		if (left_index <= 50) draw_text_marker (dc, _T ("0.5"), x + locations [50]);
		draw_text_marker (dc, _T ("1.5"), x + locations [150]);
	}
	TrigSTDecUp (int height) : TrigBase (height) {left_index = 5; right_index = 1000;}
};

class TrigSTDecDown : public TrigSTDecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSTDecDown (int height) : TrigSTDecUp (height) {}
};

class ScaleSQRTSINEdecUp : public TrigSDecUp {
public:
	virtual double getLocation (double x) {x = sin (x * _PI / 180.0) * 10.0; return x * x / 100.0;}
	virtual double getValue (double x) {x *= 100.0; x = sqrt (x); return asin (x * 0.1) * 180.0 / _PI;}
	void init_sqrt_sine_locations (void) {
		int index = 0;
		for (int degree = 0; degree <= 90; degree++) {
			double angle = (double) degree / 180.0;
			for (int minute = 0; minute < 100; minute++) {
				double sub_angle = (double) minute / 18000.0;
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
	virtual void draw (wxDC & dc, double x) {TrigSDecUp :: draw (dc, x); draw_index_location (dc, x, _T ("0"));}
	ScaleSQRTSINEdecUp (int height) : TrigSDecUp (height) {}
};

class ScaleSQRTSINEdecDown : public ScaleSQRTSINEdecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_sqrt_sine_locations ();}
	ScaleSQRTSINEdecDown (int height) : ScaleSQRTSINEdecUp (height) {}
};

bool check_trig_dec_scales (SetupFileReader & fr, Sliderule * slide_rule) {
	if (fr . id ("scale_Sdec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSDecUp (fr . int_symbol));}
	if (fr . id ("scale_Sdec_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSDecDown (fr . int_symbol));}
/* GJM -- New high accuracy S1 scales */
	if (fr . id ("scale_S1dec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigS1DecUp (fr . int_symbol));}
	if (fr . id ("scale_S1dec_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigS1DecDown (fr . int_symbol));}
	if (fr . id ("scale_SAdec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSADecUp (fr . int_symbol));}
	if (fr . id ("scale_SAdec_down") || fr . id ("scale_SBdec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSADecDown (fr . int_symbol));}
	if (fr . id ("scale_Tdec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTDecUp (fr . int_symbol));}
	if (fr . id ("scale_Tdec_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTDecDown (fr . int_symbol));}
/* GJM -- New high accuracy T1 scales */
	if (fr . id ("scale_T1dec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigT1DecUp (fr . int_symbol));}
	if (fr . id ("scale_T1dec_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigT1DecDown (fr . int_symbol));}

	if (fr . id ("scale_TAdec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTADecUp (fr . int_symbol));}
	if (fr . id ("scale_TBdec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTADecDown (fr . int_symbol));}
	if (fr . id ("scale_T2dec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigT2DecUp (fr . int_symbol));}
	if (fr . id ("scale_T2dec_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigT2DecDown (fr . int_symbol));}
	if (fr . id ("scale_STdec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSTDecUp (fr . int_symbol));}
	if (fr . id ("scale_STdec_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSTDecDown (fr . int_symbol));}

	if (fr . id ("scale_SCKdec")) {
		if (! fr . get_int ()) return false;
		int factor = fr . int_symbol;			
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new TrigSADecUp (factor, fr . int_symbol));
	}
	if (fr . id ("scale_SDKdec")) {
		if (! fr . get_int ()) return false;
		int factor = fr . int_symbol;			
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new TrigSADecDown (factor, fr . int_symbol));
	}
	if (fr . id ("scale_TCKdec")) {
		if (! fr . get_int ()) return false;
		int factor = fr . int_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new TrigTADecUp (factor, fr . int_symbol));
	}
	if (fr . id ("scale_TDKdec")) {
		if (! fr . get_int ()) return false;
		int factor = fr . int_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new TrigTADecDown (factor, fr . int_symbol));
	}

	if (fr . id ("scale_SQRT_Sdec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSQRTSINEdecUp (fr . int_symbol));}
	if (fr . id ("scale_SQRT_Sdec_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSQRTSINEdecDown (fr . int_symbol));}
	return true;
}

