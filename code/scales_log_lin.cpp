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

#include "scale_log_base.h"
#include "setup_file_reader.h"
#include "sliderule.h"

#define _E        2.71828182845904523536
#define _PI       3.14159265358979323846

#define REVERSE_MARKERS reversed = ! reversed; dc . SetTextForeground (reversed ? wxColour (red_marking_colour . red, red_marking_colour . green, red_marking_colour . blue) : wxColour (marking_colour . red, marking_colour . green, marking_colour . blue))

class LogC : public LogBase {
public:
	virtual double getLocation (double x) {return log10 (x);}
	virtual double getValue (double x) {return pow (10.0, x);}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		draw_log_base (dc, x);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		draw_c1 (dc, x + c1_location);
		draw_index_location (dc, x);
		if (left_extension_index < 0) return;
		REVERSE_MARKERS;
		draw_left_log_base (dc, reversed ? x - (double) scale_length : x + (double) scale_length, left_extension_index);
		draw_right_log_base (dc, reversed ? x + (double) scale_length : x - (double) scale_length, right_extension_index);
		if (c_location <= right_extension * (double) scale_length) draw_c (dc, x + c_location + (double) scale_length);
		REVERSE_MARKERS;
	}
	virtual void scaleInit (void) {faceUp (); log_init ();}
	LogC (int height) : LogBase (height) {}

};

class LogD : public LogC {
public:
	virtual void scaleInit (void) {faceDown (); log_init ();}
	LogD (int height) : LogC (height) {}
};

class LogCI : public LogBase {
public:
	virtual double getLocation (double x) {return 1.0 - log10 (x);}
	virtual double getValue (double x) {return pow (10.0, 1.0 - x);}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		x += (double) scale_length;
		draw_log_base (dc, x);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		draw_c1 (dc, x + c1_location);
		draw_index_location (dc, x);
		if (left_extension_index < 0) return;
		REVERSE_MARKERS;
		draw_left_log_base (dc, reversed ? x - (double) scale_length : x + (double) scale_length, left_extension_index);
		draw_right_log_base (dc, reversed ? x + (double) scale_length : x - (double) scale_length, right_extension_index);
		if (-c_location <= left_extension * (double) scale_length) draw_c (dc, x + c_location - (double) scale_length);
		REVERSE_MARKERS;
	}
	virtual void scaleInit (void) {faceUp (); log_init (-1.0);}
	LogCI (int height) : LogBase (height) {}
};

class LogDI : public LogCI {
public:
	virtual void scaleInit (void) {faceDown (); log_init (-1.0);}
	LogDI (int height) : LogCI (height) {}
};

class LogCF : public LogBase {
public:
	virtual double getLocation (double x) {return log10 (10.0 * x / _PI);}
	virtual double getValue (double x) {return pow (10.0, x) * _PI * 0.1;}
	virtual void draw (wxDC & dc, double x) {
		x += (double) scale_length - pi_location;
		setArialFont (dc);
		draw_left_log_base (dc, x - (double) scale_length, left_extension_index); //200);
		draw_right_log_base (dc, x, right_extension_index); //221);
		draw_index_location (dc, x);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		x -= (double) scale_length;
		draw_pi (dc, x + pi_location);
		draw_c1 (dc, x + c1_location);
	}
	virtual void scaleInit (void) {faceUp (); extension_index_shift = log10 (_PI); log_init ();}
	LogCF (int height) : LogBase (height) {}
};

class LogDF : public LogCF {
public:
	virtual void scaleInit (void) {faceDown (); extension_index_shift = log10 (_PI); log_init ();}
	LogDF (int height) : LogCF (height) {}
};

class LogCIF : public LogBase {
public:
	virtual double getLocation (double x) {return 1.0 - log10 (x * _PI);}
	virtual double getValue (double x) {return pow (10.0, 1.0 - x) / _PI;}
	virtual void draw (wxDC & dc, double x) {
		x += (double) scale_length + pi_location;
		setArialFont (dc);
		draw_left_log_base (dc, x + (double) scale_length, left_extension_index); //200);
		draw_right_log_base (dc, x, right_extension_index); //221);
		draw_index_location (dc, x);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		x += (double) scale_length;
		draw_pi (dc, x + pi_location);
		draw_c1 (dc, x + c1_location);
	}
	virtual void scaleInit (void) {faceUp (); extension_index_shift = log10 (_PI); log_init (-1.0);}
	LogCIF (int height) : LogBase (height) {}
};

class LogDIF : public LogCIF {
public:
	virtual void scaleInit (void) {faceDown (); extension_index_shift = log10 (_PI); log_init (-1.0);}
	LogDIF (int height) : LogCIF (height) {}
};

/* GJM -- CFI DFI scales */
class LogCFI : public LogBase {
public:
	virtual double getLocation (double x) {return 0.0 - log10 (x / _PI);}
	virtual double getValue (double x) {return pow (10.0, 0.0 - x) * _PI;}
	virtual void draw (wxDC & dc, double x) {
		x -= pi_location;
		setArialFont (dc);
		draw_left_log_base (dc, x + (double) scale_length, left_extension_index); //200);
		draw_right_log_base (dc, x, right_extension_index); //221);
		draw_index_location (dc, x);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		x += (double) scale_length;
		draw_pi (dc, x + pi_location);
		draw_c1 (dc, x + c1_location);
	}
	virtual void scaleInit (void) {faceUp (); extension_index_shift = log10 (_PI); log_init (-1.0);}
	LogCFI (int height) : LogBase (height) {}
};

class LogDFI : public LogCFI {
public:
	virtual void scaleInit (void) {faceDown (); extension_index_shift = log10 (_PI); log_init (-1.0);}
	LogDFI (int height) : LogCFI (height) {}
};

class LogCF10 : public LogBase {
public:
	virtual double getLocation (double x) {return log10 (x * sqrt (10.0));}
	virtual double getValue (double x) {return pow (10.0, x) * sqrt (10.0) * 0.1;}
	virtual void scaleInit (void) {faceUp (); extension_index_shift = log10 (sqrt (10.0)); log_init ();}
	virtual void draw (wxDC & dc, double x) {
		x += (double) scale_length * 0.5;
		setArialFont (dc);
		draw_left_log_base (dc, x - (double) scale_length, left_extension_index);
		draw_right_log_base (dc, x, right_extension_index);
		draw_index_location (dc, x);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		x -= (double) scale_length;
		draw_pi (dc, x + pi_location);
		draw_c1 (dc, x + c1_location);
	}
	LogCF10 (int height) : LogBase (height) {}
};

class LogDF10 : public LogCF10 {
public:
	virtual void scaleInit (void) {faceDown (); extension_index_shift = log10 (sqrt (10.0)); log_init ();}
	LogDF10 (int height) : LogCF10 (height) {}
};

class LogCIF10 : public LogBase {
public:
	virtual double getLocation (double x) {return 1.0 - log10 (x * sqrt (10.0));}
	virtual double getValue (double x) {return pow (10.0, 1.0 - x) / sqrt (10.0);}
	virtual void scaleInit (void) {faceUp (); extension_index_shift = log10 (sqrt (10.0)); log_init (-1.0);}
	virtual void draw (wxDC & dc, double x) {
		x += (double) scale_length * 0.5;
		setArialFont (dc);
		draw_left_log_base (dc, x + (double) scale_length, left_extension_index); //200);
		draw_right_log_base (dc, x, right_extension_index); //221);
		draw_index_location (dc, x);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		x += (double) scale_length;
		draw_pi (dc, x + pi_location);
		draw_c1 (dc, x + c1_location);
	}
	LogCIF10 (int height) : LogBase (height) {}
};

class LogDIF10 : public LogCIF10 {
public:
	virtual void scaleInit (void) {faceDown (); extension_index_shift = log10 (sqrt (10.0)); log_init (-1.0);}
	LogDIF10 (int height) : LogCIF10 (height) {}
};

class LogCF36 : public LogBase {
public:
	virtual double getLocation (double x) {return log10 (x * 10.0 / 3.6);}
	virtual double getValue (double x) {return pow (10.0, x) * 0.36;}
	virtual void scaleInit (void) {faceUp (); extension_index_shift = log10 (3.6); log_init ();}
	virtual void draw (wxDC & dc, double x) {
		x += (double) scale_length * (1.0 - log10 (3.6));
		setArialFont (dc);
		draw_left_log_base (dc, x - (double) scale_length, left_extension_index);
		draw_right_log_base (dc, x, right_extension_index);
		draw_index_location (dc, x);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		draw_c1 (dc, x + c1_location);
	}
	LogCF36 (int height) : LogBase (height) {}
};

class LogDF36 : public LogCF36 {
public:
	virtual void scaleInit (void) {faceDown (); extension_index_shift = log10 (3.6); log_init ();}
	LogDF36 (int height) : LogCF36 (height) {}
};

class LogCIF36 : public LogBase {
public:
	virtual double getLocation (double x) {return 1.0 - log10 (x * 3.6);}
	virtual double getValue (double x) {return pow (10.0, 1.0 - x) / 3.6;}
	virtual void scaleInit (void) {faceUp (); extension_index_shift = log10 (3.6); log_init (-1.0);}
	virtual void draw (wxDC & dc, double x) {
		x += (double) scale_length * (1.0 - log10 (3.6));
		setArialFont (dc);
		draw_left_log_base (dc, x + (double) scale_length, left_extension_index);
		draw_right_log_base (dc, x, right_extension_index);
		draw_index_location (dc, x);
		draw_e (dc, x + e_location);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		x += (double) scale_length;
		draw_pi (dc, x + pi_location);
		draw_c1 (dc, x + c1_location);
	}
	LogCIF36 (int height) : LogBase (height) {}
};

class LogDIF36 : public LogCIF36 {
public:
	virtual void scaleInit (void) {faceDown (); extension_index_shift = log10 (3.6); log_init (-1.0);}
	LogDIF36 (int height) : LogCIF36 (height) {}
};

class LogCFM : public LogBase {
public:
	virtual double getLocation (double x) {return log10 (x * 10.0 * log10 (_E));}
	virtual double getValue (double x) {return pow (10.0, x - 1.0) / log10 (_E);}
	virtual void scaleInit (void) {faceUp (); extension_index_shift = log10 (1.0 / log10 (_E)); log_init ();}
	virtual void draw (wxDC & dc, double x) {
		x += (double) scale_length * (1.0 - extension_index_shift); //log10 (3.6));
		setArialFont (dc);
		draw_left_log_base (dc, x - (double) scale_length, left_extension_index);
		draw_right_log_base (dc, x, right_extension_index);
		draw_index_location (dc, x);
		x -= (double) scale_length;
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		draw_c1 (dc, x + c1_location);
	}
	LogCFM (int height) : LogBase (height) {}
};

class LogDFM : public LogCFM {
public:
	virtual void scaleInit (void) {faceDown (); extension_index_shift = log10 (1.0 / log10 (_E)); log_init ();}
	LogDFM (int height) : LogCFM (height) {}
};

class LogCIFM : public LogBase {
public:
	virtual double getLocation (double x) {return 1.0 - log10 (x / log10 (_E));}
	virtual double getValue (double x) {return pow (10.0, 1.0 - x) * log10 (_E);}
	virtual void scaleInit (void) {faceUp (); extension_index_shift = log10 (1.0 / log10 (_E)); log_init (-1.0);}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		x += (double) scale_length * (1.0 - extension_index_shift);
		draw_left_log_base (dc, x + (double) scale_length, left_extension_index);
		draw_right_log_base (dc, x, right_extension_index);
		draw_index_location (dc, x);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		draw_c1 (dc, x + c1_location);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
	}
	LogCIFM (int height) : LogBase (height) {}
};

class LogDIFM : public LogCIFM {
public:
	virtual void scaleInit (void) {faceDown (); extension_index_shift = log10 (1.0 / log10 (_E)); log_init (-1.0);}
	LogDIFM (int height) : LogCIFM (height) {}
};

class LogCF1M : public LogBase {
public:
	virtual double getLocation (double x) {return log10 (x / log10 (_E));}
	virtual double getValue (double x) {return pow (10.0, x) * log10 (_E);}
	virtual void scaleInit (void) {faceUp (); extension_index_shift = 1.0 + log10 (log10 (_E)); log_init ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		x += (double) scale_length * (1.0 - extension_index_shift);
		draw_left_log_base (dc, x - (double) scale_length, left_extension_index);
		draw_right_log_base (dc, x, right_extension_index);
		draw_index_location (dc, x);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		draw_c1 (dc, x + c1_location);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
	}
	LogCF1M (int height) : LogBase (height) {}
};

class LogDF1M : public LogCF1M {
public:
	virtual void scaleInit (void) {faceDown (); extension_index_shift = 1.0 + log10 (log10 (_E)); log_init ();}
	LogDF1M (int height) : LogCF1M (height) {}
};

class LogCIF1M : public LogBase {
public:
	virtual double getLocation (double x) {return - log10 (x * log10 (_E));}
	virtual double getValue (double x) {return pow (10.0, - x) / log10 (_E);}
	virtual void scaleInit (void) {faceUp (); extension_index_shift = 1.0 + log10 (log10 (_E)); log_init (-1.0);}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		x += (double) scale_length * (1.0 - extension_index_shift);
		draw_left_log_base (dc, x + (double) scale_length, left_extension_index);
		draw_right_log_base (dc, x, right_extension_index);
		draw_index_location (dc, x);
		draw_c (dc, x + c_location);
		draw_degree (dc, x + degree_location);
		x += (double) scale_length;
		draw_c1 (dc, x + c1_location);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
	}
	LogCIF1M (int height) : LogBase (height) {}
};

class LogDIF1M : public LogCIF1M {
public:
	virtual void scaleInit (void) {faceDown (); extension_index_shift = 1.0 + log10 (log10 (_E)); log_init (-1.0);}
	LogDIF1M (int height) : LogCIF1M (height) {}
};

class LogA : public LogBase {
public:
	virtual double getLocation (double x) {return log10 (x) * 0.5;}
	virtual double getValue (double x) {return pow (10.0, x * 2.0);}
	virtual void scaleInit (void) {faceUp (); log_init (0.5);}
	virtual void draw (wxDC & dc, double x) {
		double half_scale = (double) scale_length * 0.5;
		setArialFont (dc);
		draw_log_base (dc, x);
		draw_log_base (dc, x + half_scale);
		draw_pi (dc, x + pi_location); if (no_m) draw_pi (dc, x + pi_location + half_scale);
		draw_e (dc, x + e_location); draw_e (dc, x + e_location + half_scale);
		draw_m (dc, x + m_location);
		draw_mm (dc, x + mm_location);
		draw_index_location (dc, x);
		if (left_extension_index < 0) return;
		REVERSE_MARKERS;
		draw_left_log_base (dc, reversed ? x - half_scale : x + half_scale, left_extension_index);
		draw_right_log_base (dc, reversed ? x + (double) scale_length : x - (double) scale_length, right_extension_index);
		REVERSE_MARKERS;
	}
	LogA (int height) : LogBase (height) {}
};

class LogB : public LogA {
public:
	virtual void scaleInit (void) {faceDown (); log_init (0.5);}
	LogB (int height) : LogA (height) {}
};

class LogAI : public LogBase {
public:
	virtual double getLocation (double x) {return 1.0 - log10 (x) * 0.5;}
	virtual double getValue (double x) {return pow (10.0, 2.0 - x * 2.0);}
	virtual void scaleInit (void) {faceUp (); log_init (-0.5);}
	virtual void draw (wxDC & dc, double x) {
		double half_scale = (double) scale_length * 0.5;
		setArialFont (dc);
		draw_index_location (dc, x);
		draw_log_base (dc, x + (double) scale_length);
		draw_log_base (dc, x + half_scale);
		draw_pi (dc, x + pi_location + (double) scale_length); if (no_m) draw_pi (dc, x + pi_location + half_scale);
		draw_e (dc, x + e_location + (double) scale_length); draw_e (dc, x + e_location + half_scale);
		draw_m (dc, x + m_location + (double) scale_length);
		draw_mm (dc, x + mm_location + (double) scale_length);
		draw_index_location (dc, x + (double) scale_length);
		if (left_extension_index < 0) return;
		x += (double) scale_length;
		REVERSE_MARKERS;
		draw_left_log_base (dc, reversed ? x - half_scale : x + half_scale, left_extension_index);
		draw_right_log_base (dc, reversed ? x + (double) scale_length : x - (double) scale_length, right_extension_index);
		REVERSE_MARKERS;
	}
	LogAI (int height) : LogBase (height) {}
};

class LogBI : public LogAI {
public:
	virtual void scaleInit (void) {faceDown (); log_init (-0.5);}
	LogBI (int height) : LogAI (height) {}
};

class LogK : public LogBase {
public:
	virtual double getLocation (double x) {return log10 (x) / 3.0;}
	virtual double getValue (double x) {return pow (10.0, x * 3.0);}
	virtual void scaleInit (void) {faceUp (); log_init (1.0 / 3.0);}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		draw_log_base (dc, x);
		draw_log_base (dc, x + (double) scale_length / 3.0);
		draw_log_base (dc, x + (double) scale_length * 2.0 / 3.0);
		draw_index_location (dc, x);
		if (left_extension_index < 0) return;
		REVERSE_MARKERS;
		draw_left_log_base (dc, reversed ? x - (double) scale_length / 3.0 : x + (double) scale_length / 3.0, left_extension_index);
		draw_right_log_base (dc, reversed ? x + (double) scale_length : x - (double) scale_length, right_extension_index);
		REVERSE_MARKERS;
	}
	LogK (int height) : LogBase (height) {}
};

class LogJ : public LogK {
public:
	virtual void scaleInit (void) {faceDown (); log_init (1.0 / 3.0);}
	LogJ (int height) : LogK (height) {}
};

class LogKI : public LogBase {
public:
	virtual double getLocation (double x) {return 1.0 - log10 (x) / 3.0;}
	virtual double getValue (double x) {return pow (10.0, 3.0 - x * 3.0);}
	virtual void scaleInit (void) {faceUp (); log_init (-1.0 / 3.0);}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		draw_index_location (dc, x);
		draw_log_base (dc, x + (double) scale_length);
		draw_log_base (dc, x + (double) scale_length * 2.0 / 3.0);
		draw_log_base (dc, x + (double) scale_length / 3.0);
		draw_index_location (dc, x + (double) scale_length);
		if (left_extension_index < 0) return;
		x += (double) scale_length;
		REVERSE_MARKERS;
		draw_left_log_base (dc, reversed ? x - (double) scale_length / 3.0 : x + (double) scale_length / 3.0, left_extension_index);
		draw_right_log_base (dc, reversed ? x + (double) scale_length : x - (double) scale_length, right_extension_index);
		REVERSE_MARKERS;
	}
	LogKI (int height) : LogBase (height) {}
};

class LogJI : public LogKI {
public:
	virtual void scaleInit (void) {faceDown (); log_init (-1.0 / 3.0);}
	LogJI (int height) : LogKI (height) {}
};

class LogF : public LogBase {
public:
	virtual double getLocation (double x) {return log10 (x) * 0.25;}
	virtual double getValue (double x) {return pow (10.0, x * 4.0);}
	virtual void scaleInit (void) {faceUp (); log_init (0.25);}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		draw_log_base (dc, x);
		draw_log_base (dc, x + (double) scale_length * 0.25);
		draw_log_base (dc, x + (double) scale_length * 0.5);
		draw_log_base (dc, x + (double) scale_length * 0.75);
		draw_index_location (dc, x);
		if (left_extension_index < 0) return;
		REVERSE_MARKERS;
		draw_left_log_base (dc, reversed ? x - (double) scale_length * 0.25 : x + (double) scale_length * 0.25, left_extension_index);
		draw_right_log_base (dc, reversed ? x + (double) scale_length : x - (double) scale_length, right_extension_index);
		REVERSE_MARKERS;
	}
	LogF (int height) : LogBase (height) {}
};

class LogG : public LogF {
public:
	virtual void scaleInit (void) {faceDown (); log_init (0.25);}
	LogG (int height) : LogF (height) {}
};

class LogFI : public LogBase {
public:
	virtual double getLocation (double x) {return 1.0 - log10 (x) * 0.25;}
	virtual double getValue (double x) {return pow (10.0, 4.0 - x * 4.0);}
	virtual void scaleInit (void) {faceUp (); log_init (-0.25);}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		draw_index_location (dc, x);
		draw_log_base (dc, x + (double) scale_length);
		draw_log_base (dc, x + (double) scale_length * 0.75);
		draw_log_base (dc, x + (double) scale_length * 0.5);
		draw_log_base (dc, x + (double) scale_length * 0.25);
		draw_index_location (dc, x + (double) scale_length);
		if (left_extension_index < 0) return;
		x += (double) scale_length;
		REVERSE_MARKERS;
		draw_left_log_base (dc, reversed ? x - (double) scale_length * 0.25 : x + (double) scale_length * 0.25, left_extension_index);
		draw_right_log_base (dc, reversed ? x + (double) scale_length : x - (double) scale_length, right_extension_index);
		REVERSE_MARKERS;
	}
	LogFI (int height) : LogBase (height) {}
};

class LogGI : public LogFI {
public:
	virtual void scaleInit (void) {faceDown (); log_init (-0.25);}
	LogGI (int height) : LogFI (height) {}
};

class LogR1 : public LogBase {
public:
	bool change_extension;
	virtual double getLocation (double x) {return log10 (x) * 2.0;}
	virtual double getValue (double x) {return pow (10.0, x * 0.5);}
	virtual void scaleInit (void) {faceUp (); if (change_extension) right_extension += 1.0; change_extension = false;log_init (2.0);}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		draw_index_location (dc, x);
		draw_pi (dc, x + pi_location);
		draw_e (dc, x + e_location);
		draw_right_log_base (dc, x, right_extension_index);
		REVERSE_MARKERS;
		draw_left_log_base (dc, x - (double) scale_length - (double) scale_length, left_extension_index);
		REVERSE_MARKERS;
	}
	LogR1 (int height) : LogBase (height) {change_extension = true;}
};

class LogW1 : public LogR1 {
public:
	virtual void scaleInit (void) {faceDown (); if (change_extension) right_extension += 1.0; change_extension = false; log_init (2.0);}
	LogW1 (int height) : LogR1 (height) {}
};

class LogR2 : public LogBase {
public:
	bool change_extension;
	virtual double getLocation (double x) {return log10 (x) * 2.0 - 1.0;}
	virtual double getValue (double x) {return pow (10.0, 0.5 + x * 0.5);}
	virtual void scaleInit (void) {faceUp (); if (change_extension) left_extension += 1.0; change_extension = false; log_init (2.0);}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		REVERSE_MARKERS;
		draw_right_log_base (dc, x + (double) scale_length, right_extension_index);
		REVERSE_MARKERS;
		x -= (double) scale_length;
		draw_left_log_base (dc, x, left_extension_index);
		draw_index_location (dc, x + (double) scale_length + (double) scale_length);
	}
	LogR2 (int height) : LogBase (height) {change_extension = true;}
};

class LogW2 : public LogR2 {
public:
	virtual void scaleInit (void) {faceDown (); if (change_extension) left_extension += 1.0; change_extension = false; log_init (2.0);}
	LogW2 (int height) : LogR2 (height) {}
};

class Log3R1 : public LogBase {
public:
	bool change_extension;
	double scale, shift;
	double left_border, right_border;
	virtual double getLocation (double x) {return log10 (x) * scale - shift;}
	virtual double getValue (double x) {return pow (10.0, (shift + x) / scale);}
	void scaleInitInternal (void) {
		left_border = (0.0 - left_extension + shift) * (double) scale_length;
		right_border = (1.0 + right_extension + shift) * (double) scale_length;
		left_extension -= shift;
		if (left_extension < 0.0) left_extension = 0.0;
		right_extension += shift;
		if (change_extension) right_extension += 1.0;
		change_extension = false;
		log_init (scale);
	}
	virtual void scaleInit (void) {faceUp (); scaleInitInternal ();}
	virtual void draw (wxDC & dc, double x) {
		x -= (double) scale_length * shift;
		setArialFont (dc);
		draw_index_location (dc, x);
		draw_right_log_base (dc, x, right_extension_index);
		if (pi_location <= right_border && pi_location >= left_border) draw_pi (dc, x + pi_location);
		if (e_location <= right_border && e_location >= left_border) draw_e (dc, x + e_location);
		if (c_location <= right_border && c_location >= left_border) draw_c (dc, x + c_location);
		if (degree_location <= right_border && degree_location >= left_border) draw_degree (dc, x + degree_location);
		if (c1_location <= right_border && c1_location >= left_border) draw_c1 (dc, x + c1_location);
		REVERSE_MARKERS;
		draw_left_log_base (dc, x - (double) scale_length * scale, left_extension_index);
		REVERSE_MARKERS;
	}
	Log3R1 (int height, double scale, double shift) : LogBase (height) {
		this -> scale = scale != 0.0 ? scale : 1.0;
		this -> shift = shift;
		change_extension = true;
	}
};

class Log3W1 : public Log3R1 {
public:
	virtual void scaleInit (void) {faceDown (); scaleInitInternal ();}
	Log3W1 (int height, double scale, double shift) : Log3R1 (height, scale, shift) {}
};

class Log3R2 : public LogBase {
public:
	bool change_extension;
	double scale, shift;
	double left_border, right_border;
	virtual double getLocation (double x) {return log10 (x) * scale - shift;}
	virtual double getValue (double x) {return pow (10.0, (shift + x) / scale);}
	void scaleInitInternal (void) {
		left_border = (0.0 - left_extension + shift) * (double) scale_length;
		right_border = (1.0 + right_extension + shift) * (double) scale_length;
		if (change_extension) {double x = left_extension; left_extension = scale - 1.0 - shift - right_extension; right_extension = shift - x;}
		if (left_extension < 0.0) left_extension = 0.0; if (right_extension < 0.0) right_extension = 0.0;
		change_extension = false;
		log_init (scale);
	}
	virtual void scaleInit (void) {faceUp (); scaleInitInternal ();}
	virtual void draw (wxDC & dc, double x) {
		x -= (double) scale_length * shift;
		setArialFont (dc);
		draw_both_sides_log_base (dc, x, right_extension_index, left_extension_index);//, right_extension_index);
		if (pi_location <= right_border && pi_location >= left_border) draw_pi (dc, x + pi_location);
		if (e_location <= right_border && e_location >= left_border) draw_e (dc, x + e_location);
		if (c_location <= right_border && c_location >= left_border) draw_c (dc, x + c_location);
		if (degree_location <= right_border && degree_location >= left_border) draw_degree (dc, x + degree_location);
		if (c1_location <= right_border && c1_location >= left_border) draw_c1 (dc, x + c1_location);
	}
	Log3R2 (int height, double scale, double shift) : LogBase (height) {
		this -> scale = scale != 0.0 ? scale : 1.0;
		this -> shift = shift;
		change_extension = true;
	}
};

class Log3W2 : public Log3R2 {
public:
	virtual void scaleInit (void) {faceDown (); scaleInitInternal ();}
	Log3W2 (int height, double scale, double shift) : Log3R2 (height, scale, shift) {}
};

class Log3R3 : public LogBase {
public:
	bool change_extension;
	double scale, shift;
	double left_border, right_border;
	virtual double getLocation (double x) {return log10 (x) * scale - shift;}
	virtual double getValue (double x) {return pow (10.0, (shift + x) / scale);}
	void scaleInitInternal (void) {
		left_border = (0.0 - left_extension + shift) * (double) scale_length;
		right_border = (1.0 + right_extension + shift) * (double) scale_length;
		left_extension += (scale - shift - 1.0);
		if (left_extension < -1.0) left_extension = -1.0;
		right_extension -= (scale - shift - 1.0);
		if (change_extension) left_extension += 1.0;
		change_extension = false;
		log_init (scale);
	}
	virtual void scaleInit (void) {faceUp (); scaleInitInternal ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		x += (double) scale_length * (scale - shift);
		draw_index_location (dc, x);
		REVERSE_MARKERS;
		draw_right_log_base (dc, x, right_extension_index);
		REVERSE_MARKERS;
		x -= (double) scale_length * scale;
		draw_left_log_base (dc, x, left_extension_index);
		if (pi_location <= right_border && pi_location >= left_border) draw_pi (dc, x + pi_location);
		if (e_location <= right_border && e_location >= left_border) draw_e (dc, x + e_location);
		if (c_location <= right_border && c_location >= left_border) draw_c (dc, x + c_location);
		if (degree_location <= right_border && degree_location >= left_border) draw_degree (dc, x + degree_location);
		if (c1_location <= right_border && c1_location >= left_border) draw_c1 (dc, x + c1_location);
	}
	Log3R3 (int height, double scale, double shift) : LogBase (height) {
		this -> scale = scale != 0.0 ? scale : 1.0;
		this -> shift = shift;
		change_extension = true;
	}
};

class Log3W3 : public Log3R3 {
public:
	virtual void scaleInit (void) {faceDown (); scaleInitInternal ();}
	Log3W3 (int height, double scale, double shift) : Log3R3 (height, scale, shift) {}
};

class LogCentigrade0Up : public LogBase {
public:
	double locations[500];
	int left_index, right_index;
	virtual double getLocation (double x) {return log10( 273.15 + x );}
	virtual double getValue (double x) {return pow( 10, x ) - 273.15;}
	void init_locations (void) {
		for (int ind = 86; ind <= 300; ind++) { // from -273 to -270
			locations [ind] = (double) scale_length * (log10(0.15 + (double) ind / 100.0));
		}
		for (int ind = 1; ind <= 100; ind++) { // from -270 to -260
			locations [ind + 300] = (double) scale_length * (log10(3.15 + (double) ind / 10.0));
		}
		for (int ind = 0; ind <= 85; ind++) {
			locations [ind] = locations [86] - (86-ind)*100.0;
		}
		left_index = find_left_index (locations, 1, 400, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 400, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		location = draw_markings_for_100 (dc, locations, _T ("-272"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations [100], _T ("-271"), x, location);
		location = draw_markings_for_100 (dc, & locations [200], _T ("-270"), x, location);
		draw_text_marker (dc, _T ("-269"), x + locations [310]);
		draw_text_marker (dc, _T ("-268"), x + locations [320]);
		draw_text_marker (dc, _T ("-267"), x + locations [330]);
		draw_text_marker (dc, _T ("-266"), x + locations [340]);
		draw_text_marker (dc, _T ("-265"), x + locations [350]);
		draw_text_marker (dc, _T ("-264"), x + locations [360]);
		location = draw_markings_for_100 (dc, & locations [300], _T ("-260"), x, location, 0, right_index - 300);
	}
	LogCentigrade0Up (int height) : LogBase (height) {left_index = 0; right_index = 400;}
};

class LogCentigrade0Down : public LogCentigrade0Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	LogCentigrade0Down (int height) : LogCentigrade0Up (height) {}
};

class LogCentigrade1Up : public LogBase {
public:
	double locations[500];
	int left_index, right_index;
	virtual double getLocation (double x) {return -1.0 + log10( 273.15 + x );}
	virtual double getValue (double x) {return pow( 10, x + 1.0) - 273.15;}
	void init_locations (void) {
		for (int ind = 0; ind <= 300; ind++) { // from -270 to -240
			locations [ind] = (double) scale_length * (-1.0 + log10(3.15 + (double) ind / 10.0));
		}
		for (int ind = 1; ind <= 100; ind++) { // from -240 to -140
			locations [ind + 300] = (double) scale_length * (-1.0 + log10(33.15 + (double) ind));
		}
		left_index = find_left_index (locations, 1, 400, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 400, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		location = draw_markings_for_100 (dc, locations, _T ("-260"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations [100], _T ("-250"), x, location);
		location = draw_markings_for_100 (dc, & locations [200], _T ("-240"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T (""), x, location, 0, right_index - 300);
		draw_text_marker (dc, _T ("-230"), x + locations [310]);
		draw_text_marker (dc, _T ("-220"), x + locations [320]);
		draw_text_marker (dc, _T ("-210"), x + locations [330]);
		draw_text_marker (dc, _T ("-200"), x + locations [340]);
		draw_text_marker (dc, _T ("-190"), x + locations [350]);
		draw_text_marker (dc, _T ("-180"), x + locations [360]);
	}
	LogCentigrade1Up (int height) : LogBase (height) {left_index = 0; right_index = 400;}
};

class LogCentigrade1Down : public LogCentigrade1Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	LogCentigrade1Down (int height) : LogCentigrade1Up (height) {}
};


class LogCentigrade2Up : public LogBase {
public:
	double locations[600];
	int left_index, right_index;
	virtual double getLocation (double x) {return -2.0 + log10( 273.15 + x );}
	virtual double getValue (double x) {return pow( 10, x + 2.0 ) - 273.15;}
	void init_locations (void) {
		for (int ind = 0; ind <= 400; ind++) { // from -200 to 200
			locations [ind] = (double) scale_length * (-2.0 + log10(73.15 + (double) ind));
		}
		for (int ind = 1; ind <= 100; ind++) { // from 200 to 1200
			locations [ind + 400] = (double) scale_length * (-2.0 + log10(473.15 + (double) ind * 10.0));
		}
		left_index = find_left_index (locations, 1, 500, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 500, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		draw_text_marker (dc, _T ("-150"), x + locations [50]);
		location = draw_markings_for_100 (dc, locations, _T ("-100"), x, location, left_index, 100);
		draw_text_marker (dc, _T ("-50"), x + locations [150]);
		location = draw_markings_for_100 (dc, & locations [100], _T ("0"), x, location);
		draw_text_marker (dc, _T ("50"), x + locations [250]);
		location = draw_markings_for_100 (dc, & locations [200], _T ("100"), x, location);
		draw_text_marker (dc, _T ("150"), x + locations [350]);
		location = draw_markings_for_100 (dc, & locations [300], _T ("200"), x, location);
		location = draw_markings_for_100 (dc, & locations [400], _T (""), x, location, 0, right_index - 400);
		draw_text_marker (dc, _T ("300"), x + locations [410]);
		draw_text_marker (dc, _T ("400"), x + locations [420]);
		draw_text_marker (dc, _T ("500"), x + locations [430]);
		draw_text_marker (dc, _T ("600"), x + locations [440]);
		draw_text_marker (dc, _T ("700"), x + locations [450]);
	}
	LogCentigrade2Up (int height) : LogBase (height) {left_index = 0; right_index = 500;}
};

class LogCentigrade2Down : public LogCentigrade2Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	LogCentigrade2Down (int height) : LogCentigrade2Up (height) {}
};

class LogCentigrade3Up : public LogBase {
public:
	double locations[500];
	int left_index, right_index;
	virtual double getLocation (double x) {return -3.0 + log10( 273.15 + x );}
	virtual double getValue (double x) {return pow( 10, x + 3.0) - 273.15;}
	void init_locations (void) {
		for (int ind = 0; ind <= 300; ind++) { // from 0 to 3000
			locations [ind] = (double) scale_length * (-3.0 + log10(273.15 + (double) ind * 10.0));
		}
		for (int ind = 1; ind <= 100; ind++) { // from 3000 to 13000
			locations [ind + 300] = (double) scale_length * (-3.0 + log10(3273.15 + (double) ind * 100.0));
		}
		left_index = find_left_index (locations, 1, 400, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 400, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		location = draw_markings_for_100 (dc, locations, _T ("1000"), x, location, left_index, 100);
		draw_text_marker (dc, _T ("1500"), x + locations [150]);
		location = draw_markings_for_100 (dc, & locations [100], _T ("2000"), x, location);
		draw_text_marker (dc, _T ("2500"), x + locations [250]);
		location = draw_markings_for_100 (dc, & locations [200], _T ("3000"), x, location);
		draw_text_marker (dc, _T ("4000"), x + locations [310]);
		draw_text_marker (dc, _T ("5000"), x + locations [320]);
		draw_text_marker (dc, _T ("6000"), x + locations [330]);
		draw_text_marker (dc, _T ("7000"), x + locations [340]);
		draw_text_marker (dc, _T ("8000"), x + locations [350]);
		draw_text_marker (dc, _T ("9000"), x + locations [360]);
		location = draw_markings_for_100 (dc, & locations [300], _T (""), x, location, 0, right_index - 300);
	}
	LogCentigrade3Up (int height) : LogBase (height) {left_index = 0; right_index = 400;}
};

class LogCentigrade3Down : public LogCentigrade3Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	LogCentigrade3Down (int height) : LogCentigrade3Up (height) {}
};


class LogFahrenheit0Up : public LogBase {
public:
	double locations[1000];
	int left_index, right_index;
	virtual double getLocation (double x) {return log10( ( x - 32.0 ) / 1.8 + 273.15 );}
	virtual double getValue (double x) {return ( pow( 10, x ) - 273.15 ) * 1.8 + 32.0;}
	void init_locations (void) {
		for (int ind = 0; ind <= 800; ind++) { // from -458 to -450
			locations [ind] = (double) scale_length * (log10( ( -490.0 + (double) ind / 100.0) / 1.8 + 273.15 ) );
		}
		for (int ind = 1; ind <= 100; ind++) { // from -450 to -440
			locations [ind + 800] = (double) scale_length * (log10( ( -482.0 + (double) ind / 10.0) / 1.8 + 273.15 ) );
		}
		left_index = find_left_index (locations, 1, 900, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 900, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		location = draw_markings_for_100 (dc, locations, _T ("-457"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations [100], _T ("-456"), x, location);
		location = draw_markings_for_100 (dc, & locations [200], _T ("-455"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T ("-454"), x, location);
		location = draw_markings_for_100 (dc, & locations [400], _T ("-453"), x, location);
		location = draw_markings_for_100 (dc, & locations [500], _T ("-452"), x, location);
		location = draw_markings_for_100 (dc, & locations [600], _T ("-451"), x, location);
		location = draw_markings_for_100 (dc, & locations [700], _T ("-450"), x, location);
		draw_text_marker (dc, _T ("-445"), x + locations [850]);
		location = draw_markings_for_100 (dc, & locations [800], _T ("-440"), x, location, 0, right_index - 800);
	}
	LogFahrenheit0Up (int height) : LogBase (height) {left_index = 0; right_index = 900;}
};

class LogFahrenheit0Down : public LogFahrenheit0Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	LogFahrenheit0Down (int height) : LogFahrenheit0Up (height) {}
};

class LogFahrenheit1Up : public LogBase {
public:
	double locations[800];
	int left_index, right_index;
	virtual double getLocation (double x) {return -1.0 + log10( ( x - 32.0 ) / 1.8 + 273.15 );}
	virtual double getValue (double x) {return ( pow( 10, x + 1.0 ) - 273.15 ) * 1.8 + 32.0;}
	void init_locations (void) {
		for (int ind = 0; ind <= 500; ind++) { // from -450 to -400
			locations [ind] = (double) scale_length * (-1.0 + (log10( ( -482.0 + (double) ind / 10.0) / 1.8 + 273.15 ) ) );
		}
		for (int ind = 1; ind <= 200; ind++) { // from -400 to -200
			locations [ind + 500] = (double) scale_length * (-1.0 + (log10( ( -432.0 + (double) ind) / 1.8 + 273.15 ) ) );
		}
		left_index = find_left_index (locations, 1, 700, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 700, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		location = draw_markings_for_100 (dc, locations, _T ("-440"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations [100], _T ("-430"), x, location);
		location = draw_markings_for_100 (dc, & locations [200], _T ("-420"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T ("-410"), x, location);
		location = draw_markings_for_100 (dc, & locations [400], _T ("-400"), x, location);
		location = draw_markings_for_100 (dc, & locations [500], _T ("-300"), x, location);
		draw_text_marker (dc, _T ("-350"), x + locations [550]);
		location = draw_markings_for_100 (dc, & locations [600], _T ("-200"), x, location, 0, right_index - 600);
	}
	LogFahrenheit1Up (int height) : LogBase (height) {left_index = 0; right_index = 700;}
};

class LogFahrenheit1Down : public LogFahrenheit1Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	LogFahrenheit1Down (int height) : LogFahrenheit1Up (height) {}
};


class LogFahrenheit2Up : public LogBase { // -280 to 1500
public:
	double locations[800];
	int left_index, right_index;
	virtual double getLocation (double x) {return -2.0 + log10( ( x - 32.0 ) / 1.8 + 273.15 );}
	virtual double getValue (double x) {return ( pow( 10, x + 2.0 ) - 273.15 ) * 1.8 + 32.0;}
	void init_locations (void) {
		for (int ind = 0; ind <= 500; ind++) { // from -300 to 200
			locations [ind] = (double) scale_length * (-2.0 + ( log10( ( -332.0 + (double) ind) / 1.8 + 273.15 ) ) );
		}
		for (int ind = 1; ind <= 200; ind++) { // from 200 to 2200
			locations [ind + 500] = (double) scale_length * (-2.0 + ( log10( ( 168.0 + (double) ind * 10.0) / 1.8 + 273.15 ) ) );
		}
		left_index = find_left_index (locations, 1, 700, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 700, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		location = draw_markings_for_100 (dc, locations, _T ("-200"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations [100], _T ("-100"), x, location);
		location = draw_markings_for_100 (dc, & locations [200], _T ("0"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T ("100"), x, location);
		location = draw_markings_for_100 (dc, & locations [400], _T ("200"), x, location);
		location = draw_markings_for_100 (dc, & locations [500], _T (""), x, location, 0, 30);
		location = draw_markings_for_100 (dc, & locations [530], _T (""), x, location, 0, right_index - 530);
		draw_text_marker (dc, _T ("300"), x + locations [510]);
		draw_text_marker (dc, _T ("400"), x + locations [520]);
		draw_text_marker (dc, _T ("500"), x + locations [530]);
		draw_text_marker (dc, _T ("1000"), x + locations [580]);
	}
	LogFahrenheit2Up (int height) : LogBase (height) {left_index = 0; right_index = 700;}
};

class LogFahrenheit2Down : public LogFahrenheit2Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	LogFahrenheit2Down (int height) : LogFahrenheit2Up (height) {}
};

class LogFahrenheit3Up : public LogBase { // 1300 to 18000
public:
	double locations[700];
	int left_index, right_index;
	virtual double getLocation (double x) {return -3.0 + log10( ( x - 32.0 ) / 1.8 + 273.15 );}
	virtual double getValue (double x) {return ( pow( 10, x + 3.0 ) - 273.15 ) * 1.8 + 32.0;}
	void init_locations (void) {
		for (int ind = 0; ind <= 400; ind++) { // from 1000 to 5000
			locations [ind] = (double) scale_length * (-3.0 + ( log10( ( 968.0 + (double) ind * 10.0) / 1.8 + 273.15 ) ) );
		}
		for (int ind = 1; ind <= 200; ind++) { // from 5000 to 25000
			locations [ind + 400] = (double) scale_length * (-3.0 + ( log10( ( 4968.0 + (double) ind * 100.0) / 1.8 + 273.15 ) ) );
		}
		left_index = find_left_index (locations, 1, 600, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 600, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		location = draw_markings_for_100 (dc, locations, _T ("2000"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations [100], _T ("3000"), x, location);
		location = draw_markings_for_100 (dc, & locations [200], _T ("4000"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T ("5000"), x, location);
		draw_text_marker (dc, _T ("1500"), x + locations [50]);
		draw_text_marker (dc, _T ("10000"), x + locations [450]);
		location = draw_markings_for_100 (dc, & locations [400], _T ("15000"), x, location);
		location = draw_markings_for_100 (dc, & locations [500], _T ("25000"), x, location, 0, right_index - 500);
	}
	LogFahrenheit3Up (int height) : LogBase (height) {left_index = 0; right_index = 600;}
};

class LogFahrenheit3Down : public LogFahrenheit3Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	LogFahrenheit3Down (int height) : LogFahrenheit3Up (height) {}
};


#define READ_SCALE_R_W(s)\
			int scale_height = -1;\
			double scale = 3.0;\
			double shift = s;\
			bool scale_entered = false;\
			fr . get_symbol ();\
			while (fr . symbol_control >= 5 && fr . symbol_control <= 6) {\
				switch (fr . symbol_control) {\
				case 5: scale_height = fr . int_symbol; break;\
				case 6: if (scale_entered) shift = fr . float_symbol; else scale = fr . float_symbol; scale_entered = true; break;\
				default: break;\
				}\
				fr . get_symbol ();\
			}\
			should_skip = false;\
			if (scale_height < 0) return false;

class Imperial : public Scale {
public:
	double imperial;
	virtual double getLocation (double x) {return x / imperial - left_extension;}
	virtual double getValue (double x) {return imperial * (x + left_extension);}
	virtual void scaleInit (void) {faceUp ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double ind = 0.0;
		x -= (double) scale_length * left_extension;
		double sentinel = imperial * (1.0 + left_extension + right_extension);
		double factor = (double) scale_length / imperial;
		draw_line (dc, lcp, x, y0, y1);
		draw_text_marker (dc, _T ("0"), x);
		int inch = 1;
		double progress = 1.0 / 32.0;
		if (factor < minimum_step * 32.0) {
			while (ind < imperial) {
				for (int sub = 1; sub <= 32; sub++) {
					ind += progress; draw_thin_line (dc, lcp, x + ind * factor, y0, sub % 16 == 0 ? y1 : sub % 4 == 0 ? y2 : sub % 2 == 0 ? y3 : y4);
				}
				draw_text_marker (dc, wxString :: Format (_T ("%i"), inch++), x + ind * factor);
			}
		} else {
			while (ind < imperial) {
				for (int sub = 1; sub <= 32; sub++) {
					ind += progress; draw_line (dc, lcp, x + ind * factor, y0, sub % 16 == 0 ? y1 : sub % 4 == 0 ? y2 : sub % 2 == 0 ? y3 : y4);
				}
				draw_text_marker (dc, wxString :: Format (_T ("%i"), inch++), x + ind * factor);
			}
		}
	}
	Imperial (double imperial, int height) : Scale (height) {this -> imperial = imperial;}
};

class ImperialDown : public Imperial {
public:
	virtual void scaleInit (void) {faceDown ();}
	ImperialDown (double imperial, int height) : Imperial (imperial, height) {}
};

class Metric : public Scale {
public:
	double metric;
	double visible_metric;
	int integer_step;
	double double_step;
	double begin_at;
	double location_shift;
	virtual double getLocation (double x) {return 10.0 * x / metric - left_extension - location_shift;}
	virtual double getValue (double x) {return 0.1 * metric * (x + left_extension + location_shift);}
	virtual void scaleInit (void) {faceUp (); location_shift = 10.0 * begin_at / metric;}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double ind = 0.0;
		x -= (double) scale_length * left_extension;
		double start_point = x;
		double reminder = begin_at - (double_step * (double) ((int) (begin_at / double_step)));
		int cm = (int) begin_at + integer_step;
		double cmd = begin_at + double_step;
		if (reminder < 0.0) {reminder += 1.0; cm -= integer_step; cmd -= double_step;}
		reminder *= 10.0 * (double) scale_length / metric;
		x -= reminder;
		double factor = (double) scale_length / visible_metric;
		if (x >= start_point) draw_index_location (dc, x, double_step == 1.0 ? wxString :: Format (_T ("%i"), (int) begin_at) : wxString :: Format (_T ("%2g"), begin_at));
		double sentinel = reminder / factor + visible_metric * (1.0 + left_extension + right_extension);
		double location;
		if (factor < minimum_step) {
			while (ind <= sentinel) {
				ind += 1.0; if (ind > sentinel) return; draw_thin_line (dc, lcp, x + ind * factor, y0, y3);
				ind += 1.0; if (ind > sentinel) return; draw_thin_line (dc, lcp, x + ind * factor, y0, y3);
				ind += 1.0; if (ind > sentinel) return; draw_thin_line (dc, lcp, x + ind * factor, y0, y3);
				ind += 1.0; if (ind > sentinel) return; draw_thin_line (dc, lcp, x + ind * factor, y0, y3);
				ind += 1.0; if (ind > sentinel) return; draw_thin_line (dc, lcp, x + ind * factor, y0, y2);
				ind += 1.0; if (ind > sentinel) return; draw_thin_line (dc, lcp, x + ind * factor, y0, y3);
				ind += 1.0; if (ind > sentinel) return; draw_thin_line (dc, lcp, x + ind * factor, y0, y3);
				ind += 1.0; if (ind > sentinel) return; draw_thin_line (dc, lcp, x + ind * factor, y0, y3);
				ind += 1.0; if (ind > sentinel) return; draw_thin_line (dc, lcp, x + ind * factor, y0, y3);
				ind += 1.0; if (ind > sentinel) return; draw_thin_line (dc, lcp, x + ind * factor, y0, y1);
				if (double_step == 1.0) draw_text_marker (dc, wxString :: Format (_T ("%i"), cm), x + ind * factor);
				else draw_text_marker (dc, wxString :: Format (_T ("%2g"), cmd), x + ind * factor);
				cm += integer_step;
				cmd += double_step;
			}
		} else {
			while (ind <= sentinel) {
				ind += 1.0; if (ind > sentinel) return; location = x + ind * factor; if (location >= start_point) draw_line (dc, lcp, location, y0, y3);
				ind += 1.0; if (ind > sentinel) return; location = x + ind * factor; if (location >= start_point) draw_line (dc, lcp, location, y0, y3);
				ind += 1.0; if (ind > sentinel) return; location = x + ind * factor; if (location >= start_point) draw_line (dc, lcp, location, y0, y3);
				ind += 1.0; if (ind > sentinel) return; location = x + ind * factor; if (location >= start_point) draw_line (dc, lcp, location, y0, y3);
				ind += 1.0; if (ind > sentinel) return; location = x + ind * factor; if (location >= start_point) draw_line (dc, lcp, location, y0, y2);
				ind += 1.0; if (ind > sentinel) return; location = x + ind * factor; if (location >= start_point) draw_line (dc, lcp, location, y0, y3);
				ind += 1.0; if (ind > sentinel) return; location = x + ind * factor; if (location >= start_point) draw_line (dc, lcp, location, y0, y3);
				ind += 1.0; if (ind > sentinel) return; location = x + ind * factor; if (location >= start_point) draw_line (dc, lcp, location, y0, y3);
				ind += 1.0; if (ind > sentinel) return; location = x + ind * factor; if (location >= start_point) draw_line (dc, lcp, location, y0, y3);
				ind += 1.0; if (ind > sentinel) return; location = x + ind * factor; if (location >= start_point) draw_line (dc, lcp, location, y0, y1);
				location = x + ind * factor;
				if (location >= start_point) draw_text_marker (dc, double_step == 1.0 ? wxString :: Format (_T ("%i"), cm): wxString :: Format (_T ("%2g"), cmd), location);
				cm += integer_step;
				cmd += double_step;
			}
		}
	}
	Metric (double metric, int height, double begin_at) : Scale (height) {
		this -> begin_at = begin_at * 0.1;
		this -> location_shift = 0.0;
//		if (scale == 0.0) scale = 1.0;
		this -> metric = metric;// / scale;
		this -> visible_metric = this -> metric;
		integer_step = 1; double_step = 1.0;
		while (visible_metric >= 1000.0) {visible_metric *= 0.1; integer_step *= 10;}
		while (visible_metric <= 40.0) {visible_metric *= 10.0; double_step *= 0.1;}
	}
};

class MetricDown : public Metric {
public:
	virtual void scaleInit (void) {faceDown (); location_shift = 10.0 * begin_at / metric;}
	MetricDown (double metric, int height, double begin_at) : Metric (metric, height, begin_at) {}
};

class LUp : public LogBase {
public:
	int left_index, right_index;
	void init_values (void) {
		for (int ind = 0; ind <= 1000; ind++) logs [ind] = (double) scale_length * (double) ind / 1000.0;
		left_index = find_left_index (logs, 0, 1000, (double) scale_length * (1.0 - left_extension));
		right_index = find_right_index (logs, 0, 1000, (double) scale_length * right_extension);
	}
	virtual void scaleInit (void) {faceUp (); init_values ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location;
		int description = -9;
		x -= (double) scale_length; location = logs [0];
		for (int ind = 0; ind < 1000; ind += 100) {
			if (left_index <= ind + 100) location = draw_markings_for_100 (dc, & logs [ind], wxString :: Format (_T ("%i"), description), x, location, left_index <= ind ? 0 : left_index - ind, 100);
			description++;
		}
		x += (double) scale_length; location = logs [0];
		for (int ind = 0; ind < 1000; ind += 100) {location = draw_markings_for_100 (dc, & logs [ind], wxString :: Format (_T ("%i"), description++), x, location);}
		x += (double) scale_length; location = logs [0];
		for (int ind = 0; ind < 1000; ind += 100) {
			if (right_index > ind) location = draw_markings_for_100 (dc, & logs [ind], wxString :: Format (_T ("%i"), description), x, location, 0, right_index > ind + 100 ? 100 : right_index - ind);
			description++;
		}
	}
	LUp (int height) : LogBase (height) {}
};

class LDown : public LUp {
public:
	virtual void scaleInit (void) {faceDown (); init_values ();}
	LDown (int height) : LUp (height) {}
};

/* GJM -- Ln scale */
class LnUp : public LogBase {
public:
	int left_index, right_index;
	double locations [2500];
	virtual double getLocation (double x) {return x / log(10.0);}
	virtual double getValue (double x) {return log(10.0) * x;}
	void init_values (void) {
		for (int ind = 0; ind <= 2400; ind++) locations [ind] = (double) scale_length * (double) ind / log(10.0) / 1000.0;
		left_index = find_left_index (locations, 0, 2400, (double) scale_length * (1.0 - left_extension));
		right_index = find_right_index (locations, 0, 2400, (double) scale_length * right_extension);
	}
	virtual void scaleInit (void) {faceUp (); init_values ();}
	virtual void draw (wxDC & dc, double x)
	{
		setArialFont (dc);
		double location;
		int description = -22;
		x -= locations [2300]; location = locations [0];
		for (int ind = 0; ind < 2400; ind += 100) {
			if (left_index <= ind + 200) location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%i"), description), x, location, left_index <= ind ? 0 : left_index - ind, 100);
			description++;
		}
		x += (double) scale_length; location = locations [0];
		for (int ind = 100; ind < 2300; ind += 100) {location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%i"), description++), x, location);}
		x += (double) scale_length; location = locations [0];
		for (int ind = 0; ind < 2400; ind += 100) {
			if (right_index > ind) location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%i"), description), x, location, 0, right_index > ind + 100 ? 100 : right_index - ind);
			description++;
		}
	}
	LnUp (int height) : LogBase (height) {}
};

class LnDown : public LnUp {
public:
	virtual void scaleInit (void) {faceDown (); init_values ();}
	LnDown (int height) : LnUp (height) {}
};


class LR : public LogBase {
public:
	int left_index, right_index;
	bool draw_1, draw_2;
	//double values [501];
	void init_values (void) {
		for (int ind = 0; ind <= 500; ind++) logs [ind] = (double) scale_length * (double) ind / 500.0;
		left_index = find_left_index (logs, 0, 500, (double) scale_length * (1.0 - left_extension));
		right_index = find_right_index (logs, 0, 500, (double) scale_length * right_extension);
	}
	virtual double getLocation (double x) {
		x *= 2.0;
		if (draw_1 && ! draw_2) return x;
		if (! draw_1 && draw_2) return x - 1.0;
		if (draw_1 && draw_2 && x > 1.0) return x - 1.0;
		return x;
	}
	virtual double getValue (double x) {if (draw_1) return x * 0.5; return 0.5 + x * 0.5;}
	virtual void scaleInit (void) {faceUp (); init_values ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location;
		int desc_1 = -4;
		int desc_2 = 1;
		x -= (double) scale_length; location = logs [0];
		for (int ind = 0; ind < 500; ind += 100) {
			if (left_index <= ind + 100) location = draw_markings_for_100 (dc, & logs [ind], draw_1 ? (draw_2 ? wxString :: Format (_T ("%i %i"), desc_1, desc_2) : wxString :: Format (_T ("%i"), desc_1)) : wxString :: Format (_T ("%i"), desc_2), x, location, left_index <= ind ? 0 : left_index - ind, 100);
			desc_1++; desc_2++;
		}
		x += (double) scale_length; location = logs [0];
		for (int ind = 0; ind < 500; ind += 100) {
			location = draw_markings_for_100 (dc, & logs [ind], draw_1 ? (draw_2 ? wxString :: Format (_T ("%i %i"), desc_1, desc_2) : wxString :: Format (_T ("%i"), desc_1)) : wxString :: Format (_T ("%i"), desc_2), x, location);
			desc_1++; desc_2++;
		}
		x += (double) scale_length; location = logs [0];
		for (int ind = 0; ind < 500; ind += 100) {
			if (right_index > ind) location = draw_markings_for_100 (dc, & logs [ind], draw_1 ? (draw_2 ? wxString :: Format (_T ("%i %i"), desc_1, desc_2) : wxString :: Format (_T ("%i"), desc_1)) : wxString :: Format (_T ("%i"), desc_2), x, location, 0, right_index > ind + 100 ? 100 : right_index - ind);
			desc_1++; desc_2++;
		}
	}
	LR (int height, bool draw_1, bool draw_2) : LogBase (height) {this -> draw_1 = draw_1; this -> draw_2 = draw_2;}
};

class LW : public LR {
public:
	virtual void scaleInit (void) {faceDown (); init_values ();}
	LW (int height, bool draw_1, bool draw_2) : LR (height, draw_1, draw_2) {}
};

class ScaleSQRTUp : public Scale {
public:
	double shift;
	double locations [2100];
	int left_index, right_index;
	virtual double getLocation (double x) {return shift + x * x / 100.0;}
	virtual double getValue (double x) {x -= shift; x *= 100.0; return sqrt (x);}
	void init_locations (void) {
		for (int ind = 0; ind <= 2000; ind++) {double x = (double) ind / 100.0; locations [ind] = (double) scale_length * (shift + x * x / 100.0);}
		left_index = find_left_index (locations, 0, 2000, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 0, 2000, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		int description = 1;
		draw_index_location (dc, x + locations [0], _T ("0"));
		for (int ind = 0; ind < 2000; ind += 100) {
			if (ind > right_index) return;
			if (left_index <= ind + 100) location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%i"), description), x, location, left_index <= ind ? 0 : left_index - ind, right_index > ind + 100 ? 100 : right_index - ind);
			description++;
		}
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleSQRTUp (double shift, int height) : Scale (height) {this -> shift = - shift; left_index = 0; right_index = 1000;}
};

class ScaleSQRTDown : public ScaleSQRTUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleSQRTDown (double shift, int height) : ScaleSQRTUp (shift, height) {}
};

class ScaleOSCUp : public Scale {
public:
	double octaves;
	double fraction;
	double locations [256];
	int right_index;
	virtual double getLocation (double x) {return log10 (x) / fraction;}
	virtual double getValue (double x) {return pow (2.0, octaves * x);}
	void init_locations (void) {
		double sub_scale_length = (double) scale_length / octaves;
		double log2 = log10 (2.0);
		double previous_location = -100.0;
		double sentinel = (double) scale_length * (1.0 + right_extension);
		right_index = 0;
		for (int ind = 0; ind < 256; ind++) {
			double location = (double) scale_length * getLocation (1.0 + (double) ind);
			if (location > sentinel || absolute (location - previous_location) <= minimum_step) return;
			previous_location = locations [ind] = location;
			right_index = ind;
		}
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		int extent = dc . GetTextExtent (_T ("00")) . x;
		int ind = 0; int description = 1;
		double previous_location = -100.0;
		while (ind <= right_index) {
			double location = locations [ind++];
			if (absolute (location - previous_location) > extent) draw_index_location (dc, x + location, wxString :: Format (_T ("%i"), description++));
			else draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), x + location, y0, y1);
			previous_location = location;
		}
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScaleOSCUp (int octaves, int height) : Scale (height) {
		if (octaves < 1) octaves = 1;
		this -> octaves = (double) octaves;
		this -> fraction = log10 (2.0) * this -> octaves;
	}
};

class ScaleOSCDown : public ScaleOSCUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScaleOSCDown (int octaves, int height) : ScaleOSCUp (octaves, height) {}
};

bool check_log_scales (bool & should_skip, SetupFileReader & fr, Sliderule * slide_rule) {
	if (fr . id ("scale_F")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogF (fr . int_symbol));}
	if (fr . id ("scale_FI")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogFI (fr . int_symbol));}
	if (fr . id ("scale_G")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogG (fr . int_symbol));}
	if (fr . id ("scale_GI")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogGI (fr . int_symbol));}
	if (fr . id ("scale_K")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogK (fr . int_symbol));}
	if (fr . id ("scale_KI")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogKI (fr . int_symbol));}
	if (fr . id ("scale_J")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogJ (fr . int_symbol));}
	if (fr . id ("scale_JI")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogJI (fr . int_symbol));}
	if (fr . id ("scale_A")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogA (fr . int_symbol));}
	if (fr . id ("scale_AI")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogAI (fr . int_symbol));}
	if (fr . id ("scale_B")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogB (fr . int_symbol));}
	if (fr . id ("scale_BI")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogBI (fr . int_symbol));}
	if (fr . id ("scale_C")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogC (fr . int_symbol));}
	if (fr . id ("scale_D")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogD (fr . int_symbol));}
	if (fr . id ("scale_CI")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCI (fr . int_symbol));}
	if (fr . id ("scale_DI")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDI (fr . int_symbol));}
	if (fr . id ("scale_CF")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCF (fr . int_symbol));}
	if (fr . id ("scale_DF")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDF (fr . int_symbol));}
	if (fr . id ("scale_CIF")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCIF (fr . int_symbol));}
	if (fr . id ("scale_DIF")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDIF (fr . int_symbol));}
/* GJM -- New CFI DFI scales */
	if (fr . id ("scale_CFI")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCFI (fr . int_symbol));}
	if (fr . id ("scale_DFI")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDFI (fr . int_symbol));}
	if (fr . id ("scale_CF10")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCF10 (fr . int_symbol));}
	if (fr . id ("scale_DF10")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDF10 (fr . int_symbol));}
	if (fr . id ("scale_CIF10")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCIF10 (fr . int_symbol));}
	if (fr . id ("scale_DIF10")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDIF10 (fr . int_symbol));}
	if (fr . id ("scale_CF36")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCF36 (fr . int_symbol));}
	if (fr . id ("scale_DF36")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDF36 (fr . int_symbol));}
	if (fr . id ("scale_CIF36")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCIF36 (fr . int_symbol));}
	if (fr . id ("scale_DIF36")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDIF36 (fr . int_symbol));}
	if (fr . id ("scale_CFM")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCFM (fr . int_symbol));}
	if (fr . id ("scale_DFM")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDFM (fr . int_symbol));}
	if (fr . id ("scale_CIFM")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCIFM (fr . int_symbol));}
	if (fr . id ("scale_DIFM")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDIFM (fr . int_symbol));}
	if (fr . id ("scale_CF1M")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCF1M (fr . int_symbol));}
	if (fr . id ("scale_DF1M")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDF1M (fr . int_symbol));}
	if (fr . id ("scale_CIF1M")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCIF1M (fr . int_symbol));}
	if (fr . id ("scale_DIF1M")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogDIF1M (fr . int_symbol));}
	if (fr . id ("scale_R1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogR1 (fr . int_symbol));}
	if (fr . id ("scale_R2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogR2 (fr . int_symbol));}
	if (fr . id ("scale_W1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogW1 (fr . int_symbol));}
	if (fr . id ("scale_W2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogW2 (fr . int_symbol));}
	if (fr . id ("scale_3R1")) {
		READ_SCALE_R_W(0.0)
		slide_rule -> insertScale (new Log3R1 (scale_height, scale, shift));
	}
	if (fr . id ("scale_3R2")) {
		READ_SCALE_R_W(1.0)
		slide_rule -> insertScale (new Log3R2 (scale_height, scale, shift));
	}
	if (fr . id ("scale_3R3")) {
		READ_SCALE_R_W(2.0)
		slide_rule -> insertScale (new Log3R3 (scale_height, scale, shift));
	}
	if (fr . id ("scale_3W1")) {
		READ_SCALE_R_W(0.0)
		slide_rule -> insertScale (new Log3W1 (scale_height, scale, shift));
	}
	if (fr . id ("scale_3W2")) {
		READ_SCALE_R_W(1.0)
		slide_rule -> insertScale (new Log3W2 (scale_height, scale, shift));
	}
	if (fr . id ("scale_3W3")) {
		READ_SCALE_R_W(2.0)
		slide_rule -> insertScale (new Log3W3 (scale_height, scale, shift));
	}
/* GJM -- New Cent and Fahr scales */
	if (fr . id ("scale_Cent0")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCentigrade0Up (fr . int_symbol));}
	if (fr . id ("scale_Cent0_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCentigrade0Down (fr . int_symbol));}
	if (fr . id ("scale_Cent1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCentigrade1Up (fr . int_symbol));}
	if (fr . id ("scale_Cent1_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCentigrade1Down (fr . int_symbol));}
	if (fr . id ("scale_Cent2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCentigrade2Up (fr . int_symbol));}
	if (fr . id ("scale_Cent2_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCentigrade2Down (fr . int_symbol));}
	if (fr . id ("scale_Cent3")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCentigrade3Up (fr . int_symbol));}
	if (fr . id ("scale_Cent3_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogCentigrade3Down (fr . int_symbol));}
	if (fr . id ("scale_Fahr0")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogFahrenheit0Up (fr . int_symbol));}
	if (fr . id ("scale_Fahr0_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogFahrenheit0Down (fr . int_symbol));}
	if (fr . id ("scale_Fahr1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogFahrenheit1Up (fr . int_symbol));}
	if (fr . id ("scale_Fahr1_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogFahrenheit1Down (fr . int_symbol));}
	if (fr . id ("scale_Fahr2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogFahrenheit2Up (fr . int_symbol));}
	if (fr . id ("scale_Fahr2_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogFahrenheit2Down (fr . int_symbol));}
	if (fr . id ("scale_Fahr3")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogFahrenheit3Up (fr . int_symbol));}
	if (fr . id ("scale_Fahr3_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LogFahrenheit3Down (fr . int_symbol));}
	if (fr . id ("scale_L")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LUp (fr . int_symbol));}
	if (fr . id ("scale_L_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LDown (fr . int_symbol));}
/* GJM -- New Ln scale */
	if (fr . id ("scale_Ln")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LnUp (fr . int_symbol));}
	if (fr . id ("scale_Ln_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LnDown (fr . int_symbol));}
	if (fr . id ("scale_LR1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LR (fr . int_symbol, true, false));}
	if (fr . id ("scale_LR2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LR (fr . int_symbol, false, true));}
	if (fr . id ("scale_LR12")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LR (fr . int_symbol, true, true));}
	if (fr . id ("scale_LW1")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LW (fr . int_symbol, true, false));}
	if (fr . id ("scale_LW2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LW (fr . int_symbol, false, true));}
	if (fr . id ("scale_LW12")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new LW (fr . int_symbol, true, true));}
	if (fr . id ("scale_metric")) {
		int scale_height = -1;
		double begin_point = 0.0;
		fr . get_symbol ();
		while (fr . symbol_control >= 5 && fr . symbol_control <= 6) {
			switch (fr . symbol_control) {
			case 5: scale_height = fr . int_symbol; break;
			case 6: begin_point = fr . float_symbol; break;
			default: break;
			}
			fr . get_symbol ();
		}
		should_skip = false;
		if (scale_height < 0) return false;
		slide_rule -> insertScale (new Metric (slide_rule -> metric, scale_height, begin_point));
	}
	if (fr . id ("scale_metric_down")) {
		int scale_height = -1;
		double begin_point = 0.0;
		fr . get_symbol ();
		while (fr . symbol_control >= 5 && fr . symbol_control <= 6) {
			switch (fr . symbol_control) {
			case 5: scale_height = fr . int_symbol; break;
			case 6: begin_point = fr . float_symbol; break;
			default: break;
			}
			fr . get_symbol ();
		}
		should_skip = false;
		if (scale_height < 0) return false;
		slide_rule -> insertScale (new MetricDown (slide_rule -> metric, scale_height, begin_point));
	}
	if (fr . id ("scale_metric_1_25")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new Metric (slide_rule -> metric / 4.0, fr . int_symbol, 0.0));}
	if (fr . id ("scale_metric_1_25_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new MetricDown (slide_rule -> metric / 4.0, fr . int_symbol, 0.0));}
	if (fr . id ("scale_metric_1_50")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new Metric (slide_rule -> metric / 2.0, fr . int_symbol, 0.0));}
	if (fr . id ("scale_metric_1_50_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new MetricDown (slide_rule -> metric / 2.0, fr . int_symbol, 0.0));}
	if (fr . id ("scale_imperial")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new Imperial (slide_rule -> metric / 25.4, fr . int_symbol));}
	if (fr . id ("scale_imperial_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ImperialDown (slide_rule -> metric / 25.4, fr . int_symbol));}

	if (fr . id ("scale_SQRT")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSQRTUp (0.0, fr . int_symbol));}
	if (fr . id ("scale_SQRT_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSQRTDown (0.0, fr . int_symbol));}
	if (fr . id ("scale_SQRT2")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSQRTUp (1.0, fr . int_symbol));}
	if (fr . id ("scale_SQRT2_down")) {if (! fr . get_int ()) return false; slide_rule -> insertScale (new ScaleSQRTDown (1.0, fr . int_symbol));}
	if (fr . id ("scale_OCT")) {
		if (! fr . get_int ()) return false;
		int octave = fr . int_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleOSCUp (octave, fr . int_symbol));
	}
	if (fr . id ("scale_OCT_down")) {
		if (! fr . get_int ()) return false;
		int octave = fr . int_symbol;
		if (! fr . get_int ()) return false;
		slide_rule -> insertScale (new ScaleOSCDown (octave, fr . int_symbol));
	}

	return true;
}

void create_default_sliderule (Sliderule * slide_rule) {
	slide_rule -> insertRule ();
	slide_rule -> root -> stator = true;
	slide_rule -> insertScale (new LogK (18));
	slide_rule -> insertSpacer (4);
	slide_rule -> insertScale (new LogA (18));
	slide_rule -> insertRule ();
	slide_rule -> root -> stator = false;
	slide_rule -> insertScale (new LogB (18));
	slide_rule -> insertSpacer (4);
	slide_rule -> insertScale (new LogC (18));
	slide_rule -> insertRule ();
	slide_rule -> root -> stator = true;
	slide_rule -> insertScale (new LogD (18));
	slide_rule -> insertSpacer (4);
	slide_rule -> insertScale (new LDown (18));
	slide_rule -> close ();
}

