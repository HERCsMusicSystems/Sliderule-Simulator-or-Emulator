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

#include "config.h"

void Config :: save (void) {
	FILE * tc = fopen ("config.txt", "wt");
	fprintf (tc, "config [\n");
	if (number_of_slide_rules != 24) fprintf (tc, "	number_of_slide_rules [%i]\n", number_of_slide_rules);
	if (readouts_on_stator_left) fprintf (tc, "	readouts_on_stator_left []\n");
	if (readouts_on_stator_right) fprintf (tc, "	readouts_on_stator_right []\n");
	if (readouts_on_slide_left) fprintf (tc, "	readouts_on_slide_left []\n");
	if (readouts_on_slide_right) fprintf (tc, "	readouts_on_slide_right []\n");
	if (readouts_on_main_hairline) fprintf (tc, "	readouts_on_main_hairline []\n");
	if (readouts_on_multi_hairlines) fprintf (tc, "	readouts_on_multi_hairlines []\n");
	if (readouts_decimal_places >= 0) fprintf (tc, "	readouts_decimal_places [%i]\n", readouts_decimal_places);
	fprintf (tc, "	length [%i]\n", length);
	fprintf (tc, "	motion_multiplier [%.02f]\n", motion_multiplier);
	if (animation_off) fprintf (tc, "	animation_off []\n");
	if (animation_linear) fprintf (tc, "	animation_linear []\n");
	if (animation_variable_speed) fprintf (tc, "	animation_variable_speed []\n");
	fprintf (tc, "	animation_steps [%i]\n", animation_steps);
	fprintf (tc, "	animation_delays [%i]\n", animation_delays);
	for (unsigned int ind = 0; ind < strlen (last_loaded); ind++) {if (last_loaded [ind] == '\\') last_loaded [ind] = '/';}
	fprintf (tc, "	last_loaded [\"%s\"]\n", last_loaded);
	if (length_override) fprintf (tc, "	length_override []\n");
	if (motion_override) fprintf (tc, "	motion_override []\n");
	if (animation_override) fprintf (tc, "	animation_override []\n");
	if (menu_inactive) fprintf (tc, "	menu_inactive []\n");
	if (donation_made) fprintf (tc, "	%s []\n", DONATION_MADE);
	else fprintf (tc, "	d_config [%i]\n", donation_launch_counter);
	fprintf (tc, "]\n");
	printf ("config saved\n");
}

Config :: Config (void) {
	number_of_slide_rules = 24;
	readouts_on_stator_left = false;
	readouts_on_stator_right = false;
	readouts_on_slide_left = false;
	readouts_on_slide_right = false;
	readouts_on_main_hairline = false;
	readouts_on_multi_hairlines = false;
	readouts_decimal_places = -1;
	length = 900;
	strcpy (last_loaded, "");
	length_override = false;
	motion_multiplier = 0.2;
	motion_override = false;
	animation_off = false;
	animation_linear = false;
	animation_variable_speed = false;
	animation_steps = 100;
	animation_delays = 50;
	animation_override = false;
	no_automatic_save = false;
	menu_inactive = false;
	donation_made = false;
	donation_launch_counter = DONATION_FREQUENCY;
	changed = false;
	SetupFileReader fr ("config.txt");
	if (fr . file_not_found ()) {changed = true; return;}
	if (! fr . get_id ("config")) return;
	while (fr . get_id ()) {
		if (fr . id ("number_of_slide_rules")) {if (! fr . get_int ()) return; number_of_slide_rules = fr . int_symbol;}
		if (fr . id ("readouts_on_stator_left")) readouts_on_stator_left = true;
		if (fr . id ("readouts_on_stator_right")) readouts_on_stator_right = true;
		if (fr . id ("readouts_on_slide_left")) readouts_on_slide_left = true;
		if (fr . id ("readouts_on_slide_right")) readouts_on_slide_right = true;
		if (fr . id ("readouts_on_main_hairline")) readouts_on_main_hairline = true;
		if (fr . id ("readouts_on_multi_hairlines")) readouts_on_multi_hairlines = true;
		if (fr . id ("readouts_decimal_places")) {if (! fr . get_int ()) return; readouts_decimal_places = fr . int_symbol;}
		if (fr . id ("length")) {if (! fr . get_int ()) return; length = fr . int_symbol;}
		if (fr . id ("last_loaded")) {if (! fr . get_string ()) return; strcpy (last_loaded, fr . symbol);}
		if (fr . id ("motion_multiplier")) {if (! fr . get_float ()) return; motion_multiplier = fr . float_symbol;}
		if (fr . id ("animation_off")) animation_off = true;
		if (fr . id ("animation_linear")) animation_linear = true;
		if (fr . id ("animation_variable_speed")) animation_variable_speed = true;
		if (fr . id ("animation_steps")) {if (! fr . get_int ()) return; animation_steps = fr . int_symbol;}
		if (fr . id ("animation_delays")) {if (! fr . get_int ()) return; animation_delays = fr . int_symbol;}
		if (fr . id ("length_override")) length_override = true;
		if (fr . id ("motion_override")) motion_override = true;
		if (fr . id ("animation_override")) animation_override = true;
		if (fr . id ("no_automatic_save")) no_automatic_save = true;
		if (fr . id ("menu_inactive")) menu_inactive = true;
		if (fr . id (DONATION_MADE)) {donation_made = true; donation_launch_counter = DONATION_FREQUENCY;}
		if (fr . id ("d_config")) {
				if (donation_made) continue;
				if (! fr . get_int ()) return;
				donation_launch_counter = fr . int_symbol - 1;
				if (donation_launch_counter > DONATION_FREQUENCY) donation_launch_counter = DONATION_FREQUENCY;
		}
		fr . skip ();
	}
	if (! donation_made) changed = true;
}

Config :: ~ Config (void) {
	if (no_automatic_save) return;
	if (! changed) return;
	save ();
}


