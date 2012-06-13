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
#include "scale_log_base.h"

class ScalePUp : public Scale {
public:
	double locations_1 [1000];
	double locations_2 [1000];
	double locations_3 [1000];
	int left_index;
	void prepare_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			double value1 = (double) ind * 0.001;
			double value2 = (double) ind * 0.0001 + 0.9;
			double value3 = (double) ind * 0.00001 + 0.99;
			value1 *= value1; value2 *= value2; value3 *= value3;
			value1 = 1.0 - value1; value2 = 1.0 - value2; value3 = 1.0 - value3;
			value1 = sqrt (value1); value2 = sqrt (value2); value3 = sqrt (value3);
			locations_1 [ind] = (double) scale_length + (double) scale_length * log10 (value1);
			locations_2 [ind] = (double) scale_length + (double) scale_length * log10 (value2);
			locations_3 [ind] = (double) scale_length + (double) scale_length * log10 (value3);
		}
		double sentinel = 0.0 - (double) scale_length * left_extension;
		left_index = 0;
		while (left_index < 899 && locations_3 [left_index + 1] >= sentinel) left_index++;
	}
	virtual double getLocation (double x) {return 1.0 + log10 (sqrt (1.0 - x * x));}
	virtual double getValue (double x) {
		double ret = pow (10.0, x - 1.0);
		ret *= ret;
		return sqrt (1.0 - ret);
	}
	virtual void scaleInit (void) {faceUp (); prepare_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations_1 [0];
		draw_index_location (dc, x + location, _T ("0"));
		int description = 1;
		for (int ind = 0; ind < 900; ind += 100) location = draw_markings_for_100 (dc, & locations_1 [ind], wxString :: Format (_T ("%i"), description++), x, location);
		description = 1;
		for (int ind = 0; ind < 900; ind += 100) location = draw_markings_for_100 (dc, & locations_2 [ind], wxString :: Format (_T ("9%i"), description++), x, location);
		description = 1;
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind < left_index) location = draw_markings_for_100 (dc, & locations_3 [ind], wxString :: Format (_T ("99%i"), description++), x, location, 0, ind + 100 <= left_index ? 100 : left_index - ind);
		}
	}
	ScalePUp (int height) : Scale (height) {left_index = 900;}
};

class ScalePDown : public ScalePUp {
public:
	virtual void scaleInit (void) {faceDown (); prepare_locations ();}
	ScalePDown (int height) : ScalePUp (height) {}
};

/* GJM -- Extended P2 scales */
class ScaleP2Up : public Scale {
public:
	double locations_1 [1000];
	double locations_2 [1000];
	double locations_3 [1000];
	int left_index;
	void prepare_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			double value1 = (double) ind * 0.00001 + 0.99;
			double value2 = (double) ind * 0.000001 + 0.999;
			double value3 = (double) ind * 0.0000001 + 0.9999;
			value1 *= value1; value2 *= value2; value3 *= value3;
			value1 = 1.0 - value1; value2 = 1.0 - value2; value3 = 1.0 - value3;
			value1 = sqrt (value1); value2 = sqrt (value2); value3 = sqrt (value3);
			locations_1 [ind] = (double) scale_length + (double) scale_length + (double) scale_length * log10 (value1);
			locations_2 [ind] = (double) scale_length + (double) scale_length + (double) scale_length * log10 (value2);
			locations_3 [ind] = (double) scale_length + (double) scale_length + (double) scale_length * log10 (value3);
		}
		double sentinel = 0.0 - (double) scale_length * left_extension;
		left_index = 0;
		while (left_index < 899 && locations_3 [left_index + 1] >= sentinel) left_index++;
	}
	virtual double getLocation (double x) {return 2.0 + log10 (sqrt (1.0 - x * x));}
	virtual double getValue (double x) {
		double ret = pow (10.0, x - 2.0);
		ret *= ret;
		return sqrt (1.0 - ret);
	}
	virtual void scaleInit (void) {faceUp (); prepare_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations_1 [500];
		draw_index_location (dc, x + location, _T ("995"));
		int description = 6;
		for (int ind = 500; ind < 900; ind += 100) location = draw_markings_for_100 (dc, & locations_1 [ind], wxString :: Format (_T ("99%i"), description++), x, location);
		description = 1;
		for (int ind = 0; ind < 900; ind += 100) location = draw_markings_for_100 (dc, & locations_2 [ind], wxString :: Format (_T ("999%i"), description++), x, location);
		description = 1;
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind < left_index) location = draw_markings_for_100 (dc, & locations_3 [ind], wxString :: Format (_T ("9999%i"), description++), x, location, 0, ind + 100 <= left_index ? 100 : left_index - ind);
		}
	}
	ScaleP2Up (int height) : Scale (height) {left_index = 900;}
};

class ScaleP2Down : public ScaleP2Up {
public:
	virtual void scaleInit (void) {faceDown (); prepare_locations ();}
	ScaleP2Down (int height) : ScaleP2Up (height) {}
};

class ScalePHUp : public Scale {
public:
	double locations_1 [1000];
	double locations_2 [1000];
	double locations_3 [1000];
	double left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (sqrt (x * x - 1.0));}
	virtual double getValue (double x) {double ret = pow (10.0, x - 1.0); ret *= ret; return sqrt (ret + 1.0);}//return sqrt (ret - 1.0);}
	void init_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			double value1 = (double) ind * 0.001 + 1.1;
			double value2 = (double) ind * 0.0001 + 1.01;
			double value3 = (double) ind * 0.00001 + 1.001;
			locations_1 [ind] = (double) scale_length * (1.0 + log10 (sqrt (value1 * value1 - 1.0)));
			locations_2 [ind] = (double) scale_length * (1.0 + log10 (sqrt (value2 * value2 - 1.0)));
			locations_3 [ind] = (double) scale_length * (1.0 + log10 (sqrt (value3 * value3 - 1.0)));
		}
		left_index = find_left_index (locations_3, 0, 900, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations_1, 0, 900, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double l1 = locations_1 [0]; double l2 = locations_2 [0]; double l3 = locations_3 [0];
		int desc = 2;
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind + 100 >= left_index) l3 = draw_markings_for_100 (dc, & locations_3 [ind], ind == 800 ? _T ("") : wxString :: Format (_T ("1.%03i"), desc), x, l3, left_index <= ind ? 0 : left_index - ind, 100);
			l2 = draw_markings_for_100 (dc, & locations_2 [ind], ind == 800 ? _T ("") : wxString :: Format (_T ("1.%02i"), desc), x, l2);
			if (ind <= right_index) l1 = draw_markings_for_100 (dc, & locations_1 [ind], ind == 800 ? _T ("") : wxString :: Format (_T ("1.%i"), desc), x, l1, 0, right_index >= ind + 100 ? 100 : right_index - ind);
			desc++;
		}
		draw_text_marker (dc, _T ("1.01"), x + locations_2 [0]);
		draw_text_marker (dc, _T ("1.1"), x + locations_1 [0]);
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScalePHUp (int height) : Scale (height) {left_index = 0; right_index = 900;}
};

class ScalePHDown : public ScalePHUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScalePHDown (int height) : ScalePHUp (height) {}
};

class ScalePH2Up : public Scale {
public:
	double locations_2 [1000];
	double locations_3 [1000];
	double left_index;
	virtual double getLocation (double x) {return log10 (sqrt (x * x - 1.0));}
	virtual double getValue (double x) {double ret = pow (10.0, x); ret *= ret; return sqrt (ret + 1.0);}//return sqrt (ret - 1.0);}
	void init_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			double value2 = (double) ind * 0.01 + 1.0;
			double value3 = (double) ind * 0.001 + 1.1;
			locations_2 [ind] = (double) scale_length * (log10 (sqrt (value2 * value2 - 1.0)));
			locations_3 [ind] = (double) scale_length * (log10 (sqrt (value3 * value3 - 1.0)));
		}
		left_index = find_left_index (locations_3, 0, 900, 0.0 - (double) scale_length * left_extension);
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double l2 = locations_2 [0]; double l3 = locations_3 [0];
		int desc = 2;
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind + 100 >= left_index) l3 = draw_markings_for_100 (dc, & locations_3 [ind], ind == 800 ? _T ("2") : wxString :: Format (_T ("1.%1i"), desc), x, l3, left_index <= ind ? 0 : left_index - ind, 100);
			desc++;
		}
		desc = 3;
		for (int ind = 100; ind < 900; ind += 100) {
			l2 = draw_markings_for_100 (dc, & locations_2 [ind], wxString :: Format (_T ("%i"), desc++), x, l2);
		}
		if (absolute (locations_2 [200] - locations_2 [150]) > minimum_step * 4) draw_text_marker (dc, _T ("2.5"), x + locations_2 [150]);
		if (absolute (locations_2 [300] - locations_2 [250]) > minimum_step * 4) draw_text_marker (dc, _T ("3.5"), x + locations_2 [250]);
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScalePH2Up (int height) : Scale (height) {left_index = 0;}
};

class ScalePH2Down : public ScalePH2Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScalePH2Down (int height) : ScalePH2Up (height) {}
};

class ScalePTUp : public LogBase {
public:
	virtual double getLocation (double x) {return log10 (sqrt (1.0 + x * x));}
	virtual double getValue (double x) {x = pow (10.0, x); x *= x; x -= 1.0; x = sqrt (x); return x;}
	void init_pt (void) {
		for (int ind = 0; ind <= 1000; ind++) {
			double x = (double) ind / 100.0;
			x = 1.0 + x * x;
			logs [ind] = (double) scale_length * log10 (sqrt (x));
		}
	}
	virtual void scaleInit (void) {faceUp (); init_pt ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		draw_index_location (dc, x, _T ("0"));
//		draw_line (dc, get_bordercolour (x - 1.0), get_bordercolour (x + 1.0), x, y0, y1);
//		draw_text_marker (dc, _T ("0"), x);
		double location = logs [0];
		int description = 1;
		draw_markings_for_100 (dc, logs, _T ("1"), x, location, 20, 49);
		location = draw_markings_for_100 (dc, logs, _T ("1"), x, location, 50, 100);
		for (int ind = 100; ind < 1000; ind += 100) {
			location = draw_markings_for_100 (dc, & logs [ind], description < 9 ? wxString :: Format (_T ("%i"), description + 1) : _T ("10"), x, location);
			description++;
		}
	}
	ScalePTUp (int height) : LogBase (height) {}
};

class ScalePTDown : public ScalePTUp {
public:
	virtual void scaleInit (void) {faceDown (); init_pt ();}
	ScalePTDown (int height) : ScalePTUp (height) {}
};

bool check_pythagorean_scales (SetupFileReader & fr, Sliderule * slide_rule) {
	if (fr . id ("scale_P")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScalePUp (fr . int_symbol));}
	if (fr . id ("scale_P_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScalePDown (fr . int_symbol));}
/* GJM -- New extended P2 scales */
	if (fr . id ("scale_P2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleP2Up (fr . int_symbol));}
	if (fr . id ("scale_P2_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleP2Down (fr . int_symbol));}
	if (fr . id ("scale_PH")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScalePHUp (fr . int_symbol));}
	if (fr . id ("scale_PH_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScalePHDown (fr . int_symbol));}
	if (fr . id ("scale_PH2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScalePH2Up (fr . int_symbol));}
	if (fr . id ("scale_PH2_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScalePH2Down (fr . int_symbol));}
	if (fr . id ("scale_PT")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScalePTUp (fr . int_symbol));}
	if (fr . id ("scale_PT_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScalePTDown (fr . int_symbol));}
	return true;
}

