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

#include "scale_trig_base.h"

#define _PI       3.14159265358979323846

static int degreeStepProcess (int step) {
	if (step < 2) return 2;
	if (step < 5) return 5;
	if (step < 10) return 10;
	if (step < 20) return 20;
	if (step < 30) return 30;
	if (step < 60) return 60;
	if (step < 120) return 120;
	if (step < 600) return 600;
	return 1200;
};

void TrigBase :: init_locations (double multiplier, bool tangent, int to) {
	int index = 0;
	for (int degree = 0; degree <= to; degree++) {
		double angle = (double) degree / 180.0;
		for (int minute = 0; minute < 60; minute++) {
			double sub_angle = (double) minute / 10800.0;
			double value = _PI * (angle + sub_angle);
			value = tangent ? tan (value) : sin (value);
			if (value > 0.0) value = log10 (value);
			value = (double) scale_length + (double) scale_length * value * multiplier;
			locations [index++] = value;
		}
	}
	locations [index++] = (double) scale_length;
	locations [0] = locations [1] - 100.0;
}

void TrigBase :: init_dec_locations (double multiplier, bool tangent, int to) {
	int index = 0;
	for (int degree = 0; degree <= to; degree++) {
		double angle = (double) degree / 180.0;
		for (int minute = 0; minute < 100; minute++) {
			double sub_angle = (double) minute / 18000.0;
			double value = _PI * (angle + sub_angle);
			value = tangent ? tan (value) : sin (value);
			if (value > 0.0) value = log10 (value);
			value = (double) scale_length + (double) scale_length * value * multiplier;
			locations [index++] = value;
		}
	}
	locations [index++] = (double) scale_length;
	locations [0] = locations [1] - 100.0;
}

/* GJM -- Added _1 versions for S1 and T1 scales */
void TrigBase :: init_locations_1 (double multiplier, bool tangent, int to) {
	int index = 0;
	for (int degree = 0; degree <= to; degree++) {
		double angle = (double) degree / 180.0;
		for (int minute = 0; minute < 60; minute++) {
			double sub_angle = (double) minute / 10800.0;
			double value = _PI * (angle + sub_angle);
			value = tangent ? tan (value) : sin (value);
			if (value > 0.0) value = log10 (value);
			value = (double) scale_length + (double) scale_length + (double) scale_length * value * multiplier;
			locations [index++] = value;
		}
	}
	locations [index++] = (double) scale_length;
	locations [0] = locations [1] - 100.0;
}

void TrigBase :: init_dec_locations_1 (double multiplier, bool tangent, int to) {
	int index = 0;
	for (int degree = 0; degree <= to; degree++) {
		double angle = (double) degree / 180.0;
		for (int minute = 0; minute < 100; minute++) {
			double sub_angle = (double) minute / 18000.0;
			double value = _PI * (angle + sub_angle);
			value = tangent ? tan (value) : sin (value);
			if (value > 0.0) value = log10 (value);
			value = (double) scale_length + (double) scale_length + (double) scale_length * value * multiplier;
			locations [index++] = value;
		}
	}
	locations [index++] = (double) scale_length;
	locations [0] = locations [1] - 100.0;
}

void TrigBase :: init_small_locations (int to) {
	for (int ind = 1; ind <= to; ind++) {
		double angle = (double) ind * _PI / 10800.0;
		locations [ind] = (double) scale_length + (double) scale_length + (double) scale_length * log10 (angle);
	}
	locations [0] = locations [1] - 100.0;
}

void TrigBase :: init_small_dec_locations (int to) {
	for (int ind = 1; ind <= to; ind++) {
		double angle = (double) ind * _PI / 18000.0;
		locations [ind] = (double) scale_length + (double) scale_length + (double) scale_length * log10 (angle);
	}
	locations [0] = locations [1] - 100.0;
}

void TrigBase :: init_sine (double multiplier) {init_locations (multiplier, false, 90);}
void TrigBase :: init_tan (double multiplier) {init_locations (multiplier, true, 89);}
void TrigBase :: init_dec_sine (double multiplier) {init_dec_locations (multiplier, false, 90);}
void TrigBase :: init_dec_tan (double multiplier) {init_dec_locations (multiplier, true, 89);}
/* GJM -- small sine and tangent inits for S1 / T1 scales */
void TrigBase :: init_small_sine (double multiplier) {init_locations_1 (multiplier, false, 90);}
void TrigBase :: init_small_tan (double multiplier) {init_locations_1 (multiplier, true, 89);}
void TrigBase :: init_small_dec_sine (double multiplier) {init_dec_locations_1 (multiplier, false, 90);}
void TrigBase :: init_small_dec_tan (double multiplier) {init_dec_locations_1 (multiplier, true, 89);}

double TrigBase :: drawing_footer (wxDC & dc, double * markings, int description, double x, double last_drawn, int to, bool next_extent) {
	double distance = absolute (markings [to] - last_drawn);
	if (distance < minimum_step) return last_drawn;
	double location = x + markings [to];
	draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y1);
	wxString text = wxString :: Format (_T ("%i"), description);
	wxSize extent = dc . GetTextExtent (text);
	if (distance < (double) extent . x || next_extent) return markings [to];
	dc . DrawText (text, location - extent . x / 2, font_y);
	return markings [to];
}

double TrigBase :: draw_markings_for_1 (wxDC & dc, double * markings, int description, double x, double last_drawn, int from, int to) {
	if (from > to) return last_drawn;
	bool to_changed = false;
	if (to < 60) {to++; to_changed = true;}
	int step = 1;
	int index, counter; bool half;
	lanczos_colour * lp = get_bordercolour (x + markings [from] + 1.0);
	while (step < 60 && smaller (markings [to] - markings [to - step], markings [from + step] - markings [from]) < minimum_step) step = degreeStepProcess (step);
	switch (step) {
	case 60: index = 60; break;
	case 30: index = 30; if (from <= 30 && to >= 30) draw_line (dc, lp, x + markings [30], y0, y2); break;
	case 20:
		index = 20;
		while (index < from && index < to) index += 20;
		while (index < to) {draw_line (dc, lp, x + markings [index], y0, y2); index += 20;}
		break;
	case 10:
		index = 10;
		while (index < from && index < to) index += 10;
		while (index < to) {draw_line (dc, lp, x + markings [index], y0, index == 30 ? y2 : y3); index += 10;}
		break;
	case 5:
		index = 5; half = true;
		while (index < from && index < to) {index += 5; half = ! half;}
		while (index < to) {
			draw_line (dc, lp, x + markings [index], y0, half ? y4 : index == 30 ? y2 : y3);
			index += 5; half = ! half;
		}
		break;
	case 2:
		index = 2; counter = 4;
		while (index < from && index < to) {index += 2; if (--counter < 0) counter = 4;}
		while (index < to) {
			draw_line (dc, lp, x + markings [index], y0, index == 30 ? y2 : counter < 1 ? y3 : y4);
			index += 2;
			if (--counter < 0) counter = 4;
		}
		break;
	case 1:
		index = 1; counter = 9;
		while (index < from && index < to) {index++; if (--counter < 0) counter = 9;}
		while (index < to) {
			draw_line (dc, lp, x + markings [index], y0, index == 30 ? y1 : counter < 1 ? y2 : counter == 5 ? y3 : y4);
			index++;
			if (--counter < 0) counter = 9;
		}
		break;
	default: break;
	}
	if (to_changed) to--;
	if (to < 60) return markings [index];
	return drawing_footer (dc, markings, description, x, last_drawn, to, false);
}

double TrigBase :: draw_markings_for_5 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from, int to) {
	if (from > to) return last_drawn;
	double distance = smaller (markings [to] - markings [to - 60], markings [from + 60] - markings [from]);
	wxString text5 = _T ("00");
	wxSize extent5 = dc . GetTextExtent (text5);
	if (distance > minimum_step) {
		if (distance > (double) extent5 . x) {
			last_drawn = draw_markings_for_1 (dc, markings, base_description + 1, x, last_drawn, from, to > 60 ? 60 : to);
			last_drawn = draw_markings_for_1 (dc, & markings [60], base_description + 2, x, last_drawn, from > 60 ? from - 60 : 0, to > 120 ? 60 : to - 60);
			last_drawn = draw_markings_for_1 (dc, & markings [120], base_description + 3, x, last_drawn, from > 120 ? from - 120 : 0, to > 180 ? 60 : to - 120);
			last_drawn = draw_markings_for_1 (dc, & markings [180], base_description + 4, x, last_drawn, from > 180 ? from - 180 : 0, to > 240 ? 60 : to - 120);
			last_drawn = draw_markings_for_1 (dc, & markings [240], base_description + 5, x, last_drawn, from > 240 ? from - 240 : 0, to - 240);
			return last_drawn;
		}
		lanczos_colour * lp = get_bordercolour (x + markings [from] + 1.0);
		for (int ind = 60; ind < 300; ind += 60) {
			if (from <= ind && ind <= to) draw_line (dc, lp, x + markings [ind], y0, y2);
		}
	}
	if (to < 300) return last_drawn;
	return drawing_footer (dc, markings, base_description + 5, x, last_drawn, to, extent);
}

double TrigBase :: draw_markings_for_dec_5 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from, int to) {
	if (from > to) return last_drawn;
	double distance = smaller (markings [to] - markings [to - 100], markings [from + 100] - markings [from]);
	wxString text5 = base_description >= 95 ? _T ("000") : _T ("00");
	wxSize extent5 = dc . GetTextExtent (text5);
	if (distance > minimum_step) {
		if (distance > (double) extent5 . x) {
			if (from <= 100) last_drawn = draw_markings_for_100 (dc, markings, wxString :: Format (_T ("%i"), base_description + 1), x, last_drawn, from, to > 100 ? 100 : to);
			if (from <= 200) last_drawn = draw_markings_for_100 (dc, & markings [100], wxString :: Format (_T ("%i"), base_description + 2), x, last_drawn, from > 100 ? from - 100 : 0, to > 200 ? 100 : to - 100);
			if (from <= 300) last_drawn = draw_markings_for_100 (dc, & markings [200], wxString :: Format (_T ("%i"), base_description + 3), x, last_drawn, from > 200 ? from - 200 : 0, to > 300 ? 100 : to - 200);
			if (from <= 400) last_drawn = draw_markings_for_100 (dc, & markings [300], wxString :: Format (_T ("%i"), base_description + 4), x, last_drawn, from > 300 ? from - 300 : 0, to > 400 ? 100 : to - 300);
			if (from <= 500) last_drawn = draw_markings_for_100 (dc, & markings [400], wxString :: Format (_T ("%i"), base_description + 5), x, last_drawn, from > 400 ? from - 400 : 0, to - 400);
			return last_drawn;
		}
		lanczos_colour * lp = get_bordercolour (x + markings [from] + 1.0);
		for (int ind = 100; ind < 500; ind += 100) {
			if (from <= ind && ind <= to) draw_line (dc, lp, x + markings [ind], y0, y2);
		}
	}
	if (to < 500) return last_drawn;
	return drawing_footer (dc, markings, base_description + 5, x, last_drawn, to, extent);
}

double TrigBase :: draw_markings_for_10 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from, int to) {
	double distance = smaller (markings [to] - markings [to - 300], markings [from + 300] - markings [from]);
	if (distance > minimum_step) {
		last_drawn = draw_markings_for_5 (dc, markings, base_description, x, last_drawn, extent, from, to > 300 ? 300 : to);
		return draw_markings_for_5 (dc, & markings [300], base_description + 5, x, last_drawn, extent, from > 300 ? from - 300 : 0, to - 300);
	}
	if (to < 600) return last_drawn;
	return drawing_footer (dc, markings, base_description + 10, x, last_drawn, 600, extent);
}

double TrigBase :: draw_markings_for_dec_10 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from, int to) {
	double distance = smaller (markings [to] - markings [to - 500], markings [from + 500] - markings [from]);
	if (distance > minimum_step) {
		last_drawn = draw_markings_for_dec_5 (dc, markings, base_description, x, last_drawn, extent, from, to > 500 ? 500 : to);
		return draw_markings_for_dec_5 (dc, & markings [500], base_description + 5, x, last_drawn, extent, from > 500 ? from - 500 : 0, to - 500);
	}
	if (to < 1000) return last_drawn;
	return drawing_footer (dc, markings, base_description + 10, x, last_drawn, 1000, extent);
}

int TrigBase :: find_left_trig_index (double x) {
	for (int ind = 1; ind < 6000; ind++) {if (locations [ind] >= x) return ind;}
	return 0;
}

int TrigBase :: find_right_trig_index (double x) {
	for (int ind = 1; ind < 6000; ind++) {if (locations [ind] > x) return ind - 1;}
	return 6000;
}

int TrigBase :: find_left_dec_trig_index (double x) {
	for (int ind = 1; ind < 10000; ind++) {if (locations [ind] >= x) return ind;}
	return 0;
}

int TrigBase :: find_right_dec_trig_index (double x) {
	for (int ind = 1; ind < 10000; ind++) {if (locations [ind] > x) return ind - 1;}
	return 10000;
}

TrigBase :: TrigBase (int height) : Scale (height) {}


