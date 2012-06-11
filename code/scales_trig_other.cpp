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

class TrigSgUp : public Scale {
public:
	double locations [1100];
	double left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (sin (x * _PI / 200.0));}
	virtual double getValue (double x) {return asin (pow (10.0, x - 1.0)) * 200.0 / _PI;}
	void init_locations (void) {
		for (int ind = 1; ind <= 1000; ind++) {locations [ind] = (double) scale_length * (1.0 + log10 (sin ((double) ind * _PI / 2000.0)));}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 1000, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 1000, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		int description = 10;
		for (int ind = 0; ind < 800; ind += 100) {
			if (ind + 100 >= left_index && ind <= right_index) {
				location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%i"), description), x, location, left_index <= ind ? 0 : left_index - ind, right_index >= ind + 100 ? 100 : right_index - ind);
			}
			description += 10;
		}
		if (left_index <= 90) draw_text_marker (dc, _T ("9"), x + locations [90]);
		if (left_index <= 80) draw_text_marker (dc, _T ("8"), x + locations [80]);
		if (left_index <= 70) draw_text_marker (dc, _T ("7"), x + locations [70]);
		if (left_index <= 60) draw_text_marker (dc, _T ("6"), x + locations [60]);
		if (left_index <= 50) draw_text_marker (dc, _T ("5"), x + locations [50]);
		if (left_index <= 40) draw_text_marker (dc, _T ("4"), x + locations [40]);
		if (left_index <= 30) draw_text_marker (dc, _T ("3"), x + locations [30]);
		if (left_index <= 20) draw_text_marker (dc, _T ("2"), x + locations [20]);
		if (left_index <= 10) draw_text_marker (dc, _T ("1"), x + locations [10]);
		if (right_index <= 800) return;
		double distance = absolute (locations [1000] - locations [900]);
		wxSize extent = dc . GetTextExtent (_T ("100"));
		if (distance >= minimum_step) {
			location = draw_markings_for_100 (dc, & locations [800], distance > extent . x ? _T ("90") : _T (""), x, locations [800]);
			location = draw_markings_for_100 (dc, & locations [900], distance > extent . x ? _T ("100") : _T (""), x, locations [900]);
			if (distance <= extent . x) draw_text_marker (dc, _T ("100"), x + locations [1000]);
		} else {
			draw_index_location (dc, x + locations [1000], _T ("100"));
		}
	}
	TrigSgUp (int height) : Scale (height) {left_index = 1; right_index = 1000;}
};

class TrigSgDown : public TrigSgUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	TrigSgDown (int height) : TrigSgUp (height) {}
};

class TrigTgUp : public Scale {
public:
	double locations [1100];
	int left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (tan (x * _PI / 200.0));}
	virtual double getValue (double x) {return atan (pow (10.0, x - 1.0)) * 200.0 / _PI;}
	void init_locations (void) {
		for (int ind = 1; ind < 1000; ind++) {locations [ind] = (double) scale_length * (1.0 + log10 (tan ((double) ind * _PI / 2000.0)));}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 999, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 999, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		int description = 10;
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind + 100 >= left_index && ind <= right_index) {
				location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%i"), description), x, location, left_index <= ind ? 0 : left_index - ind, right_index >= ind + 100 ? 100 : right_index - ind);
			}
			description += 10;
		}
		if (left_index <= 90) draw_text_marker (dc, _T ("9"), x + locations [90]);
		if (left_index <= 80) draw_text_marker (dc, _T ("8"), x + locations [80]);
		if (left_index <= 70) draw_text_marker (dc, _T ("7"), x + locations [70]);
		if (left_index <= 60) draw_text_marker (dc, _T ("6"), x + locations [60]);
		if (left_index <= 50) draw_text_marker (dc, _T ("5"), x + locations [50]);
		if (left_index <= 40) draw_text_marker (dc, _T ("4"), x + locations [40]);
		if (left_index <= 30) draw_text_marker (dc, _T ("3"), x + locations [30]);
		if (left_index <= 20) draw_text_marker (dc, _T ("2"), x + locations [20]);
		if (left_index <= 10) draw_text_marker (dc, _T ("1"), x + locations [10]);
	}
	TrigTgUp (int height) : Scale (height) {left_index = 1; right_index = 999;}
};

class TrigTgDown : public TrigTgUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	TrigTgDown (int height) : TrigTgUp (height) {}
};

class TrigSrUp : public Scale {
public:
	double locations [1600];
	int left_index;
	virtual double getLocation (double x) {return 1.0 + log10 (sin (x));}
	virtual double getValue (double x) {return asin (pow (10.0, x - 1.0));}
	void init_locations (void) {
		for (int ind = 1; ind < 1600; ind++) {
			locations [ind] = (double) scale_length * (1.0 + log10 (sin ((double) ind / 1000.0)));
		}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 0, 100, 0.0 - (double) scale_length * left_extension);
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		location = draw_markings_for_100 (dc, locations, _T (".1"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations [100], _T (".2"), x, location);
		location = draw_markings_for_100 (dc, & locations [200], _T (".3"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T (".4"), x, location);
		location = draw_markings_for_100 (dc, & locations [400], _T (".5"), x, location);
		location = draw_markings_for_100 (dc, & locations [500], _T (".6"), x, location);
		location = draw_markings_for_100 (dc, & locations [600], _T (".7"), x, location);
		location = draw_markings_for_100 (dc, & locations [700], _T (".8"), x, location);
		location = draw_markings_for_100 (dc, & locations [800], _T (".9"), x, location);
		location = draw_markings_for_100 (dc, & locations [900], _T ("1"), x, location);
		location = draw_markings_for_100 (dc, & locations [1000], _T ("1.1"), x, location);
		location = draw_markings_for_100 (dc, & locations [1100], _T ("1.2"), x, location);
		location = draw_markings_for_100 (dc, & locations [1200], _T ("1.3"), x, location);
		location = draw_markings_for_100 (dc, & locations [1300], _T ("1.4"), x, location);
		location = draw_markings_for_100 (dc, & locations [1400], _T ("1.5"), x, location);
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	TrigSrUp (int height) : Scale (height) {left_index = 100;}
};

class TrigSrDown : public TrigSrUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	TrigSrDown (int height) : TrigSrUp (height) {}
};

class TrigTrUp : public Scale {
public:
	double locations [3200];
	int left_index, right_index;
	double shift;
	virtual double getLocation (double x) {return shift + log10 (tan (x));}
	virtual double getValue (double x) {return atan (pow (10.0, x - shift));}
	void init_locations (void) {
		for (int ind = 1; ind < 3140; ind++) {
			locations [ind] = (double) scale_length * (shift + log10 (tan ((double) ind / 1000.0)));
		}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 0, 3000, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 0, 3000, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		double description = 0.1;
		for (int ind = 0; ind < 3000; ind += 100) {
			if (ind > right_index) return;
			if (left_index <= ind + 100) location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%.1f"), description), x, location, left_index <= ind ? 0 : left_index - ind, right_index > ind + 100 ? 100 : right_index - ind);
			description += 0.1;
		}
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	TrigTrUp (double shift, int height) : Scale (height) {this -> shift = shift; left_index = 100; right_index = 3100;}
};

class TrigTrDown : public TrigTrUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	TrigTrDown (double shift, int height) : TrigTrUp (shift, height) {}
};

class TrigISTdUp : public Scale {
public:
	double sd [1000];
	double isd [1100];
	double td [1000];
	double itd [2000];
	void init_angles (void) {
		td [0] = sd [0] = (double) scale_length * log10 (18.0 / _PI);
		for (int ind = 1; ind <= 900; ind++) {
			double alpha = (double) ind / 10.0;
			double sine = sin (alpha * _PI / 180.0);
			double tangent = tan (alpha * _PI / 180.0);
			alpha *= 0.1;
			sd [ind] = (double) scale_length * log10 (alpha / sine);
			td [ind] = (double) scale_length * log10 (alpha / tangent);
		}
		itd [0] = isd [0] = (double) scale_length - td [0];
		for (int ind = 1; ind <= 1000; ind++) {
			double sine = (double) ind / 1000.0;
			double alpha = asin (sine);
			alpha *= 18.0 / _PI;
			isd [ind] = (double) scale_length * (1.0 - log10 (alpha / sine));
		}
		for (int ind = 1; ind <= 1800; ind++) {
			double tangent = (double) ind / 1000.0;
			double alpha = atan (tangent);
			alpha *= 18.0 / _PI;
			itd [ind] = (double) scale_length * (1.0 - log10 (alpha / tangent));
		}
	}
	virtual double getLocation (double x) {
		if (x == 0.0) return log10 (180.0 / _PI);
		if (x < 0.0) {
			x = -x;
			if (x < 2.0) {double alpha = atan (x); alpha *= 18.0 / _PI; return 1.0 - log10 (alpha / x);}
			double tangent = tan (x * _PI / 180.0);
			return log10 (x * 0.1 / tangent);
		}
		if (x <= 1.0) {double alpha = asin (x); alpha *= 18.0 / _PI; return 1.0 - log10 (alpha / x);}
		double sine = sin (x * _PI / 180.0); return log10 (x * 0.1 / sine);
	}
	virtual double getValue (double x) {return x;}
	virtual void scaleInit (void) {faceUp (); init_angles ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		draw_line (dc, lcp, x + sd [0], y0, y1);
		draw_text_marker (dc, _T ("0"), x + sd [0]);
		double location = sd [0];
		int description = 10;
		for (int ind = 0; ind < 900; ind += 100) {location = draw_markings_for_100 (dc, & sd [ind], wxString :: Format (_T ("%i"), description), x, location); description += 10;}

		description = 1; location = itd [0];
		for (int ind = 0; ind < 1800; ind += 100) {location = draw_markings_for_100 (dc, & itd [ind], wxString :: Format (_T ("%i"), description), x, location); description++;}

		dc . SetTextForeground (wxColour (red_marking_colour . red, red_marking_colour . green, red_marking_colour . blue));
		reversed = ! reversed;
		location = td [0]; description = 10;
		for (int ind = 0; ind < 600; ind += 100) {location = draw_markings_for_100 (dc, & td [ind], wxString :: Format (_T ("%i"), description), x, location); description += 10;}
		draw_line (dc, lcp, x + isd [0], y0, y1);
		draw_text_marker (dc, _T ("0"), x + isd [0]);

		description = 1; location = isd [0];
		for (int ind = 0; ind < 900; ind += 100) {location = draw_markings_for_100 (dc, & isd [ind], wxString :: Format (_T (".%i"), description), x, location); description++;}
		draw_markings_for_100 (dc, & isd [900], _T ("1.0"), x, location, 0, 50);
		draw_markings_for_100 (dc, & isd [900], _T ("1.0"), x, location, 50, 100);
		wxString marking95 = _T (".95");
		if (dc . GetTextExtent (marking95) . x < absolute (isd [900] - isd [950])) draw_text_marker (dc, marking95, x + isd [950]);

		reversed = ! reversed;
		dc . SetTextForeground (wxColour (marking_colour . red, marking_colour . green, marking_colour . blue));
	}
	TrigISTdUp (int height) : Scale (height) {}
};

class TrigISTdDown : public TrigISTdUp {
public:
	virtual void scaleInit (void) {faceDown (); init_angles ();}
	TrigISTdDown (int height) : TrigISTdUp (height) {}
};

class ScaleSQRTSINErUp : public TrigSrUp {
public:
	virtual double getLocation (double x) {x = sin (x) * 10.0; return x * x / 100.0;}
	virtual double getValue (double x) {x *= 100.0; x = sqrt (x); return asin (x * 0.1);}
	void init_sine_locations (void) {
		for (int ind = 0; ind < 1600; ind++) {
			double x = 10.0 * sin ((double) ind / 1000.0);
			locations [ind] = (double) scale_length * (x * x / 100.0);
		}
		left_index = 0;
	}
	virtual void scaleInit (void) {faceUp (); init_sine_locations ();}
	virtual void draw (wxDC & dc, double x) {TrigSrUp :: draw (dc, x); draw_index_location (dc, x, _T ("0"));}
	ScaleSQRTSINErUp (int height) : TrigSrUp (height) {}
};

class ScaleSQRTSINErDown : public ScaleSQRTSINErUp {
public:
	virtual void scaleInit (void) {faceDown (); init_sine_locations ();}
	ScaleSQRTSINErDown (int height) : ScaleSQRTSINErUp (height) {}
};

bool check_trig_other_scales (SetupFileReader & fr, Sliderule * slide_rule) {
	if (fr . id ("scale_Sg")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSgUp (fr . int_symbol));}
	if (fr . id ("scale_Sg_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSgDown (fr . int_symbol));}
	if (fr . id ("scale_Tg")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTgUp (fr . int_symbol));}
	if (fr . id ("scale_Tg_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTgDown (fr . int_symbol));}
	if (fr . id ("scale_Sr")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSrUp (fr . int_symbol));}
	if (fr . id ("scale_Sr_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigSrDown (fr . int_symbol));}
	if (fr . id ("scale_Tr")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTrUp (1.0, fr . int_symbol));}
	if (fr . id ("scale_Tr_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTrDown (1.0, fr . int_symbol));}
	if (fr . id ("scale_T2r")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTrUp (0.0, fr . int_symbol));}
	if (fr . id ("scale_T2r_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigTrDown (0.0, fr . int_symbol));}
	if (fr . id ("scale_ISTd")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigISTdUp (fr . int_symbol));}
	if (fr . id ("scale_ISTd_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new TrigISTdDown (fr . int_symbol));}
	if (fr . id ("scale_SQRT_Sr")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSQRTSINErUp (fr . int_symbol));}
	if (fr . id ("scale_SQRT_Sr_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSQRTSINErDown (fr . int_symbol));}
	return true;
}

