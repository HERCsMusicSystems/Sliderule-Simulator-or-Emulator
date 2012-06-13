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

#define _E        2.71828182845904523536
#define _PI       3.14159265358979323846

void LogBase :: prepare_extensions (double extension_multiplier, int maximum_index) {
	if (reversed) {
		left_extension_index = 0;
		double sentinel = -1.0 - right_extension / extension_multiplier + extension_index_shift;
		sentinel *= (double) scale_length * (- extension_multiplier);
		while (left_extension_index < maximum_index && logs [left_extension_index] > sentinel) left_extension_index++;
		right_extension_index = 0;
		//sentinel = left_extension / extension_multiplier - extension_index_shift;
		sentinel = 1.0 - extension_index_shift - left_extension / extension_multiplier;
		while (sentinel >= 1.0) sentinel -= 1.0;
		sentinel *= (double) scale_length * extension_multiplier;
		while (right_extension_index < maximum_index && logs [right_extension_index] > sentinel) right_extension_index++;
		if (logs [right_extension_index] < sentinel) right_extension_index--;
	} else {
		left_extension_index = 0;
		double sentinel = extension_index_shift - left_extension / extension_multiplier;
		while (sentinel <= 0.0) sentinel += 1.0;
		sentinel *= (double) scale_length * extension_multiplier;
		while (left_extension_index < maximum_index && logs [left_extension_index] < sentinel) left_extension_index++;
		right_extension_index = 0;
		sentinel = extension_index_shift + right_extension / extension_multiplier;
		sentinel *= (double) scale_length * extension_multiplier;
		while (right_extension_index < maximum_index && logs [right_extension_index] < sentinel) right_extension_index++;
		if (logs [right_extension_index] > sentinel) right_extension_index--;
	}
}

void LogBase :: log_init (double multiplier) {
	double extension_multiplier = multiplier;
	multiplier *= (double) scale_length;
	for (int ind = 0; ind < 1000; ind++) {
		logs [ind] = multiplier * log10 (1.0 + (double) ind / 100.0);
	}
	pi_location = multiplier * log10 (_PI);
	e_location = multiplier * log10 (_E);
	c_location = multiplier * log10 (1.0 / sqrt (_PI / 4.0));
	c1_location = c_location + multiplier * 0.5;
	degree_location = multiplier * log10 (_PI / 1.8);
	m_location = multiplier * log10 (100.0 / _PI);
	mm_location = multiplier * log10 (100.0 * _PI / 4.0);
	if (multiplier < 0.0) reversed = true;
	prepare_extensions (extension_multiplier, 1000);
}

void LogBase :: draw_left_log_base (wxDC & dc, double x, int index) {
	int description = 2;
	double location = logs [0];
	for (int ind = 0; ind <= 800; ind += 100) {
		if (index <= 100 + ind) location = draw_markings_for_100 (dc, & logs [ind], wxString :: Format (_T ("%i"), description), x, location, index <= ind ? 0 : index - ind, ind == 800 ? 99 : 100);
		else location = logs [ind];
		description++;
	}
}

void LogBase :: draw_right_log_base (wxDC & dc, double x, int index) {
	int description = 2;
	double location = logs [0];
	for (int ind = 0; ind <= 800; ind += 100) {
		if (ind > index) return;
		location = draw_markings_for_100 (dc, & logs [ind], wxString :: Format (_T ("%i"), description), x, location, 0, index - ind < 100 ? index - ind : 100);
		description++;
	}
}

void LogBase :: draw_both_sides_log_base (wxDC & dc, double x, int left_index, int right_index) {
	int description = 2;
	double location = logs [0];
	for (int ind = 0; ind <= 800; ind += 100) {
		if (ind > right_index) return;
		if (left_index <= 100 + ind) location = draw_markings_for_100 (dc, & logs [ind], wxString :: Format (_T ("%i"), description), x, location, left_index <= ind ? 0 : left_index - ind, right_index - 100 >= ind ? 100 : right_index - ind);
		else location = logs [ind];
		description++;
	}
}

void LogBase :: draw_log_base (wxDC & dc, double x) {
	double location = logs [0];
	for (int ind = 0; ind < 9; ind++) {
		location = draw_markings_for_100 (dc, & logs [ind * 100], ind == 8 ? _T ("1") : wxString :: Format (_T ("%i"), ind + 2), x, location);
	}
}

LogBase :: LogBase (int height) : Scale (height) {left_extension_index = right_extension_index = -1; extension_index_shift = 0.0;}

