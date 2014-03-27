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

#ifndef _SLIDERULE_CLASS_
#define _SLIDERULE_CLASS_

#include "scale_base.h"

#include "wx/dcbuffer.h"

struct HA {
	double d1, d2, d3, d4, d5;
	bool vertical;
	wxString s;
	HA * next;
	HA (HA * next = NULL);
	~ HA (void);
};

class Rule {
private:
	wxMemoryDC * stator_dc;
	wxBitmap * stator_bitmap;
public:
	HA * HA_ROOT;
	HA * HA_NEXT;
	double HA_SCALING;
	bool stator;
	bool free;
	bool channel;
	int free_index;
	bool empty_space;
	double slide;
	double visible_left_marginal_factor, visible_right_marginal_factor;
	int x, y;
	int x_scale_position;
	int rule_length;
	int rule_height;
	int corner;
	colour rule_colour, marker_colour, hairline_colour, background_colour;
	bool no_borderline;
	double motion_multiplier;
	Scale * root;
	Rule * next;
	lanczos_colour lcp [256];
	lanczos_colour hairline_lp [256];
	void insert_HA (double d1, double d2, double d3, double d4, double d5);
	void insert_HA (double d1, double d2, double d3, char * string);
	void draw_HA (wxDC & dc, double x);
	void draw (wxDC & dc);
	void draw_bitmape (wxDC & dc);
	Rule * captureRule (int y);
	Scale * captureScale (int y);
	void reBitmap (void);
	void initialise_lanczos (void);
	Rule (Rule * next);
	~ Rule (void);
};

class Hairline {
public:
	int from, to;
	double location;
	double last_drawn;
	void draw (wxDC & dc, lanczos_colour * lcp, double x, double scale_length);
	void draw_readout (wxDC & dc, int y, double v, colour * c, colour * b);
	bool in_range (int y);
	bool in_range (int x, int y);
	Hairline (void);
	Hairline (double location);
};

typedef Hairline * HairlinePointer;

class Sliderule {
public:
	int x, y;
	int rule_y;
	int corner;
	int scale_length;
	double marginal_factor;
	double cursor_factor;
	colour rule_colour;
	colour marker_colour;
	colour hairline_colour;
	colour multi_hairlines_colour;
	colour cursor_colour;
	colour background_colour;
	colour background_marker_colour;
	double cursor_location;
	bool draw_cursor_window;
	bool draw_cursor_2_hairlines;
	bool draw_cursor_hairline;
	bool draw_digital_readouts_on_slide_left;
	bool draw_digital_readouts_on_slide_right;
	bool draw_digital_readouts_on_stator_left;
	bool draw_digital_readouts_on_stator_right;
	bool draw_digital_readouts_on_multi_hairlines;
	bool draw_digital_readouts_on_main_hairline;
	int draw_decimal_places;
	wxString decimal_string;
	bool no_borderline;
	bool working_no_borderline;
	double metric;
	double motion_multiplier;
	lanczos_colour hairline_lp [256];
	lanczos_colour other_hairlines_lp [256];
	lanczos_colour background_lp [256];
	int animation_steps;
	int animation_delay;
	bool animation_linear;
	bool animation_variable_speed;
	double C;
	double location_360;
	double location_36010;
	double mechanical_hp, metric_hp;
	double sqrt_mechanical_hp, sqrt_metric_hp;
	bool display_metric_hp, display_mechanical_hp;
	Hairline s_hairline, d_hairline, hairline_360, hairline_36010, hairline_360st, metric_kw_hairline, mechanical_kw_hairline, metric_hp_hairline, mechanical_hp_hairline;
	int number_of_extra_hairlines;
	HairlinePointer * extra_hairlines;
	bool no_pi, no_e, no_c, no_c1, no_degree, no_m, no_mm;
	bool draw_lines [16];
	double marker_fractions [16];
	double left_extension, right_extension;
	double drawed_hairline, drawed_s, drawed_d, drawed_mechanical_hp, drawed_metric_hp, drawed_mechanical_kw, drawed_metric_kw, drawed_360, drawed_36010, drawed_360st;
	wxString name;
	Rule * root;
	int nesting_level;
	void apply_visible_margins (double left, double right);
	void insertBlankSpacer (int height = 1);
	Rule * insertRule (int channel_index = 0);
	void resize_length (int new_length);
	void change_motion_multipliers (double multiplier);
	void close (void);
	Scale * insertScale (Scale * scale);
	Sliderule * insertSpacer (int height = 1);
	void draw_readout (wxDC & dc, double value, int x, int y, colour * c);
	void draw_readouts (wxDC & dc);
	void draw (wxBufferedPaintDC & dc);
	void draw_cursor (wxDC & dc);
	Rule * captureRule (int y);
	Scale * captureScale (int y);
	void insertExtraHairlines (int extra_hairlines);
	Sliderule (int x, int y, int scale_length);
	~ Sliderule (void);
};

#endif

