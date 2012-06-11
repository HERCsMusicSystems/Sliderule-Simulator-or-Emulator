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

#define _E        2.71828182845904523536
#define _PI       3.14159265358979323846

class ScaleSINHUp : public Scale {
public:
	double shift;
	double locations [4100];
	int left_index, right_index;
	void init_locations (void) {
		for (int ind = 1; ind <= 4000; ind++) {double location = (double) ind / 1000.0; locations [ind] = (double) scale_length * (shift + log10 (0.5 * (pow (_E, location) - pow (_E, - location))));}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 4000, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 4000, (double) scale_length * (1.0 + right_extension));
	}
	virtual double getLocation (double x) {return shift + log10 (0.5 * (pow (_E, x) - pow (_E, - x)));}
	virtual double getValue (double x) {x = pow (10.0, x - shift); return log (x + sqrt (x * x + 1.0));}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		double description = 0.1;
		for (int ind = 0; ind < 4000; ind += 100) {
			if (ind + 100 >= left_index && ind <= right_index) {
				location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%.1f"), description), x, location, left_index <= ind ? 0 : left_index - ind, right_index >= ind + 100 ? 100 : right_index - ind);
			}
			description += 0.1;
		}
	}
	ScaleSINHUp (double shift, int height) : Scale (height) {this -> shift = shift; left_index = 1; right_index = 3000;}
};

class ScaleSINHDown : public ScaleSINHUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleSINHDown (double shift, int height) : ScaleSINHUp (shift, height) {}
};

class ScaleCOSHUp : public Scale {
public:
	double locations [4100];
	int left_index, right_index;
	void init_locations (void) {
		for (int ind = 0; ind <= 4000; ind++) {double location = (double) ind / 1000.0; locations [ind] = (double) scale_length * log10 (0.5 * (pow (_E, location) + pow (_E, - location)));}
		left_index = 100;
		right_index = find_right_index (locations, 0, 4000, (double) scale_length * (1.0 + right_extension));
	}
	virtual double getLocation (double x) {return log10 (0.5 * (pow (_E, x) + pow (_E, - x)));}
	virtual double getValue (double x) {x = pow (10.0, x); return log (x + sqrt (x - 1.0) * sqrt (x + 1.0));}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		double description = 0.1;
		for (int ind = 0; ind < 4000; ind += 100) {
			if (ind + 100 >= left_index && ind <= right_index) {
				location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%.1f"), description), x, location, left_index <= ind ? 0 : left_index - ind, right_index >= ind + 100 ? 100 : right_index - ind);
			}
			description += 0.1;
		}
		draw_index_location (dc, x + locations [0], _T ("0"));
		if (absolute (locations [100] - locations [0]) >= minimum_step) {
			double location = x + locations [100];
			draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y1);
		}
	}
	ScaleCOSHUp (int height) : Scale (height) {}
};

class ScaleCOSHDown : public ScaleCOSHUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleCOSHDown (int height) : ScaleCOSHUp (height) {}
};

class ScaleTANHUp : public Scale {
public:
	double locations [4100];
	int left_index, right_index;
	void init_locations (void) {
		for (int ind = 1; ind <= 4000; ind++) {double location = (double) ind / 500.0; location = pow (_E, location); locations [ind] = (double) scale_length * (1.0 + log10 ((location - 1.0) / (location + 1.0)));}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 4000, 0.0 - (double) scale_length * left_extension);
		right_index = 2000;
	}
	virtual double getLocation (double x) {x = pow (_E, 2.0 * x); return 1.0 + log10 ((x - 1.0) / (x + 1.0));}
	virtual double getValue (double x) {x = pow (10.0, x - 1.0); if (x >= 1.0) x = 2.0; return 0.5 * log ((1.0 + x) / (1.0 - x));}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		double description = 0.1;
		for (int ind = 0; ind < 4000; ind += 100) {
			if (ind + 100 >= left_index && ind <= right_index) {
				location = draw_markings_for_100 (dc, & locations [ind], ind < 1000 ? wxString :: Format (_T ("%.1f"), description) : _T (""), x, location, left_index <= ind ? 0 : left_index - ind, right_index >= ind + 100 ? 100 : right_index - ind);
			}
			description += 0.1;
		}
		draw_text_marker (dc, _T ("1.5"), x + locations [1500]);
		draw_text_marker (dc, _T ("2"), x + locations [2000]);
		draw_index_location (dc, x + locations [3000], _T ("3"));
	}
	ScaleTANHUp (int height) : Scale (height) {left_index = 1; right_index = 2000;}
};

class ScaleTANHDown : public ScaleTANHUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleTANHDown (int height) : ScaleTANHUp (height) {}
};

class ScaleSINHgUp : public Scale {
public:
	double shift;
	double locations [3100];
	int left_index, right_index;
	virtual void init_locations (void) {
		for (int ind = 1; ind <= 3000; ind++) {
			double location = (double) ind * _PI / 2000.0;
			locations [ind] = (double) scale_length * (shift + log10 (0.5 * (pow (_E, location) - pow (_E, - location))));
		}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 3000, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 3000, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		int description = 10;
		for (int ind = 0; ind < 3000; ind += 100) {
			if (ind + 100 >= left_index && ind <= right_index) {
				location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%i"), description), x, location, left_index <= ind ? 0 : left_index - ind, right_index >= ind + 100 ? 100 : right_index - ind);
			}
			description += 10;
		}
		description = 1;
		for (int ind = 10; ind < 100; ind += 10) {
			if (left_index <= ind) draw_text_marker (dc, wxString :: Format (_T ("%i"), description), x + locations [ind]);
			description++;
		}
	}
	virtual double getLocation (double x) {x *= _PI / 200.0; return shift + log10 (0.5 * (pow (_E, x) - pow (_E, -x)));}
	virtual double getValue (double x) {x = pow (10.0, x - shift); return log (x + sqrt (x * x + 1.0)) * 200 / _PI;}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleSINHgUp (double shift, int height) : Scale (height) {this -> shift = shift; left_index = 1; right_index = 3000;}
};

class ScaleSINHgDown : public ScaleSINHgUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleSINHgDown (double shift, int height) : ScaleSINHgUp (shift, height) {}
};

class ScaleCOSHgUp : public ScaleSINHgUp {
public:
	virtual void init_locations (void) {
		for (int ind = 0; ind <= 3000; ind++) {
			double location = (double) ind * _PI / 2000.0;
			locations [ind] = (double) scale_length * (shift + log10 (0.5 * (pow (_E, location) + pow (_E, - location))));
		}
		left_index = 101;
		right_index = find_right_index (locations, 0, 3000, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		ScaleSINHgUp :: draw (dc, x);
		draw_index_location (dc, x + locations [0], _T ("0"));
		if (absolute (locations [100] - locations [0]) >= minimum_step) {
			double location = x + locations [100];
			draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y1);
		}
	}
	virtual double getLocation (double x) {x *= _PI / 200.0; return shift + log10 (0.5 * (pow (_E, x) + pow (_E, -x)));}
	virtual double getValue (double x) {x = pow (10.0, x); return log (x + sqrt (x - 1.0) * sqrt (x + 1.0)) * 200.0 / _PI;}
	ScaleCOSHgUp (int height) : ScaleSINHgUp (0.0, height) {}
};

class ScaleCOSHgDown : public ScaleCOSHgUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleCOSHgDown (int height) : ScaleCOSHgUp (height) {}
};

class ScaleTANHgUp : public Scale {
public:
	double locations [2100];
	int left_index, right_index;
	void init_locations (void) {
		for (int ind = 1; ind <= 2000; ind++) {
			double location = (double) ind * _PI / 1000.0;
			location = pow (_E, location);
			locations [ind] = (double) scale_length * (1.0 + log10 ((location - 1.0) / (location + 1.0)));
		}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 1500, 0.0 - (double) scale_length * left_extension);
		right_index = 2000; //find_right_index (locations, 1, 30000, (double) scale_length * (1.0 + right_extension));
	}
	virtual double getLocation (double x) {x *= _PI / 100.0; x = pow (_E, x); return 1.0 + log10 ((x - 1.0) / (x + 1.0));}
	virtual double getValue (double x) {x = pow (10.0, x - 1.0); if (x >= 1.0) x = 2.0; return 100.0 * log ((1.0 + x) / (1.0 - x)) / _PI;}
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
		location = draw_markings_for_100 (dc, & locations [800], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations [900], _T (""), x, location);
		draw_text_marker (dc, _T ("100"), x + locations [1000]);
		location = locations [1100];
		if (absolute (locations [1200] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [1200];
		if (absolute (locations [1300] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [1300];
		if (absolute (locations [1400] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [1400];
		if (absolute (locations [1500] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		draw_index_location (dc, x + locations [1500], _T (""));
		draw_index_location (dc, x + locations [2000], _T ("200"));
		description = 1;
		for (int ind = 10; ind < 100; ind += 10) {
			if (left_index <= ind) draw_text_marker (dc, wxString :: Format (_T ("%i"), description), x + locations [ind]);
			description++;
		}
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleTANHgUp (int height) : Scale (height) {}
};

class ScaleTANHgDown : public ScaleTANHgUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleTANHgDown (int height) : ScaleTANHgUp (height) {}
};

class ScaleSINHdUp : public TrigBase {
public:
	double shift;
	double locations [20100];
	int left_index, right_index;
	virtual void init_locations (void) {
		for (int ind = 1; ind <= 20000; ind++) {double location = (double) ind * _PI / 10800; locations [ind] = (double) scale_length * (shift + log10 (0.5 * (pow (_E, location) - pow (_E, - location))));}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 20000, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 20000, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		int description = 0;
		for (int ind = 0; ind < 20000; ind += 600) {
			if (ind + 600 >= left_index && ind <= right_index) {
				location = draw_markings_for_10 (dc, & locations [ind], description, x, location, false, left_index <= ind ? 0 : left_index - ind, right_index >= ind + 600 ? 600 : right_index - ind);
			}
			description += 10;
		}
	}
	virtual double getLocation (double x) {x = dec_recorrection (x); x *= _PI / 180.0; return shift + log10 (0.5 * (pow (_E, x) - pow (_E, - x)));}
	virtual double getValue (double x) {x = pow (10.0, x - shift); return dec_correction (log (x + sqrt (x * x + 1.0)) * 180.0 / _PI);}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleSINHdUp (double shift, int height) : TrigBase (height) {this -> shift = shift; left_index = 1; right_index = 20000;}
};

class ScaleSINHdDown : public ScaleSINHdUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleSINHdDown (double shift, int height) : ScaleSINHdUp (shift, height) {}
};

class ScaleCOSHdUp : public ScaleSINHdUp {
public:
	virtual void init_locations (void) {
		for (int ind = 0; ind <= 20000; ind++) {double location = (double) ind * _PI / 10800; locations [ind] = (double) scale_length * log10 (0.5 * (pow (_E, location) + pow (_E, - location)));}
		left_index = 601;
		right_index = find_right_index (locations, 0, 20000, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		ScaleSINHdUp :: draw (dc, x);
		draw_index_location (dc, x + locations [0], _T ("0"));
		if (absolute (locations [600] - locations [0]) >= minimum_step) {
			double location = x + locations [600];
			draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y1);
		}
	}
	virtual double getLocation (double x) {x = dec_recorrection (x); x *= _PI / 180.0; return log10 (0.5 * (pow (_E, x) + pow (_E, - x)));}
	virtual double getValue (double x) {x = pow (10.0, x); return dec_correction (log (x + sqrt (x - 1.0) * sqrt (x + 1.0)) * 180.0 / _PI);}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleCOSHdUp (int height) : ScaleSINHdUp (0.0, height) {}
};

class ScaleCOSHdDown : public ScaleCOSHdUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleCOSHdDown (int height) : ScaleCOSHdUp (height) {}
};

class ScaleTANHdUp : public TrigBase {
public:
	double locations [12100];
	int left_index, right_index;
	void init_locations (void) {
		for (int ind = 1; ind <= 12000; ind++) {
			double location = (double) ind * _PI / 5400.0;
			location = pow (_E, location);
			locations [ind] = (double) scale_length * (1.0 + log10 ((location - 1.0) / (location + 1.0)));
		}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 12000, 0.0 - (double) scale_length * left_extension);
		right_index = 12000;
	}
	virtual double getLocation (double x) {x = dec_recorrection (x); x *= _PI / 90.0; x = pow (_E, x); return 1.0 + log10 ((x - 1.0) / (x + 1.0));}
	virtual double getValue (double x) {x = pow (10.0, x - 1.0); if (x >= 1.0) x = 2.0; return dec_correction (90.0 * log ((1.0 + x) / (1.0 - x)) / _PI);}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		int description = 0;
		for (int ind = 0; ind < 5400; ind += 600) {
			if (ind + 600 >= left_index && ind <= right_index) {
				location = draw_markings_for_10 (dc, & locations [ind], description, x, location, ind >= 3000, left_index <= ind ? 0 : left_index - ind, right_index >= ind + 600 ? 600 : right_index - ind);
			}
			description += 10;
		}
		wxSize extent = dc . GetTextExtent (_T ("00"));
		draw_text_marker (dc, _T ("90"), x + locations [5400]);
		if (absolute (locations [5400] - locations [4800]) >= extent . x) draw_text_marker (dc, _T ("80"), x + locations [4800]);
		if (absolute (locations [4800] - locations [4200]) >= extent . x) draw_text_marker (dc, _T ("70"), x + locations [4200]);
		if (absolute (locations [4200] - locations [3600]) >= extent . x) draw_text_marker (dc, _T ("60"), x + locations [3600]);
		location = locations [6000];
		if (absolute (locations [6600] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [6600];
		if (absolute (locations [7200] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [7200];
		if (absolute (locations [7800] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [7800];
		if (absolute (locations [8400] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [8400];
		if (absolute (locations [9000] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [9000];
		if (absolute (locations [9600] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [9600];
		if (absolute (locations [10200] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [10200];
		if (absolute (locations [10800] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		draw_index_location (dc, x + locations [10800], _T ("180"));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleTANHdUp (int height) : TrigBase (height) {}
};

class ScaleTANHdDown : public ScaleTANHdUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleTANHdDown (int height) : ScaleTANHdUp (height) {}
};

class ScaleSINHdecUp : public TrigBase {
public:
	double shift;
	double locations [33400];
	int left_index, right_index;
	virtual void init_locations (void) {
		for (int ind = 1; ind <= 33300; ind++) {double location = (double) ind * _PI / 18000; locations [ind] = (double) scale_length * (shift + log10 (0.5 * (pow (_E, location) - pow (_E, - location))));}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 33300, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 33300, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		int description = 0;
		for (int ind = 0; ind < 33300; ind += 1000) {
			if (ind + 1000 >= left_index && ind <= right_index) {
				location = draw_markings_for_dec_10 (dc, & locations [ind], description, x, location, false, left_index <= ind ? 0 : left_index - ind, right_index >= ind + 1000 ? 1000 : right_index - ind);
			}
			description += 10;
		}
	}
	virtual double getLocation (double x) {x *= _PI / 180.0; return shift + log10 (0.5 * (pow (_E, x) - pow (_E, - x)));}
	virtual double getValue (double x) {x = pow (10.0, x - shift); return log (x + sqrt (x * x + 1.0)) * 180.0 / _PI;}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleSINHdecUp (double shift, int height) : TrigBase (height) {this -> shift = shift; left_index = 1; right_index = 20000;}
};

class ScaleSINHdecDown : public ScaleSINHdecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleSINHdecDown (double shift, int height) : ScaleSINHdecUp (shift, height) {}
};

class ScaleCOSHdecUp : public ScaleSINHdecUp {
public:
	virtual void init_locations (void) {
		for (int ind = 0; ind <= 33300; ind++) {double location = (double) ind * _PI / 18000; locations [ind] = (double) scale_length * log10 (0.5 * (pow (_E, location) + pow (_E, - location)));}
		left_index = 1001;
		right_index = find_right_index (locations, 0, 33300, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		ScaleSINHdecUp :: draw (dc, x);
		draw_index_location (dc, x + locations [0], _T ("0"));
		if (absolute (locations [1000] - locations [0]) >= minimum_step) {
			double location = x + locations [1000];
			draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y1);
		}
	}
	virtual double getLocation (double x) {x *= _PI / 180.0; return log10 (0.5 * (pow (_E, x) + pow (_E, - x)));}
	virtual double getValue (double x) {x = pow (10.0, x); return log (x + sqrt (x - 1.0) * sqrt (x + 1.0)) * 180.0 / _PI;}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleCOSHdecUp (int height) : ScaleSINHdecUp (0.0, height) {}
};

class ScaleCOSHdecDown : public ScaleCOSHdecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleCOSHdecDown (int height) : ScaleCOSHdecUp (height) {}
};

class ScaleTANHdecUp : public TrigBase {
public:
	double locations [20100];
	int left_index, right_index;
	void init_locations (void) {
		for (int ind = 1; ind <= 20000; ind++) {
			double location = (double) ind * _PI / 9000.0;
			location = pow (_E, location);
			locations [ind] = (double) scale_length * (1.0 + log10 ((location - 1.0) / (location + 1.0)));
		}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 20000, 0.0 - (double) scale_length * left_extension);
		right_index = 20000;
	}
	virtual double getLocation (double x) {x *= _PI / 90.0; x = pow (_E, x); return 1.0 + log10 ((x - 1.0) / (x + 1.0));}
	virtual double getValue (double x) {x = pow (10.0, x - 1.0); if (x >= 1.0) x = 2.0; return 90.0 * log ((1.0 + x) / (1.0 - x)) / _PI;}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		int description = 0;
		for (int ind = 0; ind < 9000; ind += 1000) {
			if (ind + 1000 >= left_index && ind <= right_index) {
				location = draw_markings_for_dec_10 (dc, & locations [ind], description, x, location, ind >= 5000, left_index <= ind ? 0 : left_index - ind, right_index >= ind + 1000 ? 1000 : right_index - ind);
			}
			description += 10;
		}
		wxSize extent = dc . GetTextExtent (_T ("00"));
		draw_text_marker (dc, _T ("90"), x + locations [9000]);
		if (absolute (locations [9000] - locations [8000]) >= extent . x) draw_text_marker (dc, _T ("80"), x + locations [8000]);
		if (absolute (locations [8000] - locations [7000]) >= extent . x) draw_text_marker (dc, _T ("70"), x + locations [7000]);
		if (absolute (locations [7000] - locations [6000]) >= extent . x) draw_text_marker (dc, _T ("60"), x + locations [6000]);
		location = locations [10000];
		if (absolute (locations [11000] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [11000];
		if (absolute (locations [12000] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [12000];
		if (absolute (locations [13000] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [13000];
		if (absolute (locations [14000] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [14000];
		if (absolute (locations [15000] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [15000];
		if (absolute (locations [16000] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [16000];
		if (absolute (locations [17000] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		location = locations [17000];
		if (absolute (locations [18000] - location) >= minimum_step) {location += x; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);}
		draw_index_location (dc, x + locations [18000], _T ("180"));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleTANHdecUp (int height) : TrigBase (height) {}
};

class ScaleTANHdecDown : public ScaleTANHdecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleTANHdecDown (int height) : ScaleTANHdecUp (height) {}
};

bool check_hyperbolic_scales (SetupFileReader & fr, Sliderule * slide_rule) {
	if (fr . id ("scale_SINH1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHUp (1.0, fr . int_symbol));}
	if (fr . id ("scale_SINH1_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHDown (1.0, fr . int_symbol));}
	if (fr . id ("scale_SINH2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHUp (0.0, fr . int_symbol));}
	if (fr . id ("scale_SINH2_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHDown (0.0, fr . int_symbol));}
	if (fr . id ("scale_COSH")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleCOSHUp (fr . int_symbol));}
	if (fr . id ("scale_COSH_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleCOSHDown (fr . int_symbol));}
	if (fr . id ("scale_TANH")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleTANHUp (fr . int_symbol));}
	if (fr . id ("scale_TANH_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleTANHDown (fr . int_symbol));}
	if (fr . id ("scale_SINHg1") || fr . id ("scale_SINH1g")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHgUp (1.0, fr . int_symbol));}
	if (fr . id ("scale_SINHg1_down") || fr . id ("scale_SINH1g_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHgDown (1.0, fr . int_symbol));}
	if (fr . id ("scale_SINHg2") || fr . id ("scale_SINH2g")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHgUp (0.0, fr . int_symbol));}
	if (fr . id ("scale_SINHg2_down") || fr . id ("scale_SINH2g_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHgDown (0.0, fr . int_symbol));}
	if (fr . id ("scale_COSHg")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleCOSHgUp (fr . int_symbol));}
	if (fr . id ("scale_COSHg_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleCOSHgDown (fr . int_symbol));}
	if (fr . id ("scale_TANHg")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleTANHgUp (fr . int_symbol));}
	if (fr . id ("scale_TANHg_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleTANHgDown (fr . int_symbol));}
	if (fr . id ("scale_SINHd1") || fr . id ("scale_SINH1d")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHdUp (1.0, fr . int_symbol));}
	if (fr . id ("scale_SINHd1_down") || fr . id ("scale_SINH1d_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHdDown (1.0, fr . int_symbol));}
	if (fr . id ("scale_SINHd2") || fr . id ("scale_SINH2d")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHdUp (0.0, fr . int_symbol));}
	if (fr . id ("scale_SINHd2_down") || fr . id ("scale_SINH2d_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHdDown (0.0, fr . int_symbol));}
	if (fr . id ("scale_COSHd")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleCOSHdUp (fr . int_symbol));}
	if (fr . id ("scale_COSHd_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleCOSHdDown (fr . int_symbol));}
	if (fr . id ("scale_TANHd")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleTANHdUp (fr . int_symbol));}
	if (fr . id ("scale_TANHd_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleTANHdDown (fr . int_symbol));}
	if (fr . id ("scale_SINHdec1") || fr . id ("scale_SINH1dec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHdecUp (1.0, fr . int_symbol));}
	if (fr . id ("scale_SINHdec1_down") || fr . id ("scale_SINH1dec_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHdecDown (1.0, fr . int_symbol));}
	if (fr . id ("scale_SINHdec2") || fr . id ("scale_SINH2dec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHdecUp (0.0, fr . int_symbol));}
	if (fr . id ("scale_SINHdec2_down") || fr . id ("scale_SINH2dec_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSINHdecDown (0.0, fr . int_symbol));}
	if (fr . id ("scale_COSHdec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleCOSHdecUp (fr . int_symbol));}
	if (fr . id ("scale_COSHdec_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleCOSHdecDown (fr . int_symbol));}
	if (fr . id ("scale_TANHdec")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleTANHdecUp (fr . int_symbol));}
	if (fr . id ("scale_TANHdec_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleTANHdecDown (fr . int_symbol));}
	return true;
}

