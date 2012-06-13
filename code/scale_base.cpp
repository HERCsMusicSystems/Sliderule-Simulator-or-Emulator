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

#include "scale_base.h"

double absolute (double x) {return x >= 0.0 ? x : - x;}
double smaller (double x, double y)  {x = absolute (x); y = absolute (y); return x < y ? x : y;}

int stepProgress (int step) {
	if (step < 2) return 2;
	if (step < 5) return 5;
	if (step < 10) return 10;
	if (step < 20) return 20;
	if (step < 50) return 50;
	return 100;
};

int get_starter (int starter, int shift) {
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

int find_left_index (double * markers, int from, int to, double threshold) {
	for (int ind = from; ind <= to; ind++) {if (markers [ind] >= threshold) return ind;}
	return to;
}

int find_right_index (double * markers, int from, int to, double threshold) {
	for (int ind = to; ind >= from; ind--) {if (markers [ind] <= threshold) return ind;}
	return from;
}

double dec_correction (double value) {
	double remainder = value - (double) ((int) value);
	if (remainder > 0.99999999) return value;
	remainder *= 6.0 / 10.0;
	return (double) ((int) value) + remainder;
}

double dec_recorrection (double value) {
	double remainder = value - (double) ((int) value);
	remainder *= 10.0 / 6.0;
	return (double) ((int) value) + remainder;
}

void Scale :: faceUp (int h1, int h2, int h3, int h4) {
	y_base = y; y0 = y + height - 1 + os_compensation; y1 = y0 - h1; y2 = y0 - h2; y3 = y0 - h3; y4 = y0 - h4; font_y = y - height / 8;
}

void Scale :: faceDown (int h1, int h2, int h3, int h4) {
	y_base = y + height - 1 + os_compensation; y0 = y; y1 = y0 + h1; y2 = y0 + h2; y3 = y0 + h3; y4 = y0 + h4; font_y = y - height / 8 + height / 2;
}

void Scale :: faceUp (void) {
	faceUp (height * marker_fractions [0], height * marker_fractions [1], height * marker_fractions [2], height * marker_fractions [3]);
}

void Scale :: faceDown (void) {
	faceDown (height * marker_fractions [0], height * marker_fractions [1], height * marker_fractions [2], height * marker_fractions [3]);
}

void Scale :: setArialFont (wxDC & dc) {
	wxFont f = dc . GetFont ();
	f . SetFaceName (_T ("arial"));
	int point_size = inactive ? height * 3 / 4: height / 2;
	if (point_size < 6) point_size = 6;
	f . SetPointSize (point_size);
	dc . SetFont (f);
	dc . SetTextForeground (reversed ? wxColour (red_lp -> r0, red_lp -> g0, red_lp -> b0) : wxColour (lcp -> r0, lcp -> g0, lcp -> b0));
}

lanczos_colour * Scale :: get_bordercolour (double x) {
	x -= local_x;
	if (highlight) {
		if (x < left_highlight_location || x > right_highlight_location) return reversed ? red_lp : lcp;
		return reversed ? alt_red_lp : alt_lcp;
	}
	return reversed ? red_lp : lcp;
}

void Scale :: draw_text_marker (wxDC & dc, wxString text, double x) {dc . DrawText (text, x - dc . GetTextExtent (text) . x / 2, font_y);}

void Scale :: draw_index_location (wxDC & dc, double x, wxString description) {
	draw_line (dc, get_bordercolour (x - 1.0), get_bordercolour (x + 1.0), x, y0, y1);
	draw_text_marker (dc, description, x);
}

// starters allowed: 10, 50
double Scale :: draw_markings_for_100 (wxDC & dc, double * markings, wxString description, double x, double last_drawn, int from, int to) {
	bool to_changed = false;
	if (to < 100) {to++; to_changed = true;}
	int step = 1;
	int index;
	bool half;
	int counter;
	lanczos_colour * lp = get_bordercolour (x + markings [from] + 1.0);
	while (step < 100
		&& (absolute (markings [to] - markings [to - step]) < minimum_step
		|| absolute (markings [from + step] - markings [from]) < minimum_step)) step = stepProgress (step);
	switch (step) {
	case 100: break;
	case 50: if (from <= 50 && to >= 50) draw_line (dc, lp, x + markings [50], y0, y2); break;
	case 20:
		index = 20;
		while (index < from && index < to) index += 20;
		while (index < to) {draw_line (dc, lp, x + markings [index], y0, y2); index += 20;}
		break;
	case 10:
		index = 10;
		while (index < from && index < to) index += 10;
		while (index < to) {draw_line (dc, lp, x + markings [index], y0, index == 50 ? y2 : y3); index += 10;}
		break;
	case 5:
		index = 5; half = true;
		while (index < from && index < to) {index += 5; half = ! half;}
		while (index < to) {
			draw_line (dc, lp, x + markings [index], y0, half ? y4 : index == 50 ? y2 : y3);
			index += 5; half = ! half;
		}
		break;
	case 2:
		index = 2; counter = 4;
		while (index < from && index < to) {index += 2; if (--counter < 0) counter = 4;}
		while (index < to) {
			draw_line (dc, lp, x + markings [index], y0, index == 50 ? y2 : counter < 1 ? y3 : y4);
			index += 2;
			if (--counter < 0) counter = 4;
		}
		break;
	case 1:
		index = 1; counter = 9;
		while (index < from && index < to) {index++; if (--counter < 0) counter = 9;}
		while (index < to) {
			draw_line (dc, lp, x + markings [index], y0, index == 50 ? y1 : counter < 1 ? y2 : counter == 5 ? y3 : y4);
			index++;
			if (--counter < 0) counter = 9;
		}
		break;
	default: break;
	}
	if (to_changed) to--;
	if (to < 100) return last_drawn;
	double distance = absolute (markings [100] - last_drawn);
	if (distance < minimum_step) return last_drawn;
	double location = x + markings [100];
	draw_line (dc, lp, get_bordercolour (location + 1.0), location, y0, y1);
	wxSize extent = dc . GetTextExtent (description);
	if (distance < (double) extent . x) return markings [100];
	dc . DrawText (description, x + markings [100] - extent . x / 2, font_y);
	return markings [100];
}

void Scale :: draw_markings (wxDC & dc, double * markings, double x, int starter, int sentinel) {
	int step = 1;
	lanczos_colour * lcp = get_bordercolour (x + markings [starter] + 1.0);
	while (step < 100 && absolute (markings [markings_increasing ? 0 : sentinel] - markings [markings_increasing ? step : sentinel - step]) < minimum_step) step = stepProgress (step);
	switch (step) {
	case 1:
		for (int ind = get_starter (starter, step); ind < sentinel; ind += 10) {
			if (ind > 0) draw_line (dc, lcp, x + markings [ind], y0, ind == 50 ? y1 : y2);
			for (int sub = 1; sub < 10; sub++) draw_line (dc, lcp, x + markings [ind + sub], y0, sub == 5 ? y3 : y4);
		}
		break;
	case 2: case 5:
		for (int ind = get_starter (starter, step); ind < sentinel; ind += 10) {
			if (ind > 0) draw_line (dc, lcp, x + markings [ind], y0, ind == 50 ? y2 : y3);
			if (step == 5) draw_line (dc, lcp, x + markings [ind + 5], y0, y4);
			else {for (int sub = 2; sub < 10; sub += 2) draw_line (dc, lcp, x + markings [ind + sub], y0, y4);}
		}
		break;
	case 10: for (int ind = get_starter (starter, step); ind < sentinel; ind += 10) draw_line (dc, lcp, x + markings [ind], y0, ind == 50 ? y2 : y3); break;
	case 20: for (int ind = get_starter (starter, step); ind < sentinel; ind += 20) draw_line (dc, lcp, x + markings [ind], y0, y2); break;
	case 50: if (starter <= 50) draw_line (dc, lcp, x + markings [50], y0, y2); break;
	default: break;
	}
}

void Scale :: draw_dense_markings (wxDC & dc, double * markings, double x) {
	int step = 1;
	lanczos_colour * lcp = get_bordercolour (markings [0] + 1.0);
	while (step < 10 && absolute (markings [markings_increasing ? 0 : 10] - markings [markings_increasing ? step : 10 - step]) < minimum_step) step = stepProgress (step);
	switch (step) {
	case 1: for (int ind = 1; ind < 10; ind++) draw_line (dc, lcp, x + markings [ind], y0, ind == 5 ? y2 : y3); break;
	case 2: for (int ind = 2; ind < 10; ind += 2) draw_line (dc, lcp, x + markings [ind], y0, y2); break;
	case 5: draw_line (dc, lcp, x + markings [5], y0, y2); break;
	default: break;
	}
}

void Scale :: draw_horizontal_lines (wxDC & dc, double x) {
	int left_x = (int) (x - (double) scale_length * left_horizontal_extension);
	int right_x = (int) (x + (double) scale_length * (1.0 + right_horizontal_extension));
	dc . SetPen (wxPen (reversed ? wxColour (red_marking_colour . red, red_marking_colour . green, red_marking_colour . blue) : wxColour (marking_colour . red, marking_colour . green, marking_colour . blue)));
	if (draw_lines [0]) dc . DrawLine (left_x, y0, right_x, y0);
	if (draw_lines [1]) dc . DrawLine (left_x, y1, right_x, y1);
	if (draw_lines [2]) dc . DrawLine (left_x, y2, right_x, y2);
	if (draw_lines [3]) dc . DrawLine (left_x, y3, right_x, y3);
	if (draw_lines [4]) dc . DrawLine (left_x, y4, right_x, y4);
}

void Scale :: draw (wxDC & dc, double x) {if (inactive) setArialFont (dc);}

void Scale :: scaleInit (void) {}

void Scale :: draw_highlight (wxDC & dc, double x) {
	left_highlight_location = 0.0 - (double) left_highlight_pixel_shift - left_highlight_relative_shift * (double) scale_length;
	right_highlight_location = (double) scale_length + (double) right_highlight_pixel_shift + right_highlight_relative_shift * (double) scale_length;
	if (build_alternate_lanczos_colours) {
		alt_lcp = new lanczos_colour [256];
		initialise_lanczos_colours (alt_lcp, highlight_colour . red, highlight_colour . green, highlight_colour . blue, marking_colour . red, marking_colour . green, marking_colour . blue);
		alt_red_lp = new lanczos_colour [256];
		initialise_lanczos_colours (alt_red_lp, highlight_colour . red, highlight_colour . green, highlight_colour . blue, red_marking_colour . red, red_marking_colour . green, red_marking_colour . blue);
		build_alternate_lanczos_colours = false;
	}
	if (build_individual_marker_lanczos_colour) {
		individual_lcp = lcp = new lanczos_colour [256];
		initialise_lanczos_colours (lcp, background_colour . red, background_colour . green, background_colour . blue, marking_colour . red, marking_colour . green, marking_colour . blue);
		build_individual_marker_lanczos_colour = false;
	}
	if (build_individual_reversed_marker_lanczos_colour) {
		individual_red_lp = red_lp = new lanczos_colour [256];
		initialise_lanczos_colours (red_lp, background_colour . red, background_colour . green, background_colour . blue, red_marking_colour . red, red_marking_colour . green, red_marking_colour . blue);
		build_individual_reversed_marker_lanczos_colour = false;
	}
	if (! highlight) return;
	dc . SetPen (wxPen (wxColour (highlight_colour . red, highlight_colour . green, highlight_colour . blue)));
	dc . SetBrush (wxBrush (wxColour (highlight_colour . red, highlight_colour . green, highlight_colour . blue)));
	dc . DrawRectangle ((int) (x + left_highlight_location), y, (int) (right_highlight_location - left_highlight_location), height);
}

void Scale :: draw_descriptions (wxDC & dc, double x) {
	dc . DrawText (left_text, (int) (x - (double) left_text_pixel_shift - left_text_relative_shift * (double) scale_length) - (left_text_justify_right ? dc . GetTextExtent (left_text) . x : 0), y);
	dc . DrawText (right_text, (int) (x + (double) scale_length + (double) right_text_pixel_shift + right_text_relative_shift * (double) scale_length) - (right_text_justify_right ? dc . GetTextExtent (right_text) . x : 0), y);
	dc . DrawText (middle_text, (int) x + (scale_length - dc . GetTextExtent (middle_text) . x) / 2, y);
}

void Scale :: position_scale (int y, int scale_length, colour markers, colour red_markers, colour rule, lanczos_colour * lcp, lanczos_colour * red_lp) {
	this -> y = y;
	y_base = y0 = y1 = y2 = y3 = y4 = y;
	this -> scale_length = scale_length;
	this -> lcp = lcp;
	this -> red_lp = red_lp;
	this -> marking_colour = markers;

	this -> red_marking_colour = red_markers;
	this -> background_colour = rule;
	left_text = _T (""); right_text = _T (""); middle_text = _T ("");//strcpy (left_text, ""); strcpy (right_text, "");
	left_text_pixel_shift = right_text_pixel_shift = 0;
	left_text_relative_shift = right_text_relative_shift = 0.0;
	left_text_justify_right = right_text_justify_right = false;
}

Scale * Scale :: captureScale (int y) {
	if (! inactive && y >= this -> y && y < this -> y + height) return this;
	if (next == NULL) return NULL;
	return next -> captureScale (y);
}

double Scale :: getValue (double x) {return x;}

double Scale :: getLocation (double x) {return x;}

void Scale :: draw_pi (wxDC & dc, double x) {
	if (no_pi) return;
	draw_line (dc, get_bordercolour (x - 1.0), get_bordercolour (x + 1.0), x, y2, y_base);
	dc . DrawText (_T ("pi"), x + (reversed ? - dc . GetTextExtent (_T ("pi")) . x - 4 : 4), font_y);
}

void Scale :: draw_e (wxDC & dc, double x) {
	if (no_e) return;
	draw_line (dc, get_bordercolour (x - 1.0), get_bordercolour (x + 1.0), x, y2, y_base);
	dc . DrawText (_T ("e"), x + (reversed ? 4 : - dc . GetTextExtent (_T ("e")) . x), font_y);
}

void Scale :: draw_c (wxDC & dc, double x) {
	if (no_c) return;
	draw_line (dc, get_bordercolour (x), x, y2, y_base);
	dc . DrawText (_T ("c"), x + (reversed ? - dc . GetTextExtent (_T ("c")) . x - 4 : 4), font_y);
}

void Scale :: draw_c1 (wxDC & dc, double x) {
	if (no_c1) return;
	draw_line (dc, get_bordercolour (x), x, y2, y_base);
	dc . DrawText (_T ("c1"), x + (reversed ? - dc . GetTextExtent (_T ("c1")) . x - 4 : 4), font_y);
}

void Scale :: draw_degree (wxDC & dc, double x) {
	if (no_degree) return;
	draw_line (dc, get_bordercolour (x), x, y2, y_base);
	dc . DrawText (_T ("q"), x + (reversed ? - dc . GetTextExtent (_T ("q")) . x - 4 : 4), font_y);
}

void Scale :: draw_m (wxDC & dc, double x) {
	if (no_m) return;
	draw_line (dc, get_bordercolour (x), x, y2, y_base);
	dc . DrawText (_T ("M"), x + (reversed ? - dc . GetTextExtent (_T ("M")) . x - 4 : 4), font_y);
}

void Scale :: draw_mm (wxDC & dc, double x) {
	if (no_mm) return;
	draw_thin_line (dc, get_bordercolour (x), x, y2, y_base);
}

void Scale :: drawer (wxDC & dc, double x) {local_x = x; draw (dc, x); draw_horizontal_lines (dc, x);}

Scale :: Scale (int height) {
	local_x = 0;
	this -> height = height;
	this -> next = NULL;
	scale_type = -1;
	minimum_step = 4.0; maximum_step = 100;
	markings_increasing = false;
	inactive = false;
	reversed = false;
	no_pi = no_e = no_c = no_c1 = false;
	no_degree = true;
	no_m = no_mm = true;
	left_text_pixel_shift = right_text_pixel_shift = 0;
	left_text_relative_shift = right_text_relative_shift = 0.0;
	left_extension = right_extension = 0.0;
	left_text_justify_right = right_text_justify_right = false;
//		highlight_vertical_shift = 0;
	highlight = false;
	background_colour . red = 255; background_colour . green = 255; background_colour . blue = 255; background_colour . alpha = 255;
	highlight_colour . red = 255; highlight_colour . green = 255; highlight_colour . blue = 255; highlight_colour . alpha = 255;
	left_highlight_pixel_shift = right_highlight_pixel_shift = 0;
	left_highlight_relative_shift = right_highlight_relative_shift = 0.0;
	left_highlight_location = right_highlight_location = 0.0;
	build_alternate_lanczos_colours = build_individual_marker_lanczos_colour = build_individual_reversed_marker_lanczos_colour = false;
	individual_lcp = individual_red_lp = alt_lcp = alt_red_lp = NULL;
	for (int ind = 0; ind < 16; ind++) {draw_lines [ind] = false; marker_fractions [ind] = 1.0;}
	marker_fractions [0] = 0.5; marker_fractions [1] = 0.4; marker_fractions [2] = 0.3; marker_fractions [3] = 0.2;
	os_compensation = 0;
	left_horizontal_extension = right_horizontal_extension = 0.0;
	y_base = y0 = y1 = y2 = y3 = y4 = 0;
}

Scale :: ~ Scale (void) {
	if (alt_lcp != NULL) delete [] alt_lcp; alt_lcp = NULL;
	if (alt_red_lp != NULL) delete [] alt_red_lp; alt_red_lp = NULL;
	if (individual_lcp != NULL) delete [] individual_lcp; individual_lcp = NULL;
	if (individual_red_lp != NULL) delete [] individual_red_lp; individual_red_lp = NULL;
	if (next != NULL) delete next; next = NULL;
}

