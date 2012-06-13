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

#define _E        2.71828182845904523536

class ScaleLLUp : public Scale {
public:
	double locations_1 [1200];
	double locations_2 [1200];
	double locations_3 [1200];
	double locations_4 [1200];
	double locations_5 [1200];
	double locations_6 [1200];
	double locations_7 [1200];
	void init_locations (void) {
		for (int ind = 0; ind <= 1000; ind++) {
			locations_1 [ind] = (double) scale_length * (2.0 + log10 (log (1.01 + (double) ind / 10000.0))) / 3.0;
			locations_2 [ind] = (double) scale_length * (2.0 +  log10 (log (1.1 + (double) ind / 1000.0))) / 3.0;
			locations_3 [ind] = (double) scale_length * (2.0 +  log10 (log (2.0 + (double) ind / 100.0))) / 3.0;
		}
		for (int ind = 0; ind < 120; ind++) {
			locations_4 [ind] = (double) scale_length * (2.0 + log10 (log (10.0 + (double) ind))) / 3.0;
			locations_5 [ind] = (double) scale_length * (2.0 + log10 (log (100.0 + (double) ind * 10.0))) / 3.0;
			locations_6 [ind] = (double) scale_length * (2.0 + log10 (log (1000.0 + (double) ind * 100.0))) / 3.0;
			locations_7 [ind] = (double) scale_length * (2.0 + log10 (log (10000.0 + (double) ind * 1000.0))) / 3.0;
		}
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind < 500) {
				dc . DrawText (wxString :: Format (_T ("1.%02i"), 1 + ind / 100), x + locations_1 [ind] - height / 2, font_y);
				dc . DrawText (wxString :: Format (_T ("1.%i"), 1 + ind / 100), x + locations_2 [ind] - height / 3, font_y);
			}
			draw_line (dc, lcp, x + locations_1 [ind], y0, y1);
			draw_line (dc, lcp, x + locations_2 [ind], y0, y1);
			draw_markings (dc, & locations_1 [ind], x);
			draw_markings (dc, & locations_2 [ind], x);
		}
		for (int ind = 0; ind < 800; ind += 100) {
			if (ind < 600) dc . DrawText (wxString :: Format (_T ("%i"), 2 + ind / 100), x + locations_3 [ind] - height / 6, font_y);
			draw_line (dc, lcp, x + locations_3 [ind], y0, y1);
			draw_markings (dc, & locations_3 [ind], x);
		}
		dc . DrawText (_T ("10"), x + locations_4 [0] - height / 3, font_y);
		dc . DrawText (_T ("2"), x + locations_4 [10] - height / 6, font_y);
		dc . DrawText (_T ("5"), x + locations_4 [40] - height / 6, font_y);
		for (int ind = 0; ind < 90; ind += 10) {
			if (absolute (locations_4 [ind + 10] - locations_4 [ind]) > minimum_step) draw_line (dc, lcp, x + locations_4 [ind], y0, y1);
			draw_dense_markings (dc, & locations_4 [ind], x);
		}
		dc . DrawText (_T ("100"), x + locations_5 [0] - height / 2, font_y);
		for (int ind = 0; ind < 90; ind += 10) {
			if (absolute (locations_5 [ind + 10] - locations_5 [ind]) > minimum_step) draw_line (dc, lcp, x + locations_5 [ind], y0, y1);
			draw_dense_markings (dc, & locations_5 [ind], x);
		}
		dc . DrawText (_T ("1k"), x + locations_6 [0] - height / 3, font_y);
		for (int ind = 0; ind < 90; ind += 10) {
			if (absolute (locations_6 [ind + 10] - locations_6 [ind]) > minimum_step) draw_line (dc, lcp, x + locations_6 [ind], y0, y1);
			draw_dense_markings (dc, & locations_6 [ind], x);
		}
		dc . DrawText (_T ("10k"), x + locations_7 [0] - height / 2, font_y);
		for (int ind = 0; ind < 90; ind += 10) {
			if (absolute (locations_7 [ind + 10] - locations_7 [ind]) > minimum_step) draw_line (dc, lcp, x + locations_7 [ind], y0, y1);
			draw_dense_markings (dc, & locations_7 [ind], x);
		}
		dc . DrawText (_T ("100k"), x + locations_7 [90] - height / 2, font_y);
		draw_line (dc, lcp, x + locations_7 [90], y0, y1);
	}
	virtual double getLocation (double x) {return (2.0 + log10 (log (x))) / 3.0;}
	virtual double getValue (double x) {
		x = x * 3.0 - 2.0;
		return pow (_E, pow (10.0, x));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleLLUp (int height) : Scale (height) {}
};

class ScaleLLDown : public ScaleLLUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLLDown (int height) : ScaleLLUp (height) {}
};

class ScaleLLRUp : public ScaleLLUp {
public:
	virtual double getLocation (double x) {return ScaleLLUp :: getLocation (1.0 / x);}
	virtual double getValue (double x) {return 1.0 / ScaleLLUp :: getValue (x);}
	void init_reciprocal_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			locations_1 [ind] = (double) scale_length * (2.0 + log10 (log (1.0 / (0.9 + (double) ind / 10000.0)))) / 3.0;
		}
		for (int ind = 0; ind <= 1200; ind++) {
			locations_2 [ind] = (double) scale_length * (2.0 + log10 (log (1.0 / (0.8 + (double) ind / 10000.0)))) / 3.0;
		}
		for (int ind = 0; ind < 900; ind++) {
			locations_3 [ind] = (double) scale_length * (2.0 + log10 (log (1.0 / (0.1 + (double) ind / 1000.0)))) / 3.0;
		}
		for (int ind = 0; ind <= 1000; ind++) {
			locations_4 [ind] = (double) scale_length * (2.0 + log10 (log (1.0 / (0.01 + (double) ind / 10000.0)))) / 3.0;
			locations_5 [ind] = (double) scale_length * (2.0 + log10 (log (1.0 / (0.001 + (double) ind / 100000.0)))) / 3.0;
			locations_6 [ind + 10] = (double) scale_length * (2.0 + log10 (log (1.0 / (0.00001 + (double) ind / 1000000.0)))) / 3.0;
		}
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		markings_increasing = true;
		for (int ind = 0; ind <= 900; ind += 100) {
			dc . DrawText (wxString :: Format (_T (".9%i"), ind / 100), x + locations_1 [ind] - height / 2, font_y);
			draw_line (dc, lcp, x + locations_1 [ind], y0, y1);
			if (ind < 900) draw_markings (dc, & locations_1 [ind], x);
		}
		for (int ind = 0; ind <= 900; ind += 100) {
			draw_line (dc, lcp, x + locations_2 [ind], y0, y1);
			if (ind < 1100) draw_markings (dc, & locations_2 [ind], x);
		}
		dc . DrawText (_T (".8"), x + locations_2 [0] - height / 6, font_y);
		dc . DrawText (_T (".85"), x + locations_2 [500] - height / 3, font_y);
		for (int ind = 0; ind < 700; ind += 100) {
			dc . DrawText (wxString :: Format (_T (".%i"), 1 + ind / 100), x + locations_3 [ind] - height / 3, font_y);
			draw_line (dc, lcp, x + locations_3 [ind], y0, y1);
			draw_markings (dc, & locations_3 [ind], x, 0, 100);
		}
		markings_increasing = false;
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind == 0 || ind == 400) dc . DrawText (wxString :: Format (_T (".%02i"), 1 + ind / 100), x + locations_4 [ind] - height / 3, font_y);
			draw_line (dc, lcp, x + locations_4 [ind], y0, y1);
			draw_markings (dc, & locations_4 [ind], x);
			if (ind == 0) dc . DrawText (wxString :: Format (_T (".%03i"), 1 + ind / 100), x + locations_5 [ind] - height / 2, font_y);
			if (absolute (locations_5 [ind + 100] - locations_5 [ind]) > minimum_step) {
				draw_line (dc, lcp, x + locations_5 [ind], y0, y1);
				draw_markings (dc, & locations_5 [ind], x);
			}
		}
		draw_line (dc, lcp, x + locations_6 [50], y0, y2);
		draw_line (dc, lcp, x + locations_6 [100], y0, y1);
		draw_line (dc, lcp, x + locations_6 [500], y0, y2);
		dc . DrawText (_T (".0001"), x + locations_6 [100] - height / 2, font_y);
	}
	virtual void scaleInit (void) {faceUp (); init_reciprocal_locations ();}
	ScaleLLRUp (int height) : ScaleLLUp (height) {}
};

class ScaleLLRDown : public ScaleLLRUp {
public:
	virtual void scaleInit (void) {faceDown (); init_reciprocal_locations ();}
	ScaleLLRDown (int height) : ScaleLLRUp (height) {}
};

/* GJM -- LLD scales (log10 log10) */
class ScaleLLD0Up : public Scale {
public:
	double locations1 [900];
	double locations2 [300];
	int left_index, right_index;
	virtual double getLocation (double x) {return 4.0 + log10 (log10 (x));}
	virtual double getValue (double x) {return pow (10.0, pow (10.0, x - 4.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 800; ind++) {
			locations1 [ind] = (double) scale_length * (4.0 + log10 (log10 (1.0002 + (double) ind / 1000000.0)));
		}
		for (int ind = 0; ind <= 200; ind++) {
			locations2 [ind] = (double) scale_length * (4.0 + log10 (log10 (1.001 + (double) ind / 100000.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 10, 800, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 10, 200, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("1.0003"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("1.0004"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("1.0005"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("1.0006"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("1.0007"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("1.0008"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("1.0009"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T ("1.001"), x, location);
		location = draw_markings_for_100 (dc, locations2, _T ("1.002"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T ("1.003"), x, location, 0, right_index - 100);
	}
	ScaleLLD0Up (int height) : Scale (height) {left_index = 10; right_index = 1000;}
};

class ScaleLLD0Down : public ScaleLLD0Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLLD0Down (int height) : ScaleLLD0Up (height) {}
};

class ScaleLLD1Up : public Scale {
public:
	double locations1 [900];
	double locations2 [300];
	int left_index, right_index;
	virtual double getLocation (double x) {return 3.0 + log10 (log10 (x));}
	virtual double getValue (double x) {return pow (10.0, pow (10.0, x - 3.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 800; ind++) {
			locations1 [ind] = (double) scale_length * (3.0 + log10 (log10 (1.002 + (double) ind / 100000.0)));
		}
		for (int ind = 0; ind <= 200; ind++) {
			locations2 [ind] = (double) scale_length * (3.0 + log10 (log10 (1.01 + (double) ind / 10000.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 10, 800, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 10, 200, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("1.003"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("1.004"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("1.005"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("1.006"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("1.007"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("1.008"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("1.009"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T ("1.01"), x, location);
		location = draw_markings_for_100 (dc, locations2, _T ("1.02"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T ("1.03"), x, location, 0, right_index - 100);
	}
	ScaleLLD1Up (int height) : Scale (height) {left_index = 10; right_index = 1000;}
};

class ScaleLLD1Down : public ScaleLLD1Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLLD1Down (int height) : ScaleLLD1Up (height) {}
};

class ScaleLLD2Up : public Scale {
public:
	double locations1 [1900];
	double locations2 [200];
	int left_index, right_index;
	virtual double getLocation (double x) {return 2.0 + log10 (log10 (x));}
	virtual double getValue (double x) {return pow (10.0, pow (10.0, x - 2.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 1800; ind++) {
			locations1 [ind] = (double) scale_length * (2.0 + log10 (log10 (1.02 + (double) ind / 10000.0)));
		}
		for (int ind = 0; ind <= 100; ind++) {
			locations2 [ind] = (double) scale_length * (2.0 + log10 (log10 (1.2 + (double) ind / 1000.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 10, 1800, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 10, 100, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("1.03"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("1.04"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("1.05"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("1.06"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("1.07"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("1.08"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("1.09"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T ("1.1"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [800], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [900], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1000], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1100], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1200], _T ("1.15"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1300], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1400], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1500], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1600], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1700], _T ("1.2"), x, location);
		location = draw_markings_for_100 (dc, locations2, _T ("1.3"), x, location, 0, right_index);
	}
	ScaleLLD2Up (int height) : Scale (height) {left_index = 10; right_index = 1900;}
};

class ScaleLLD2Down : public ScaleLLD2Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLLD2Down (int height) : ScaleLLD2Up (height) {}
};

class ScaleLLD3Up : public Scale {
public:
	double locations1 [900];
	double locations2 [1000];
	int left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (log10 (x));}
	virtual double getValue (double x) {return pow (10.0, pow (10.0, x - 1.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 800; ind++) {
			locations1 [ind] = (double) scale_length * (1.0 + log10 (log10 (1.2 + (double) ind / 1000.0)));
		}
		for (int ind = 0; ind <= 900; ind++) {
			locations2 [ind] = (double) scale_length * (1.0 + log10 (log10 (2.0 + (double) ind / 100.0)));
		}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 10, 800, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 10, 900, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("1.3"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T ("1.4"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T ("1.5"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T ("1.6"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T ("1.7"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T ("1.8"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T ("1.9"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T ("2"), x, location);
		location = draw_markings_for_100 (dc, locations2, _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [200], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [300], _T ("6"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [400], _T ("7"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [500], _T ("8"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [600], _T ("9"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [700], _T ("10"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [800], _T ("11"), x, location, 0, right_index-800);
	}
	ScaleLLD3Up (int height) : Scale (height) {left_index = 10; right_index = 1600;}
};

class ScaleLLD3Down : public ScaleLLD3Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLLD3Down (int height) : ScaleLLD3Up (height) {}
};

class ScaleLLD4Up : public Scale {
public:
	double locations1 [200];
	double locations2 [1000];
	double locations3 [1000];
	double locations4 [1000];
	double locations5 [5500];
	int left_index, right_index;
	virtual double getLocation (double x) {return log10 (log10 (x));}
	virtual double getValue (double x) {return pow (10.0, pow (10.0, x));}
	void init_locations (void) {
		for (int ind = 1; ind <= 100; ind++) {
			locations1 [ind] = (double) scale_length * (log10 (log10 (9.0 + (double) ind / 100.0)));
		}
		for (int ind = 0; ind <= 900; ind++) {locations2 [ind] = (double) scale_length * log10 (log10 (10.0 + (double) ind / 10.0));}
		for (int ind = 0; ind <= 900; ind++) {locations3 [ind] = (double) scale_length * log10 (log10 (100.0 + (double) ind));}
		for (int ind = 0; ind <= 900; ind++) {locations4 [ind] = (double) scale_length * log10 (log10 (1000.0 + (double) ind * 10.0));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [ind] = (double) scale_length * log10 (log10 (10000.0 + (double) ind * 100.0));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [900 + ind] = (double) scale_length * log10 (log10 (100000.0 + (double) ind * 1000.0));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [1800 + ind] = (double) scale_length * log10 (log10 (1000000.0 + (double) ind * 10000.0));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [2700 + ind] = (double) scale_length * log10 (log10 (10000000.0 + (double) ind * 100000.0));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [3600 + ind] = (double) scale_length * log10 (log10 (100000000.0 + (double) ind * 1000000.0));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [4500 + ind] = (double) scale_length * log10 (log10 (1000000000.0 + (double) ind * 10000000.0));}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 10, 900, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations4, 10, 5400, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T ("10"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, locations2, _T ("20"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T ("30"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [200], _T ("40"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [300], _T ("50"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [400], _T ("60"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [500], _T ("70"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [600], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations2 [700], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations2 [800], _T (""), x, location);
		draw_text_marker (dc, _T ("100"), x + locations2 [900]);
		location = draw_markings_for_100 (dc, locations3, _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations3 [100], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations3 [200], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations3 [300], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations3 [400], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations3 [500], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations3 [600], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations3 [700], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations3 [800], _T (""), x, location);
		draw_text_marker (dc, _T ("1k"), x + locations3 [900]);
		location = draw_markings_for_100 (dc, locations4, _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations4 [100], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations4 [200], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations4 [300], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations4 [400], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations4 [500], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations4 [600], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations4 [700], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations4 [800], _T (""), x, location);
		draw_text_marker (dc, _T ("10k"), x + locations4 [900]);
		location = draw_markings_for_100 (dc, locations5, _T ("20"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [100], _T ("30"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [200], _T ("40"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [300], _T ("50"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [400], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [500], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [600], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [700], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [800], _T (""), x, location);
		draw_text_marker (dc, _T ("100k"), x + locations5 [900]);
		location = draw_markings_for_100 (dc, & locations5 [900], _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1000], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1100], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1200], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1300], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1400], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1500], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [1600], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1700], _T (""), x, location);
		draw_text_marker (dc, _T ("1M"), x + locations5 [1800]);
		location = draw_markings_for_100 (dc, & locations5 [1800], _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1900], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2000], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2100], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2200], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2300], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2400], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [2500], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2600], _T (""), x, location);
		draw_text_marker (dc, _T ("10M"), x + locations5 [2700]);
		location = draw_markings_for_100 (dc, & locations5 [2700], _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2800], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2900], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [3000], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [3100], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [3200], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [3300], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [3400], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [3500], _T (""), x, location);
		draw_text_marker (dc, _T ("100M"), x + locations5 [3600]);
		location = draw_markings_for_100 (dc, & locations5 [3600], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [3700], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [3800], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [3800], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4000], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4100], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4200], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4300], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [4400], _T (""), x, location);
		draw_text_marker (dc, _T ("1G"), x + locations5 [4500]);
		location = draw_markings_for_100 (dc, & locations5 [4500], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4600], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4700], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [4800], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4900], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [5000], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [5100], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [5200], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [5300], _T (""), x, location);
		draw_text_marker (dc, _T ("10G"), x + locations5 [5400]);
	}
	ScaleLLD4Up (int height) : Scale (height) {left_index = 10; right_index = 1000;}
};

class ScaleLLD4Down : public ScaleLLD4Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLLD4Down (int height) : ScaleLLD4Up (height) {}
};

class ScaleLLD00Up : public Scale {
public:
	double locations1 [900];
	double locations2 [300];
	int left_index, right_index;
	virtual double getLocation (double x) {return 4.0 + log10 (log10 (1.0 / x));}
	virtual double getValue (double x) {return 1.0 / pow (10.0, pow (10.0, x - 4.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 800; ind++) {locations1 [ind] = (double) scale_length * (4.0 + log10 (log10 (1.0 / (0.9998 - (double) ind / 1000000.0))));}
		for (int ind = 0; ind <= 200; ind++) {locations2 [ind] = (double) scale_length * (4.0 + log10 (log10 (1.0 / (0.999 - (double) ind / 100000.0))));}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 800, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 1, 200, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T (".9997"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T (".9996"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T (".9995"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T (".9994"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T (".9993"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T (".9992"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T (".9991"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T (".999"), x, location);
		location = draw_markings_for_100 (dc, locations2, _T (".998"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T (".997"), x, location, 0, right_index - 100);
	}
	ScaleLLD00Up (int height) : Scale (height) {left_index = 1; right_index = 1000;}
};

class ScaleLLD00Down : public ScaleLLD00Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLLD00Down (int height) : ScaleLLD00Up (height) {}
};

class ScaleLLD01Up : public Scale {
public:
	double locations1 [900];
	double locations2 [300];
	int left_index, right_index;
	virtual double getLocation (double x) {return 3.0 + log10 (log10 (1.0 / x));}
	virtual double getValue (double x) {return 1.0 / pow (10.0, pow (10.0, x - 3.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 800; ind++) {locations1 [ind] = (double) scale_length * (3.0 + log10 (log10 (1.0 / (0.998 - (double) ind / 100000.0))));}
		for (int ind = 0; ind <= 200; ind++) {locations2 [ind] = (double) scale_length * (3.0 + log10 (log10 (1.0 / (0.99 - (double) ind / 10000.0))));}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 800, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 1, 200, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T (".997"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T (".996"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T (".995"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T (".994"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T (".993"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T (".992"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T (".991"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T (".99"), x, location);
		location = draw_markings_for_100 (dc, locations2, _T (".98"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T (".97"), x, location, 0, right_index - 100);
	}
	ScaleLLD01Up (int height) : Scale (height) {left_index = 1; right_index = 1000;}
};

class ScaleLLD01Down : public ScaleLLD01Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLLD01Down (int height) : ScaleLLD01Up (height) {}
};

class ScaleLLD02Up : public Scale {
public:
	double locations1 [1900];
	double locations2 [200];
	int left_index, right_index;
	virtual double getLocation (double x) {return 2.0 + log10 (log10 (1.0 / x));}
	virtual double getValue (double x) {return 1.0 / pow (10.0, pow (10.0, x - 2.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 1800; ind++) {locations1 [ind] = (double) scale_length * (2.0 + log10 (log10 (1.0 / (0.98 - (double) ind / 10000.0))));}
		for (int ind = 0; ind <= 100; ind++) {locations2 [ind] = (double) scale_length * (2.0 + log10 (log10 (1.0 / (0.8 - (double) ind / 1000.0))));}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 1800, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations2, 1, 100, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T (".97"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations1 [100], _T (".96"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [200], _T (".95"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [300], _T (".94"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [400], _T (".93"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [500], _T (".92"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [600], _T (".91"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [700], _T (".9"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [800], _T (".89"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [900], _T (".88"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1000], _T (".87"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1100], _T (".86"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1200], _T (".85"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1300], _T (".84"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1400], _T (".83"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1500], _T (".82"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1600], _T (".81"), x, location);
		location = draw_markings_for_100 (dc, & locations1 [1700], _T (".8"), x, location);
		location = draw_markings_for_100 (dc, locations2, _T (".7"), x, location, 0, right_index);
	}
	ScaleLLD02Up (int height) : Scale (height) {left_index = 1; right_index = 1900;}
};

class ScaleLLD02Down : public ScaleLLD02Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLLD02Down (int height) : ScaleLLD02Up (height) {}
};

class ScaleLLD03Up : public Scale {
public:
	double locations [900];
	int left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (log10 (1.0 / x));}
	virtual double getValue (double x) {return 1.0 / pow (10.0, pow (10.0, x - 1.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 800; ind++) {locations [ind] = (double) scale_length * (1.0 + log10 (log10 (1.0 / (0.9 - (double) ind / 1000.0))));}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 800, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 800, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		location = draw_markings_for_100 (dc, locations, _T (".8"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations [100], _T (".7"), x, location);
		location = draw_markings_for_100 (dc, & locations [200], _T (".6"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T (".5"), x, location);
		location = draw_markings_for_100 (dc, & locations [400], _T (".4"), x, location);
		location = draw_markings_for_100 (dc, & locations [500], _T (".3"), x, location);
		location = draw_markings_for_100 (dc, & locations [600], _T (".2"), x, location);
		location = draw_markings_for_100 (dc, & locations [700], _T (".1"), x, location);
	}
	ScaleLLD03Up (int height) : Scale (height) {left_index = 1; right_index = 800;}
};

class ScaleLLD03Down : public ScaleLLD03Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLLD03Down (int height) : ScaleLLD03Up (height) {}
};

class ScaleLLD04Up : public Scale {
public:
	double locations1 [200];
	double locations2 [1000];
	double locations3 [1000];
	double locations4 [1000];
	double locations5 [5500];
	int left_index, right_index;
	virtual double getLocation (double x) {return log10 (log10 (1.0 / x));}
	virtual double getValue (double x) {return 1.0 / pow (10.0, pow (10.0, x));}
	void init_locations (void) {
		for (int ind = 1; ind <= 100; ind++) {locations1 [ind] = (double) scale_length * (log10 (log10 (1.0 / (0.11 - (double) ind / 10000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations2 [ind] = (double) scale_length * (log10 (log10 (1.0 / (0.1 - (double) ind / 10000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations3 [ind] = (double) scale_length * (log10 (log10 (1.0 / (0.01 - (double) ind / 100000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations4 [ind] = (double) scale_length * (log10 (log10 (1.0 / (0.001 - (double) ind / 1000000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [ind] = (double) scale_length * (log10 (log10 (1.0 / (0.0001 - (double) ind / 10000000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [900 + ind] = (double) scale_length * (log10 (log10 (1.0 / (0.00001 - (double) ind / 100000000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [1800 + ind] = (double) scale_length * (log10 (log10 (1.0 / (0.000001 - (double) ind / 1000000000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [2700 + ind] = (double) scale_length * (log10 (log10 (1.0 / (0.0000001 - (double) ind / 10000000000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [3600 + ind] = (double) scale_length * (log10 (log10 (1.0 / (0.00000001 - (double) ind / 100000000000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations5 [4500 + ind] = (double) scale_length * (log10 (log10 (1.0 / (0.000000001 - (double) ind / 1000000000000.0))));}
		locations1 [0] = locations1 [1] - 100.0;
		left_index = find_left_index (locations1, 1, 800, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations5, 1, 5400, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations1 [0];
		location = draw_markings_for_100 (dc, locations1, _T (".1"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, locations2, _T (".09"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [100], _T (".08"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [200], _T (".07"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [300], _T (".06"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [400], _T (".05"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [500], _T (".04"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [600], _T (".03"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [700], _T (".02"), x, location);
		location = draw_markings_for_100 (dc, & locations2 [800], _T (".01"), x, location);
		location = draw_markings_for_100 (dc, locations3, _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations3 [100], _T ("8"), x, location);
		location = draw_markings_for_100 (dc, & locations3 [200], _T ("7"), x, location);
		location = draw_markings_for_100 (dc, & locations3 [300], _T ("6"), x, location);
		location = draw_markings_for_100 (dc, & locations3 [400], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations3 [500], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations3 [600], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations3 [700], _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations3 [800], _T ("10-3"), x, location);
		location = draw_markings_for_100 (dc, locations4, _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations4 [100], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations4 [200], _T ("7"), x, location);
		location = draw_markings_for_100 (dc, & locations4 [300], _T ("6"), x, location);
		location = draw_markings_for_100 (dc, & locations4 [400], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations4 [500], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations4 [600], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations4 [700], _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations4 [800], _T (""), x, location);
		draw_text_marker (dc, _T ("10-4"), x + locations4 [900]);
		location = draw_markings_for_100 (dc, locations5, _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [100], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [200], _T ("7"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [300], _T ("6"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [400], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [500], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [600], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [700], _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [800], _T (""), x, location);
		draw_text_marker (dc, _T ("10-5"), x + locations5 [900]);
		location = draw_markings_for_100 (dc, & locations5 [900], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1000], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1100], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1200], _T ("6"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1300], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1400], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1500], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1600], _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1700], _T (""), x, location);
		draw_text_marker (dc, _T ("10-6"), x + locations5 [1800]);
		location = draw_markings_for_100 (dc, & locations5 [1800], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [1900], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2000], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2100], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2200], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2300], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2400], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2500], _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2600], _T (""), x, location);
		draw_text_marker (dc, _T ("10-7"), x + locations5 [2700]);
		location = draw_markings_for_100 (dc, & locations5 [2700], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2800], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [2900], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [3000], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [3100], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [3200], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [3300], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [3400], _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations5 [3500], _T (""), x, location);
		draw_text_marker (dc, _T ("10-8"), x + locations5 [3600]);
		//location = draw_markings_for_100 (dc, & locations5 [3600], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [3700], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [3800], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [3900], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [4000], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4100], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4200], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [4300], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [4400], _T (""), x, location);
		draw_text_marker (dc, _T ("10-9"), x + locations5 [4500]);
		//location = draw_markings_for_100 (dc, & locations5 [4500], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4600], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4700], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [4800], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [4900], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [5000], _T (""), x, location);
		//location = draw_markings_for_100 (dc, & locations5 [5100], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [5200], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations5 [5300], _T (""), x, location);
		draw_text_marker (dc, _T ("10-10"), x + locations5 [5400]);
	}
	ScaleLLD04Up (int height) : Scale (height) {left_index = 1; right_index = 1000;}
};

class ScaleLLD04Down : public ScaleLLD04Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLLD04Down (int height) : ScaleLLD04Up (height) {}
};

class ScaleLL0Up : public Scale {
public:
	double locations [1500];
	int left_index, right_index;
	virtual double getLocation (double x) {return 3.0 + log10 (log (x));}
	virtual double getValue (double x) {return pow (_E, pow (10.0, x - 3.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 1400; ind++) {
			locations [ind] = (double) scale_length * (3.0 + log10 (log (1.0 + (double) ind / 100000.0)));
		}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 10, 1400, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 10, 1400, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T ("1.001"), x, location, left_index, 100);
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T ("1.002"), x, location, left_index <= 100 ? 0 : left_index - 100, 100);
		location = draw_markings_for_100 (dc, & locations [200], _T ("1.003"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T ("1.004"), x, location);
		location = draw_markings_for_100 (dc, & locations [400], _T ("1.005"), x, location);
		location = draw_markings_for_100 (dc, & locations [500], _T ("1.006"), x, location);
		location = draw_markings_for_100 (dc, & locations [600], _T ("1.007"), x, location);
		location = draw_markings_for_100 (dc, & locations [700], _T ("1.008"), x, location);
		location = draw_markings_for_100 (dc, & locations [800], _T ("1.009"), x, location);
		location = draw_markings_for_100 (dc, & locations [900], _T ("1.010"), x, location);
		if (right_index > 1000) location = draw_markings_for_100 (dc, & locations [1000], _T ("1.011"), x, location, 0, right_index > 1100 ? 100 : right_index - 1000);
		if (right_index > 1100) location = draw_markings_for_100 (dc, & locations [1100], _T ("1.012"), x, location, 0, right_index > 1200 ? 100 : right_index - 1100);
		if (right_index > 1200) location = draw_markings_for_100 (dc, & locations [1200], _T ("1.013"), x, location, 0, right_index > 1300 ? 100 : right_index - 1200);
		if (right_index > 1300) location = draw_markings_for_100 (dc, & locations [1300], _T ("1.014"), x, location, 0, right_index > 1400 ? 100 : right_index - 1300);
	}
	ScaleLL0Up (int height) : Scale (height) {left_index = 10; right_index = 1200;}
};

class ScaleLL0Down : public ScaleLL0Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLL0Down (int height) : ScaleLL0Up (height) {}
};

class ScaleLL1Up : public Scale {
public:
	double locations [1700];
	int left_index, right_index;
	virtual double getLocation (double x) {return 2.0 + log10 (log (x));}
	virtual double getValue (double x) {return pow (_E, pow (10.0, x - 2.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 1600; ind++) {
			locations [ind] = (double) scale_length * (2.0 + log10 (log (1.0 + (double) ind / 10000.0)));
		}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 10, 1600, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 10, 1600, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T ("1.01"), x, location, left_index, 100);
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T ("1.02"), x, location, left_index <= 100 ? 0 : left_index - 100, 100);
		location = draw_markings_for_100 (dc, & locations [200], _T ("1.03"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T ("1.04"), x, location);
		location = draw_markings_for_100 (dc, & locations [400], _T ("1.05"), x, location);
		location = draw_markings_for_100 (dc, & locations [500], _T ("1.06"), x, location);
		location = draw_markings_for_100 (dc, & locations [600], _T ("1.07"), x, location);
		location = draw_markings_for_100 (dc, & locations [700], _T ("1.08"), x, location);
		location = draw_markings_for_100 (dc, & locations [800], _T ("1.09"), x, location);
		location = draw_markings_for_100 (dc, & locations [900], _T ("1.10"), x, location);
		if (right_index > 1000) location = draw_markings_for_100 (dc, & locations [1000], _T ("1.11"), x, location, 0, right_index > 1100 ? 100 : right_index - 1000);
		if (right_index > 1100) location = draw_markings_for_100 (dc, & locations [1100], _T ("1.12"), x, location, 0, right_index > 1200 ? 100 : right_index - 1100);
		if (right_index > 1200) location = draw_markings_for_100 (dc, & locations [1200], _T ("1.13"), x, location, 0, right_index > 1300 ? 100 : right_index - 1200);
		if (right_index > 1300) location = draw_markings_for_100 (dc, & locations [1300], _T ("1.14"), x, location, 0, right_index > 1400 ? 100 : right_index - 1300);
		if (right_index > 1400) location = draw_markings_for_100 (dc, & locations [1400], _T ("1.15"), x, location, 0, right_index > 1500 ? 100 : right_index - 1400);
		if (right_index > 1500) location = draw_markings_for_100 (dc, & locations [1500], _T ("1.16"), x, location, 0, right_index > 1600 ? 100 : right_index - 1500);
	}
	ScaleLL1Up (int height) : Scale (height) {left_index = 10; right_index = 1200;}
};

class ScaleLL1Down : public ScaleLL1Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLL1Down (int height) : ScaleLL1Up (height) {}
};

class ScaleLL2Up : public Scale {
public:
	double low_locations [1100];
	double high_locations [500];
	int left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (log (x));}
	virtual double getValue (double x) {return pow (_E, pow (10.0, x - 1.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 1000; ind++) {
			low_locations [ind] = (double) scale_length * (1.0 + log10 (log (1.0 + (double) ind / 1000.0)));
		}
		for (int ind = 0; ind <= 400; ind++) {
			high_locations [ind] = (double) scale_length * (1.0 + log10 (log (2.0 + (double) ind / 100.0)));
		}
		low_locations [0] = low_locations [1] - 100.0;
		left_index = find_left_index (low_locations, 10, 1000, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (high_locations, 0, 400, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = low_locations [0];
		if (left_index <= 100) location = draw_markings_for_100 (dc, low_locations, _T ("1.1"), x, location, left_index, 100);
		if (left_index <= 200) location = draw_markings_for_100 (dc, & low_locations [100], _T ("1.2"), x, location, left_index <= 100 ? 0 : left_index - 100, 100);
		location = draw_markings_for_100 (dc, & low_locations [200], _T ("1.3"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [300], _T ("1.4"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [400], _T ("1.5"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [500], _T ("1.6"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [600], _T ("1.7"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [700], _T ("1.8"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [800], _T ("1.9"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [900], _T ("2.0"), x, location);
		if (right_index > 0) location = draw_markings_for_100 (dc, high_locations, _T ("3.0"), x, location, 0, right_index > 100 ? 100 : right_index);
		if (right_index > 100) location = draw_markings_for_100 (dc, & high_locations [100], _T ("4.0"), x, location, 0, right_index > 200 ? 100 : right_index - 100);
		if (right_index > 200) location = draw_markings_for_100 (dc, & high_locations [200], _T ("5.0"), x, location, 0, right_index > 300 ? 100 : right_index - 200);
		if (right_index > 300) location = draw_markings_for_100 (dc, & high_locations [300], _T ("6.0"), x, location, 0, right_index > 400 ? 100 : right_index - 300);
	}
	ScaleLL2Up (int height) : Scale (height) {left_index = 10; right_index = 1200;}
};

class ScaleLL2Down : public ScaleLL2Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLL2Down (int height) : ScaleLL2Up (height) {}
};

class ScaleLL3Up : public Scale {
public:
	double locations_1 [1000];
	double locations_2 [1000];
	double locations_3 [1000];
	double locations_4 [1000];
	double locations_5 [3000];
	int left_index, right_index;
	virtual double getLocation (double x) {return log10 (log (x));}
	virtual double getValue (double x) {return pow (_E, pow (10.0, x));}
	void init_locations (void) {
		for (int ind = 1; ind <= 900; ind++) {locations_1 [ind] = (double) scale_length * log10 (log (1.0 + (double) ind / 100.0));}
		for (int ind = 0; ind <= 900; ind++) {locations_2 [ind] = (double) scale_length * log10 (log (10.0 + (double) ind / 10.0));}
		for (int ind = 0; ind <= 900; ind++) {locations_3 [ind] = (double) scale_length * log10 (log (100.0 + (double) ind));}
		for (int ind = 0; ind <= 900; ind++) {locations_4 [ind] = (double) scale_length * log10 (log (1000.0 + (double) ind * 10.0));}
		for (int ind = 0; ind <= 900; ind++) {locations_5 [ind] = (double) scale_length * log10 (log (10000.0 + (double) ind * 100.0));}
		for (int ind = 0; ind <= 900; ind++) {locations_5 [900 + ind] = (double) scale_length * log10 (log (100000.0 + (double) ind * 1000.0));}
		for (int ind = 0; ind <= 900; ind++) {locations_5 [1800 + ind] = (double) scale_length * log10 (log (1000000.0 + (double) ind * 10000.0));}
		left_index = find_left_index (locations_1, 1, 200, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations_5, 0, 2700, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations_1 [1];
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations_1, _T ("2"), x, location, left_index, 100);
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations_1 [100], _T ("3"), x, location, left_index <= 100 ? 0 : left_index - 100, 100);
		location = draw_markings_for_100 (dc, & locations_1 [200], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations_1 [300], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations_1 [400], _T ("6"), x, location);
		location = draw_markings_for_100 (dc, & locations_1 [500], _T ("7"), x, location);
		location = draw_markings_for_100 (dc, & locations_1 [600], _T ("8"), x, location);
		location = draw_markings_for_100 (dc, & locations_1 [700], _T ("9"), x, location);
		location = draw_markings_for_100 (dc, & locations_1 [800], _T ("10"), x, location);
		location = draw_markings_for_100 (dc, locations_2, _T ("20"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [100], _T ("30"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [200], _T ("40"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [300], _T ("50"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [400], _T ("60"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [500], _T ("70"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [600], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [700], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [800], _T (""), x, location);
		draw_text_marker (dc, _T ("100"), x + locations_2 [900]);
		location = draw_markings_for_100 (dc, locations_3, _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [100], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [200], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [300], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [400], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [500], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [600], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [700], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [800], _T (""), x, location);
		draw_text_marker (dc, _T ("1k"), x + locations_3 [900]);
		location = draw_markings_for_100 (dc, locations_4, _T ("2"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [100], _T ("3"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [200], _T ("4"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [300], _T ("5"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [400], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [500], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [600], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [700], _T (""), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [800], _T (""), x, location);
		draw_text_marker (dc, _T ("10k"), x + locations_4 [900]);
		if (right_index >= 0) location = draw_markings_for_100 (dc, locations_5, _T ("20"), x, location, 0, right_index > 100 ? 100 : right_index);
		if (right_index >= 100) location = draw_markings_for_100 (dc, & locations_5 [100], _T ("30"), x, location, 0, right_index > 200 ? 100 : right_index - 100);
		if (right_index >= 200) location = draw_markings_for_100 (dc, & locations_5 [200], _T ("40"), x, location, 0, right_index > 300 ? 100 : right_index - 200);
		if (right_index >= 300) location = draw_markings_for_100 (dc, & locations_5 [300], _T ("50"), x, location, 0, right_index > 400 ? 100 : right_index - 300);
		if (right_index >= 400) location = draw_markings_for_100 (dc, & locations_5 [400], _T (""), x, location, 0, right_index > 500 ? 100 : right_index - 400);
		if (right_index >= 500) location = draw_markings_for_100 (dc, & locations_5 [500], _T (""), x, location, 0, right_index > 600 ? 100 : right_index - 500);
		if (right_index >= 600 && absolute (locations_5 [900] - locations_5 [700]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [600], _T (""), x, location, 0, right_index > 700 ? 100 : right_index - 600);
		if (right_index >= 700 && absolute (locations_5 [900] - locations_5 [800]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [700], _T (""), x, location, 0, right_index > 800 ? 100 : right_index - 700);
		if (right_index >= 800) location = draw_markings_for_100 (dc, & locations_5 [800], _T (""), x, location, 0, right_index > 900 ? 100 : right_index - 800);
		if (right_index >= 900) draw_text_marker (dc, _T ("100k"), x + locations_5 [900]);
		if (right_index >= 900) location = draw_markings_for_100 (dc, & locations_5 [900], _T ("2"), x, location, 0, right_index > 1000 ? 100 : right_index - 900);
		if (right_index >= 1000) location = draw_markings_for_100 (dc, & locations_5 [1000], _T ("3"), x, location, 0, right_index > 1100 ? 100 : right_index - 1000);
		if (right_index >= 1100) location = draw_markings_for_100 (dc, & locations_5 [1100], _T ("4"), x, location, 0, right_index > 1200 ? 100 : right_index - 1100);
		if (right_index >= 1200) location = draw_markings_for_100 (dc, & locations_5 [1200], _T ("5"), x, location, 0, right_index > 1300 ? 100 : right_index - 1200);
		if (right_index >= 1300 && absolute (locations_5 [1800] - locations_5 [1400]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1300], _T (""), x, location, 0, right_index > 1400 ? 100 : right_index - 1300);
		if (right_index >= 1400 && absolute (locations_5 [1800] - locations_5 [1500]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1400], _T (""), x, location, 0, right_index > 1500 ? 100 : right_index - 1400);
		if (right_index >= 1500 && absolute (locations_5 [1800] - locations_5 [1600]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1500], _T (""), x, location, 0, right_index > 1600 ? 100 : right_index - 1500);
		if (right_index >= 1600 && absolute (locations_5 [1800] - locations_5 [1700]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1600], _T (""), x, location, 0, right_index > 1700 ? 100 : right_index - 1600);
		if (right_index >= 1700) location = draw_markings_for_100 (dc, & locations_5 [1700], _T (""), x, location, 0, right_index > 1800 ? 100 : right_index - 1700);
		if (right_index >= 1800) draw_text_marker (dc, _T ("1M"), x + locations_5 [1800]);
		if (right_index >= 1800) location = draw_markings_for_100 (dc, & locations_5 [1800], _T ("2"), x, location, 0, right_index > 1900 ? 100 : right_index - 1800);
		if (right_index >= 1900) location = draw_markings_for_100 (dc, & locations_5 [1900], _T ("3"), x, location, 0, right_index > 2000 ? 100 : right_index - 1900);
		if (right_index >= 2000) location = draw_markings_for_100 (dc, & locations_5 [2000], _T ("4"), x, location, 0, right_index > 2100 ? 100 : right_index - 2000);
		if (right_index >= 2100) location = draw_markings_for_100 (dc, & locations_5 [2100], _T ("5"), x, location, 0, right_index > 2200 ? 100 : right_index - 2100);
		if (right_index >= 2200 && absolute (locations_5 [2700] - locations_5 [2300]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [2200], _T (""), x, location, 0, right_index > 2300 ? 100 : right_index - 2200);
		if (right_index >= 2300 && absolute (locations_5 [2700] - locations_5 [2400]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [2300], _T (""), x, location, 0, right_index > 2400 ? 100 : right_index - 2300);
		if (right_index >= 2400 && absolute (locations_5 [2700] - locations_5 [2500]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [2400], _T (""), x, location, 0, right_index > 2500 ? 100 : right_index - 2400);
		if (right_index >= 2500 && absolute (locations_5 [2700] - locations_5 [2600]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [2500], _T (""), x, location, 0, right_index > 2600 ? 100 : right_index - 2500);
		if (right_index >= 2600) location = draw_markings_for_100 (dc, & locations_5 [2600], _T (""), x, location, 0, right_index > 2700 ? 100 : right_index - 2600);
		if (right_index >= 2700) draw_text_marker (dc, _T ("10M"), x + locations_5 [2700]);
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleLL3Up (int height) : Scale (height) {left_index = 1; right_index = 100;}
};

class ScaleLL3Down : public ScaleLL3Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLL3Down (int height) : ScaleLL3Up (height) {}
};

class ScaleLL00Up : public Scale {
public:
	double locations [1600];
	int left_index, right_index;
	virtual double getLocation (double x) {return 3.0 + log10 (log (1.0 / x));}
	virtual double getValue (double x) {return 1.0 / pow (_E, pow (10.0, x - 3.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 1500; ind++) {locations [ind] = (double) scale_length * (3.0 + log10 (log (1.0 / (1.0 - (double) ind / 100000.0))));}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 500, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 1500, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T (".999"), x, location, left_index, 100);
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T (".998"), x, location, left_index <= 100 ? 0 : left_index - 100, 100);
		location = draw_markings_for_100 (dc, & locations [200], _T (".997"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T (".996"), x, location);
		location = draw_markings_for_100 (dc, & locations [400], _T (".995"), x, location);
		location = draw_markings_for_100 (dc, & locations [500], _T (".994"), x, location);
		location = draw_markings_for_100 (dc, & locations [600], _T (".993"), x, location);
		location = draw_markings_for_100 (dc, & locations [700], _T (".992"), x, location);
		location = draw_markings_for_100 (dc, & locations [800], _T (".991"), x, location);
		if (right_index > 900) location = draw_markings_for_100 (dc, & locations [900], _T (".99"), x, location, 0, right_index > 1000 ? 100 : right_index - 900);
		if (right_index > 1000) location = draw_markings_for_100 (dc, & locations [1000], _T (".989"), x, location, 0, right_index > 1100 ? 100 : right_index - 1000);
		if (right_index > 1100) location = draw_markings_for_100 (dc, & locations [1100], _T (".988"), x, location, 0, right_index > 1200 ? 100 : right_index - 1100);
		if (right_index > 1200) location = draw_markings_for_100 (dc, & locations [1200], _T (".987"), x, location, 0, right_index > 1300 ? 100 : right_index - 1200);
		if (right_index > 1300) location = draw_markings_for_100 (dc, & locations [1300], _T (".986"), x, location, 0, right_index > 1400 ? 100 : right_index - 1300);
		if (right_index > 1400) location = draw_markings_for_100 (dc, & locations [1400], _T (".985"), x, location, 0, right_index > 1500 ? 100 : right_index - 1400);
	}
	ScaleLL00Up (int height) : Scale (height) {left_index = 1; right_index = 1000;}
};

class ScaleLL00Down : public ScaleLL00Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLL00Down (int height) : ScaleLL00Up (height) {}
};

class ScaleLL01Up : public Scale {
public:
	double locations [1600];
	int left_index, right_index;
	virtual double getLocation (double x) {return 2.0 + log10 (log (1.0 / x));}
	virtual double getValue (double x) {return 1.0 / pow (_E, pow (10.0, x - 2.0));}
	void init_locations (void) {
		for (int ind = 1; ind <= 1500; ind++) {locations [ind] = (double) scale_length * (2.0 + log10 (log (1.0 / (1.0 - (double) ind / 10000.0))));}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 500, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 1500, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations, _T (".99"), x, location, left_index, 100);
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations [100], _T (".98"), x, location, left_index <= 100 ? 0 : left_index - 100, 100);
		location = draw_markings_for_100 (dc, & locations [200], _T (".97"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T (".96"), x, location);
		location = draw_markings_for_100 (dc, & locations [400], _T (".95"), x, location);
		location = draw_markings_for_100 (dc, & locations [500], _T (".94"), x, location);
		location = draw_markings_for_100 (dc, & locations [600], _T (".93"), x, location);
		location = draw_markings_for_100 (dc, & locations [700], _T (".92"), x, location);
		location = draw_markings_for_100 (dc, & locations [800], _T (".91"), x, location);
		if (right_index > 900) location = draw_markings_for_100 (dc, & locations [900], _T (".9"), x, location, 0, right_index > 1000 ? 100 : right_index - 900);
		if (right_index > 1000) location = draw_markings_for_100 (dc, & locations [1000], _T (".89"), x, location, 0, right_index > 1100 ? 100 : right_index - 1000);
		if (right_index > 1100) location = draw_markings_for_100 (dc, & locations [1100], _T (".88"), x, location, 0, right_index > 1200 ? 100 : right_index - 1100);
		if (right_index > 1200) location = draw_markings_for_100 (dc, & locations [1200], _T (".87"), x, location, 0, right_index > 1300 ? 100 : right_index - 1200);
		if (right_index > 1300) location = draw_markings_for_100 (dc, & locations [1300], _T (".86"), x, location, 0, right_index > 1400 ? 100 : right_index - 1300);
		if (right_index > 1400) location = draw_markings_for_100 (dc, & locations [1400], _T (".85"), x, location, 0, right_index > 1500 ? 100 : right_index - 1400);
	}
	ScaleLL01Up (int height) : Scale (height) {left_index = 1; right_index = 1000;}
};

class ScaleLL01Down : public ScaleLL01Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLL01Down (int height) : ScaleLL01Up (height) {}
};

class ScaleLL02Up : public Scale {
public:
	double low_locations [2000];
	double high_locations [800];
	int left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (log (1.0 / x));}
	virtual double getValue (double x) {return 1.0 / pow (_E, pow (10.0, x - 1.0));}
	void init_locations (void) {
		for (int ind = 0; ind <= 1900; ind++) {low_locations [ind] = (double) scale_length * (1.0 + log10 (log (1.0 / (0.99 - (double) ind / 10000.0))));}
		for (int ind = 0; ind <= 700; ind++) {high_locations [ind] = (double) scale_length * (1.0 + log10 (log (1.0 / (0.8 - (double) ind / 1000.0))));}
		left_index = find_left_index (low_locations, 0, 1900, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (high_locations, 0, 700, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = low_locations [0];
		if (left_index <= 100) location = draw_markings_for_100 (dc, low_locations, _T (".98"), x, location, left_index, 100);
		if (left_index <= 200) location = draw_markings_for_100 (dc, & low_locations [100], _T (".97"), x, location, left_index <= 100 ? 0 : left_index - 100, 100);
		if (left_index <= 300) location = draw_markings_for_100 (dc, & low_locations [200], _T (".96"), x, location, left_index <= 200 ? 0 : left_index - 200, 100);
		if (left_index <= 400) location = draw_markings_for_100 (dc, & low_locations [300], _T (".95"), x, location, left_index <= 300 ? 0 : left_index - 300, 100);
		if (left_index <= 500) location = draw_markings_for_100 (dc, & low_locations [400], _T (".94"), x, location, left_index <= 400 ? 0 : left_index - 400, 100);
		if (left_index <= 600) location = draw_markings_for_100 (dc, & low_locations [500], _T (".93"), x, location, left_index <= 500 ? 0 : left_index - 500, 100);
		if (left_index <= 700) location = draw_markings_for_100 (dc, & low_locations [600], _T (".92"), x, location, left_index <= 600 ? 0 : left_index - 600, 100);
		if (left_index <= 800) location = draw_markings_for_100 (dc, & low_locations [700], _T (".91"), x, location, left_index <= 700 ? 0 : left_index - 700, 100);
		if (left_index <= 900) location = draw_markings_for_100 (dc, & low_locations [800], _T (".9"), x, location, left_index <= 800 ? 0 : left_index - 800, 100);
		if (left_index <= 1000) location = draw_markings_for_100 (dc, & low_locations [900], _T (".89"), x, location, left_index <= 900 ? 0 : left_index - 900, 100);
		if (left_index <= 1100) location = draw_markings_for_100 (dc, & low_locations [1000], _T (".88"), x, location, left_index <= 1000 ? 0 : left_index - 1000, 100);
		location = draw_markings_for_100 (dc, & low_locations [1100], _T (".87"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [1200], _T (".86"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [1300], _T (".85"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [1400], _T (".84"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [1500], _T (".83"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [1600], _T (".82"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [1700], _T (".81"), x, location);
		location = draw_markings_for_100 (dc, & low_locations [1800], _T (".8"), x, location);
		if (right_index > 0) location = draw_markings_for_100 (dc, high_locations, _T (".7"), x, location, 0, right_index > 100 ? 100 : right_index);
		if (right_index > 100) location = draw_markings_for_100 (dc, & high_locations [100], _T (".6"), x, location, 0, right_index > 200 ? 100 : right_index - 100);
		if (right_index > 200) location = draw_markings_for_100 (dc, & high_locations [200], _T (".5"), x, location, 0, right_index > 300 ? 100 : right_index - 200);
		if (right_index > 300) location = draw_markings_for_100 (dc, & high_locations [300], _T (".4"), x, location, 0, right_index > 400 ? 100 : right_index - 300);
		if (right_index > 400) location = draw_markings_for_100 (dc, & high_locations [400], _T (".3"), x, location, 0, right_index > 500 ? 100 : right_index - 400);
		if (right_index > 500) location = draw_markings_for_100 (dc, & high_locations [500], _T (".2"), x, location, 0, right_index > 600 ? 100 : right_index - 500);
		if (right_index > 600) location = draw_markings_for_100 (dc, & high_locations [600], _T (".1"), x, location, 0, right_index > 700 ? 100 : right_index - 600);
	}
	ScaleLL02Up (int height) : Scale (height) {left_index = 0; right_index = 700;}
};

class ScaleLL02Down : public ScaleLL02Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLL02Down (int height) : ScaleLL02Up (height) {}
};

class ScaleLL03Up : public Scale {
public:
	double locations_1 [800];
	double locations_2 [1000];
	double locations_3 [1000];
	double locations_4 [1000];
	double locations_5 [3000];
	int left_index, right_index;
	virtual double getLocation (double x) {return log10 (log (1.0 / x));}
	virtual double getValue (double x) {return 1.0 / pow (_E, pow (10.0, x));}
	void init_locations (void) {
		for (int ind = 0; ind <= 700; ind++) {locations_1 [ind] = (double) scale_length * (log10 (log (1.0 / (0.8 - (double) ind / 1000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations_2 [ind] = (double) scale_length * (log10 (log (1.0 / (0.1 - (double) ind / 10000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations_3 [ind] = (double) scale_length * (log10 (log (1.0 / (0.01 - (double) ind / 100000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations_4 [ind] = (double) scale_length * (log10 (log (1.0 / (0.001 - (double) ind / 1000000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations_5 [ind] = (double) scale_length * (log10 (log (1.0 / (0.0001 - (double) ind / 10000000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations_5 [900 + ind] = (double) scale_length * (log10 (log (1.0 / (0.00001 - (double) ind / 100000000.0))));}
		for (int ind = 0; ind <= 900; ind++) {locations_5 [1800 + ind] = (double) scale_length * (log10 (log (1.0 / (0.000001 - (double) ind / 1000000000.0))));}
		left_index = find_left_index (locations_1, 0, 700, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations_5, 0, 2700, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations_1 [0];
		if (left_index <= 100) location = draw_markings_for_100 (dc, locations_1, _T (".7"), x, location, left_index, 100);
		if (left_index <= 200) location = draw_markings_for_100 (dc, & locations_1 [100], _T (".6"), x, location, left_index <= 100 ? 0 : left_index - 100, 100);
		if (left_index <= 300) location = draw_markings_for_100 (dc, & locations_1 [200], _T (".5"), x, location, left_index <= 200 ? 0 : left_index - 200, 100);
		if (left_index <= 400) location = draw_markings_for_100 (dc, & locations_1 [300], _T (".4"), x, location, left_index <= 300 ? 0 : left_index - 300, 100);
		if (left_index <= 500) location = draw_markings_for_100 (dc, & locations_1 [400], _T (".3"), x, location, left_index <= 400 ? 0 : left_index - 400, 100);
		if (left_index <= 600) location = draw_markings_for_100 (dc, & locations_1 [500], _T (".2"), x, location, left_index <= 500 ? 0 : left_index - 500, 100);
		if (left_index <= 700) location = draw_markings_for_100 (dc, & locations_1 [600], _T (".1"), x, location, left_index <= 600 ? 0 : left_index - 600, 100);
		location = draw_markings_for_100 (dc, locations_2, _T (".09"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [100], _T (".08"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [200], _T (".07"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [300], _T (".06"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [400], _T (".05"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [500], _T (".04"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [600], _T (".03"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [700], _T (".02"), x, location);
		location = draw_markings_for_100 (dc, & locations_2 [800], _T (".01"), x, location);
		location = draw_markings_for_100 (dc, locations_3, _T (".009"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [100], _T (".008"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [200], _T (".007"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [300], _T (".006"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [400], _T (".005"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [500], _T (".004"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [600], _T (".003"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [700], _T (".002"), x, location);
		location = draw_markings_for_100 (dc, & locations_3 [800], _T (".001"), x, location);
		location = draw_markings_for_100 (dc, locations_4, _T (".0009"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [100], _T (".0008"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [200], _T (".0007"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [300], _T (".0006"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [400], _T (".0005"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [500], _T (".0004"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [600], _T (".0003"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [700], _T (".0002"), x, location);
		location = draw_markings_for_100 (dc, & locations_4 [800], _T (""), x, location);
		draw_text_marker (dc, _T ("10-4"), x + locations_4 [900]);
		if (right_index > 0) location = draw_markings_for_100 (dc, locations_5, _T (".00009"), x, location, 0, right_index > 100 ? 100 : right_index);
		if (right_index > 100) location = draw_markings_for_100 (dc, & locations_5 [100], _T (".00008"), x, location, 0, right_index > 200 ? 100 : right_index - 100);
		if (right_index > 200) location = draw_markings_for_100 (dc, & locations_5 [200], _T (".00007"), x, location, 0, right_index > 300 ? 100 : right_index - 200);
		if (right_index > 300) location = draw_markings_for_100 (dc, & locations_5 [300], _T (".00006"), x, location, 0, right_index > 400 ? 100 : right_index - 300);
		if (right_index > 400) location = draw_markings_for_100 (dc, & locations_5 [400], _T (".00005"), x, location, 0, right_index > 500 ? 100 : right_index - 400);
		if (right_index > 500) location = draw_markings_for_100 (dc, & locations_5 [500], _T (".00004"), x, location, 0, right_index > 600 ? 100 : right_index - 500);
		if (right_index > 600) location = draw_markings_for_100 (dc, & locations_5 [600], _T (".00003"), x, location, 0, right_index > 700 ? 100 : right_index - 600);
		if (right_index > 700) location = draw_markings_for_100 (dc, & locations_5 [700], _T (".00002"), x, location, 0, right_index > 800 ? 100 : right_index - 700);
		if (right_index > 800) location = draw_markings_for_100 (dc, & locations_5 [800], _T (""), x, location, 0, right_index > 900 ? 100 : right_index - 800);
		if (right_index >= 900) draw_text_marker (dc, _T ("10-5"), x + locations_5 [900]);
		if (right_index >= 900) location = draw_markings_for_100 (dc, & locations_5 [900], _T (""), x, location, 0, right_index > 1000 ? 100 : right_index - 900); // 2
		if (right_index >= 1000 && absolute (locations_5 [1800] - locations_5 [1100]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1000], _T (""), x, location, 0, right_index > 1100 ? 100 : right_index - 1000); // 3
		if (right_index >= 1100 && absolute (locations_5 [1800] - locations_5 [1200]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1100], _T (""), x, location, 0, right_index > 1200 ? 100 : right_index - 1100); // 4
		if (right_index >= 1200 && absolute (locations_5 [1800] - locations_5 [1300]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1200], _T (""), x, location, 0, right_index > 1300 ? 100 : right_index - 1200); // 5
		if (right_index >= 1300 && absolute (locations_5 [1800] - locations_5 [1400]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1300], _T (""), x, location, 0, right_index > 1400 ? 100 : right_index - 1300); // 6
		if (right_index >= 1400 && absolute (locations_5 [1800] - locations_5 [1500]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1400], _T (""), x, location, 0, right_index > 1500 ? 100 : right_index - 1400); // 7
		if (right_index >= 1500 && absolute (locations_5 [1800] - locations_5 [1600]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1500], _T (""), x, location, 0, right_index > 1600 ? 100 : right_index - 1500); // 8
		if (right_index >= 1600 && absolute (locations_5 [1800] - locations_5 [1700]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1600], _T (""), x, location, 0, right_index > 1700 ? 100 : right_index - 1600); // 9
		if (right_index >= 1700) location = draw_markings_for_100 (dc, & locations_5 [1700], _T (""), x, location, 0, right_index > 1800 ? 100 : right_index - 900); // 10
		if (right_index >= 1800) draw_text_marker (dc, _T ("10-6"), x + locations_5 [1800]);
		if (right_index >= 1800) location = draw_markings_for_100 (dc, & locations_5 [900], _T (""), x, location, 0, right_index > 1900 ? 100 : right_index - 1800); // 2
		if (right_index >= 1900 && absolute (locations_5 [2700] - locations_5 [2000]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [1900], _T (""), x, location, 0, right_index > 2000 ? 100 : right_index - 1900); // 3
		if (right_index >= 2000 && absolute (locations_5 [2700] - locations_5 [2100]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [2000], _T (""), x, location, 0, right_index > 2100 ? 100 : right_index - 2000); // 4
		if (right_index >= 2100 && absolute (locations_5 [2700] - locations_5 [2200]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [2100], _T (""), x, location, 0, right_index > 2200 ? 100 : right_index - 2100); // 5
		if (right_index >= 2200 && absolute (locations_5 [2700] - locations_5 [2300]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [2200], _T (""), x, location, 0, right_index > 2300 ? 100 : right_index - 2200); // 6
		if (right_index >= 2300 && absolute (locations_5 [2700] - locations_5 [2400]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [2300], _T (""), x, location, 0, right_index > 2400 ? 100 : right_index - 2300); // 7
		if (right_index >= 2400 && absolute (locations_5 [2700] - locations_5 [2500]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [2400], _T (""), x, location, 0, right_index > 2500 ? 100 : right_index - 2400); // 8
		if (right_index >= 2500 && absolute (locations_5 [2700] - locations_5 [2600]) > minimum_step) location = draw_markings_for_100 (dc, & locations_5 [2500], _T (""), x, location, 0, right_index > 2600 ? 100 : right_index - 2500); // 9
		if (right_index >= 2600) location = draw_markings_for_100 (dc, & locations_5 [2600], _T (""), x, location, 0, right_index > 2700 ? 100 : right_index - 2600); // 10
		if (right_index >= 2700) draw_text_marker (dc, _T ("10-7"), x + locations_5 [2700]);
	}
	ScaleLL03Up (int height) : Scale (height) {left_index = 0; right_index = 1000;}
};

class ScaleLL03Down : public ScaleLL03Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleLL03Down (int height) : ScaleLL03Up (height) {}
};

bool check_log_log_scales (SetupFileReader & fr, Sliderule * slide_rule) {
	if (fr . id ("scale_LL")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLUp (fr . int_symbol));}
	if (fr . id ("scale_LL_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLDown (fr . int_symbol));}
	if (fr . id ("scale_LLR")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLRUp (fr . int_symbol));}
	if (fr . id ("scale_LLR_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLRDown (fr . int_symbol));}
/* GJM -- New LLD Scales (Log10 Log10) */
	if (fr . id ("scale_LLD0")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD0Up (fr . int_symbol));}
	if (fr . id ("scale_LLD1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD1Up (fr . int_symbol));}
	if (fr . id ("scale_LLD2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD2Up (fr . int_symbol));}
	if (fr . id ("scale_LLD3")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD3Up (fr . int_symbol));}
	if (fr . id ("scale_LLD4")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD4Up (fr . int_symbol));}
	if (fr . id ("scale_LLD0_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD0Down (fr . int_symbol));}
	if (fr . id ("scale_LLD1_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD1Down (fr . int_symbol));}
	if (fr . id ("scale_LLD2_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD2Down (fr . int_symbol));}
	if (fr . id ("scale_LLD3_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD3Down (fr . int_symbol));}
	if (fr . id ("scale_LLD4_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD4Down (fr . int_symbol));}
	if (fr . id ("scale_LLD00")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD00Up (fr . int_symbol));}
	if (fr . id ("scale_LLD01")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD01Up (fr . int_symbol));}
	if (fr . id ("scale_LLD02")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD02Up (fr . int_symbol));}
	if (fr . id ("scale_LLD03")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD03Up (fr . int_symbol));}
	if (fr . id ("scale_LLD04")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD04Up (fr . int_symbol));}
	if (fr . id ("scale_LLD00_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD00Down (fr . int_symbol));}
	if (fr . id ("scale_LLD01_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD01Down (fr . int_symbol));}
	if (fr . id ("scale_LLD02_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD02Down (fr . int_symbol));}
	if (fr . id ("scale_LLD03_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD03Down (fr . int_symbol));}
	if (fr . id ("scale_LLD04_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLLD04Down (fr . int_symbol));}
	if (fr . id ("scale_LL0")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL0Up (fr . int_symbol));}
	if (fr . id ("scale_LL1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL1Up (fr . int_symbol));}
	if (fr . id ("scale_LL2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL2Up (fr . int_symbol));}
	if (fr . id ("scale_LL3")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL3Up (fr . int_symbol));}
	if (fr . id ("scale_LL0_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL0Down (fr . int_symbol));}
	if (fr . id ("scale_LL1_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL1Down (fr . int_symbol));}
	if (fr . id ("scale_LL2_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL2Down (fr . int_symbol));}
	if (fr . id ("scale_LL3_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL3Down (fr . int_symbol));}
	if (fr . id ("scale_LL00")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL00Up (fr . int_symbol));}
	if (fr . id ("scale_LL01")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL01Up (fr . int_symbol));}
	if (fr . id ("scale_LL02")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL02Up (fr . int_symbol));}
	if (fr . id ("scale_LL03")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL03Up (fr . int_symbol));}
	if (fr . id ("scale_LL00_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL00Down (fr . int_symbol));}
	if (fr . id ("scale_LL01_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL01Down (fr . int_symbol));}
	if (fr . id ("scale_LL02_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL02Down (fr . int_symbol));}
	if (fr . id ("scale_LL03_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleLL03Down (fr . int_symbol));}
	return true;
}

