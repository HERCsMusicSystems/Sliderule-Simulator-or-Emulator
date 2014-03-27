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

#include "lanczos.h"
#include "sliderule.h"

HA :: HA (HA * next) {this -> next = next;}
HA :: ~ HA (void) {if (next != NULL) delete next; next = NULL;}

void Rule :: insert_HA (double d1, double d2, double d3, double d4, double d5) {
	HA * ha_root = new HA (NULL);
	ha_root -> d1 = d1; ha_root -> d2 = d2; ha_root -> d3 = d3; ha_root -> d4 = d4; ha_root -> d5 = d5;
	ha_root -> vertical = d2 == d4;
	if (HA_NEXT == NULL) {
		HA_NEXT = HA_ROOT = ha_root;
	} else {
		HA_NEXT -> next = ha_root;
		HA_NEXT = ha_root;
	}
}

void Rule :: insert_HA (double d1, double d2, double d3, char * string) {
	HA * ha_root = new HA (NULL);
	ha_root -> d1 = d1; ha_root -> d2 = d2; ha_root -> d3 = d3; ha_root -> s = wxString :: From8BitData (string);
	ha_root -> vertical = false;
	if (HA_NEXT == NULL) {
		HA_NEXT = HA_ROOT = ha_root;
	} else {
		HA_NEXT -> next = ha_root;
		HA_NEXT = ha_root;
	}
}

void Rule :: draw_HA (wxDC & dc, double x) {
	wxFont f = dc . GetFont ();
	f . SetFaceName (_T ("arial"));
	dc . SetFont (f);
	HA * ha = HA_ROOT;
	int previous_font_size = 24;
	double xsh = slide + (double) x;
	while (ha != NULL) {
		if (ha -> d1 == 0.0) {
			if (ha -> vertical) draw_line (dc, lcp, (stator ? 0.0 : xsh) + ha -> d2 * HA_SCALING, (stator ? rule_height : rule_height + y) - ha -> d3 * HA_SCALING, (stator ? rule_height : rule_height + y) - ha -> d5 * HA_SCALING);
			else dc . DrawLine ((stator ? 0 : (int) xsh) + ha -> d2 * HA_SCALING, (stator ? rule_height : rule_height + y) - ha -> d3 * HA_SCALING, (stator ? 0 : (int) xsh) + ha -> d4 * HA_SCALING, (stator ? rule_height : rule_height + y) - ha -> d5 * HA_SCALING);
		} else {
			int new_font_size = (int) (ha -> d1 * HA_SCALING);
			if (new_font_size != previous_font_size) {
				wxFont f = dc . GetFont ();
				f . SetPointSize (new_font_size);
				dc . SetFont (f);
				previous_font_size = new_font_size;
			}
			dc . DrawText (ha -> s, (stator ? 0 : (int) xsh) + ha -> d2 * HA_SCALING, (stator ? rule_height : rule_height + y) - previous_font_size - ha -> d3 * HA_SCALING);
		}
		ha = ha -> next;
	}
}

void Rule :: draw (wxDC & dc) {
	if (stator) {
		if (stator_bitmap == NULL) {stator_bitmap = new wxBitmap (rule_length, rule_height, 32);}
		if (stator_dc == NULL) {
			stator_dc = new wxMemoryDC (* stator_bitmap);
			stator_dc -> SetBrush (* wxWHITE_BRUSH);
			stator_dc -> Clear ();
			x_scale_position -= x;
			stator_dc -> SetBackground (wxBrush (wxColour (background_colour . red, background_colour . green, background_colour . blue)));
			stator_dc -> Clear ();
			draw_bitmape (* stator_dc);
		}
		dc . Blit (x, y, rule_length, rule_height, stator_dc, 0, 0);
	} else draw_bitmape (dc);
	if (next != NULL) next -> draw (dc);
}

void Rule :: draw_bitmape (wxDC & dc) {
	if (! empty_space) {
		dc . SetPen (wxPen (no_borderline ? wxColour (rule_colour . red, rule_colour . green, rule_colour . blue) : wxColour (marker_colour . red, marker_colour . green, marker_colour . blue)));
		dc . SetBrush (wxBrush (wxColour (rule_colour . red, rule_colour . green, rule_colour . blue)));
		dc . DrawRoundedRectangle (stator ? 0 : (int) (slide + (double) x), stator ? 0 : y, rule_length, rule_height, corner);
	}
	dc . SetPen (wxPen (wxColour (marker_colour . red, marker_colour . green, marker_colour . blue)));
	Scale * skala = root;
	while (skala != NULL) {
		double location = (double) x_scale_position + slide;
		skala -> draw_highlight (dc, location);
		skala -> drawer (dc, location);
		skala -> draw_descriptions (dc, location);
		skala = skala -> next;
	}
	if (HA_ROOT != NULL) draw_HA (dc, 0);
}

Rule * Rule :: captureRule (int y) {
	if (y >= this -> y && y < this -> y + rule_height && ! channel) return this;
	if (next == NULL) return NULL;
	return next -> captureRule (y);
}

Scale * Rule :: captureScale (int y) {
	if (root == NULL) return NULL;
	return root -> captureScale (y - (stator ? this -> y : 0));
}

void Rule :: reBitmap (void) {
	if (stator_dc != NULL) delete stator_dc; stator_dc = NULL;
	if (stator_bitmap != NULL) delete stator_bitmap; stator_bitmap = NULL;
}

void Rule :: initialise_lanczos (void) {
	initialise_lanczos_colours (lcp, rule_colour . red, rule_colour . green, rule_colour . blue, marker_colour . red, marker_colour . green, marker_colour . blue);// 0xd0, 0xd0, 0, 0, 0);
	initialise_lanczos_colours (hairline_lp, rule_colour . red, rule_colour . green, rule_colour . blue, hairline_colour . red, hairline_colour . green, hairline_colour . blue);
}

Rule :: Rule (Rule * next) {
	stator = true;
	free = false;
	channel = false;
	free_index = 0;
	empty_space = false;
	no_borderline = false;
	slide = 0.0;
	corner = 6;
	rule_colour . red = rule_colour . green = rule_colour . blue = 0xd0;
	marker_colour . red = marker_colour . green = marker_colour . blue = 0x00;
	hairline_colour . red = 0xff; hairline_colour . green = hairline_colour . blue = 0x00;
	background_colour . red = background_colour . green = background_colour . blue = 0x00;
	rule_height = 40;
	motion_multiplier = 0.2;
	root = NULL;
	this -> next = next;
	stator_dc = NULL;
	stator_bitmap = NULL;
	HA_ROOT = NULL;
	HA_NEXT = NULL;
	HA_SCALING = 200.0;
	visible_left_marginal_factor = visible_right_marginal_factor = 0.2;
}

Rule :: ~ Rule (void) {
	if (root != NULL) delete root; root = NULL;
	if (next != NULL) delete next; next = NULL;
	if (HA_ROOT != NULL) delete HA_ROOT; HA_ROOT = NULL;
	reBitmap ();
}

void Hairline :: draw (wxDC & dc, lanczos_colour * lcp, double x, double scale_length) {
	if (from < 0) return;
	last_drawn = x + location * scale_length;
	draw_thin_line (dc, lcp, last_drawn, from, to);
}

void Hairline :: draw_readout (wxDC & dc, int y, double v, colour * c, colour * b) {
	if (y < from || y >= to) return;
	wxString text = wxString :: Format (_T ("%f"), v);
	wxSize extent = dc . GetTextExtent (text);
	dc . SetPen (wxPen (wxColour (c -> red, c -> green, c -> blue)));
	dc . SetBrush (wxBrush (wxColour (b -> red, b -> green, b -> blue)));
	int x_location = last_drawn + (location >= 0.0 ? 4 : -4 - extent . x);
	dc . DrawRectangle (x_location - 1, y, extent . x + 2, extent . y);
	dc . SetTextForeground (wxColour (c -> red, c -> green, c -> blue));
	dc . DrawText (text, x_location, y);
}

bool Hairline :: in_range (int y) {return y >= from && y < to;}

bool Hairline :: in_range (int x, int y) {
	if (y < from || y >= to || x + 6 < (int) last_drawn || x - 6 > (int) last_drawn) return false;
	return true;
}

Hairline :: Hairline (void) {from = to = -1; last_drawn = 0.0;}
Hairline :: Hairline (double location) {from = to = -1; last_drawn = this -> location = location;}

void Sliderule :: apply_visible_margins (double left, double right) {
	if (root == NULL) return;
	root -> visible_left_marginal_factor = left; root -> visible_right_marginal_factor = right;
	root -> rule_length = (int) ((double) scale_length * (left + 1.0 + right));
	root -> x_scale_position = this -> x + (int) ((double) scale_length * marginal_factor);
	root -> x = root -> x_scale_position - (int) ((double) scale_length * left);
}

void Sliderule :: insertBlankSpacer (int height) {
	insertSpacer (height);
	if (root == 0) return;
	if (root -> root == 0) return;
	for (int ind = 0; ind < 16; ind++) {root -> root -> draw_lines [ind] = false;}
}

Rule * Sliderule :: insertRule (int channel_index) {
	if (root != NULL && ! working_no_borderline) insertBlankSpacer ();
	if (root != NULL) insertBlankSpacer (os_compensation);
	working_no_borderline = no_borderline;
	root = new Rule (root);
	apply_visible_margins (marginal_factor, marginal_factor);
	root -> y = this -> rule_y;
	Rule * rp = root;
	while (channel_index-- > 0 && rp != NULL) {
		rp = rp -> next;
		if (rp != NULL) this -> rule_y = root -> y = rp -> y;
	}
	root -> corner = this -> corner;
	root -> rule_colour = rule_colour;
	root -> marker_colour = marker_colour;
	root -> hairline_colour = hairline_colour;
	root -> background_colour = background_colour;
	root -> no_borderline = this -> no_borderline;
	root -> motion_multiplier = this -> motion_multiplier;
	if (! no_borderline) insertBlankSpacer ();
	return root;
}

void Sliderule :: resize_length (int new_length) {
	double slide_ratio = (double) new_length / (double) scale_length;
	scale_length = new_length;
	Rule * rp = root;
	while (rp != NULL) {
		rp -> x_scale_position = x + (int) ((double) scale_length * marginal_factor);
		rp -> rule_length = (int) ((double) scale_length * (rp -> visible_left_marginal_factor + 1.0 + rp -> visible_right_marginal_factor));
		rp -> x = rp -> x_scale_position - (int) ((double) scale_length * rp -> visible_left_marginal_factor);
		rp -> slide *= slide_ratio;
		rp -> reBitmap ();
		Scale * sp = rp -> root;
		while (sp != NULL) {
			sp -> scale_length = scale_length;
			sp -> scaleInit ();
			sp = sp -> next;
		}
		rp = rp -> next;
	}
}

void Sliderule :: change_motion_multipliers (double multiplier) {
	this -> motion_multiplier = multiplier;
	Rule * rp = root;
	while (rp != NULL) {
		rp -> motion_multiplier = multiplier;
		rp = rp -> next;
	}
}

void Sliderule :: close (void) {
	if (! no_borderline) insertBlankSpacer ();
	insertBlankSpacer (os_compensation);
	initialise_lanczos_colours (hairline_lp, rule_colour . red, rule_colour . green, rule_colour . blue, hairline_colour . red, hairline_colour . green, hairline_colour . blue);
	initialise_lanczos_colours (other_hairlines_lp, rule_colour . red, rule_colour . green, rule_colour . blue, multi_hairlines_colour . red, multi_hairlines_colour . green, multi_hairlines_colour . blue);
	initialise_lanczos_colours (background_lp, background_colour . red, background_colour . green, background_colour . blue, background_marker_colour . red, background_marker_colour . green, background_marker_colour . blue);
	Rule * rp = root;
	while (rp != NULL) {
		rp -> initialise_lanczos ();
		rp = rp -> next;
	}
}

Scale * Sliderule :: insertScale (Scale * scale) {
	if (scale == NULL) return NULL;
	if (root != NULL) {
		scale -> position_scale (root -> stator ? rule_y - root -> y : rule_y, scale_length, root -> empty_space ? background_marker_colour : marker_colour, hairline_colour, root -> empty_space ? background_colour : rule_colour, root -> empty_space ? background_lp : root -> lcp, root -> empty_space ? background_lp : root -> hairline_lp);
		scale -> no_pi = this -> no_pi; scale -> no_e = this -> no_e; scale -> no_c = this -> no_c; scale -> no_c1 = this -> no_c1; scale -> no_degree = this -> no_degree; scale -> no_m = this -> no_m; scale -> no_mm = this -> no_mm;
		scale -> left_extension = left_extension; scale -> right_extension = right_extension;
		for (int ind = 0; ind < 16; ind++) {scale -> draw_lines [ind] = this -> draw_lines [ind]; scale -> marker_fractions [ind] = this -> marker_fractions [ind];}
		scale -> next = root -> root;
		root -> root = scale;
		scale -> scaleInit ();
		rule_y += scale -> height;
		root -> rule_height = rule_y - root -> y;
	}
	return scale;
}

Sliderule * Sliderule :: insertSpacer (int height) {
	if (root == NULL) return this;
	if (height < 1) return this;
	Scale * spacer_scale = new Scale (height);
	spacer_scale -> inactive = true;
	insertScale (spacer_scale);
	return this;
}

void Sliderule :: draw_readout (wxDC & dc, double value, int x, int y, colour * c) {
	//wxString readout = wxString :: Format (_T ("%.2f"), value);
	wxString readout = wxString :: Format (decimal_string, value);
	wxSize extent = dc . GetTextExtent (readout);
	dc . SetPen (wxPen (wxColour (c -> red, c -> green, c -> blue)));
	dc . SetBrush (wxBrush (wxColour (rule_colour . red, rule_colour . green, rule_colour . blue)));
	dc . DrawRectangle (x - 1, y, extent . x + 2, extent . y);
	dc . SetTextForeground (wxColour (c -> red, c -> green, c -> blue));
	dc . DrawText (readout, x, y);
}

void Sliderule :: draw_readouts (wxDC & dc) {
	Rule * rule = root;
	while (rule != NULL) {
		Scale * scale = rule -> root;
		while (! rule -> channel && scale != NULL) {
			if (scale -> inactive) {scale = scale -> next; continue;}
			int scale_y = (rule -> stator ? rule -> y : 0) + scale -> y;
			wxFont f = dc . GetFont ();
			f . SetPointSize (scale -> height / 2);
			dc . SetFont (f);
			double value = scale -> getValue (cursor_location - rule -> slide / (double) scale_length);
			if ((draw_digital_readouts_on_stator_left && rule -> stator) || (draw_digital_readouts_on_slide_left && ! rule -> stator))
				draw_readout (dc, value, x + 12 + (int) rule -> slide, scale_y, scale -> reversed ? & scale -> red_marking_colour : & scale -> marking_colour);
			if ((draw_digital_readouts_on_stator_right && rule -> stator) || (draw_digital_readouts_on_slide_right && ! rule -> stator))
				draw_readout (dc, value, x - 100 + (int) rule -> slide + scale_length * (1.0 + 2.0 * marginal_factor), scale_y, scale -> reversed ? & scale -> red_marking_colour : & scale -> marking_colour);
			if (draw_digital_readouts_on_main_hairline) {
				draw_readout (dc, value, x + 4 + (int) ((cursor_location + marginal_factor) * (double) scale_length), scale_y, & hairline_colour);
			}
			if (draw_digital_readouts_on_multi_hairlines) {
				double shift = cursor_location - rule -> slide / (double) scale_length;
				s_hairline . draw_readout (dc, scale_y, scale -> getValue (shift + s_hairline . location), & multi_hairlines_colour, & rule_colour);
				if (display_metric_hp) {
					metric_hp_hairline . draw_readout (dc, scale_y, scale -> getValue (shift + metric_hp_hairline . location), & multi_hairlines_colour, & rule_colour);
					metric_kw_hairline . draw_readout (dc, scale_y, scale -> getValue (shift + metric_kw_hairline . location), & multi_hairlines_colour, & rule_colour);
				}
				if (display_mechanical_hp) {
					mechanical_hp_hairline . draw_readout (dc, scale_y, scale -> getValue (shift + mechanical_hp_hairline . location), & multi_hairlines_colour, & rule_colour);
					mechanical_kw_hairline . draw_readout (dc, scale_y, scale -> getValue (shift + mechanical_kw_hairline . location), & multi_hairlines_colour, & rule_colour);
				}
				d_hairline . draw_readout (dc, scale_y, scale -> getValue (shift + d_hairline . location), & multi_hairlines_colour, & rule_colour);
				hairline_360 . draw_readout (dc, scale_y, scale -> getValue (shift + hairline_360 . location), & multi_hairlines_colour, & rule_colour);
				hairline_36010 . draw_readout (dc, scale_y, scale -> getValue (shift + hairline_36010 . location), & multi_hairlines_colour, & rule_colour);
				hairline_360st . draw_readout (dc, scale_y, scale -> getValue (shift + hairline_360st . location), & multi_hairlines_colour, & rule_colour);
				for (int ind = 0; ind < number_of_extra_hairlines; ind++) extra_hairlines [ind] -> draw_readout (dc, scale_y, scale -> getValue (shift + extra_hairlines [ind] -> location), & multi_hairlines_colour, & rule_colour);
			}
			scale = scale -> next;
		}
		rule = rule -> next;
	}
}

void Sliderule :: draw (wxBufferedPaintDC & dc) {
	if (root == NULL) return;
	dc . SetBackground (wxBrush (wxColour (background_colour . red, background_colour . green, background_colour . blue)));
	dc . Clear ();
	root -> draw (dc);
	draw_cursor (dc);
	draw_readouts (dc);
}

void Sliderule :: draw_cursor (wxDC & dc) {
	int cursor_left = x + (int) ((double) scale_length * (marginal_factor + cursor_location - cursor_factor * 0.5));
	int cursor_right = cursor_left + (int) ((double) scale_length * cursor_factor);
	dc . SetBrush (wxBrush (wxColour (cursor_colour . red, cursor_colour . green, cursor_colour . blue, cursor_colour . alpha)));
	if (draw_cursor_window) {
		dc . DrawRoundedRectangle (cursor_left, y - 2, cursor_right - cursor_left, rule_y - y + 4, 4);
	}
	dc . SetPen (wxPen (wxColour (cursor_colour . red, cursor_colour . green, cursor_colour . blue)));
	dc . SetBrush (wxBrush (wxColour (0, 0,  0), wxTRANSPARENT));
	dc . DrawRoundedRectangle (cursor_left, y - 2, cursor_right - cursor_left, rule_y - y + 4, 4);
	dc . DrawRoundedRectangle (cursor_left - 1, y - 3, cursor_right - cursor_left + 2, rule_y - y + 6, 4);
//	drawed_hairline = x + (double) scale_length * (cursor_location + marginal_factor);
	drawed_hairline = x + (int) ((double) scale_length * marginal_factor);
	drawed_hairline += (double) scale_length * cursor_location;
	double S = (double) scale_length * C;
	drawed_d = drawed_hairline + S;
	drawed_s = drawed_hairline - S;
	drawed_mechanical_kw = drawed_d + (double) scale_length * mechanical_hp;
	drawed_metric_kw = drawed_d + (double) scale_length * metric_hp;
	drawed_mechanical_hp = drawed_hairline - 0.5 * (double) scale_length * mechanical_hp;
	drawed_metric_hp = drawed_hairline - 0.5 * (double) scale_length * metric_hp;
	double shift_360 = (double) scale_length * location_360;
	drawed_360 = drawed_hairline + shift_360;
	drawed_36010 = drawed_hairline + (double) scale_length * location_36010;
	drawed_360st = drawed_hairline - shift_360;
//	if (process_cursor_hairline) draw_alpha_line (dc, hairline_lp, x + scale_length * (cursor_location + marginal_factor), y, rule_y);
	if (draw_cursor_2_hairlines) {
		draw_thin_line (dc, hairline_lp, drawed_hairline, y, rule_y);
		d_hairline . draw (dc, other_hairlines_lp, drawed_hairline, (double) scale_length);
		if (display_metric_hp) {
			metric_hp_hairline . draw (dc, other_hairlines_lp, drawed_hairline, (double) scale_length);
			metric_kw_hairline . draw (dc, other_hairlines_lp, drawed_hairline, (double) scale_length);
		}
		if (display_mechanical_hp) {
			mechanical_hp_hairline . draw (dc, other_hairlines_lp, drawed_hairline, (double) scale_length);
			mechanical_kw_hairline . draw (dc, other_hairlines_lp, drawed_hairline, (double) scale_length);
		}
		s_hairline . draw (dc, other_hairlines_lp, drawed_hairline, (double) scale_length);
		hairline_360 . draw (dc, other_hairlines_lp, drawed_hairline, (double) scale_length);
		hairline_36010 . draw (dc, other_hairlines_lp, drawed_hairline, (double) scale_length);
		hairline_360st . draw (dc, other_hairlines_lp, drawed_hairline, (double) scale_length);
		for (int ind = 0; ind < number_of_extra_hairlines; ind++) extra_hairlines [ind] -> draw (dc, other_hairlines_lp, drawed_hairline, (double) scale_length);
	}
	if (draw_cursor_hairline) draw_line (dc, hairline_lp, (double) x + (double) scale_length * (cursor_location + marginal_factor), y, rule_y);
}

Rule * Sliderule :: captureRule (int y) {
	if (root == NULL) return NULL;
	return root -> captureRule (y);
}

Scale * Sliderule :: captureScale (int y) {
	Rule * captured_rule = captureRule (y);
	if (captured_rule == NULL) return NULL;
	return captured_rule -> captureScale (y);
}

void Sliderule :: insertExtraHairlines (int extra_hairlines) {
	number_of_extra_hairlines = extra_hairlines;
	if (number_of_extra_hairlines < 0) number_of_extra_hairlines = 0;
	if (number_of_extra_hairlines > 128) number_of_extra_hairlines = 128;
	this -> extra_hairlines = new HairlinePointer [number_of_extra_hairlines + 16];
	for (int ind = 0; ind < number_of_extra_hairlines; ind++) this -> extra_hairlines [ind] = new Hairline ();
}

Sliderule :: Sliderule (int x, int y, int scale_length) {
	this -> x = x; this -> rule_y = this -> y = y; this -> scale_length = scale_length;
	this -> marginal_factor = 0.2;
	this -> cursor_factor = 0.2;
	this -> corner = 6;
	rule_colour . red = rule_colour . green = rule_colour . blue = 0xd0;
	marker_colour . red = marker_colour . green = marker_colour . blue = 0x00;
	hairline_colour . red = 255; hairline_colour . green = hairline_colour . blue = 0;
	multi_hairlines_colour . red = 255; multi_hairlines_colour . green = multi_hairlines_colour . blue = 0;
	cursor_colour . red = 255; cursor_colour . green = 255; cursor_colour . blue = 0; cursor_colour . alpha = 48;
	background_colour . red = 0; background_colour . green = 0; background_colour . blue = 255;
	background_marker_colour . red = background_marker_colour . green = background_marker_colour . blue = 255;
	root = NULL;
	nesting_level = 4;
	cursor_location = 1.0;
	draw_cursor_window = false;
//	process_cursor_window = false;
//	process_cursor_hairline = false;
	draw_cursor_2_hairlines = true;
	draw_cursor_hairline = false;
	draw_digital_readouts_on_slide_left = draw_digital_readouts_on_slide_right = true;
	draw_digital_readouts_on_stator_left = draw_digital_readouts_on_stator_right = true;
	draw_digital_readouts_on_main_hairline = draw_digital_readouts_on_multi_hairlines = true;
	draw_decimal_places = -1;
	decimal_string = _T ("%f");
	no_borderline = false;
	working_no_borderline = false;
	metric = 304.800;
//	top_auto_spacer = 0;
//	bottom_auto_spacer = 0;
//	scale_height_compensation = 0;
	motion_multiplier = 0.2;
	animation_steps = 100;
	animation_delay = 50;
	animation_linear = false;
	animation_variable_speed = false;
	C = log10 (1.0 / sqrt (3.14159265358979323846 / 4.0));
	location_360 = log10 (3.6 / 3.14159265358979323846);
	location_36010 = log10 (3.6 / sqrt (10.0));
	mechanical_hp = log10 (0.74569987158227022);
	sqrt_mechanical_hp = mechanical_hp * 0.5;
	metric_hp = log10 (0.73549875);
	sqrt_metric_hp = metric_hp * 0.5;
	d_hairline . location = C;
	s_hairline . location = - C;
	metric_hp_hairline . location = - sqrt_metric_hp;
	mechanical_hp_hairline . location = - sqrt_mechanical_hp;
	metric_kw_hairline . location = C + metric_hp;
	mechanical_kw_hairline . location = C + mechanical_hp;
	hairline_360 . location = location_360;
	hairline_36010 . location = location_36010;
	hairline_360st . location = - location_360;
	display_metric_hp = display_mechanical_hp = false;
	number_of_extra_hairlines = 0;
	this -> extra_hairlines = NULL;
	left_extension = right_extension = 0.0;
	no_pi = no_e = no_c = no_c1 = false;
	no_degree = true;
	no_m = no_mm = true;
	for (int ind = 0; ind < 16; ind++) {draw_lines [ind] = false; marker_fractions [ind] = 1.0;}
	marker_fractions [0] = 0.5; marker_fractions [1] = 0.4; marker_fractions [2] = 0.3; marker_fractions [3] = 0.2;
	name = _T ("Slide Rule");
}

Sliderule :: ~ Sliderule (void) {
	if (root != NULL) delete root; root = NULL;
	if (extra_hairlines != NULL) {
		for (int ind = 0; ind < number_of_extra_hairlines; ind++) delete extra_hairlines [ind];
		delete [] extra_hairlines;
		extra_hairlines = NULL;
	}
}

