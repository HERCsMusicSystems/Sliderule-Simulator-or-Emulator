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

#ifndef _SCALE_BASE_
#define _SCALE_BASE_

#include "lanczos.h"
#include "wx/wx.h"

extern double absolute (double x);
extern double smaller (double x, double y);
extern int find_left_index (double * markers, int from, int to, double threshold);
extern int find_right_index (double * markers, int from, int to, double threshold);
extern double dec_correction (double value);
extern double dec_recorrection (double value);

/*

static int stepProgress (int step) {
	if (step < 2) return 2;
	if (step < 5) return 5;
	if (step < 10) return 10;
	if (step < 20) return 20;
	if (step < 50) return 50;
	return 100;
};

static int get_starter (int starter, int shift) {
	switch (shift) {
	case 1: case 2: case 5: return starter;
	case 10:
		for (int ind = 10; ind < 90; ind += 10) {
			if (starter <= ind) return ind;
		}
		return shift;
		break;
	case 20: if (starter <= 20) return 20; if (starter <= 40) return 40; if (starter <= 60) return 60; return shift;
	case 50: if (starter == 10) return 0; return shift;
	default: return shift;
	}
	return shift;
}

*/
class Scale {
public:
	int local_x;
	int y;
	int scale_length;
	int height;
	colour marking_colour;
	colour red_marking_colour;
	lanczos_colour * lcp;
	lanczos_colour * red_lp;
	Scale * next;
	int y_base, y0, y1, y2, y3, y4;
	int font_y;
	int scale_type;
	wxString left_text;
	bool left_text_justify_right;
	int left_text_pixel_shift;
	double left_text_relative_shift;
	wxString right_text;
	bool right_text_justify_right;
	int right_text_pixel_shift;
	double right_text_relative_shift;
	wxString middle_text;
	bool highlight;
	bool build_alternate_lanczos_colours;
	bool build_individual_marker_lanczos_colour, build_individual_reversed_marker_lanczos_colour;
	colour background_colour;
	colour highlight_colour;
	int left_highlight_pixel_shift;
	double left_highlight_relative_shift;
	int right_highlight_pixel_shift;
	double right_highlight_relative_shift;
	double left_highlight_location, right_highlight_location;
	lanczos_colour * alt_lcp;
	lanczos_colour * alt_red_lp;
	lanczos_colour * individual_lcp;
	lanczos_colour * individual_red_lp;
	double left_extension;
	double right_extension;
	double minimum_step;
	int maximum_step;
	bool markings_increasing;
	bool inactive;
	bool reversed;
	bool no_pi, no_e, no_c, no_c1, no_degree, no_m, no_mm;
	bool draw_lines [16];
	double marker_fractions [16];
	double left_horizontal_extension, right_horizontal_extension;
	int os_compensation;
private:
	void faceUp (int h1, int h2, int h3, int h4);
	void faceDown (int h1, int h2, int h3, int h4);
public:
	void faceUp (void);
	void faceDown (void);
	void setArialFont (wxDC & dc);
	lanczos_colour * get_bordercolour (double x);
	void draw_text_marker (wxDC & dc, wxString text, double x);
	void draw_index_location (wxDC & dc, double x, wxString description = _T ("1"));
	double draw_markings_for_100 (wxDC & dc, double * markings, wxString description, double x, double last_drawn, int from = 0, int to = 100);
	void draw_markings (wxDC & dc, double * markings, double x, int starter = 0, int sentinel = 100);
	void draw_dense_markings (wxDC & dc, double * markings, double x);
	virtual void draw_horizontal_lines (wxDC & dc, double x);
	virtual void draw (wxDC & dc, double x);
	virtual void scaleInit (void);
	void draw_highlight (wxDC & dc, double x);
	void draw_descriptions (wxDC & dc, double x);
	void position_scale (int y, int scale_length, colour markers, colour red_markers, colour rule, lanczos_colour * lcp, lanczos_colour * red_lp);
	Scale * captureScale (int y);
	virtual double getValue (double x);
	virtual double getLocation (double x);
	void draw_pi (wxDC & dc, double x);
	void draw_e (wxDC & dc, double x);
	void draw_c (wxDC & dc, double x);
	void draw_c1 (wxDC & dc, double x);
	void draw_degree (wxDC & dc, double x);
	void draw_m (wxDC & dc, double x);
	void draw_mm (wxDC & dc, double x);
	void drawer (wxDC & dc, double x);
	Scale (int height);
	~ Scale (void);
};

#endif

