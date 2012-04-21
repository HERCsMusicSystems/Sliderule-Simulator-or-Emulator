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

#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "wx/filedlg.h"
#include "wx/tooltip.h"
#include "wx/html/helpctrl.h"
#include "wx/dnd.h"

#define _E        2.71828182845904523536
#define _PI       3.14159265358979323846

#define DONATION_MADE "I_made_my_donation"
#define DONATION_FREQUENCY 19
#define USE_PACKED_COLOURS

#define SYMBOL_SIZE 1000
class SetupFileReader {
public:
	int symbol_control;
		// 0: EOF
		// 1: [
		// 2: ]
		// 3: identifier
		// 4: string
		// 5: integer
		// 6: float
		// 7: unknown
	int int_symbol;
	double float_symbol;
	char symbol [SYMBOL_SIZE];
	FILE * setup_file;
	int act_char;
	SetupFileReader (char * file_name) {
		setup_file = NULL;
		setup_file = fopen (file_name, "rb");
		act_char = 0;
	}
	~ SetupFileReader (void) {close ();}
	void close (void) {
		if (setup_file == NULL) return;
		fclose (setup_file);
		setup_file = NULL;
	}
	bool file_not_found (void) {
		if (setup_file == NULL) return true;
		return false;
	}
	int get_char (void) {
		act_char = fgetc (setup_file);
		return act_char;
	}
	void get_symbol (void) {
		bool negative = false;
		char * symbol_pointer;
		double fraction;
		if (setup_file == NULL) {
			symbol_control = 0;
			return;
		}
		while (act_char < 33) {
			if (act_char == EOF) {
				symbol_control = 0;
				close ();
				return;
			}
			get_char ();
		}
		if (act_char == '[') {
			symbol_control = 1;
			get_char ();
			return;
		}
		if (act_char == ']') {
			symbol_control = 2;
			get_char ();
			return;
		}
		if ((act_char >= 'a' && act_char <= 'z') || act_char == '_' || (act_char >= 'A' && act_char <= 'Z')) {
			symbol_pointer = symbol;
			while ((act_char >= 'a' && act_char <= 'z') || act_char == '_' || (act_char >= 'A' && act_char <= 'Z') || (act_char >= '0' && act_char <= '9')) {
				* (symbol_pointer++) = act_char;
				get_char ();
			}
			symbol_control = 3;
			* symbol_pointer = '\0';
			return;
		}
		if (act_char == '-') {
			negative = true;
			get_char ();
		}
		if (act_char >= '0' && act_char <= '9') {
			int_symbol = act_char - '0';
			get_char ();
			while (act_char >= '0' && act_char <= '9') {
				int_symbol *= 10;
				int_symbol += act_char - '0';
				get_char ();
			}
			if (act_char == 'e') {
				if (negative) int_symbol = - int_symbol;
				negative = false;
				float_symbol = int_symbol;
				symbol_control = 6;
				int shift = 0;
				get_char ();
				if (act_char == '-') {negative = true; get_char ();}
				while (act_char >= '0' && act_char <= '9') {
					shift *= 10;
					shift += act_char - '0';
					get_char ();
				}
				while (shift > 0) {
					if (negative) float_symbol *= 0.1;
					else float_symbol *= 10.0;
					shift--;
				}
				return;
			}
			if (act_char == '.') {
				get_char ();
				float_symbol = int_symbol;
				symbol_control = 6;
				fraction = 1.0 / 10.0;
				int_symbol = 0;
				while (act_char >= '0' && act_char <= '9') {
					float_symbol += ((double) (act_char - '0')) * fraction;
					fraction /= 10.0;
					get_char ();
				}
				if (negative) float_symbol = - float_symbol;
				if (act_char == 'e') {
					int shift = 0;
					get_char ();
					negative = false;
					if (act_char == '-') {negative = true; get_char ();}
					while (act_char >= '0' && act_char <= '9') {
						shift *= 10;
						shift += act_char - '0';
						get_char ();
					}
					while (shift > 0) {
						if (negative) float_symbol *= 0.1;
						else float_symbol *= 10.0;
						shift--;
					}
				}
				return;
			}
			symbol_control = 5;
			if (negative) int_symbol = 0 - int_symbol;
			float_symbol = (double) int_symbol;
			return;
		}
		if (negative) {
			symbol_control = 7;
			return;
		}
		if (act_char == '"') {
			get_char ();
			symbol_pointer = symbol;
			while (act_char > 0 && act_char != '"') {
				if (act_char == '\\')
					get_char ();
				* (symbol_pointer++) = act_char;
				get_char ();
			}
			* symbol_pointer = '\0';
			if (act_char <= 0) {
				symbol_control = 0;
				return;
			}
			symbol_control = 4;
			get_char ();
			return;
		}
		get_char ();
		symbol_control = 7;
	}
	void skip (void) {
		int ind = 1;
		while (ind > 0) {
			get_symbol ();
			switch (symbol_control) {
			case 0: ind = 0; break;
			case 1: ind++; break;
			case 2: ind--; break;
			default: break;
			}
		}
	}
	bool id (char * name) {
		if (symbol_control != 1) return false;
		return strcmp (symbol, name) == 0;
	}
	bool get_string (void) {
		get_symbol ();
		return symbol_control == 4;
	}
	bool get_int (void) {
		get_symbol ();
		return symbol_control == 5;
	}
	bool get_float (void) {
		get_symbol ();
		return symbol_control == 6;
	}
	bool get_id (void) {
		get_symbol ();
		if (symbol_control != 3) return false;
		get_symbol ();
		return symbol_control == 1;
	}
	bool get_id (char * name) {
		get_symbol ();
		if (symbol_control != 3 || strcmp (symbol, name) != 0) return false;
		get_symbol ();
		return symbol_control == 1;
	}
};

class Config {
public:
	int number_of_slide_rules;
	bool readouts_on_stator_left;
	bool readouts_on_stator_right;
	bool readouts_on_slide_left;
	bool readouts_on_slide_right;
	bool readouts_on_main_hairline;
	bool readouts_on_multi_hairlines;
	int readouts_decimal_places;
	int length;
	char last_loaded [1024];
	bool length_override;
	double motion_multiplier;
	bool motion_override;
	bool animation_off;
	bool animation_linear;
	bool animation_variable_speed;
	int animation_steps;
	int animation_delays;
	bool animation_override;
	bool menu_inactive;
	bool donation_made;
	int donation_launch_counter;
	bool no_automatic_save;
	bool changed;
	void save (void) {
		FILE * tc = fopen ("config.txt", "wt");
		fprintf (tc, "config [\n");
		fprintf (tc, "	number_of_slide_rules [%i]\n", number_of_slide_rules);
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
	Config (void) {
		number_of_slide_rules = 3;
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
	~ Config (void) {
		if (no_automatic_save) return;
		if (! changed) return;
		save ();
	}
};

static Config * config = NULL;

static double sinc (double x) {if (x == 0.0) return 1.0; x *= _PI; return sin (x) / x;}

static double lanczos3 (double x) {
	if (x < -3.0) return 0.0;
	if (x > 3.0) return 0.0;
	return sinc (x) * sinc (x / 3.0);
}

struct colour {unsigned char red, green, blue, alpha;};

struct lanczos_colour {
	unsigned char rm2, gm2, bm2, am2;
	unsigned char rm1, gm1, bm1, am1;
	unsigned char r0, g0, b0, a0;
	unsigned char r1, g1, b1, a1;
	unsigned char r2, g2, b2, a2;
	unsigned char r3, g3, b3, a3;
};

struct lanczos_packed_colour {unsigned long int cm2, cm1, c0, c1, c2, c3;};

#define CLBANG(c, c1, c2, lx)\
	colour = (int) (0.4999 + c2 * lx + c1 - c1 * lx);\
	colours [ind] . c = (unsigned char) (colour < 0 ? 0 : (colour > 255 ? 255 : colour));
static void initialise_lanczos_colours (lanczos_colour * colours, double r1, double g1, double b1, double r2, double g2, double b2) {
	for (int ind = 0; ind < 256; ind++) {
		double shift = (double) ind / 256.0;
		double lm2 = lanczos3 (-2.0 - shift);
		double lm1 = lanczos3 (-1.0 - shift);
		double l0 = lanczos3 (-shift);
		double l1 = lanczos3 (1.0 - shift);
		double l2 = lanczos3 (2.0 - shift);
		double l3 = lanczos3 (3.0 - shift);
		int colour;
		CLBANG(rm2, r1, r2, lm2); CLBANG(gm2, g1, g2, lm2); CLBANG(bm2, b1, b2, lm2); CLBANG(am2, 0, 255, lm2); //colours [ind] . am2 = lm2;
		CLBANG(rm1, r1, r2, lm1); CLBANG(gm1, g1, g2, lm1); CLBANG(bm1, b1, b2, lm1); CLBANG(am1, 0, 255, lm1); //colours [ind] . am1 = lm1;
		CLBANG(r0, r1, r2, l0); CLBANG(g0, g1, g2, l0); CLBANG(b0, b1, b2, l0); CLBANG(a0, 0, 255, l0); //colours [ind] . a0 = l0;
		CLBANG(r1, r1, r2, l1); CLBANG(g1, g1, g2, l1); CLBANG(b1, b1, b2, l1); CLBANG(a1, 0, 255, l1); //colours [ind] . a1 = l1;
		CLBANG(r2, r1, r2, l2); CLBANG(g2, g1, g2, l2); CLBANG(b2, b1, b2, l2); CLBANG(a2, 0, 255, l2); //colours [ind] . a2 = l2;
		CLBANG(r3, r1, r2, l3); CLBANG(g3, g1, g2, l3); CLBANG(b3, b1, b2, l3); CLBANG(a3, 0, 255, l3); //colours [ind] . a3 = l3;
	}
}

#ifdef USE_PACKED_COLOURS
#define RKBANG(c)\
	dc . SetPen (wxPen (wxColour (lpc -> c)));\
	dc . DrawLine (base_x, y1, base_x, y2); base_x++;
void draw_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	colour_table += (int) (256.0 * (x - (double) base_x));
	lanczos_packed_colour * lpc = (lanczos_packed_colour *) colour_table;
	base_x -= 2;
	if (base_x < 0) return;
	RKBANG(cm2);
	RKBANG(cm1);
	RKBANG(c0);
	RKBANG(c1);
	RKBANG(c2);
	RKBANG(c3);
}
void draw_line (wxDC & dc, lanczos_colour * left_colour_table, lanczos_colour * right_colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	int shift = (int) (256.0 * (x - (double) base_x));
	left_colour_table += shift;
	lanczos_packed_colour * lpc = (lanczos_packed_colour *) left_colour_table;
	base_x -= 2;
	if (base_x < 0) return;
	RKBANG(cm2);
	RKBANG(cm1);
	RKBANG(c0);
	right_colour_table += shift;
	lpc = (lanczos_packed_colour *) right_colour_table;
	RKBANG(c1);
	RKBANG(c2);
	RKBANG(c3);
}
void draw_thin_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	int offset = (int) (256.0 * (x - (double) base_x));
	colour_table += offset;
	lanczos_packed_colour * lpc = (lanczos_packed_colour *) colour_table;
//	base_x -= 2;
	if (base_x < 0) return;
//	RKBANG(rm2, gm2, bm2);
//	RKBANG(rm1, gm1, bm1);
	RKBANG(c0);
	if (offset == 0) return;
	RKBANG(c1);
//	RKBANG(r2, g2, b2);
//	RKBANG(r3, g3, b3);
}
#else
#define RKBANG(r, g, b)\
	dc . SetPen (wxPen (wxColour (colour_table -> r, colour_table -> g, colour_table -> b)));\
	dc . DrawLine (base_x, y1, base_x, y2); base_x++;
void draw_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	colour_table += (int) (256.0 * (x - (double) base_x));
	base_x -= 2;
	if (base_x < 0) return;
	RKBANG(rm2, gm2, bm2);
	RKBANG(rm1, gm1, bm1);
	RKBANG(r0, g0, b0);
	RKBANG(r1, g1, b1);
	RKBANG(r2, g2, b2);
	RKBANG(r3, g3, b3);
}
void draw_line (wxDC & dc, lanczos_colour * left_colour_table, lanczos_colour * right_colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	int shift = (int) (256.0 * (x - (double) base_x));
	lanczos_colour * colour_table = left_colour_table + shift;
	base_x -= 2;
	if (base_x < 0) return;
	RKBANG(rm2, gm2, bm2);
	RKBANG(rm1, gm1, bm1);
	RKBANG(r0, g0, b0);
	colour_table = right_colour_table + shift;
	RKBANG(r1, g1, b1);
	RKBANG(r2, g2, b2);
	RKBANG(r3, g3, b3);
}
void draw_thin_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	int offset = (int) (256.0 * (x - (double) base_x));
	colour_table += offset;
//	base_x -= 2;
	if (base_x < 0) return;
//	RKBANG(rm2, gm2, bm2);
//	RKBANG(rm1, gm1, bm1);
	RKBANG(r0, g0, b0);
	if (offset == 0) return;
	RKBANG(r1, g1, b1);
//	RKBANG(r2, g2, b2);
//	RKBANG(r3, g3, b3);
}
#endif


/*static void blend (wxColour * c, double r, double g, double b, double a, double am) {
	r = r * a + (double) c -> Red () * am;
	g = g * a + (double) c -> Green () * am;
	b = b * a + (double) c -> Blue () * am;
	if (r < 0.0) r = 0.0; if (r > 255.0) r = 255.0;
	if (g < 0.0) g = 0.0; if (g > 255.0) g = 255.0;
	if (b < 0.0) b = 0.0; if (b > 255.0) b = 255.0;
	c -> Set ((unsigned char) r, (unsigned char) g, (unsigned char) b);
}

void draw_alpha_line (wxDC & dc, lanczos_colour * colour_table, double x, int y1, int y2) {
	int base_x = (int) x;
	colour_table += (int) (256.0 * (x - (double) base_x));
	base_x -= 2;
	if (base_x < 0) return;
	for (int ind = y1; ind <= y2; ind++) {
		wxColour c;
		dc . GetPixel (wxPoint (base_x + 0, ind), & c);
		blend (& c, colour_table -> rm2, colour_table -> gm2, colour_table -> bm2, colour_table -> am2, 1.0 - colour_table -> am2);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 0, ind);
		//
		double am1 = colour_table -> am1;
		dc . GetPixel (wxPoint (base_x + 1, ind), & c);
		blend (& c, colour_table -> rm1, colour_table -> gm1, colour_table -> bm1, colour_table -> am1, 1.0 - colour_table -> am1);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 1, ind);
		//
		dc . GetPixel (wxPoint (base_x + 2, ind), & c);
		blend (& c, colour_table -> r0, colour_table -> g0, colour_table -> b0, colour_table -> a0, 1.0 - colour_table -> a0);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 2, ind);
		//
		dc . GetPixel (wxPoint (base_x + 3, ind), & c);
		blend (& c, colour_table -> r1, colour_table -> g1, colour_table -> b1, colour_table -> a1, 1.0 - colour_table -> a1);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 3, ind);
		//
		dc . GetPixel (wxPoint (base_x + 4, ind), & c);
		blend (& c, colour_table -> r2, colour_table -> g2, colour_table -> b2, colour_table -> a2, 1.0 - colour_table -> a2);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 4, ind);
		//
		dc . GetPixel (wxPoint (base_x + 5, ind), & c);
		blend (& c, colour_table -> r3, colour_table -> g3, colour_table -> b3, colour_table -> a3, 1.0 - colour_table -> a3);
		dc . SetPen (wxPen (c));
		dc . DrawPoint (base_x + 5, ind);
		//
	}
}*/

static double absolute (double x) {return x >= 0.0 ? x : - x;}
static double smaller (double x, double y) {x = absolute (x); y = absolute (y); return x < y ? x : y;}

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

static int find_left_index (double * markers, int from, int to, double threshold) {
	for (int ind = from; ind <= to; ind++) {if (markers [ind] >= threshold) return ind;}
	return to;
}

static int find_right_index (double * markers, int from, int to, double threshold) {
	for (int ind = to; ind >= from; ind--) {if (markers [ind] <= threshold) return ind;}
	return from;
}

static double dec_correction (double value) {
	double remainder = value - (double) ((int) value);
	if (remainder > 0.99999999) return value;
	remainder *= 6.0 / 10.0;
	return (double) ((int) value) + remainder;
}

static double dec_recorrection (double value) {
	double remainder = value - (double) ((int) value);
	remainder *= 10.0 / 6.0;
	return (double) ((int) value) + remainder;
}

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
	void faceUp (int h1, int h2, int h3, int h4) {
		y_base = y; y0 = y + height - 1 + os_compensation; y1 = y0 - h1; y2 = y0 - h2; y3 = y0 - h3; y4 = y0 - h4; font_y = y - height / 8;
	}
	void faceDown (int h1, int h2, int h3, int h4) {
		y_base = y + height - 1 + os_compensation; y0 = y; y1 = y0 + h1; y2 = y0 + h2; y3 = y0 + h3; y4 = y0 + h4; font_y = y - height / 8 + height / 2;
	}
public:
	void faceUp (void) {
		faceUp (height * marker_fractions [0], height * marker_fractions [1], height * marker_fractions [2], height * marker_fractions [3]);
	}
	void faceDown (void) {
		faceDown (height * marker_fractions [0], height * marker_fractions [1], height * marker_fractions [2], height * marker_fractions [3]);
	}
	void setArialFont (wxDC & dc) {
		wxFont f = dc . GetFont ();
		f . SetFaceName (_T ("arial"));
		int point_size = inactive ? height * 3 / 4: height / 2;
		if (point_size < 6) point_size = 6;
		f . SetPointSize (point_size);
		dc . SetFont (f);
		dc . SetTextForeground (reversed ? wxColour (red_lp -> r0, red_lp -> g0, red_lp -> b0) : wxColour (lcp -> r0, lcp -> g0, lcp -> b0));
	}
	lanczos_colour * get_bordercolour (double x) {
		x -= local_x;
		if (highlight) {
			if (x < left_highlight_location || x > right_highlight_location) return reversed ? red_lp : lcp;
			return reversed ? alt_red_lp : alt_lcp;
		}
		return reversed ? red_lp : lcp;
	}
	void draw_text_marker (wxDC & dc, wxString text, double x) {dc . DrawText (text, x - dc . GetTextExtent (text) . x / 2, font_y);}
	void draw_index_location (wxDC & dc, double x, wxString description = _T ("1")) {
		draw_line (dc, get_bordercolour (x - 1.0), get_bordercolour (x + 1.0), x, y0, y1);
		draw_text_marker (dc, description, x);
	}
	// starters allowed: 10, 50
	double draw_markings_for_100 (wxDC & dc, double * markings, wxString description, double x, double last_drawn, int from = 0, int to = 100) {
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
	void draw_markings (wxDC & dc, double * markings, double x, int starter = 0, int sentinel = 100) {
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
	void draw_dense_markings (wxDC & dc, double * markings, double x) {
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
	virtual void draw_horizontal_lines (wxDC & dc, double x) {
		int left_x = (int) (x - (double) scale_length * left_horizontal_extension);
		int right_x = (int) (x + (double) scale_length * (1.0 + right_horizontal_extension));
		dc . SetPen (wxPen (reversed ? wxColour (red_marking_colour . red, red_marking_colour . green, red_marking_colour . blue) : wxColour (marking_colour . red, marking_colour . green, marking_colour . blue)));
		if (draw_lines [0]) dc . DrawLine (left_x, y0, right_x, y0);
		if (draw_lines [1]) dc . DrawLine (left_x, y1, right_x, y1);
		if (draw_lines [2]) dc . DrawLine (left_x, y2, right_x, y2);
		if (draw_lines [3]) dc . DrawLine (left_x, y3, right_x, y3);
		if (draw_lines [4]) dc . DrawLine (left_x, y4, right_x, y4);
	}
	virtual void draw (wxDC & dc, double x) {if (inactive) setArialFont (dc);}
	virtual void scaleInit (void) {}
	void draw_highlight (wxDC & dc, double x) {
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
	void draw_descriptions (wxDC & dc, double x) {
		dc . DrawText (left_text, (int) (x - (double) left_text_pixel_shift - left_text_relative_shift * (double) scale_length) - (left_text_justify_right ? dc . GetTextExtent (left_text) . x : 0), y);
		dc . DrawText (right_text, (int) (x + (double) scale_length + (double) right_text_pixel_shift + right_text_relative_shift * (double) scale_length) - (right_text_justify_right ? dc . GetTextExtent (right_text) . x : 0), y);
		dc . DrawText (middle_text, (int) x + (scale_length - dc . GetTextExtent (middle_text) . x) / 2, y);
	}
	void position_scale (int y, int scale_length, colour markers, colour red_markers, colour rule, lanczos_colour * lcp, lanczos_colour * red_lp) {
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
	Scale * captureScale (int y) {
		if (! inactive && y >= this -> y && y < this -> y + height) return this;
		if (next == NULL) return NULL;
		return next -> captureScale (y);
	}
	virtual double getValue (double x) {return x;}
	virtual double getLocation (double x) {return x;}
	void draw_pi (wxDC & dc, double x) {
		if (no_pi) return;
		draw_line (dc, get_bordercolour (x - 1.0), get_bordercolour (x + 1.0), x, y2, y_base);
		dc . DrawText (_T ("pi"), x + (reversed ? - dc . GetTextExtent (_T ("pi")) . x - 4 : 4), font_y);
	}
	void draw_e (wxDC & dc, double x) {
		if (no_e) return;
		draw_line (dc, get_bordercolour (x - 1.0), get_bordercolour (x + 1.0), x, y2, y_base);
		dc . DrawText (_T ("e"), x + (reversed ? 4 : - dc . GetTextExtent (_T ("e")) . x), font_y);
	}
	void draw_c (wxDC & dc, double x) {
		if (no_c) return;
		draw_line (dc, get_bordercolour (x), x, y2, y_base);
		dc . DrawText (_T ("c"), x + (reversed ? - dc . GetTextExtent (_T ("c")) . x - 4 : 4), font_y);
	}
	void draw_c1 (wxDC & dc, double x) {
		if (no_c1) return;
		draw_line (dc, get_bordercolour (x), x, y2, y_base);
		dc . DrawText (_T ("c1"), x + (reversed ? - dc . GetTextExtent (_T ("c1")) . x - 4 : 4), font_y);
	}
	void draw_degree (wxDC & dc, double x) {
		if (no_degree) return;
		draw_line (dc, get_bordercolour (x), x, y2, y_base);
		dc . DrawText (_T ("q"), x + (reversed ? - dc . GetTextExtent (_T ("q")) . x - 4 : 4), font_y);
	}
	void draw_m (wxDC & dc, double x) {
		if (no_m) return;
		draw_line (dc, get_bordercolour (x), x, y2, y_base);
		dc . DrawText (_T ("M"), x + (reversed ? - dc . GetTextExtent (_T ("M")) . x - 4 : 4), font_y);
	}
	void draw_mm (wxDC & dc, double x) {
		if (no_mm) return;
		draw_thin_line (dc, get_bordercolour (x), x, y2, y_base);
	}
	void drawer (wxDC & dc, double x) {local_x = x; draw (dc, x); draw_horizontal_lines (dc, x);}
	Scale (int height) {
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
	virtual ~ Scale (void) {
		if (alt_lcp != NULL) delete [] alt_lcp; alt_lcp = NULL;
		if (alt_red_lp != NULL) delete [] alt_red_lp; alt_red_lp = NULL;
		if (individual_lcp != NULL) delete [] individual_lcp; individual_lcp = NULL;
		if (individual_red_lp != NULL) delete [] individual_red_lp; individual_red_lp = NULL;
		if (next != NULL) delete next; next = NULL;
	}
};

class LogBase : public Scale {
public:
	double logs [1100];
	double pi_location;
	double e_location;
	double c_location;
	double c1_location;
	double degree_location;
	double m_location;
	double mm_location;
	double extension_index_shift;
	int left_extension_index;
	int right_extension_index;
	void prepare_extensions (double extension_multiplier, int maximum_index) {
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
	void log_init (double multiplier = 1.0) {
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
	void draw_left_log_base (wxDC & dc, double x, int index) {
		int description = 2;
		double location = logs [0];
		for (int ind = 0; ind <= 800; ind += 100) {
			if (index <= 100 + ind) location = draw_markings_for_100 (dc, & logs [ind], wxString :: Format (_T ("%i"), description), x, location, index <= ind ? 0 : index - ind, ind == 800 ? 99 : 100);
			else location = logs [ind];
			description++;
		}
	}
	void draw_right_log_base (wxDC & dc, double x, int index) {
		int description = 2;
		double location = logs [0];
		for (int ind = 0; ind <= 800; ind += 100) {
			if (ind > index) return;
			location = draw_markings_for_100 (dc, & logs [ind], wxString :: Format (_T ("%i"), description), x, location, 0, index - ind < 100 ? index - ind : 100);
			description++;
		}
	}
	void draw_both_sides_log_base (wxDC & dc, double x, int left_index, int right_index) {
		int description = 2;
		double location = logs [0];
		for (int ind = 0; ind <= 800; ind += 100) {
			if (ind > right_index) return;
			if (left_index <= 100 + ind) location = draw_markings_for_100 (dc, & logs [ind], wxString :: Format (_T ("%i"), description), x, location, left_index <= ind ? 0 : left_index - ind, right_index - 100 >= ind ? 100 : right_index - ind);
			else location = logs [ind];
			description++;
		}
	}
	void draw_log_base (wxDC & dc, double x) {
		double location = logs [0];
		for (int ind = 0; ind < 9; ind++) {
			location = draw_markings_for_100 (dc, & logs [ind * 100], ind == 8 ? _T ("1") : wxString :: Format (_T ("%i"), ind + 2), x, location);
		}
	}
	LogBase (int height) : Scale (height) {left_extension_index = right_extension_index = -1; extension_index_shift = 0.0;}
};

class ScalePTUp : public LogBase {
public:
	virtual double getLocation (double x) {return log10 (sqrt (1.0 + x * x));}
	virtual double getValue (double x) {x = pow (10.0, x); x *= x; x -= 1.0; x = sqrt (x); return x;}
	void init_pt (void) {
		for (int ind = 0; ind <= 1000; ind++) {
			double x = (double) ind / 100.0;
			x = 1.0 + x * x;
			logs [ind] = (double) scale_length * log10 (sqrt (x));
		}
	}
	virtual void scaleInit (void) {faceUp (); init_pt ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		draw_index_location (dc, x, _T ("0"));
//		draw_line (dc, get_bordercolour (x - 1.0), get_bordercolour (x + 1.0), x, y0, y1);
//		draw_text_marker (dc, _T ("0"), x);
		double location = logs [0];
		int description = 1;
		draw_markings_for_100 (dc, logs, _T ("1"), x, location, 20, 49);
		location = draw_markings_for_100 (dc, logs, _T ("1"), x, location, 50, 100);
		for (int ind = 100; ind < 1000; ind += 100) {
			location = draw_markings_for_100 (dc, & logs [ind], description < 9 ? wxString :: Format (_T ("%i"), description + 1) : _T ("10"), x, location);
			description++;
		}
	}
	ScalePTUp (int height) : LogBase (height) {}
};

class ScalePTDown : public ScalePTUp {
public:
	virtual void scaleInit (void) {faceDown (); init_pt ();}
	ScalePTDown (int height) : ScalePTUp (height) {}
};

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

class ScalePHUp : public Scale {
public:
	double locations_1 [1000];
	double locations_2 [1000];
	double locations_3 [1000];
	double left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (sqrt (x * x - 1.0));}
	virtual double getValue (double x) {double ret = pow (10.0, x - 1.0); ret *= ret; return sqrt (ret + 1.0);}//return sqrt (ret - 1.0);}
	void init_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			double value1 = (double) ind * 0.001 + 1.1;
			double value2 = (double) ind * 0.0001 + 1.01;
			double value3 = (double) ind * 0.00001 + 1.001;
			locations_1 [ind] = (double) scale_length * (1.0 + log10 (sqrt (value1 * value1 - 1.0)));
			locations_2 [ind] = (double) scale_length * (1.0 + log10 (sqrt (value2 * value2 - 1.0)));
			locations_3 [ind] = (double) scale_length * (1.0 + log10 (sqrt (value3 * value3 - 1.0)));
		}
		left_index = find_left_index (locations_3, 0, 900, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations_1, 0, 900, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double l1 = locations_1 [0]; double l2 = locations_2 [0]; double l3 = locations_3 [0];
		int desc = 2;
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind + 100 >= left_index) l3 = draw_markings_for_100 (dc, & locations_3 [ind], ind == 800 ? _T ("") : wxString :: Format (_T ("1.%03i"), desc), x, l3, left_index <= ind ? 0 : left_index - ind, 100);
			l2 = draw_markings_for_100 (dc, & locations_2 [ind], ind == 800 ? _T ("") : wxString :: Format (_T ("1.%02i"), desc), x, l2);
			if (ind <= right_index) l1 = draw_markings_for_100 (dc, & locations_1 [ind], ind == 800 ? _T ("") : wxString :: Format (_T ("1.%i"), desc), x, l1, 0, right_index >= ind + 100 ? 100 : right_index - ind);
			desc++;
		}
		draw_text_marker (dc, _T ("1.01"), x + locations_2 [0]);
		draw_text_marker (dc, _T ("1.1"), x + locations_1 [0]);
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScalePHUp (int height) : Scale (height) {left_index = 0; right_index = 900;}
};

class ScalePHDown : public ScalePHUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScalePHDown (int height) : ScalePHUp (height) {}
};

class ScalePH2Up : public Scale {
public:
	double locations_2 [1000];
	double locations_3 [1000];
	double left_index;
	virtual double getLocation (double x) {return log10 (sqrt (x * x - 1.0));}
	virtual double getValue (double x) {double ret = pow (10.0, x); ret *= ret; return sqrt (ret + 1.0);}//return sqrt (ret - 1.0);}
	void init_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			double value2 = (double) ind * 0.01 + 1.0;
			double value3 = (double) ind * 0.001 + 1.1;
			locations_2 [ind] = (double) scale_length * (log10 (sqrt (value2 * value2 - 1.0)));
			locations_3 [ind] = (double) scale_length * (log10 (sqrt (value3 * value3 - 1.0)));
		}
		left_index = find_left_index (locations_3, 0, 900, 0.0 - (double) scale_length * left_extension);
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double l2 = locations_2 [0]; double l3 = locations_3 [0];
		int desc = 2;
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind + 100 >= left_index) l3 = draw_markings_for_100 (dc, & locations_3 [ind], ind == 800 ? _T ("2") : wxString :: Format (_T ("1.%1i"), desc), x, l3, left_index <= ind ? 0 : left_index - ind, 100);
			desc++;
		}
		desc = 3;
		for (int ind = 100; ind < 900; ind += 100) {
			l2 = draw_markings_for_100 (dc, & locations_2 [ind], wxString :: Format (_T ("%i"), desc++), x, l2);
		}
		if (absolute (locations_2 [200] - locations_2 [150]) > minimum_step * 4) draw_text_marker (dc, _T ("2.5"), x + locations_2 [150]);
		if (absolute (locations_2 [300] - locations_2 [250]) > minimum_step * 4) draw_text_marker (dc, _T ("3.5"), x + locations_2 [250]);
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	ScalePH2Up (int height) : Scale (height) {left_index = 0;}
};

class ScalePH2Down : public ScalePH2Up {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	ScalePH2Down (int height) : ScalePH2Up (height) {}
};

class ScalePUp : public Scale {
public:
	double locations_1 [1000];
	double locations_2 [1000];
	double locations_3 [1000];
	int left_index;
	void prepare_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			double value1 = (double) ind * 0.001;
			double value2 = (double) ind * 0.0001 + 0.9;
			double value3 = (double) ind * 0.00001 + 0.99;
			value1 *= value1; value2 *= value2; value3 *= value3;
			value1 = 1.0 - value1; value2 = 1.0 - value2; value3 = 1.0 - value3;
			value1 = sqrt (value1); value2 = sqrt (value2); value3 = sqrt (value3);
			locations_1 [ind] = (double) scale_length + (double) scale_length * log10 (value1);
			locations_2 [ind] = (double) scale_length + (double) scale_length * log10 (value2);
			locations_3 [ind] = (double) scale_length + (double) scale_length * log10 (value3);
		}
		double sentinel = 0.0 - (double) scale_length * left_extension;
		left_index = 0;
		while (left_index < 899 && locations_3 [left_index + 1] >= sentinel) left_index++;
	}
	virtual double getLocation (double x) {return 1.0 + log10 (sqrt (1.0 - x * x));}
	virtual double getValue (double x) {
		double ret = pow (10.0, x - 1.0);
		ret *= ret;
		return sqrt (1.0 - ret);
	}
	virtual void scaleInit (void) {faceUp (); prepare_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations_1 [0];
		draw_index_location (dc, x + location, _T ("0"));
		int description = 1;
		for (int ind = 0; ind < 900; ind += 100) location = draw_markings_for_100 (dc, & locations_1 [ind], wxString :: Format (_T ("%i"), description++), x, location);
		description = 1;
		for (int ind = 0; ind < 900; ind += 100) location = draw_markings_for_100 (dc, & locations_2 [ind], wxString :: Format (_T ("9%i"), description++), x, location);
		description = 1;
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind < left_index) location = draw_markings_for_100 (dc, & locations_3 [ind], wxString :: Format (_T ("99%i"), description++), x, location, 0, ind + 100 <= left_index ? 100 : left_index - ind);
		}
	}
	ScalePUp (int height) : Scale (height) {left_index = 900;}
};

class ScalePDown : public ScalePUp {
public:
	virtual void scaleInit (void) {faceDown (); prepare_locations ();}
	ScalePDown (int height) : ScalePUp (height) {}
};

/* GJM -- Extended P2 scales */
class ScaleP2Up : public Scale {
public:
	double locations_1 [1000];
	double locations_2 [1000];
	double locations_3 [1000];
	int left_index;
	void prepare_locations (void) {
		for (int ind = 0; ind <= 900; ind++) {
			double value1 = (double) ind * 0.00001 + 0.99;
			double value2 = (double) ind * 0.000001 + 0.999;
			double value3 = (double) ind * 0.0000001 + 0.9999;
			value1 *= value1; value2 *= value2; value3 *= value3;
			value1 = 1.0 - value1; value2 = 1.0 - value2; value3 = 1.0 - value3;
			value1 = sqrt (value1); value2 = sqrt (value2); value3 = sqrt (value3);
			locations_1 [ind] = (double) scale_length + (double) scale_length + (double) scale_length * log10 (value1);
			locations_2 [ind] = (double) scale_length + (double) scale_length + (double) scale_length * log10 (value2);
			locations_3 [ind] = (double) scale_length + (double) scale_length + (double) scale_length * log10 (value3);
		}
		double sentinel = 0.0 - (double) scale_length * left_extension;
		left_index = 0;
		while (left_index < 899 && locations_3 [left_index + 1] >= sentinel) left_index++;
	}
	virtual double getLocation (double x) {return 2.0 + log10 (sqrt (1.0 - x * x));}
	virtual double getValue (double x) {
		double ret = pow (10.0, x - 2.0);
		ret *= ret;
		return sqrt (1.0 - ret);
	}
	virtual void scaleInit (void) {faceUp (); prepare_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations_1 [500];
		draw_index_location (dc, x + location, _T ("995"));
		int description = 6;
		for (int ind = 500; ind < 900; ind += 100) location = draw_markings_for_100 (dc, & locations_1 [ind], wxString :: Format (_T ("99%i"), description++), x, location);
		description = 1;
		for (int ind = 0; ind < 900; ind += 100) location = draw_markings_for_100 (dc, & locations_2 [ind], wxString :: Format (_T ("999%i"), description++), x, location);
		description = 1;
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind < left_index) location = draw_markings_for_100 (dc, & locations_3 [ind], wxString :: Format (_T ("9999%i"), description++), x, location, 0, ind + 100 <= left_index ? 100 : left_index - ind);
		}
	}
	ScaleP2Up (int height) : Scale (height) {left_index = 900;}
};

class ScaleP2Down : public ScaleP2Up {
public:
	virtual void scaleInit (void) {faceDown (); prepare_locations ();}
	ScaleP2Down (int height) : ScaleP2Up (height) {}
};

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

static int getTrigSentinel (int ind) {
	if (ind < 900) return 60;
	if (ind < 1500) return 300;
	return 600;
};

class TrigBase : public Scale {
public:
	double locations [10000];
	void init_locations (double multiplier, bool tangent, int to) {
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
	void init_dec_locations (double multiplier, bool tangent, int to) {
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
	void init_locations_1 (double multiplier, bool tangent, int to) {
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
	void init_dec_locations_1 (double multiplier, bool tangent, int to) {
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
	void init_small_locations (int to) {
		for (int ind = 1; ind <= to; ind++) {
			double angle = (double) ind * _PI / 10800.0;
			locations [ind] = (double) scale_length + (double) scale_length + (double) scale_length * log10 (angle);
		}
		locations [0] = locations [1] - 100.0;
	}
	void init_small_dec_locations (int to) {
		for (int ind = 1; ind <= to; ind++) {
			double angle = (double) ind * _PI / 18000.0;
			locations [ind] = (double) scale_length + (double) scale_length + (double) scale_length * log10 (angle);
		}
		locations [0] = locations [1] - 100.0;
	}
	void init_sine (double multiplier = 1.0) {init_locations (multiplier, false, 90);}
	void init_tan (double multiplier = 1.0) {init_locations (multiplier, true, 89);}
	void init_dec_sine (double multiplier = 1.0) {init_dec_locations (multiplier, false, 90);}
	void init_dec_tan (double multiplier = 1.0) {init_dec_locations (multiplier, true, 89);}
	/* GJM -- small sine and tangent inits for S1 / T1 scales */
	void init_small_sine (double multiplier = 1.0) {init_locations_1 (multiplier, false, 90);}
	void init_small_tan (double multiplier = 1.0) {init_locations_1 (multiplier, true, 89);}
	void init_small_dec_sine (double multiplier = 1.0) {init_dec_locations_1 (multiplier, false, 90);}
	void init_small_dec_tan (double multiplier = 1.0) {init_dec_locations_1 (multiplier, true, 89);}
	double drawing_footer (wxDC & dc, double * markings, int description, double x, double last_drawn, int to, bool next_extent) {
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
	double draw_markings_for_1 (wxDC & dc, double * markings, int description, double x, double last_drawn, int from = 0, int to = 60) {
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
	double draw_markings_for_5 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from = 0, int to = 300) {
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
	double draw_markings_for_dec_5 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from = 0, int to = 500) {
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
	double draw_markings_for_10 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from = 0, int to = 600) {
		double distance = smaller (markings [to] - markings [to - 300], markings [from + 300] - markings [from]);
		if (distance > minimum_step) {
			last_drawn = draw_markings_for_5 (dc, markings, base_description, x, last_drawn, extent, from, to > 300 ? 300 : to);
			return draw_markings_for_5 (dc, & markings [300], base_description + 5, x, last_drawn, extent, from > 300 ? from - 300 : 0, to - 300);
		}
		if (to < 600) return last_drawn;
		return drawing_footer (dc, markings, base_description + 10, x, last_drawn, 600, extent);
	}
	double draw_markings_for_dec_10 (wxDC & dc, double * markings, int base_description, double x, double last_drawn, bool extent, int from = 0, int to = 1000) {
		double distance = smaller (markings [to] - markings [to - 500], markings [from + 500] - markings [from]);
		if (distance > minimum_step) {
			last_drawn = draw_markings_for_dec_5 (dc, markings, base_description, x, last_drawn, extent, from, to > 500 ? 500 : to);
			return draw_markings_for_dec_5 (dc, & markings [500], base_description + 5, x, last_drawn, extent, from > 500 ? from - 500 : 0, to - 500);
		}
		if (to < 1000) return last_drawn;
		return drawing_footer (dc, markings, base_description + 10, x, last_drawn, 1000, extent);
	}
	int find_left_trig_index (double x) {
		for (int ind = 1; ind < 6000; ind++) {if (locations [ind] >= x) return ind;}
		return 0;
	}
	int find_right_trig_index (double x) {
		for (int ind = 1; ind < 6000; ind++) {if (locations [ind] > x) return ind - 1;}
		return 6000;
	}
	int find_left_dec_trig_index (double x) {
		for (int ind = 1; ind < 10000; ind++) {if (locations [ind] >= x) return ind;}
		return 0;
	}
	int find_right_dec_trig_index (double x) {
		for (int ind = 1; ind < 10000; ind++) {if (locations [ind] > x) return ind - 1;}
		return 10000;
	}
	TrigBase (int height) : Scale (height) {}
};

class TrigSgUp : public Scale {
public:
	double locations [1100];
	double left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (sin (x * _PI / 200.0));}
	virtual double getValue (double x) {return asin (pow (10.0, x - 1.0)) * 200.0 / _PI;}
	void init_locations (void) {
		for (int ind = 1; ind <= 1000; ind++) {locations [ind] = (double) scale_length * (1.0 + log10 (sin ((double) ind * _PI / 2000.0)));}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 1000, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 1000, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
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
		if (left_index <= 90) draw_text_marker (dc, _T ("9"), x + locations [90]);
		if (left_index <= 80) draw_text_marker (dc, _T ("8"), x + locations [80]);
		if (left_index <= 70) draw_text_marker (dc, _T ("7"), x + locations [70]);
		if (left_index <= 60) draw_text_marker (dc, _T ("6"), x + locations [60]);
		if (left_index <= 50) draw_text_marker (dc, _T ("5"), x + locations [50]);
		if (left_index <= 40) draw_text_marker (dc, _T ("4"), x + locations [40]);
		if (left_index <= 30) draw_text_marker (dc, _T ("3"), x + locations [30]);
		if (left_index <= 20) draw_text_marker (dc, _T ("2"), x + locations [20]);
		if (left_index <= 10) draw_text_marker (dc, _T ("1"), x + locations [10]);
		if (right_index <= 800) return;
		double distance = absolute (locations [1000] - locations [900]);
		wxSize extent = dc . GetTextExtent (_T ("100"));
		if (distance >= minimum_step) {
			location = draw_markings_for_100 (dc, & locations [800], distance > extent . x ? _T ("90") : _T (""), x, locations [800]);
			location = draw_markings_for_100 (dc, & locations [900], distance > extent . x ? _T ("100") : _T (""), x, locations [900]);
			if (distance <= extent . x) draw_text_marker (dc, _T ("100"), x + locations [1000]);
		} else {
			draw_index_location (dc, x + locations [1000], _T ("100"));
		}
	}
	TrigSgUp (int height) : Scale (height) {left_index = 1; right_index = 1000;}
};

class TrigSgDown : public TrigSgUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	TrigSgDown (int height) : TrigSgUp (height) {}
};

class TrigTgUp : public Scale {
public:
	double locations [1100];
	int left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (tan (x * _PI / 200.0));}
	virtual double getValue (double x) {return atan (pow (10.0, x - 1.0)) * 200.0 / _PI;}
	void init_locations (void) {
		for (int ind = 1; ind < 1000; ind++) {locations [ind] = (double) scale_length * (1.0 + log10 (tan ((double) ind * _PI / 2000.0)));}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 1, 999, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 1, 999, (double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		int description = 10;
		for (int ind = 0; ind < 900; ind += 100) {
			if (ind + 100 >= left_index && ind <= right_index) {
				location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%i"), description), x, location, left_index <= ind ? 0 : left_index - ind, right_index >= ind + 100 ? 100 : right_index - ind);
			}
			description += 10;
		}
		if (left_index <= 90) draw_text_marker (dc, _T ("9"), x + locations [90]);
		if (left_index <= 80) draw_text_marker (dc, _T ("8"), x + locations [80]);
		if (left_index <= 70) draw_text_marker (dc, _T ("7"), x + locations [70]);
		if (left_index <= 60) draw_text_marker (dc, _T ("6"), x + locations [60]);
		if (left_index <= 50) draw_text_marker (dc, _T ("5"), x + locations [50]);
		if (left_index <= 40) draw_text_marker (dc, _T ("4"), x + locations [40]);
		if (left_index <= 30) draw_text_marker (dc, _T ("3"), x + locations [30]);
		if (left_index <= 20) draw_text_marker (dc, _T ("2"), x + locations [20]);
		if (left_index <= 10) draw_text_marker (dc, _T ("1"), x + locations [10]);
	}
	TrigTgUp (int height) : Scale (height) {left_index = 1; right_index = 999;}
};

class TrigTgDown : public TrigTgUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	TrigTgDown (int height) : TrigTgUp (height) {}
};

class TrigSUp : public TrigBase {
public:
	int left_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 1.0 + log10 (sin (x * _PI / 180.0));}
	virtual double getValue (double x) {return dec_correction (asin (pow (10.0, x - 1.0)) * 180.0 / _PI);}
	void init_this (void) {init_sine (); left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 300, 600);
		for (int ind = 10; ind < 70; ind += 10) location = draw_markings_for_10 (dc, & locations [ind * 60], ind, x, location, false);
		double distance = absolute (locations [5400] - locations [4800]);
		wxString text90 = _T ("90");
		wxSize extent = dc . GetTextExtent (text90);
		if (distance < minimum_step) {draw_index_location (dc, x + locations [5400], text90); return;}
		location = draw_markings_for_10 (dc, & locations [4200], 70, x, location, distance < (double) extent . x);
		if (distance < (double) extent . x) draw_index_location (dc, x + locations [5400], text90);
		else location = draw_markings_for_10 (dc, & locations [4800], 80, x, location, false);
	}
	TrigSUp (int height) : TrigBase (height) {left_index = 0;}
};

class TrigSDown : public TrigSUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSDown (int height) : TrigSUp (height) {}
};

class TrigTAUp : public TrigBase {
public:
	int left_index, right_index;
	double fraction, shift;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 1.0 + log10 (tan (x * _PI / 180.0)) / fraction - shift;}
	virtual double getValue (double x) {x += shift; return dec_correction (atan (pow (10.0, x * fraction - fraction)) * 180.0 / _PI);}
	void init_this (void) {
		init_tan (1.0 / fraction);
		left_index = find_left_trig_index (0.0 - (left_extension - shift) * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension + shift));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		x -= shift * (double) scale_length;
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 300, 600);
		for (int ind = 10; ind < 40; ind += 10) location = draw_markings_for_10 (dc, & locations [ind * 60], ind, x, location, false);
		if (right_index > 2400) location = draw_markings_for_10 (dc, & locations [2400], 40, x, location, false, 0, right_index < 3000 ? right_index - 2400 : 600);
		if (right_index > 3000) location = draw_markings_for_10 (dc, & locations [3000], 50, x, location, false, 0, right_index < 3600 ? right_index - 3000 : 600);
		if (right_index > 3600) location = draw_markings_for_10 (dc, & locations [3600], 60, x, location, false, 0, right_index < 4200 ? right_index - 3600 : 600);
		if (right_index > 4200) location = draw_markings_for_10 (dc, & locations [4200], 70, x, location, false, 0, right_index < 4800 ? right_index - 4200 : 600);
		if (right_index > 4800) location = draw_markings_for_10 (dc, & locations [4800], 80, x, location, false, 0, right_index < 5400 ? right_index - 4800 : 600);
	}
	TrigTAUp (int height) : TrigBase (height) {fraction = 2.0; shift = 0.0; left_index = 0; right_index = 2700;}
	TrigTAUp (int fraction, int height) : TrigBase (height) {
		if (fraction < 1) fraction = 1;
		this -> fraction = (double) fraction;
		shift = 0.5;
		left_index = 0; right_index = 2700;
	}
};

class TrigTADown : public TrigTAUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigTADown (int height) : TrigTAUp (height) {}
	TrigTADown (int fraction, int height) : TrigTAUp (fraction, height) {}
};

class TrigTUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 1.0 + log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return dec_correction (atan (pow (10.0, x - 1.0)) * 180.0 / _PI);}
	void init_this (void) {
		init_tan ();
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 300, 600);
		for (int ind = 10; ind < 40; ind += 10) location = draw_markings_for_10 (dc, & locations [ind * 60], ind, x, location, false);
		if (right_index > 2400) location = draw_markings_for_10 (dc, & locations [2400], 40, x, location, false, 0, right_index < 3000 ? right_index - 2400 : 600);
		if (right_index > 3000) location = draw_markings_for_10 (dc, & locations [3000], 50, x, location, false, 0, right_index < 3600 ? right_index - 3000 : 600);
		if (right_index > 3600) location = draw_markings_for_10 (dc, & locations [3600], 60, x, location, false, 0, right_index < 4200 ? right_index - 3600 : 600);
		if (right_index > 4200) location = draw_markings_for_10 (dc, & locations [4200], 70, x, location, false, 0, right_index < 4800 ? right_index - 4200 : 600);
		if (right_index > 4800) location = draw_markings_for_10 (dc, & locations [4800], 80, x, location, false, 0, right_index < 5400 ? right_index - 4800 : 600);
	}
	TrigTUp (int height) : TrigBase (height) {left_index = 0; right_index = 2700;}
};

class TrigTDown : public TrigTUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigTDown (int height) : TrigTUp (height) {}
};

/* GJM -- High accuracy small degree T scales (replaces ST/SRT scale) */
class TrigT1Up : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 2.0 + log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return dec_correction (atan (pow (10.0, x - 2.0)) * 180.0 / _PI);}
	void init_this (void) {
		init_small_tan ();
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 30, right_index != 6000 ? right_index : 360);
		if (left_index <= 30) draw_text_marker (dc, _T ("30'"), x + locations [30]);
		draw_text_marker (dc, _T ("1:30'"), x + locations [90]);
	}
	TrigT1Up (int height) : TrigBase (height) {left_index = 5; right_index = 600;}
};

class TrigT1Down : public TrigT1Up {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigT1Down (int height) : TrigT1Up (height) {}
};

class TrigT2Up : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return dec_correction (atan (pow (10.0, x)) * 180.0 / _PI);}
	void init_this (void) {
		init_tan ();
		left_index = find_left_trig_index ((double) scale_length * (1.0 - left_extension)); //(0.0 - (double) left_extension * ((double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (2.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		x -= (double) scale_length;
		double location = locations [1200];
//		draw_index_location (dc, x + locations [2700], _T ("45"));
		if (left_index <= 1800) location = draw_markings_for_10 (dc, & locations [1200], 20, x, location, false, left_index <= 1200 ? 0 : left_index - 1200, 600);
		if (left_index <= 2400) location = draw_markings_for_10 (dc, & locations [1800], 30, x, location, false, left_index <= 1800 ? 0 : left_index - 1800, 600);
		if (left_index <= 3000) location = draw_markings_for_10 (dc, & locations [2400], 40, x, location, false, left_index <= 2400 ? 0 : left_index - 2400, 600);
		location = draw_markings_for_10 (dc, & locations [3000], 50, x, location, false);
		location = draw_markings_for_10 (dc, & locations [3600], 60, x, location, false);
		location = draw_markings_for_10 (dc, & locations [4200], 70, x, location, false);
		if (right_index > 4800) location = draw_markings_for_10 (dc, & locations [4800], 80, x, location, false, 0, right_index < 5400 ? right_index - 4800 : 600);
	}
	TrigT2Up (int height) : TrigBase (height) {left_index = 0; right_index = 6000;}
};

class TrigT2Down : public TrigT2Up {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigT2Down (int height) : TrigT2Up (height) {}
};

class TrigSAUp : public TrigBase {
public:
	int left_index;
	double factor;
	double shift;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 1.0 + log10 (sin (x * _PI / 180.0)) / factor - shift;}
	virtual double getValue (double x) {x += shift; return dec_correction (asin (pow (10.0, x * factor - factor)) * 180.0 / _PI);}
	void init_this (void) {init_sine (1.0 / factor); left_index = find_left_trig_index (0.0 - (left_extension - shift) * (double) scale_length);}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		x -= shift * (double) scale_length;
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 30, 600);
		for (int ind = 10; ind < 70; ind += 10) location = draw_markings_for_10 (dc, & locations [ind * 60], ind, x, location, false);
		double distance = absolute (locations [5400] - locations [4800]);
		wxString text90 = _T ("90");
		wxSize extent = dc . GetTextExtent (text90);
		if (distance < minimum_step) {draw_index_location (dc, x + locations [5400], text90); return;}
		location = draw_markings_for_10 (dc, & locations [4200], 70, x, location, distance < (double) extent . x);
		if (distance < (double) extent . x) draw_index_location (dc, x + locations [5400], text90);
		else location = draw_markings_for_10 (dc, & locations [4800], 80, x, location, false);
	}
	TrigSAUp (int height) : TrigBase (height) {this -> factor = 2.0; left_index = 0; this -> shift = 0.0;}
	TrigSAUp (int factor, int height) : TrigBase (height) {if (factor < 1) factor = 1; this -> factor = (double) factor; left_index = 0; this -> shift = 0.5;}
};

class TrigSADown : public TrigSAUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSADown (int height) : TrigSAUp (height) {}
	TrigSADown (int factor, int height) : TrigSAUp (factor, height) {}
};

class TrigSTUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return log10 (x * _PI / 1.8);}
	virtual double getValue (double x) {return dec_correction (pow (10.0, x) * 1.8 / _PI);}
	void init_this (void) {
		init_small_locations (600);
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 30, right_index != 6000 ? right_index : 360);
		if (left_index <= 30) draw_text_marker (dc, _T ("30'"), x + locations [30]);
		draw_text_marker (dc, _T ("1:30'"), x + locations [90]);
	}
	TrigSTUp (int height) : TrigBase (height) {left_index = 5; right_index = 600;}
};

class TrigSTDown : public TrigSTUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSTDown (int height) : TrigSTUp (height) {}
};

/* GJM -- High accuracy small degree S scales (replaces ST/SRT scale) */
class TrigS1Up : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {x = dec_recorrection (x); return 2.0 + log10 (sin (x * _PI / 180.0));}
	virtual double getValue (double x) {return dec_correction (asin (pow (10.0, x - 2.0)) * 180.0 / _PI);}
	void init_this (void) {
		init_small_sine ();
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 30, right_index != 6000 ? right_index : 360);
		if (left_index <= 30) draw_text_marker (dc, _T ("30'"), x + locations [30]);
		draw_text_marker (dc, _T ("1:30'"), x + locations [90]);
	}
	TrigS1Up (int height) : TrigBase (height) {left_index = 5; right_index = 600;}
};

class TrigS1Down : public TrigS1Up {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigS1Down (int height) : TrigS1Up (height) {}
};

class TrigSDecUp : public TrigBase {
public:
	int left_index;
	virtual double getLocation (double x) {return 1.0 + log10 (sin (x * _PI / 180.0));}
	virtual double getValue (double x) {return asin (pow (10.0, x - 1.0)) * 180.0 / _PI;}
	void init_this (void) {init_dec_sine (); left_index = find_left_dec_trig_index (0.0 - left_extension * (double) scale_length);}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 500, 1000);
		for (int ind = 10; ind < 70; ind += 10) location = draw_markings_for_dec_10 (dc, & locations [ind * 100], ind, x, location, false);
		double distance = absolute (locations [9000] - locations [8000]);
		wxString text90 = _T ("90");
		wxSize extent = dc . GetTextExtent (text90);
		if (distance < minimum_step) {draw_index_location (dc, x + locations [9000], text90); return;}
		location = draw_markings_for_dec_10 (dc, & locations [7000], 70, x, location, distance < (double) extent . x);
		if (distance < (double) extent . x) draw_index_location (dc, x + locations [9000], text90);
		else location = draw_markings_for_dec_10 (dc, & locations [8000], 80, x, location, false);
	}
	TrigSDecUp (int height) : TrigBase (height) {left_index = 0;}
};

class TrigSDecDown : public TrigSDecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSDecDown (int height) : TrigSDecUp (height) {}
};

class TrigTADecUp : public TrigBase {
public:
	int left_index, right_index;
	double fraction, shift;
	virtual double getLocation (double x) {return 1.0 + log10 (tan (x * _PI / 180.0)) / fraction - shift;}
	virtual double getValue (double x) {x += shift; return atan (pow (10.0, x * fraction - fraction)) * 180.0 / _PI;}
	void init_this (void) {
		init_dec_tan (1.0 / fraction);
		left_index = find_left_dec_trig_index (0.0 - (left_extension - shift) * (double) scale_length);
		right_index = find_right_dec_trig_index ((double) scale_length * (1.0 + right_extension + shift));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		x -= shift * (double) scale_length;
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 500, 1000);
		for (int ind = 10; ind < 40; ind += 10) location = draw_markings_for_dec_10 (dc, & locations [ind * 100], ind, x, location, false);
		if (right_index > 4000) location = draw_markings_for_dec_10 (dc, & locations [4000], 40, x, location, false, 0, right_index < 5000 ? right_index - 4000 : 1000);
		if (right_index > 5000) location = draw_markings_for_dec_10 (dc, & locations [5000], 50, x, location, false, 0, right_index < 6000 ? right_index - 5000 : 1000);
		if (right_index > 6000) location = draw_markings_for_dec_10 (dc, & locations [6000], 60, x, location, false, 0, right_index < 7000 ? right_index - 6000 : 1000);
		if (right_index > 7000) location = draw_markings_for_dec_10 (dc, & locations [7000], 70, x, location, false, 0, right_index < 8000 ? right_index - 7000 : 1000);
		if (right_index > 8000) location = draw_markings_for_dec_10 (dc, & locations [8000], 80, x, location, false, 0, right_index < 9000 ? right_index - 8000 : 900);
	}
	TrigTADecUp (int height) : TrigBase (height) {fraction = 2.0; shift = 0.0; left_index = 0; right_index = 4500;}
	TrigTADecUp (int fraction, int height) : TrigBase (height) {
		if (fraction < 1) fraction = 1;
		this -> fraction = (double) fraction;
		shift = 0.5;
		left_index = 0; right_index = 4500;
	}
};

class TrigTADecDown : public TrigTADecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigTADecDown (int height) : TrigTADecUp (height) {}
	TrigTADecDown (int fraction, int height) : TrigTADecUp (fraction, height) {}
};

class TrigTDecUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {return 1.0 + log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return atan (pow (10.0, x - 1.0)) * 180.0 / _PI;}
	void init_this (void) {
		init_dec_tan ();
		left_index = find_left_dec_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_dec_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 500, 1000);
		for (int ind = 10; ind < 40; ind += 10) location = draw_markings_for_dec_10 (dc, & locations [ind * 100], ind, x, location, false);
		if (right_index > 4000) location = draw_markings_for_dec_10 (dc, & locations [4000], 40, x, location, false, 0, right_index < 5000 ? right_index - 4000 : 1000);
		if (right_index > 5000) location = draw_markings_for_dec_10 (dc, & locations [5000], 50, x, location, false, 0, right_index < 6000 ? right_index - 5000 : 1000);
		if (right_index > 6000) location = draw_markings_for_dec_10 (dc, & locations [6000], 60, x, location, false, 0, right_index < 7000 ? right_index - 6000 : 1900);
		if (right_index > 7000) location = draw_markings_for_dec_10 (dc, & locations [7000], 70, x, location, false, 0, right_index < 8000 ? right_index - 7000 : 1000);
		if (right_index > 8000) location = draw_markings_for_dec_10 (dc, & locations [8000], 80, x, location, false, 0, right_index < 9000 ? right_index - 8000 : 1000);
	}
	TrigTDecUp (int height) : TrigBase (height) {left_index = 0; right_index = 4500;}
};

class TrigTDecDown : public TrigTDecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigTDecDown (int height) : TrigTDecUp (height) {}
};

/* GJM -- High accuracy small degree S decimal scales (replaces STDec scale) */
class TrigS1DecUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {return 2.0 + log10 (sin (x * _PI / 180.0));}
	virtual double getValue (double x) {return asin (pow (10.0, x - 2.0)) * 180.0 / _PI;}
	void init_this (void) {
		init_small_dec_sine ();
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 50, right_index != 10000 ? right_index : 600);
		if (left_index <= 50) draw_text_marker (dc, _T ("0.5"), x + locations [50]);
		draw_text_marker (dc, _T ("1.5"), x + locations [150]);
	}
	TrigS1DecUp (int height) : TrigBase (height) {left_index = 5; right_index = 1000;}
};

class TrigS1DecDown : public TrigS1DecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigS1DecDown (int height) : TrigS1DecUp (height) {}
};

class TrigT2DecUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {return log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return atan (pow (10.0, x)) * 180.0 / _PI;}
	void init_this (void) {
		init_dec_tan ();
		left_index = find_left_dec_trig_index ((double) scale_length * (1.0 - left_extension)); //(0.0 - (double) left_extension * ((double) scale_length);
		right_index = find_right_dec_trig_index ((double) scale_length * (2.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		x -= (double) scale_length;
		double location = locations [2000];
//		draw_index_location (dc, x + locations [2700], _T ("45"));
		if (left_index <= 3000) location = draw_markings_for_dec_10 (dc, & locations [2000], 20, x, location, false, left_index <= 2000 ? 0 : left_index - 2000, 1000);
		if (left_index <= 4000) location = draw_markings_for_dec_10 (dc, & locations [3000], 30, x, location, false, left_index <= 3000 ? 0 : left_index - 3000, 1000);
		if (left_index <= 5000) location = draw_markings_for_dec_10 (dc, & locations [4000], 40, x, location, false, left_index <= 4000 ? 0 : left_index - 4000, 1000);
		location = draw_markings_for_dec_10 (dc, & locations [5000], 50, x, location, false);
		location = draw_markings_for_dec_10 (dc, & locations [6000], 60, x, location, false);
		location = draw_markings_for_dec_10 (dc, & locations [7000], 70, x, location, false);
		if (right_index > 8000) location = draw_markings_for_dec_10 (dc, & locations [8000], 80, x, location, false, 0, right_index < 9000 ? right_index - 8000 : 1000);
	}
	TrigT2DecUp (int height) : TrigBase (height) {left_index = 0; right_index = 10000;}
};

class TrigT2DecDown : public TrigT2DecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigT2DecDown (int height) : TrigT2DecUp (height) {}
};

class TrigSADecUp : public TrigBase {
public:
	int left_index;
	double factor;
	double shift;
	virtual double getLocation (double x) {return 1.0 + log10 (sin (x * _PI / 180.0)) / factor - shift;}
	virtual double getValue (double x) {x += shift; return asin (pow (10.0, x * factor - factor)) * 180.0 / _PI;}
	void init_this (void) {init_dec_sine (1.0 / factor); left_index = find_left_trig_index (0.0 - (left_extension - shift) * (double) scale_length);}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		x -= shift * (double) scale_length;
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 50, 1000);
		for (int ind = 10; ind < 70; ind += 10) location = draw_markings_for_dec_10 (dc, & locations [ind * 100], ind, x, location, false);
		double distance = absolute (locations [9000] - locations [8000]);
		wxString text90 = _T ("90");
		wxSize extent = dc . GetTextExtent (text90);
		if (distance < minimum_step) {draw_index_location (dc, x + locations [9000], text90); return;}
		location = draw_markings_for_dec_10 (dc, & locations [7000], 70, x, location, distance < (double) extent . x);
		if (distance < (double) extent . x) draw_index_location (dc, x + locations [9000], text90);
		else location = draw_markings_for_dec_10 (dc, & locations [8000], 80, x, location, false);
	}
	TrigSADecUp (int height) : TrigBase (height) {this -> factor = 2.0; left_index = 0; this -> shift = 0.0;}
	TrigSADecUp (int factor, int height) : TrigBase (height) {if (factor < 1) factor = 1; this -> factor = (double) factor; left_index = 0; this -> shift = 0.5;}
};

class TrigSADecDown : public TrigSADecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSADecDown (int height) : TrigSADecUp (height) {}
	TrigSADecDown (int factor, int height) : TrigSADecUp (factor, height) {}
};

class TrigSTDecUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {return log10 (x * _PI / 1.8);}
	virtual double getValue (double x) {return pow (10.0, x) * 1.8 / _PI;}
	void init_this (void) {
		init_small_dec_locations (1000);
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 50, right_index != 10000 ? right_index : 600);
		if (left_index <= 50) draw_text_marker (dc, _T ("0.5"), x + locations [50]);
		draw_text_marker (dc, _T ("1.5"), x + locations [150]);
	}
	TrigSTDecUp (int height) : TrigBase (height) {left_index = 5; right_index = 1000;}
};

class TrigSTDecDown : public TrigSTDecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigSTDecDown (int height) : TrigSTDecUp (height) {}
};

/* GJM -- High accuracy small degree T decimal scales (replaces STDec scale) */
class TrigT1DecUp : public TrigBase {
public:
	int left_index, right_index;
	virtual double getLocation (double x) {return 2.0 + log10 (tan (x * _PI / 180.0));}
	virtual double getValue (double x) {return atan (pow (10.0, x - 2.0)) * 180.0 / _PI;}
	void init_this (void) {
		init_small_dec_tan ();
		left_index = find_left_trig_index (0.0 - left_extension * (double) scale_length);
		right_index = find_right_trig_index ((double) scale_length * (1.0 + right_extension));
	}
	virtual void scaleInit (void) {faceUp (); init_this ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = draw_markings_for_dec_10 (dc, locations, 0, x, locations [0], false, left_index != 0 ? left_index : 50, right_index != 10000 ? right_index : 600);
		if (left_index <= 50) draw_text_marker (dc, _T ("0.5"), x + locations [50]);
		draw_text_marker (dc, _T ("1.5"), x + locations [150]);
	}
	TrigT1DecUp (int height) : TrigBase (height) {left_index = 5; right_index = 1000;}
};

class TrigT1DecDown : public TrigT1DecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_this ();}
	TrigT1DecDown (int height) : TrigT1DecUp (height) {}
};

class TrigSrUp : public Scale {
public:
	double locations [1600];
	int left_index;
	virtual double getLocation (double x) {return 1.0 + log10 (sin (x));}
	virtual double getValue (double x) {return asin (pow (10.0, x - 1.0));}
	void init_locations (void) {
		for (int ind = 1; ind < 1600; ind++) {
			locations [ind] = (double) scale_length * (1.0 + log10 (sin ((double) ind / 1000.0)));
		}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 0, 100, 0.0 - (double) scale_length * left_extension);
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		location = draw_markings_for_100 (dc, locations, _T (".1"), x, location, left_index, 100);
		location = draw_markings_for_100 (dc, & locations [100], _T (".2"), x, location);
		location = draw_markings_for_100 (dc, & locations [200], _T (".3"), x, location);
		location = draw_markings_for_100 (dc, & locations [300], _T (".4"), x, location);
		location = draw_markings_for_100 (dc, & locations [400], _T (".5"), x, location);
		location = draw_markings_for_100 (dc, & locations [500], _T (".6"), x, location);
		location = draw_markings_for_100 (dc, & locations [600], _T (".7"), x, location);
		location = draw_markings_for_100 (dc, & locations [700], _T (".8"), x, location);
		location = draw_markings_for_100 (dc, & locations [800], _T (".9"), x, location);
		location = draw_markings_for_100 (dc, & locations [900], _T ("1"), x, location);
		location = draw_markings_for_100 (dc, & locations [1000], _T ("1.1"), x, location);
		location = draw_markings_for_100 (dc, & locations [1100], _T ("1.2"), x, location);
		location = draw_markings_for_100 (dc, & locations [1200], _T ("1.3"), x, location);
		location = draw_markings_for_100 (dc, & locations [1300], _T ("1.4"), x, location);
		location = draw_markings_for_100 (dc, & locations [1400], _T ("1.5"), x, location);
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	TrigSrUp (int height) : Scale (height) {left_index = 100;}
};

class TrigSrDown : public TrigSrUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	TrigSrDown (int height) : TrigSrUp (height) {}
};

class TrigTrUp : public Scale {
public:
	double locations [3200];
	int left_index, right_index;
	double shift;
	virtual double getLocation (double x) {return shift + log10 (tan (x));}
	virtual double getValue (double x) {return atan (pow (10.0, x - shift));}
	void init_locations (void) {
		for (int ind = 1; ind < 3140; ind++) {
			locations [ind] = (double) scale_length * (shift + log10 (tan ((double) ind / 1000.0)));
		}
		locations [0] = locations [1] - 100.0;
		left_index = find_left_index (locations, 0, 3000, 0.0 - (double) scale_length * left_extension);
		right_index = find_right_index (locations, 0, 3000, (double) scale_length * (1.0 + right_extension));
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		double location = locations [0];
		double description = 0.1;
		for (int ind = 0; ind < 3000; ind += 100) {
			if (ind > right_index) return;
			if (left_index <= ind + 100) location = draw_markings_for_100 (dc, & locations [ind], wxString :: Format (_T ("%.1f"), description), x, location, left_index <= ind ? 0 : left_index - ind, right_index > ind + 100 ? 100 : right_index - ind);
			description += 0.1;
		}
	}
	virtual void scaleInit (void) {faceUp (); init_locations ();}
	TrigTrUp (double shift, int height) : Scale (height) {this -> shift = shift; left_index = 100; right_index = 3100;}
};

class TrigTrDown : public TrigTrUp {
public:
	virtual void scaleInit (void) {faceDown (); init_locations ();}
	TrigTrDown (double shift, int height) : TrigTrUp (shift, height) {}
};

class TrigISTdUp : public Scale {
public:
	double sd [1000];
	double isd [1100];
	double td [1000];
	double itd [2000];
	void init_angles (void) {
		td [0] = sd [0] = (double) scale_length * log10 (18.0 / _PI);
		for (int ind = 1; ind <= 900; ind++) {
			double alpha = (double) ind / 10.0;
			double sine = sin (alpha * _PI / 180.0);
			double tangent = tan (alpha * _PI / 180.0);
			alpha *= 0.1;
			sd [ind] = (double) scale_length * log10 (alpha / sine);
			td [ind] = (double) scale_length * log10 (alpha / tangent);
		}
		itd [0] = isd [0] = (double) scale_length - td [0];
		for (int ind = 1; ind <= 1000; ind++) {
			double sine = (double) ind / 1000.0;
			double alpha = asin (sine);
			alpha *= 18.0 / _PI;
			isd [ind] = (double) scale_length * (1.0 - log10 (alpha / sine));
		}
		for (int ind = 1; ind <= 1800; ind++) {
			double tangent = (double) ind / 1000.0;
			double alpha = atan (tangent);
			alpha *= 18.0 / _PI;
			itd [ind] = (double) scale_length * (1.0 - log10 (alpha / tangent));
		}
	}
	virtual double getLocation (double x) {
		if (x == 0.0) return log10 (180.0 / _PI);
		if (x < 0.0) {
			x = -x;
			if (x < 2.0) {double alpha = atan (x); alpha *= 18.0 / _PI; return 1.0 - log10 (alpha / x);}
			double tangent = tan (x * _PI / 180.0);
			return log10 (x * 0.1 / tangent);
		}
		if (x <= 1.0) {double alpha = asin (x); alpha *= 18.0 / _PI; return 1.0 - log10 (alpha / x);}
		double sine = sin (x * _PI / 180.0); return log10 (x * 0.1 / sine);
	}
	virtual double getValue (double x) {return x;}
	virtual void scaleInit (void) {faceUp (); init_angles ();}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		draw_line (dc, lcp, x + sd [0], y0, y1);
		draw_text_marker (dc, _T ("0"), x + sd [0]);
		double location = sd [0];
		int description = 10;
		for (int ind = 0; ind < 900; ind += 100) {location = draw_markings_for_100 (dc, & sd [ind], wxString :: Format (_T ("%i"), description), x, location); description += 10;}

		description = 1; location = itd [0];
		for (int ind = 0; ind < 1800; ind += 100) {location = draw_markings_for_100 (dc, & itd [ind], wxString :: Format (_T ("%i"), description), x, location); description++;}

		dc . SetTextForeground (wxColour (red_marking_colour . red, red_marking_colour . green, red_marking_colour . blue));
		reversed = ! reversed;
		location = td [0]; description = 10;
		for (int ind = 0; ind < 600; ind += 100) {location = draw_markings_for_100 (dc, & td [ind], wxString :: Format (_T ("%i"), description), x, location); description += 10;}
		draw_line (dc, lcp, x + isd [0], y0, y1);
		draw_text_marker (dc, _T ("0"), x + isd [0]);

		description = 1; location = isd [0];
		for (int ind = 0; ind < 900; ind += 100) {location = draw_markings_for_100 (dc, & isd [ind], wxString :: Format (_T (".%i"), description), x, location); description++;}
		draw_markings_for_100 (dc, & isd [900], _T ("1.0"), x, location, 0, 50);
		draw_markings_for_100 (dc, & isd [900], _T ("1.0"), x, location, 50, 100);
		wxString marking95 = _T (".95");
		if (dc . GetTextExtent (marking95) . x < absolute (isd [900] - isd [950])) draw_text_marker (dc, marking95, x + isd [950]);

		reversed = ! reversed;
		dc . SetTextForeground (wxColour (marking_colour . red, marking_colour . green, marking_colour . blue));
	}
	TrigISTdUp (int height) : Scale (height) {}
};

class TrigISTdDown : public TrigISTdUp {
public:
	virtual void scaleInit (void) {faceDown (); init_angles ();}
	TrigISTdDown (int height) : TrigISTdUp (height) {}
};

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

class Log_dk : public LogBase {
public:
	double factor;
	double reciprocal_factor;
	virtual double getLocation (double x) {return 0.5 + log10 (x) * reciprocal_factor;}
	virtual double getValue (double x) {x -= 0.5; return pow (10.0, x * factor);}
	virtual void scaleInit (void) {faceUp (); log_init (reciprocal_factor);}
	void draw_dk_base (wxDC & dc, double x, wxString marker = _T ("1")) {
		double location;
		if (absolute (logs [900] - logs [800]) < minimum_step) {
			if (absolute (logs [900] - logs [400]) > minimum_step) {
				location = x + logs [400];
				draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			}
		} else {
			location = x + logs [100]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [200]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [300]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [400]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y1);
			location = x + logs [500]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [600]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [700]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
			location = x + logs [800]; draw_line (dc, get_bordercolour (location - 1.0), get_bordercolour (location + 1.0), location, y0, y2);
		}
		location = x + logs [900]; draw_index_location (dc, location, marker);
	}
	virtual void draw (wxDC & dc, double x) {
		setArialFont (dc);
		int marker_shift = (int) (factor * -0.5);
		draw_index_location (dc, x, wxString :: Format (_T ("%+i"), marker_shift++));
		for (int ind = 0; ind < (int) factor; ind++)
			draw_dk_base (dc, x + (double) ind * reciprocal_factor * (double) scale_length, wxString :: Format (_T ("%+i"), marker_shift++));
	}
	Log_dk (int factor, int height) : LogBase (height) {
		if (factor < 0) factor = 1;
		factor <<= 1;
		this -> factor = (double) factor;
		reciprocal_factor = 1.0 / this -> factor;
	}
};

class Log_dk_down : public Log_dk {
public:
	virtual void scaleInit (void) {faceDown (); log_init (reciprocal_factor);}
	Log_dk_down (int factor, int height) : Log_dk (factor, height) {}
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

class ScaleSQRTSINEUp : public TrigSUp {
public:
	virtual double getLocation (double x) {x = dec_recorrection (x); x = sin (x * _PI / 180.0) * 10.0; return x * x / 100.0;}
	virtual double getValue (double x) {x *= 100.0; x = sqrt (x); return dec_correction (asin (x * 0.1) * 180.0 / _PI);}
	void init_sqrt_sine_locations (void) {
		int index = 0;
		for (int degree = 0; degree <= 90; degree++) {
			double angle = (double) degree / 180.0;
			for (int minute = 0; minute < 60; minute++) {
				double sub_angle = (double) minute / 10800.0;
				double value = _PI * (angle + sub_angle);
				value = sin (value) * 10.0;
				value *= value / 100.0;
				value *= (double) scale_length;
				locations [index++] = value;
			}
		}
		locations [index++] = (double) scale_length;
		left_index = 0;
	}
	virtual void scaleInit (void) {faceUp (); init_sqrt_sine_locations ();}
	virtual void draw (wxDC & dc, double x) {TrigSUp :: draw (dc, x); draw_index_location (dc, x, _T ("0"));}
	ScaleSQRTSINEUp (int height) : TrigSUp (height) {}
};

class ScaleSQRTSINEDown : public ScaleSQRTSINEUp {
public:
	virtual void scaleInit (void) {faceDown (); init_sqrt_sine_locations ();}
	ScaleSQRTSINEDown (int height) : ScaleSQRTSINEUp (height) {}
};

class ScaleSQRTSINEdecUp : public TrigSDecUp {
public:
	virtual double getLocation (double x) {x = sin (x * _PI / 180.0) * 10.0; return x * x / 100.0;}
	virtual double getValue (double x) {x *= 100.0; x = sqrt (x); return asin (x * 0.1) * 180.0 / _PI;}
	void init_sqrt_sine_locations (void) {
		int index = 0;
		for (int degree = 0; degree <= 90; degree++) {
			double angle = (double) degree / 180.0;
			for (int minute = 0; minute < 100; minute++) {
				double sub_angle = (double) minute / 18000.0;
				double value = _PI * (angle + sub_angle);
				value = sin (value) * 10.0;
				value *= value / 100.0;
				value *= (double) scale_length;
				locations [index++] = value;
			}
		}
		locations [index++] = (double) scale_length;
		left_index = 0;
	}
	virtual void scaleInit (void) {faceUp (); init_sqrt_sine_locations ();}
	virtual void draw (wxDC & dc, double x) {TrigSDecUp :: draw (dc, x); draw_index_location (dc, x, _T ("0"));}
	ScaleSQRTSINEdecUp (int height) : TrigSDecUp (height) {}
};

class ScaleSQRTSINEdecDown : public ScaleSQRTSINEdecUp {
public:
	virtual void scaleInit (void) {faceDown (); init_sqrt_sine_locations ();}
	ScaleSQRTSINEdecDown (int height) : ScaleSQRTSINEdecUp (height) {}
};

class ScaleSQRTSINErUp : public TrigSrUp {
public:
	virtual double getLocation (double x) {x = sin (x) * 10.0; return x * x / 100.0;}
	virtual double getValue (double x) {x *= 100.0; x = sqrt (x); return asin (x * 0.1);}
	void init_sine_locations (void) {
		for (int ind = 0; ind < 1600; ind++) {
			double x = 10.0 * sin ((double) ind / 1000.0);
			locations [ind] = (double) scale_length * (x * x / 100.0);
		}
		left_index = 0;
	}
	virtual void scaleInit (void) {faceUp (); init_sine_locations ();}
	virtual void draw (wxDC & dc, double x) {TrigSrUp :: draw (dc, x); draw_index_location (dc, x, _T ("0"));}
	ScaleSQRTSINErUp (int height) : TrigSrUp (height) {}
};

class ScaleSQRTSINErDown : public ScaleSQRTSINErUp {
public:
	virtual void scaleInit (void) {faceDown (); init_sine_locations ();}
	ScaleSQRTSINErDown (int height) : ScaleSQRTSINErUp (height) {}
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

struct HA {
	double d1, d2, d3, d4, d5;
	bool vertical;
	wxString s;
	HA * next;
	HA (HA * next = NULL) {this -> next = next;}
	~ HA (void) {if (next != NULL) delete next; next = NULL;}
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
	void insert_HA (double d1, double d2, double d3, double d4, double d5) {
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
	void insert_HA (double d1, double d2, double d3, char * string) {
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
	void draw_HA (wxDC & dc, double x) {
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
	void draw (wxDC & dc) {
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
	void draw_bitmape (wxDC & dc) {
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
	Rule * captureRule (int y) {
		if (y >= this -> y && y < this -> y + rule_height && ! channel) return this;
		if (next == NULL) return NULL;
		return next -> captureRule (y);
	}
	Scale * captureScale (int y) {
		if (root == NULL) return NULL;
		return root -> captureScale (y - (stator ? this -> y : 0));
	}
	void reBitmap (void) {
		if (stator_dc != NULL) delete stator_dc; stator_dc = NULL;
		if (stator_bitmap != NULL) delete stator_bitmap; stator_bitmap = NULL;
	}
	void initialise_lanczos (void) {
		initialise_lanczos_colours (lcp, rule_colour . red, rule_colour . green, rule_colour . blue, marker_colour . red, marker_colour . green, marker_colour . blue);// 0xd0, 0xd0, 0, 0, 0);
		initialise_lanczos_colours (hairline_lp, rule_colour . red, rule_colour . green, rule_colour . blue, hairline_colour . red, hairline_colour . green, hairline_colour . blue);
	}
	Rule (Rule * next) {
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
	~ Rule (void) {
		if (root != NULL) delete root; root = NULL;
		if (next != NULL) delete next; next = NULL;
		if (HA_ROOT != NULL) delete HA_ROOT; HA_ROOT = NULL;
		reBitmap ();
	}
};

class Hairline {
public:
	int from, to;
	double location;
	double last_drawn;
	void draw (wxDC & dc, lanczos_colour * lcp, double x, double scale_length) {
		if (from < 0) return;
		last_drawn = x + location * scale_length;
		draw_thin_line (dc, lcp, last_drawn, from, to);
	}
	void draw_readout (wxDC & dc, int y, double v, colour * c, colour * b) {
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
	bool in_range (int y) {return y >= from && y < to;}
	bool in_range (int x, int y) {
		if (y < from || y >= to || x + 6 < (int) last_drawn || x - 6 > (int) last_drawn) return false;
		return true;
	}
	Hairline (void) {from = to = -1; last_drawn = 0.0;}
	Hairline (double location) {from = to = -1; last_drawn = this -> location = location;}
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
	int os_compensation;
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
	void apply_visible_margins (double left, double right) {
		if (root == NULL) return;
		root -> visible_left_marginal_factor = left; root -> visible_right_marginal_factor = right;
		root -> rule_length = (int) ((double) scale_length * (left + 1.0 + right));
		root -> x_scale_position = this -> x + (int) ((double) scale_length * marginal_factor);
		root -> x = root -> x_scale_position - (int) ((double) scale_length * left);
	}
	void insertBlankSpacer (int height = 1) {
		insertSpacer (height);
		for (int ind = 0; ind < 16; ind++) {root -> root -> draw_lines [ind] = false;}
	}
	Rule * insertRule (int channel_index = 0) {
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
	void resize_length (int new_length) {
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
	void change_motion_multipliers (double multiplier) {
		this -> motion_multiplier = multiplier;
		Rule * rp = root;
		while (rp != NULL) {
			rp -> motion_multiplier = multiplier;
			rp = rp -> next;
		}
	}
	void close (void) {
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
	Scale * insertScale (Scale * scale) {
		if (scale == NULL) return NULL;
		if (root != NULL) {
			scale -> position_scale (root -> stator ? rule_y - root -> y : rule_y, scale_length, root -> empty_space ? background_marker_colour : marker_colour, hairline_colour, root -> empty_space ? background_colour : rule_colour, root -> empty_space ? background_lp : root -> lcp, root -> empty_space ? background_lp : root -> hairline_lp);
			scale -> no_pi = this -> no_pi; scale -> no_e = this -> no_e; scale -> no_c = this -> no_c; scale -> no_c1 = this -> no_c1; scale -> no_degree = this -> no_degree; scale -> no_m = this -> no_m; scale -> no_mm = this -> no_mm;
			scale -> left_extension = left_extension; scale -> right_extension = right_extension;
			for (int ind = 0; ind < 16; ind++) {scale -> draw_lines [ind] = this -> draw_lines [ind]; scale -> marker_fractions [ind] = this -> marker_fractions [ind];}
			scale -> next = root -> root;
			root -> root = scale;
			scale -> os_compensation = this -> os_compensation;
			scale -> scaleInit ();
			rule_y += scale -> height;
			root -> rule_height = rule_y - root -> y;
		}
		return scale;
	}
	Sliderule * insertSpacer (int height = 1) {
		if (root == NULL) return this;
		if (height < 1) return this;
		Scale * spacer_scale = new Scale (height);
		spacer_scale -> inactive = true;
		insertScale (spacer_scale);
		return this;
	}
	void draw_readout (wxDC & dc, double value, int x, int y, colour * c) {
		//wxString readout = wxString :: Format (_T ("%.2f"), value);
		wxString readout = wxString :: Format (decimal_string, value);
		wxSize extent = dc . GetTextExtent (readout);
		dc . SetPen (wxPen (wxColour (c -> red, c -> green, c -> blue)));
		dc . SetBrush (wxBrush (wxColour (rule_colour . red, rule_colour . green, rule_colour . blue)));
		dc . DrawRectangle (x - 1, y, extent . x + 2, extent . y);
		dc . SetTextForeground (wxColour (c -> red, c -> green, c -> blue));
		dc . DrawText (readout, x, y);
	}
	void draw_readouts (wxDC & dc) {
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
	#ifdef CONTROL_PICTURE
	void draw (wxBufferedPaintDC & dc) {
		dc . DrawLine (102, 98, 298, 98);
		dc . DrawLine (102, 202, 298, 202);
		dc . DrawLine (98, 102, 98, 198);
		dc . DrawLine (302, 102, 302, 198);
		//
		//
		dc . DrawLine (402, 98, 598, 98);
		dc . DrawLine (402, 202, 598, 202);
		dc . DrawLine (398, 102, 398, 198);
		dc . DrawLine (602, 102, 602, 198);
		//
		dc . DrawLine (400, 100, 600, 100);
		dc . DrawLine (400, 200, 600, 200);
		dc . DrawLine (400, 100, 400, 200);
		dc . DrawLine (600, 100, 600, 200);
		//
		dc . DrawLine (102, 298, 298, 298);
		dc . DrawLine (102, 402, 298, 402);
		dc . DrawLine (98, 302, 98, 398);
		dc . DrawLine (302, 302, 302, 398);
		//
		dc . SetBrush (wxBrush (wxColour (0, 0, 0)));
		dc . DrawRoundedRectangle (100, 300, 200, 100, 6);
		//
		dc . DrawLine (402, 298, 598, 298);
		dc . DrawLine (402, 402, 598, 402);
		dc . DrawLine (398, 302, 398, 398);
		dc . DrawLine (602, 302, 602, 398);
		//
		dc . SetBrush (wxBrush (wxColour (0, 0, 255)));
		dc . DrawRoundedRectangle (400, 300, 200, 100, 6);
		//
	}
	#else
	void draw (wxBufferedPaintDC & dc) {
		if (root == NULL) return;
		dc . SetBackground (wxBrush (wxColour (background_colour . red, background_colour . green, background_colour . blue)));
		dc . Clear ();
		root -> draw (dc);
		draw_cursor (dc);
		draw_readouts (dc);
	}
	#endif
	void draw_cursor (wxDC & dc) {
		int cursor_left = x + (int) ((double) scale_length * (marginal_factor + cursor_location - cursor_factor * 0.5));
		int cursor_right = cursor_left + (int) ((double) scale_length * cursor_factor);
//		if (process_cursor_window) {
//			for (int ind = cursor_left; ind < cursor_right; ind++) {
//				for (int sub = y; sub < rule_y; sub++) {
//					wxColour c;
//					dc . GetPixel (wxPoint (ind, sub), & c);
//					blend (& c, 0.0, 0.0, 255.0, 0.1, 0.9);
//					dc . SetPen (c);
//					dc . DrawPoint (ind, sub);
//				}
//			}
//		}
		dc . SetBrush (wxBrush (wxColour (cursor_colour . red, cursor_colour . green, cursor_colour . blue, cursor_colour . alpha)));
		if (draw_cursor_window) {
			dc . DrawRoundedRectangle (cursor_left, y - 2, cursor_right - cursor_left, rule_y - y + 4, 4);
		}
		dc . SetPen (wxPen (wxColour (cursor_colour . red, cursor_colour . green, cursor_colour . blue)));
		dc . SetBrush (wxBrush (wxColour (0, 0,  0), wxTRANSPARENT));
		dc . DrawRoundedRectangle (cursor_left, y - 2, cursor_right - cursor_left, rule_y - y + 4, 4);
		dc . DrawRoundedRectangle (cursor_left - 1, y - 3, cursor_right - cursor_left + 2, rule_y - y + 6, 4);
//		drawed_hairline = x + (double) scale_length * (cursor_location + marginal_factor);
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
//		if (process_cursor_hairline) draw_alpha_line (dc, hairline_lp, x + scale_length * (cursor_location + marginal_factor), y, rule_y);
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
	Rule * captureRule (int y) {
		if (root == NULL) return NULL;
		return root -> captureRule (y);
	}
	Scale * captureScale (int y) {
		Rule * captured_rule = captureRule (y);
		if (captured_rule == NULL) return NULL;
		return captured_rule -> captureScale (y);
	}
	void insertExtraHairlines (int extra_hairlines) {
		number_of_extra_hairlines = extra_hairlines;
		if (number_of_extra_hairlines < 0) number_of_extra_hairlines = 0;
		if (number_of_extra_hairlines > 128) number_of_extra_hairlines = 128;
		this -> extra_hairlines = new HairlinePointer [number_of_extra_hairlines + 16];
		for (int ind = 0; ind < number_of_extra_hairlines; ind++) this -> extra_hairlines [ind] = new Hairline ();
	}
	Sliderule (int x, int y, int scale_length) {
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
//		process_cursor_window = false;
//		process_cursor_hairline = false;
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
//		top_auto_spacer = 0;
//		bottom_auto_spacer = 0;
//		scale_height_compensation = 0;
		os_compensation = 0;
		motion_multiplier = 0.2;
		animation_steps = 100;
		animation_delay = 50;
		animation_linear = false;
		animation_variable_speed = false;
		C = log10 (1.0 / sqrt (_PI / 4.0));
		location_360 = log10 (3.6 / _PI);
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
	~ Sliderule (void) {
		if (root != NULL) delete root; root = NULL;
		if (extra_hairlines != NULL) {
			for (int ind = 0; ind < number_of_extra_hairlines; ind++) delete extra_hairlines [ind];
			delete [] extra_hairlines;
			extra_hairlines = NULL;
		}
	}
};

typedef Sliderule * Sliderule_pointer;
//Sliderule * slide_rule = NULL;
static Sliderule * * slide_rules = NULL;
static int current_slide_rule = 0;

static void alignSlides (Rule * rule, double location) {
	if (rule -> free && rule -> free_index < 1) {rule -> slide = location; return;}
	double original_location = rule -> slide;
	for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {
		if (slide_rules [ind] != NULL) {
			Rule * r = slide_rules [ind] -> root;
			int nesting_level = slide_rules [ind] -> nesting_level;
			while (r != NULL) {
				if (! r -> stator) {
					if (! r -> free || (r -> free && rule -> free && r -> free_index > 0)) {
						if (r -> free_index == rule -> free_index) r -> slide = location;
						if (r -> free_index > rule -> free_index && r -> free_index <= rule -> free_index + nesting_level) r -> slide += location - original_location;
					}
				}
				r = r -> next;
			}
		}
	}
}

static void alignCursors (double location) {
	for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {
		if (slide_rules [ind] != NULL) {
			slide_rules [ind] -> cursor_location = location;
		}
	}
}

static double sigmoid (double x) {x *= _PI; x -= _PI * 0.5; return 0.5 + 0.5 * sin (x);}

static bool stop_threads = false;

class AnimateCursorThread : public wxThread {
public:
	double shift;
	wxWindow * w;
	AnimateCursorThread (double x, wxWindow * w) {this -> shift = x; this -> w = w; this -> Create (16384); this -> Run ();}
	virtual ExitCode Entry (void) {
		stop_threads = false;
		double location = slide_rules [current_slide_rule] -> cursor_location;
		double delta = shift - location;
		int steps = slide_rules [current_slide_rule] -> animation_variable_speed ? slide_rules [current_slide_rule] -> animation_steps : (int) absolute ((double) slide_rules [current_slide_rule] -> animation_steps * delta);
		bool linear = slide_rules [current_slide_rule] -> animation_linear;
		int animation_delay = slide_rules [current_slide_rule] -> animation_delay;
		for (int ind = 0; ind < steps; ind++) {
			double fraction = (double) ind / (double) steps;
//			for (int sub = 0; sub < config -> number_of_slide_rules; sub++) {slide_rules [sub] -> cursor_location = location + delta * (linear ? fraction : sigmoid (fraction));}
			alignCursors (location + delta * (linear ? fraction : sigmoid (fraction)));
			w -> Refresh (); Sleep (animation_delay);
			if (stop_threads) {Exit (); return Wait ();}
		}
		alignCursors (shift);
		w -> Refresh ();
		Exit ();
		return Wait ();
	}
};

class AnimateSlideThread : public wxThread {
public:
	double shift;
	wxWindow * w;
	Rule * r;
	AnimateSlideThread (Rule * r, double x, wxWindow * w) {this -> shift = x; this -> w = w; this -> r = r; this -> Create (16384); this -> Run ();}
	virtual ExitCode Entry (void) {
		stop_threads = false;
		double location = r -> slide;
		double delta = shift - location;
		int steps = slide_rules [current_slide_rule] -> animation_variable_speed ? slide_rules [current_slide_rule] -> animation_steps : (int) absolute ((double) slide_rules [current_slide_rule] -> animation_steps * delta / (double) slide_rules [current_slide_rule] -> scale_length);
		bool linear = slide_rules [current_slide_rule] -> animation_linear;
		int delay = slide_rules [current_slide_rule] -> animation_delay;
		for (int ind = 0; ind < steps; ind++) {
			double fraction = (double) ind / (double) steps;
			alignSlides (r, location + delta * (linear ? fraction : sigmoid (fraction)));
			w -> Refresh (); Sleep (delay);
			if (stop_threads) {Exit (); return Wait ();}
		}
		alignSlides (r, shift);
		w -> Refresh ();
		Exit ();
		return Wait ();
	}
};

class SlideruleFrame;

class FileReceiver : public wxFileDropTarget {
public:
	SlideruleFrame * frame;
	FileReceiver (SlideruleFrame * frame);
	virtual bool OnDropFiles (wxCoord x, wxCoord y, const wxArrayString & files);
};

class SlideruleWindow : public wxWindow {
public:
	int captured_x;
	double captured_slide;
	Rule * captured_rule;
	Scale * captured_scale;
	bool clicked_in_cursor;
	int captured_scale_length;
	wxTextCtrl * ctrl;
	double hair_relocation_shift;
	bool left_clicked;
	SlideruleWindow (wxWindow * parent, wxWindowID id) : wxWindow (parent, id) {
		SetBackgroundStyle (wxBG_STYLE_CUSTOM);
		captured_x = 0;
		captured_slide = 0.0;
		captured_rule = NULL;
		captured_scale = NULL;
		captured_scale_length = 1;
		ctrl = NULL;
		clicked_in_cursor = false;
		if (config -> donation_launch_counter <= 0) {
			wxMessageDialog buy_me_a_snack (this,
				_T ("If you like this program then you may wish to Buy Me a Beer.\nClick OK to proceed or Cancel to Buy Me a Beer later."),
				_T ("Slide Rule INFO Message"),
				wxOK | wxCANCEL | wxICON_QUESTION | wxSTAY_ON_TOP,
				wxDefaultPosition);
			if (buy_me_a_snack . ShowModal () == wxID_OK) wxLaunchDefaultBrowser (_T ("http://www.hercsmusicsystems.com/sliderule.html#Donation"));
			config -> donation_launch_counter = DONATION_FREQUENCY;
		}
	}
	~ SlideruleWindow (void) {}
	void OnErase (wxEraseEvent & event) {}
	void OnPaint (wxPaintEvent & event) {
		if (slide_rules [current_slide_rule] == NULL) return;
		wxBufferedPaintDC dc (this);
		slide_rules [current_slide_rule] -> draw (dc);
	}
	void mouse_process (wxMouseEvent & event) {
		if (captured_rule == NULL) return;
		if (ctrl != NULL) return;
		wxPoint px = event . GetPosition ();
		int delta = px . x - captured_x;
		if (captured_rule -> stator || clicked_in_cursor) alignCursors (captured_slide + (captured_rule -> motion_multiplier * (double) delta / captured_scale_length));
		else alignSlides (captured_rule, captured_slide + captured_rule -> motion_multiplier * (double) delta);
		Refresh ();
	}
	void checkHairRelocationShift (int x, int y) {
		hair_relocation_shift = 0.0;
		Sliderule * slide_rule = slide_rules [current_slide_rule];
		if (slide_rule == NULL) return;
		if (! clicked_in_cursor) return;
		if (slide_rule -> d_hairline . in_range (x, y)) hair_relocation_shift = slide_rule -> d_hairline . location;
		if (slide_rule -> display_metric_hp) {
			if (slide_rule -> metric_hp_hairline . in_range (x, y)) hair_relocation_shift = slide_rule -> metric_hp_hairline . location;
			if (slide_rule -> metric_kw_hairline . in_range (x, y)) hair_relocation_shift = slide_rule -> metric_kw_hairline . location;
		}
		if (slide_rule -> display_mechanical_hp) {
			if (slide_rule -> mechanical_hp_hairline . in_range (x, y)) hair_relocation_shift = slide_rule -> mechanical_hp_hairline . location;
			if (slide_rule -> mechanical_kw_hairline . in_range (x, y)) hair_relocation_shift = slide_rule -> mechanical_kw_hairline . location;
		}
		if (slide_rule -> hairline_360 . in_range (x, y)) hair_relocation_shift = slide_rule -> hairline_360 . location;
		if (slide_rule -> hairline_36010 . in_range (x, y)) hair_relocation_shift = slide_rule -> hairline_36010 . location;
		if (slide_rule -> hairline_360st . in_range (x, y)) hair_relocation_shift = slide_rule -> hairline_360st . location;
		if (slide_rule -> s_hairline . in_range (x, y)) hair_relocation_shift = slide_rule -> s_hairline . location;
		for (int ind = 0; ind < slide_rule -> number_of_extra_hairlines; ind++) {
			if (slide_rule -> extra_hairlines [ind] -> in_range (x, y)) hair_relocation_shift = slide_rule -> extra_hairlines [ind] -> location;
		}
	}
	void OnMouse (wxMouseEvent & event) {if (! event . Dragging ()) return; mouse_process (event);}
	void removeCtrl (void) {if (ctrl == NULL) return; RemoveChild (ctrl); delete ctrl; ctrl = NULL;}
	void checkClickedInCursor (wxPoint & location) {
		Sliderule * slide_rule = slide_rules [current_slide_rule];
		if (slide_rule == NULL) return;
		int cursor_half_width = (int) ((double) slide_rule -> scale_length * slide_rule -> cursor_factor * 0.5);
		int hairline = slide_rule -> x + (int) (slide_rule -> cursor_location * (double) slide_rule -> scale_length + (double) slide_rule -> scale_length * slide_rule -> marginal_factor);
		clicked_in_cursor = location . x > hairline - cursor_half_width && location . x < hairline + cursor_half_width;
	}
	void OnMouseDown (wxMouseEvent & event, bool left) {
		left_clicked = left;
		removeCtrl ();
		if (ctrl != NULL) return;
		Sliderule * slide_rule = slide_rules [current_slide_rule];
		if (slide_rule == NULL) return;
		wxPoint p = event . GetPosition ();
		captured_scale = slide_rule -> captureScale (p . y);
		captured_rule = slide_rule -> captureRule (p . y);
		checkClickedInCursor (p);
		if (captured_rule == NULL) return;
		captured_x = p . x;
		captured_slide = (captured_rule -> stator || clicked_in_cursor) ? slide_rule -> cursor_location : captured_rule -> slide;
		captured_scale_length = slide_rule -> scale_length;
		if (captured_scale == NULL) return;
		checkHairRelocationShift (p . x, p . y);
		if (! left || hair_relocation_shift != 0.0 || (p . x >= slide_rule -> drawed_hairline - 8.0 && p . x <= slide_rule -> drawed_hairline + 8.0)) {
			ctrl = new wxTextCtrl (this, 4, _T (""), p, wxDefaultSize, wxTE_PROCESS_ENTER);
			ctrl -> SetFocus ();
		}
	}
	void OnMouseLeftDown (wxMouseEvent & event) {OnMouseDown (event, true);}
	void OnMouseRightDown (wxMouseEvent & event) {OnMouseDown (event, false);}
	void animateSlide (Rule * r, double x) {
		Sliderule * slide_rule = slide_rules [current_slide_rule];
		if (slide_rule == NULL) return;
		x += hair_relocation_shift * (double) slide_rule -> scale_length;
		int maximal_shift = slide_rule -> scale_length + (int) ((double) slide_rule -> scale_length * slide_rule -> marginal_factor);
		if (x > maximal_shift) return;
		if (x < - maximal_shift) return;
		if (slide_rule -> animation_steps < 2) {
			alignSlides (r, x);
			Refresh ();
			return;
		}
		new AnimateSlideThread (r, x, this);
	}
	void animateCursor (double shift) {
		Sliderule * slide_rule = slide_rules [current_slide_rule];
		if (slide_rule == NULL) return;
		if (shift < - slide_rule -> marginal_factor) return;
		if (shift > 1.0 + slide_rule -> marginal_factor) return;
		shift -= hair_relocation_shift;
		if (slide_rule -> animation_steps < 2) {
			alignCursors (shift);
			Refresh ();
			return;
		}
		new AnimateCursorThread (shift, this);
	}
	void OnDigitalEnter (wxCommandEvent & event) {
		double x;
		bool ret = ctrl -> GetValue () . ToDouble (& x);
		removeCtrl ();
		Sliderule * slide_rule = slide_rules [current_slide_rule];
		if (slide_rule == NULL) return;
		if (slide_rule != NULL && ret && captured_scale != NULL && captured_rule != NULL) {
			double shift = captured_scale -> getLocation (x);
			if (clicked_in_cursor && ! captured_rule -> stator && left_clicked) animateSlide (captured_rule, (double) slide_rule -> scale_length * (slide_rule -> cursor_location - shift));
			else animateCursor (shift + captured_rule -> slide / (double) slide_rule -> scale_length);
			captured_scale = NULL;
		}
		hair_relocation_shift = 0.0;
	}
private:
	DECLARE_EVENT_TABLE()
};
BEGIN_EVENT_TABLE(SlideruleWindow, wxWindow)
EVT_PAINT(SlideruleWindow :: OnPaint)
EVT_MOTION(SlideruleWindow :: OnMouse)
EVT_LEFT_DOWN(SlideruleWindow :: OnMouseLeftDown)
EVT_RIGHT_DOWN(SlideruleWindow :: OnMouseRightDown)
EVT_TEXT_ENTER(4, SlideruleWindow :: OnDigitalEnter)
END_EVENT_TABLE()

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
			if (scale_height < 0) return slide_rule;


//static Sliderule * createSlideruleFromFile (char * file_name, wxOperatingSystemId id, int length_override = 0) {
static Sliderule * createSlideruleFromFileReader (SetupFileReader & fr, wxOperatingSystemId id, int length_override) {
//	SetupFileReader fr (file_name);
//	if (fr . file_not_found ()) return NULL;
//	if (! fr . get_id ("sliderule")) return NULL;
	Sliderule * slide_rule = new Sliderule (2, 12, length_override > 0 ? length_override : 800);
	slide_rule -> motion_multiplier = config -> motion_multiplier;
	slide_rule -> animation_steps = config -> animation_off ? 0 : config -> animation_steps;
	slide_rule -> animation_delay = config -> animation_delays;
	slide_rule -> animation_linear = config -> animation_linear;
	slide_rule -> animation_variable_speed = config -> animation_variable_speed;
	slide_rule -> draw_digital_readouts_on_stator_left = config -> readouts_on_stator_left;
	slide_rule -> draw_digital_readouts_on_stator_right = config -> readouts_on_stator_right;
	slide_rule -> draw_digital_readouts_on_slide_left = config -> readouts_on_slide_left;
	slide_rule -> draw_digital_readouts_on_slide_right = config -> readouts_on_slide_right;
	slide_rule -> draw_digital_readouts_on_main_hairline = config -> readouts_on_main_hairline;
	slide_rule -> draw_digital_readouts_on_multi_hairlines = config -> readouts_on_multi_hairlines;
	slide_rule -> draw_decimal_places = config -> readouts_decimal_places;
	slide_rule -> decimal_string = slide_rule -> draw_decimal_places < 0 ? _T ("%f") : wxString :: Format (_T ("%s.%if"), _T ("%"), slide_rule -> draw_decimal_places);
//	if (id == wxOS_UNIX_LINUX) {slide_rule -> bottom_auto_spacer = slide_rule -> top_auto_spacer = 1; slide_rule -> scale_height_compensation = 0;}
//	if (id == wxOS_WINDOWS_NT) {slide_rule -> top_auto_spacer = 1; slide_rule -> bottom_auto_spacer = 2; slide_rule -> scale_height_compensation = 1;}
//	if (id == wxOS_MAC_OSX_DARWIN) {slide_rule -> top_auto_spacer = 1; slide_rule -> bottom_auto_spacer = 2; slide_rule -> scale_height_compensation = 1;}
	if (id == wxOS_UNIX_LINUX) {slide_rule -> os_compensation = 1;}
	if (id == wxOS_WINDOWS_NT) {slide_rule -> os_compensation = 0;}
	if (id == wxOS_MAC_OSX_DARWIN) slide_rule -> draw_cursor_window = true;
	while (fr . get_id ()) {
		bool should_skip = true;
		if (fr . id ("name")) {if (! fr . get_string ()) return slide_rule; slide_rule -> name = wxString :: From8BitData (fr . symbol);}
		if (fr . id ("length")) {if (! fr . get_int ()) return slide_rule; if (length_override == 0) slide_rule -> scale_length = fr . int_symbol;}
		if (fr . id ("margin")) {if (! fr . get_float ()) return slide_rule; if (slide_rule -> root == NULL) slide_rule -> marginal_factor = fr . float_symbol;}
		if (fr . id ("cursor_width")) {if (! fr . get_float ()) return slide_rule; slide_rule -> cursor_factor = fr . float_symbol;}
		if (fr . id ("metric")) {if (! fr . get_float ()) return slide_rule; slide_rule -> metric = fr . float_symbol;}
		if (fr . id ("spacer")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertSpacer (fr . int_symbol);}
		if (fr . id ("empty_space")) {
			slide_rule -> insertRule ();
			slide_rule -> root -> empty_space = true;
			slide_rule -> root -> background_colour . red = slide_rule -> background_colour . red;
			slide_rule -> root -> background_colour . green = slide_rule -> background_colour . green;
			slide_rule -> root -> background_colour . blue = slide_rule -> background_colour . blue;
		}
		if (fr . id ("stator")) {slide_rule -> insertRule (); slide_rule -> root -> stator = true;}
		if (fr . id ("slide")) {slide_rule -> insertRule (); slide_rule -> root -> stator = false;}
		if (fr . id ("nesting_level")) {if (! fr . get_int ()) return slide_rule; slide_rule -> nesting_level = fr . int_symbol;}
		if (fr . id ("free_slide")) {
			slide_rule -> insertRule ();
			slide_rule -> root -> stator = false;
			slide_rule -> root -> free = true;
			fr . get_symbol ();
			if (fr . symbol_control == 5) slide_rule -> root -> free_index = fr . int_symbol > 0 ? fr . int_symbol : 0;
			else slide_rule -> root -> free_index = 0;
			if (fr . symbol_control == 2) should_skip = false;
		}
		if (fr . id ("channel")) {
			fr . get_symbol ();
			if (fr . symbol_control == 5) slide_rule -> insertRule (fr . int_symbol);
			else slide_rule -> insertRule (1);
			slide_rule -> root -> stator = true;
			slide_rule -> root -> channel = true;
			if (fr . symbol_control == 2) should_skip = false;
		}
		if (fr . id ("scale_F")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogF (fr . int_symbol));}
		if (fr . id ("scale_FI")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogFI (fr . int_symbol));}
		if (fr . id ("scale_G")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogG (fr . int_symbol));}
		if (fr . id ("scale_GI")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogGI (fr . int_symbol));}
		if (fr . id ("scale_K")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogK (fr . int_symbol));}
		if (fr . id ("scale_KI")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogKI (fr . int_symbol));}
		if (fr . id ("scale_J")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogJ (fr . int_symbol));}
		if (fr . id ("scale_JI")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogJI (fr . int_symbol));}
		if (fr . id ("scale_A")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogA (fr . int_symbol));}
		if (fr . id ("scale_AI")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogAI (fr . int_symbol));}
		if (fr . id ("scale_B")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogB (fr . int_symbol));}
		if (fr . id ("scale_BI")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogBI (fr . int_symbol));}
		if (fr . id ("scale_C")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogC (fr . int_symbol));}
		if (fr . id ("scale_D")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogD (fr . int_symbol));}
		if (fr . id ("scale_CI")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCI (fr . int_symbol));}
		if (fr . id ("scale_DI")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDI (fr . int_symbol));}
		if (fr . id ("scale_CF")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCF (fr . int_symbol));}
		if (fr . id ("scale_DF")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDF (fr . int_symbol));}
		if (fr . id ("scale_CIF")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCIF (fr . int_symbol));}
		if (fr . id ("scale_DIF")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDIF (fr . int_symbol));}
/* GJM -- New CFI DFI scales */
		if (fr . id ("scale_CFI")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCFI (fr . int_symbol));}
		if (fr . id ("scale_DFI")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDFI (fr . int_symbol));}
		if (fr . id ("scale_CF10")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCF10 (fr . int_symbol));}
		if (fr . id ("scale_DF10")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDF10 (fr . int_symbol));}
		if (fr . id ("scale_CIF10")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCIF10 (fr . int_symbol));}
		if (fr . id ("scale_DIF10")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDIF10 (fr . int_symbol));}
		if (fr . id ("scale_CF36")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCF36 (fr . int_symbol));}
		if (fr . id ("scale_DF36")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDF36 (fr . int_symbol));}
		if (fr . id ("scale_CIF36")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCIF36 (fr . int_symbol));}
		if (fr . id ("scale_DIF36")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDIF36 (fr . int_symbol));}
		if (fr . id ("scale_CFM")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCFM (fr . int_symbol));}
		if (fr . id ("scale_DFM")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDFM (fr . int_symbol));}
		if (fr . id ("scale_CIFM")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCIFM (fr . int_symbol));}
		if (fr . id ("scale_DIFM")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDIFM (fr . int_symbol));}
		if (fr . id ("scale_CF1M")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCF1M (fr . int_symbol));}
		if (fr . id ("scale_DF1M")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDF1M (fr . int_symbol));}
		if (fr . id ("scale_CIF1M")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogCIF1M (fr . int_symbol));}
		if (fr . id ("scale_DIF1M")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogDIF1M (fr . int_symbol));}
		if (fr . id ("scale_R1")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogR1 (fr . int_symbol));}
		if (fr . id ("scale_R2")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogR2 (fr . int_symbol));}
		if (fr . id ("scale_W1")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogW1 (fr . int_symbol));}
		if (fr . id ("scale_W2")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LogW2 (fr . int_symbol));}
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
		if (fr . id ("scale_CK")) {
			if (! fr . get_int ()) return slide_rule;
			int factor = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new Log_dk (factor, fr . int_symbol));
		}
		if (fr . id ("scale_DK")) {
			if (! fr . get_int ()) return slide_rule;
			int factor = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new Log_dk_down (factor, fr . int_symbol));
		}
		if (fr . id ("scale_Cdk")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new Log_dk (5, fr . int_symbol));}
		if (fr . id ("scale_Ddk")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new Log_dk_down (5, fr . int_symbol));}
		if (fr . id ("scale_Adk")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new Log_dk (10, fr . int_symbol));}
		if (fr . id ("scale_Bdk")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new Log_dk_down (10, fr . int_symbol));}
		if (fr . id ("scale_Kdk")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new Log_dk (15, fr . int_symbol));}
		if (fr . id ("scale_Jdk")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new Log_dk_down (15, fr . int_symbol));}
		if (fr . id ("scale_L")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LUp (fr . int_symbol));}
		if (fr . id ("scale_L_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LDown (fr . int_symbol));}
/* GJM -- New Ln scale */
		if (fr . id ("scale_Ln")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LnUp (fr . int_symbol));}
		if (fr . id ("scale_Ln_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LnDown (fr . int_symbol));}
		if (fr . id ("scale_LR1")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LR (fr . int_symbol, true, false));}
		if (fr . id ("scale_LR2")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LR (fr . int_symbol, false, true));}
		if (fr . id ("scale_LR12")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LR (fr . int_symbol, true, true));}
		if (fr . id ("scale_LW1")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LW (fr . int_symbol, true, false));}
		if (fr . id ("scale_LW2")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LW (fr . int_symbol, false, true));}
		if (fr . id ("scale_LW12")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new LW (fr . int_symbol, true, true));}
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
			if (scale_height < 0) return slide_rule;
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
			if (scale_height < 0) return slide_rule;
			slide_rule -> insertScale (new MetricDown (slide_rule -> metric, scale_height, begin_point));
//			if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new MetricDown (slide_rule -> metric, fr . int_symbol, 0.0, 1.0));
		}
		if (fr . id ("scale_metric_1_25")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new Metric (slide_rule -> metric / 4.0, fr . int_symbol, 0.0));}
		if (fr . id ("scale_metric_1_25_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new MetricDown (slide_rule -> metric / 4.0, fr . int_symbol, 0.0));}
		if (fr . id ("scale_metric_1_50")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new Metric (slide_rule -> metric / 2.0, fr . int_symbol, 0.0));}
		if (fr . id ("scale_metric_1_50_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new MetricDown (slide_rule -> metric / 2.0, fr . int_symbol, 0.0));}
		if (fr . id ("scale_imperial")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new Imperial (slide_rule -> metric / 25.4, fr . int_symbol));}
		if (fr . id ("scale_imperial_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ImperialDown (slide_rule -> metric / 25.4, fr . int_symbol));}
		if (fr . id ("scale_S")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSUp (fr . int_symbol));}
		if (fr . id ("scale_S_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSDown (fr . int_symbol));}
		if (fr . id ("scale_SA")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSAUp (fr . int_symbol));}
		if (fr . id ("scale_SA_down") || fr . id ("scale_SB")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSADown (fr . int_symbol));}
		if (fr . id ("scale_SCK")) {
			if (! fr . get_int ()) return slide_rule;
			int factor = fr . int_symbol;			
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new TrigSAUp (factor, fr . int_symbol));
		}
		if (fr . id ("scale_SDK")) {
			if (! fr . get_int ()) return slide_rule;
			int factor = fr . int_symbol;			
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new TrigSADown (factor, fr . int_symbol));
		}
		if (fr . id ("scale_TA")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTAUp (fr . int_symbol));}
		if (fr . id ("scale_TB")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTADown (fr . int_symbol));}
		if (fr . id ("scale_TCK")) {
			if (! fr . get_int ()) return slide_rule;
			int factor = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new TrigTAUp (factor, fr . int_symbol));
		}
		if (fr . id ("scale_TDK")) {
			if (! fr . get_int ()) return slide_rule;
			int factor = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new TrigTADown (factor, fr . int_symbol));
		}
		if (fr . id ("scale_T")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTUp (fr . int_symbol));}
		if (fr . id ("scale_T_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTDown (fr . int_symbol));}
/* GJM -- New high accuracy T1 scales */
		if (fr . id ("scale_T1")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigT1Up (fr . int_symbol));}
		if (fr . id ("scale_T1_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigT1Down (fr . int_symbol));}
		if (fr . id ("scale_T2")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigT2Up (fr . int_symbol));}
		if (fr . id ("scale_T2_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigT2Down (fr . int_symbol));}
		if (fr . id ("scale_ST")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSTUp (fr . int_symbol));}
		if (fr . id ("scale_ST_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSTDown (fr . int_symbol));}
/* GJM -- New high accuracy S1 scales */
		if (fr . id ("scale_S1")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigS1Up (fr . int_symbol));}
		if (fr . id ("scale_S1_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigS1Down (fr . int_symbol));}
		if (fr . id ("scale_Sg")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSgUp (fr . int_symbol));}
		if (fr . id ("scale_Sg_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSgDown (fr . int_symbol));}
		if (fr . id ("scale_Tg")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTgUp (fr . int_symbol));}
		if (fr . id ("scale_Tg_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTgDown (fr . int_symbol));}
		if (fr . id ("scale_Sr")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSrUp (fr . int_symbol));}
		if (fr . id ("scale_Sr_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSrDown (fr . int_symbol));}
		if (fr . id ("scale_Tr")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTrUp (1.0, fr . int_symbol));}
		if (fr . id ("scale_Tr_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTrDown (1.0, fr . int_symbol));}
		if (fr . id ("scale_T2r")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTrUp (0.0, fr . int_symbol));}
		if (fr . id ("scale_T2r_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTrDown (0.0, fr . int_symbol));}
		if (fr . id ("scale_Sdec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSDecUp (fr . int_symbol));}
		if (fr . id ("scale_Sdec_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSDecDown (fr . int_symbol));}
		if (fr . id ("scale_SAdec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSADecUp (fr . int_symbol));}
		if (fr . id ("scale_SAdec_down") || fr . id ("scale_SBdec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSADecDown (fr . int_symbol));}
		if (fr . id ("scale_SCKdec")) {
			if (! fr . get_int ()) return slide_rule;
			int factor = fr . int_symbol;			
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new TrigSADecUp (factor, fr . int_symbol));
		}
		if (fr . id ("scale_SDKdec")) {
			if (! fr . get_int ()) return slide_rule;
			int factor = fr . int_symbol;			
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new TrigSADecDown (factor, fr . int_symbol));
		}
		if (fr . id ("scale_TAdec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTADecUp (fr . int_symbol));}
		if (fr . id ("scale_TBdec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTADecDown (fr . int_symbol));}
		if (fr . id ("scale_TCKdec")) {
			if (! fr . get_int ()) return slide_rule;
			int factor = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new TrigTADecUp (factor, fr . int_symbol));
		}
		if (fr . id ("scale_TDKdec")) {
			if (! fr . get_int ()) return slide_rule;
			int factor = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new TrigTADecDown (factor, fr . int_symbol));
		}
		if (fr . id ("scale_Tdec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTDecUp (fr . int_symbol));}
		if (fr . id ("scale_Tdec_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigTDecDown (fr . int_symbol));}
/* GJM -- New high accuracy T1 scales */
		if (fr . id ("scale_T1dec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigT1DecUp (fr . int_symbol));}
		if (fr . id ("scale_T1dec_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigT1DecDown (fr . int_symbol));}
		if (fr . id ("scale_T2dec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigT2DecUp (fr . int_symbol));}
		if (fr . id ("scale_T2dec_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigT2DecDown (fr . int_symbol));}
		if (fr . id ("scale_STdec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSTDecUp (fr . int_symbol));}
		if (fr . id ("scale_STdec_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigSTDecDown (fr . int_symbol));}
/* GJM -- New high accuracy S1 scales */
		if (fr . id ("scale_S1dec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigS1DecUp (fr . int_symbol));}
		if (fr . id ("scale_S1dec_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigS1DecDown (fr . int_symbol));}
		if (fr . id ("scale_ISTd")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigISTdUp (fr . int_symbol));}
		if (fr . id ("scale_ISTd_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new TrigISTdDown (fr . int_symbol));}
		if (fr . id ("scale_P")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScalePUp (fr . int_symbol));}
		if (fr . id ("scale_P_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScalePDown (fr . int_symbol));}
/* GJM -- New extended P2 scales */
		if (fr . id ("scale_P2")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleP2Up (fr . int_symbol));}
		if (fr . id ("scale_P2_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleP2Down (fr . int_symbol));}
		if (fr . id ("scale_PH")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScalePHUp (fr . int_symbol));}
		if (fr . id ("scale_PH_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScalePHDown (fr . int_symbol));}
		if (fr . id ("scale_PH2")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScalePH2Up (fr . int_symbol));}
		if (fr . id ("scale_PH2_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScalePH2Down (fr . int_symbol));}
		if (fr . id ("scale_PT")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScalePTUp (fr . int_symbol));}
		if (fr . id ("scale_PT_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScalePTDown (fr . int_symbol));}
		if (fr . id ("scale_LL")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLUp (fr . int_symbol));}
		if (fr . id ("scale_LL_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLDown (fr . int_symbol));}
		if (fr . id ("scale_LLR")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLRUp (fr . int_symbol));}
		if (fr . id ("scale_LLR_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLRDown (fr . int_symbol));}
/* GJM -- New LLD Scales (Log10 Log10) */
		if (fr . id ("scale_LLD0")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD0Up (fr . int_symbol));}
		if (fr . id ("scale_LLD1")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD1Up (fr . int_symbol));}
		if (fr . id ("scale_LLD2")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD2Up (fr . int_symbol));}
		if (fr . id ("scale_LLD3")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD3Up (fr . int_symbol));}
		if (fr . id ("scale_LLD4")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD4Up (fr . int_symbol));}
		if (fr . id ("scale_LLD0_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD0Down (fr . int_symbol));}
		if (fr . id ("scale_LLD1_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD1Down (fr . int_symbol));}
		if (fr . id ("scale_LLD2_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD2Down (fr . int_symbol));}
		if (fr . id ("scale_LLD3_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD3Down (fr . int_symbol));}
		if (fr . id ("scale_LLD4_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD4Down (fr . int_symbol));}
		if (fr . id ("scale_LLD00")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD00Up (fr . int_symbol));}
		if (fr . id ("scale_LLD01")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD01Up (fr . int_symbol));}
		if (fr . id ("scale_LLD02")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD02Up (fr . int_symbol));}
		if (fr . id ("scale_LLD03")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD03Up (fr . int_symbol));}
		if (fr . id ("scale_LLD04")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD04Up (fr . int_symbol));}
		if (fr . id ("scale_LLD00_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD00Down (fr . int_symbol));}
		if (fr . id ("scale_LLD01_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD01Down (fr . int_symbol));}
		if (fr . id ("scale_LLD02_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD02Down (fr . int_symbol));}
		if (fr . id ("scale_LLD03_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD03Down (fr . int_symbol));}
		if (fr . id ("scale_LLD04_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLLD04Down (fr . int_symbol));}
		if (fr . id ("scale_LL0")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL0Up (fr . int_symbol));}
		if (fr . id ("scale_LL1")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL1Up (fr . int_symbol));}
		if (fr . id ("scale_LL2")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL2Up (fr . int_symbol));}
		if (fr . id ("scale_LL3")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL3Up (fr . int_symbol));}
		if (fr . id ("scale_LL0_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL0Down (fr . int_symbol));}
		if (fr . id ("scale_LL1_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL1Down (fr . int_symbol));}
		if (fr . id ("scale_LL2_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL2Down (fr . int_symbol));}
		if (fr . id ("scale_LL3_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL3Down (fr . int_symbol));}
		if (fr . id ("scale_LL00")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL00Up (fr . int_symbol));}
		if (fr . id ("scale_LL01")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL01Up (fr . int_symbol));}
		if (fr . id ("scale_LL02")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL02Up (fr . int_symbol));}
		if (fr . id ("scale_LL03")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL03Up (fr . int_symbol));}
		if (fr . id ("scale_LL00_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL00Down (fr . int_symbol));}
		if (fr . id ("scale_LL01_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL01Down (fr . int_symbol));}
		if (fr . id ("scale_LL02_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL02Down (fr . int_symbol));}
		if (fr . id ("scale_LL03_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleLL03Down (fr . int_symbol));}
		if (fr . id ("scale_SINH1")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHUp (1.0, fr . int_symbol));}
		if (fr . id ("scale_SINH1_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHDown (1.0, fr . int_symbol));}
		if (fr . id ("scale_SINH2")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHUp (0.0, fr . int_symbol));}
		if (fr . id ("scale_SINH2_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHDown (0.0, fr . int_symbol));}
		if (fr . id ("scale_COSH")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleCOSHUp (fr . int_symbol));}
		if (fr . id ("scale_COSH_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleCOSHDown (fr . int_symbol));}
		if (fr . id ("scale_TANH")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleTANHUp (fr . int_symbol));}
		if (fr . id ("scale_TANH_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleTANHDown (fr . int_symbol));}
		if (fr . id ("scale_SINHg1") || fr . id ("scale_SINH1g")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHgUp (1.0, fr . int_symbol));}
		if (fr . id ("scale_SINHg1_down") || fr . id ("scale_SINH1g_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHgDown (1.0, fr . int_symbol));}
		if (fr . id ("scale_SINHg2") || fr . id ("scale_SINH2g")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHgUp (0.0, fr . int_symbol));}
		if (fr . id ("scale_SINHg2_down") || fr . id ("scale_SINH2g_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHgDown (0.0, fr . int_symbol));}
		if (fr . id ("scale_COSHg")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleCOSHgUp (fr . int_symbol));}
		if (fr . id ("scale_COSHg_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleCOSHgDown (fr . int_symbol));}
		if (fr . id ("scale_TANHg")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleTANHgUp (fr . int_symbol));}
		if (fr . id ("scale_TANHg_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleTANHgDown (fr . int_symbol));}
		if (fr . id ("scale_SINHd1") || fr . id ("scale_SINH1d")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHdUp (1.0, fr . int_symbol));}
		if (fr . id ("scale_SINHd1_down") || fr . id ("scale_SINH1d_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHdDown (1.0, fr . int_symbol));}
		if (fr . id ("scale_SINHd2") || fr . id ("scale_SINH2d")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHdUp (0.0, fr . int_symbol));}
		if (fr . id ("scale_SINHd2_down") || fr . id ("scale_SINH2d_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHdDown (0.0, fr . int_symbol));}
		if (fr . id ("scale_COSHd")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleCOSHdUp (fr . int_symbol));}
		if (fr . id ("scale_COSHd_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleCOSHdDown (fr . int_symbol));}
		if (fr . id ("scale_TANHd")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleTANHdUp (fr . int_symbol));}
		if (fr . id ("scale_TANHd_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleTANHdDown (fr . int_symbol));}
		if (fr . id ("scale_SINHdec1") || fr . id ("scale_SINH1dec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHdecUp (1.0, fr . int_symbol));}
		if (fr . id ("scale_SINHdec1_down") || fr . id ("scale_SINH1dec_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHdecDown (1.0, fr . int_symbol));}
		if (fr . id ("scale_SINHdec2") || fr . id ("scale_SINH2dec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHdecUp (0.0, fr . int_symbol));}
		if (fr . id ("scale_SINHdec2_down") || fr . id ("scale_SINH2dec_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSINHdecDown (0.0, fr . int_symbol));}
		if (fr . id ("scale_COSHdec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleCOSHdecUp (fr . int_symbol));}
		if (fr . id ("scale_COSHdec_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleCOSHdecDown (fr . int_symbol));}
		if (fr . id ("scale_TANHdec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleTANHdecUp (fr . int_symbol));}
		if (fr . id ("scale_TANHdec_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleTANHdecDown (fr . int_symbol));}
		if (fr . id ("scale_SQRT")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSQRTUp (0.0, fr . int_symbol));}
		if (fr . id ("scale_SQRT_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSQRTDown (0.0, fr . int_symbol));}
		if (fr . id ("scale_SQRT2")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSQRTUp (1.0, fr . int_symbol));}
		if (fr . id ("scale_SQRT2_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSQRTDown (1.0, fr . int_symbol));}
		if (fr . id ("scale_SQRT_S")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSQRTSINEUp (fr . int_symbol));}
		if (fr . id ("scale_SQRT_S_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSQRTSINEDown (fr . int_symbol));}
		if (fr . id ("scale_SQRT_Sdec")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSQRTSINEdecUp (fr . int_symbol));}
		if (fr . id ("scale_SQRT_Sdec_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSQRTSINEdecDown (fr . int_symbol));}
		if (fr . id ("scale_SQRT_Sr")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSQRTSINErUp (fr . int_symbol));}
		if (fr . id ("scale_SQRT_Sr_down")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertScale (new ScaleSQRTSINErDown (fr . int_symbol));}
		if (fr . id ("scale_OCT")) {
			if (! fr . get_int ()) return slide_rule;
			int octave = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new ScaleOSCUp (octave, fr . int_symbol));
		}

		if (fr . id ("scale_OCT_down")) {
			if (! fr . get_int ()) return slide_rule;
			int octave = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> insertScale (new ScaleOSCDown (octave, fr . int_symbol));
		}
		if (fr . id ("corner")) {if (! fr . get_int ()) return slide_rule; slide_rule -> corner = fr . int_symbol;}
		if (fr . id ("rule_colour")) {
			if (! fr . get_int ()) return slide_rule; slide_rule -> rule_colour . red = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> rule_colour . green = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> rule_colour . blue = fr . int_symbol;
		}
		if (fr . id ("marker_colour")) {
			if (! fr . get_int ()) return slide_rule; slide_rule -> marker_colour . red = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> marker_colour . green = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> marker_colour . blue = fr . int_symbol;
		}
		if (fr . id ("hairline_colour")) {
			if (! fr . get_int ()) return slide_rule; slide_rule -> hairline_colour . red = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> hairline_colour . green = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> hairline_colour . blue = fr . int_symbol;
		}
		if (fr . id ("multi_hairline_colour")) {
			if (! fr . get_int ()) return slide_rule; slide_rule -> multi_hairlines_colour . red = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> multi_hairlines_colour . green = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> multi_hairlines_colour . blue = fr . int_symbol;
		}
		if (fr . id ("background_colour")) {
			if (! fr . get_int ()) return slide_rule; slide_rule -> background_colour . red = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> background_colour . green = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> background_colour . blue = fr . int_symbol;
		}
		if (fr . id ("background_marker_colour")) {
			if (! fr . get_int ()) return slide_rule; slide_rule -> background_marker_colour . red = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> background_marker_colour . green = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> background_marker_colour . blue = fr . int_symbol;
		}
		if (fr . id ("cursor_colour")) {
			if (! fr . get_int ()) return slide_rule; slide_rule -> cursor_colour . red = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> cursor_colour . green = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> cursor_colour . blue = fr . int_symbol;
		}
		if (fr . id ("cursor_transparency")) {if (! fr . get_int ()) return slide_rule; slide_rule -> cursor_colour . alpha = fr . int_symbol;}
		if (fr . id ("no_borderline")) slide_rule -> no_borderline = true;
		if (fr . id ("borderline")) slide_rule -> no_borderline = false;
		if (fr . id ("motion")) {if (! fr . get_float ()) return slide_rule; if (! config -> motion_override) slide_rule -> motion_multiplier = fr . float_symbol;}
		if (fr . id ("location")) {
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> x = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule;
			slide_rule -> y = fr . int_symbol;
			slide_rule -> rule_y = slide_rule -> y;
		}
		if (fr . id ("animation_steps")) {if (! fr . get_int ()) return slide_rule; if (! config -> animation_override) slide_rule -> animation_steps = fr . int_symbol;}
		if (fr . id ("animation_delays")) {if (! fr . get_int ()) return slide_rule; if (! config -> animation_override) slide_rule -> animation_delay = fr . int_symbol;}
		if (fr . id ("animation_linear")) {if (! config -> animation_override) slide_rule -> animation_linear = true;}
		if (fr . id ("animation_variable_speed")) {if (! config -> animation_override) slide_rule -> animation_variable_speed = true;}
		if (fr . id ("hairline_s")) if (slide_rule -> s_hairline . from < 0) slide_rule -> s_hairline . from = slide_rule -> rule_y; else slide_rule -> s_hairline . to = slide_rule -> rule_y;
		if (fr . id ("hairline_d")) if (slide_rule -> d_hairline . from < 0) slide_rule -> d_hairline . from = slide_rule -> rule_y; else slide_rule -> d_hairline . to = slide_rule -> rule_y;
		if (fr . id ("hairline_360")) if (slide_rule -> hairline_360 . from < 0) slide_rule -> hairline_360 . from = slide_rule -> rule_y; else slide_rule -> hairline_360 . to = slide_rule -> rule_y;
		if (fr . id ("hairline_36010")) if (slide_rule -> hairline_36010 . from < 0) slide_rule -> hairline_36010 . from = slide_rule -> rule_y; else slide_rule -> hairline_36010 . to = slide_rule -> rule_y;
		if (fr . id ("hairline_360st")) if (slide_rule -> hairline_360st . from < 0) slide_rule -> hairline_360st . from = slide_rule -> rule_y; else slide_rule -> hairline_360st . to = slide_rule -> rule_y;
		if (fr . id ("hairline_kw")) {
			if (slide_rule -> metric_kw_hairline . from < 0) slide_rule -> metric_kw_hairline . from = slide_rule -> rule_y; else slide_rule -> metric_kw_hairline . to = slide_rule -> rule_y;
			if (slide_rule -> mechanical_kw_hairline . from < 0) slide_rule -> mechanical_kw_hairline . from = slide_rule -> rule_y; else slide_rule -> mechanical_kw_hairline . to = slide_rule -> rule_y;
		}
		if (fr . id ("hairline_hp")) {
			if (slide_rule -> metric_hp_hairline . from < 0) slide_rule -> metric_hp_hairline . from = slide_rule -> rule_y; else slide_rule -> metric_hp_hairline . to = slide_rule -> rule_y;
			if (slide_rule -> mechanical_hp_hairline . from < 0) slide_rule -> mechanical_hp_hairline . from = slide_rule -> rule_y; else slide_rule -> mechanical_hp_hairline . to = slide_rule -> rule_y;
		}
		if (fr . id ("mechanical_hp")) slide_rule -> display_mechanical_hp = true;
		if (fr . id ("metric_hp")) slide_rule -> display_metric_hp = true;
		if (fr . id ("extra_hairlines")) {if (! fr . get_int ()) return slide_rule; slide_rule -> insertExtraHairlines (fr . int_symbol);}
		if (fr . id ("hairline")) {
			if (! fr . get_int ()) return slide_rule;
			int hairline_index = fr . int_symbol;
			if (hairline_index < 0 || hairline_index >= slide_rule -> number_of_extra_hairlines) return slide_rule;
			if (slide_rule -> extra_hairlines [hairline_index] -> from < 0) {
				slide_rule -> extra_hairlines [hairline_index] -> from = slide_rule -> rule_y;
				if (! fr . get_float ()) return slide_rule;
				slide_rule -> extra_hairlines [hairline_index] -> location = fr . float_symbol;
			} else slide_rule -> extra_hairlines [hairline_index] -> to = slide_rule -> rule_y;
		}
		if (fr . id ("rule_visible_margins")) {
			if (! fr . get_float ()) return slide_rule;
			double left = fr . float_symbol;
			if (! fr . get_float ()) return slide_rule;
			double right = fr . float_symbol;
			slide_rule -> apply_visible_margins (left, right);
		}
		if (fr . id ("left_description")) {
			if (slide_rule -> root == NULL) return slide_rule;
			if (slide_rule -> root -> root == NULL) return slide_rule;
			fr . get_symbol ();
			while (fr . symbol_control >= 4 && fr . symbol_control <= 6) {
				switch (fr . symbol_control) {
				case 4: slide_rule -> root -> root -> left_text = wxString :: From8BitData (fr . symbol); slide_rule -> root -> root -> left_text_justify_right = true; break;
				case 5: slide_rule -> root -> root -> left_text_pixel_shift = fr . int_symbol; slide_rule -> root -> root -> left_text_justify_right = false; break;
				case 6: slide_rule -> root -> root -> left_text_relative_shift = fr . float_symbol; slide_rule -> root -> root -> left_text_justify_right = false; break;
				default: break;
				}
				fr . get_symbol ();
			}
			should_skip = false;
		}
		if (fr . id ("right_description")) {
			if (slide_rule -> root == NULL) return slide_rule;
			if (slide_rule -> root -> root == NULL) return slide_rule;
			fr . get_symbol ();
			while (fr . symbol_control >= 4 && fr . symbol_control <= 6) {
				switch (fr . symbol_control) {
				case 4: slide_rule -> root -> root -> right_text = wxString :: From8BitData (fr . symbol); slide_rule -> root -> root -> right_text_justify_right = true; break;
				case 5: slide_rule -> root -> root -> right_text_pixel_shift = fr . int_symbol; slide_rule -> root -> root -> right_text_justify_right = false; break;
				case 6: slide_rule -> root -> root -> right_text_relative_shift = fr . float_symbol; slide_rule -> root -> root -> right_text_justify_right = false; break;
				default: break;
				}
				fr . get_symbol ();
			}
			should_skip = false;
		}
		if (fr . id ("middle_description")) {
			if (slide_rule -> root == NULL) return slide_rule;
			if (slide_rule -> root -> root == NULL) return slide_rule;
			if (! fr . get_string ()) return slide_rule;
			slide_rule -> root -> root -> middle_text = wxString :: From8BitData (fr . symbol);
		}
		if (fr . id ("highlight")) {
			if (slide_rule -> root == NULL) return slide_rule;
			if (slide_rule -> root -> root == NULL) return slide_rule;
			slide_rule -> root -> root -> highlight = true;
			slide_rule -> root -> root -> build_alternate_lanczos_colours = true;
			if (! fr . get_int ()) return slide_rule; slide_rule -> root -> root -> highlight_colour . red = (unsigned char) fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> root -> root -> highlight_colour . green = (unsigned char) fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> root -> root -> highlight_colour . blue = (unsigned char) fr . int_symbol;
			slide_rule -> root -> root -> highlight_colour . alpha = 255;
		}
		if (fr . id ("highlight_extensions")) {
			if (slide_rule -> root == NULL) return slide_rule;
			if (slide_rule -> root -> root == NULL) return slide_rule;
			if (! fr . get_float ()) return slide_rule; slide_rule -> root -> root -> left_highlight_relative_shift = fr . float_symbol;
			if (! fr . get_float ()) return slide_rule; slide_rule -> root -> root -> right_highlight_relative_shift = fr . float_symbol;
		}
		if (fr . id ("highlight_left")) {
			if (slide_rule -> root == NULL) return slide_rule;
			if (slide_rule -> root -> root == NULL) return slide_rule;
			fr . get_symbol ();
			while (fr . symbol_control >= 5 && fr . symbol_control <= 6) {
				switch (fr . symbol_control) {
				case 5: slide_rule -> root -> root -> left_highlight_pixel_shift = fr . int_symbol; break;
				case 6: slide_rule -> root -> root -> left_highlight_relative_shift = fr . float_symbol; break;
				default: break;
				}
				fr . get_symbol ();
			}
			should_skip = false;
		}
		if (fr . id ("highlight_right")) {
			if (slide_rule -> root == NULL) return slide_rule;
			if (slide_rule -> root -> root == NULL) return slide_rule;
			fr . get_symbol ();
			while (fr . symbol_control >= 5 && fr . symbol_control <= 6) {
				switch (fr . symbol_control) {
				case 5: slide_rule -> root -> root -> right_highlight_pixel_shift = fr . int_symbol; break;
				case 6: slide_rule -> root -> root -> right_highlight_relative_shift = fr . float_symbol; break;
				default: break;
				}
				fr . get_symbol ();
			}
			should_skip = false;
		}
		if (fr . id ("scale_marker_colour")) {
			if (slide_rule -> root == NULL) return slide_rule;
			if (slide_rule -> root -> root == NULL) return slide_rule;
			slide_rule -> root -> root -> build_alternate_lanczos_colours = true;
			if (! fr . get_int ()) return slide_rule; slide_rule -> root -> root -> marking_colour . red = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> root -> root -> marking_colour . green = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> root -> root -> marking_colour . blue = fr . int_symbol;
			slide_rule -> root -> root -> build_individual_marker_lanczos_colour = true;
		}
		if (fr . id ("scale_reverse_marker_colour")) {
			if (slide_rule -> root == NULL) return slide_rule;
			if (slide_rule -> root -> root == NULL) return slide_rule;
			slide_rule -> root -> root -> build_alternate_lanczos_colours = true;
			if (! fr . get_int ()) return slide_rule; slide_rule -> root -> root -> red_marking_colour . red = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> root -> root -> red_marking_colour . green = fr . int_symbol;
			if (! fr . get_int ()) return slide_rule; slide_rule -> root -> root -> red_marking_colour . blue = fr . int_symbol;
			slide_rule -> root -> root -> build_individual_reversed_marker_lanczos_colour = true;
		}
		if (fr . id ("no_pi")) slide_rule -> no_pi = true;
		if (fr . id ("no_e")) slide_rule -> no_e = true;
		if (fr . id ("no_c")) slide_rule -> no_c = true;
		if (fr . id ("no_c1")) slide_rule -> no_c1 = true;
		if (fr . id ("no_degree")) slide_rule -> no_degree = true;
		if (fr . id ("no_m")) slide_rule -> no_m = true;
		if (fr . id ("no_mm")) slide_rule -> no_mm = true;
		if (fr . id ("draw_pi")) slide_rule -> no_pi = false;
		if (fr . id ("draw_e")) slide_rule -> no_e = false;
		if (fr . id ("draw_c")) slide_rule -> no_c = false;
		if (fr . id ("draw_c1")) slide_rule -> no_c1 = false;
		if (fr . id ("draw_degree")) slide_rule -> no_degree = false;
		if (fr . id ("draw_m")) slide_rule -> no_m = false;
		if (fr . id ("draw_mm")) slide_rule -> no_mm = false;
		if (fr . id ("marker_height")) {
			if (! fr . get_int ()) return slide_rule;
			int index = fr . int_symbol;
			if (index < 0 || index >= 16) return slide_rule;
			if (! fr . get_float ()) return slide_rule;
			slide_rule -> marker_fractions [index] = fr . float_symbol;
		}
		if (fr . id ("horizontal_line")) {
			if (! fr . get_int ()) return slide_rule;
			if (fr . int_symbol >= 0 && fr . int_symbol < 16) slide_rule -> draw_lines [fr . int_symbol] = true;
		}
		if (fr . id ("no_horizontal_line")) {
			if (! fr . get_int ()) return slide_rule;
			if (fr . int_symbol >= 0 && fr . int_symbol < 16) slide_rule -> draw_lines [fr . int_symbol] = false;
		}
		if (fr . id ("horizontal_extensions")) {
			if (slide_rule -> root == NULL) return slide_rule;
			if (slide_rule -> root -> root == NULL) return slide_rule;
			if (! fr . get_float ()) return slide_rule; slide_rule -> root -> root -> left_horizontal_extension = fr . float_symbol;
			if (! fr . get_float ()) return slide_rule; slide_rule -> root -> root -> right_horizontal_extension = fr . float_symbol;
		}
		if (fr . id ("scale_extensions")) {
			if (! fr . get_float ()) return slide_rule; slide_rule -> left_extension = fr . float_symbol;
			if (! fr . get_float ()) return slide_rule; slide_rule -> right_extension = fr . float_symbol;
		}
		if (fr . id ("ha")) {
			if (slide_rule -> root == NULL) return slide_rule;
			if (! fr . get_float ()) return slide_rule;
			slide_rule -> root -> HA_SCALING = fr . float_symbol;
			if (! fr . get_string ()) return slide_rule;
			SetupFileReader ffr (fr . symbol);
			if (ffr . file_not_found ()) return slide_rule;
			ffr . get_symbol ();
			while (ffr . symbol_control >= 4 && ffr . symbol_control <= 6) {
				double d1 = ffr . float_symbol; ffr . get_float ();
				double d2 = ffr . float_symbol; ffr . get_float ();
				double d3 = ffr . float_symbol;
				if (d1 == 0.0) {
					ffr . get_float ();
					double d4 = ffr . float_symbol; ffr . get_float ();
					double d5 = ffr . float_symbol;
					slide_rule -> root -> insert_HA (d1, d2, d3, d4, d5);
				} else {
					ffr . get_string ();
					slide_rule -> root -> insert_HA (d1, d2, d3, ffr . symbol);
				}
				ffr . get_symbol ();
			}
		}
		if (should_skip) fr . skip ();
	}
	slide_rule -> close ();
	return slide_rule;
}

static void createSliderulesFromFile (char * file_name, wxOperatingSystemId id, int length_override) {
	SetupFileReader fr (file_name);
	if (fr . file_not_found ()) return;
	current_slide_rule = 0;
	bool at_least_one_created = false;;
	while (fr . get_id ("sliderule") && current_slide_rule < config -> number_of_slide_rules) {
		Sliderule * s = createSlideruleFromFileReader (fr, id, length_override);
		if (s != NULL) {
			at_least_one_created = true;
			if (slide_rules [current_slide_rule] != NULL) {
				delete slide_rules [current_slide_rule];
				slide_rules [current_slide_rule] = NULL;
			}
			slide_rules [current_slide_rule++] = s;
		}
	}
	if (at_least_one_created) {
		for (int ind = current_slide_rule; ind < config -> number_of_slide_rules; ind++) {
			if (slide_rules [ind] != NULL) {delete slide_rules [ind]; slide_rules [ind] = NULL;}
		}
	}
	current_slide_rule = 0;
}

class SlideruleFrame : public wxFrame {
private:
	wxOperatingSystemId osid;
	wxHtmlHelpController * help;
	SlideruleWindow * sliderule_window;
	int current_length;
	bool internal_check_update;
public:
	void resize_this (void) {
//		wxPoint points [1000];
//		points [0] = wxPoint (0, 0);
//		points [1] = wxPoint (slide_rule -> root -> rule_length, 0);
//		points [2] = wxPoint (slide_rule -> root -> rule_length, slide_rule -> root -> rule_height);
//		points [3] = wxPoint (0, slide_rule -> root -> rule_height);
//		SetShape (wxRegion (4, points));
		current_length = 800;
		Sliderule * slide_rule = slide_rules [current_slide_rule];
		if (slide_rule == NULL) return;
		current_length = slide_rule -> scale_length;
		if (! IsMaximized ()) this -> SetSize ((int) ((double) slide_rule -> scale_length * (1.0 + slide_rule -> marginal_factor * 2.0)) + slide_rule -> x * 2 + 8, slide_rule -> rule_y + slide_rule -> y + 40 + (osid == wxOS_WINDOWS_NT ? 24 : 0));
		this -> SetTitle (slide_rule -> name);
		wxMenuBar * bar = GetMenuBar ();
		if (bar == NULL) return;
		internal_check_update = true;
		for (int ind = 301; ind <= 317; ind++) {
			int length = 200 + (ind - 301) * 100;
			bar -> Check (ind, slide_rule -> scale_length == length);
		}
		internal_check_update = false;
	}
/*	void synchronise_slide_rules (int to, int from) {
		if (to < 0 || to >= config -> number_of_slide_rules) return;
		if (from < 0 || from >= config -> number_of_slide_rules) return;
		Sliderule * source = slide_rules [from]; if (source == NULL) return;
		Sliderule * destination = slide_rules [to]; if (destination == NULL) return;
		Rule * srp = source -> root;
		Rule * drp = destination -> root;
		while (srp != NULL && drp != NULL) {
			if (srp -> free && drp -> free) drp -> slide = srp -> slide;
			srp = srp -> next; drp = drp -> next;
		}
	}*/
	void change_side (int side) {
		if (side < 0 || side >= config -> number_of_slide_rules) return;
		if (slide_rules [side] == NULL) return;
		//synchronise_slide_rules (side, current_slide_rule);
		current_slide_rule = side;
		resize_this ();
		Refresh ();
	}
	void enableSideMenu (void) {
		wxMenuBar * bar = GetMenuBar ();
		if (bar == NULL) return;
		for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {
			bar -> Enable (701 + ind, slide_rules [ind] != NULL);
		}
		internal_check_update = true;
		bar -> Check (701, true);
		internal_check_update = false;
	}
	SlideruleFrame (wxOperatingSystemId osid, wxWindow * parent, wxWindowID id, const wxString & title) :
	wxFrame (parent, id, title, wxPoint (20, 100),
//	wxFrame (parent, id, title, wxPoint (1700, 700),
		wxSize (900, 400),
			 wxDEFAULT_FRAME_STYLE) {// | wxFRAME_SHAPED) {
//			wxPoint points [4] = {wxPoint (0, 0), wxPoint (300, 0), wxPoint (300, 50), wxPoint (0, 50)};
//			SetShape (wxRegion (4, points));
		//if (SetShape (wxRegion (0, 0, 600, 400))) wxMessageBox (_T ("yes"), _T ("yes"), wxOK, NULL);
		//else wxMessageBox (_T ("no"), _T ("no"), wxOK, NULL);
		this -> osid = osid;
//		resize_this ();
		sliderule_window = new SlideruleWindow (this, -1);
		sliderule_window -> SetSize (10, 10, 200, 60);
		sliderule_window -> SetDropTarget (new FileReceiver (this));
		help = NULL;
		if (config != NULL && config -> menu_inactive) return;
		wxMenuBar * bar = new wxMenuBar ();
		wxMenu * m1 = new wxMenu ();
		m1 -> Append (101, _T ("Load ...	L"));
		m1 -> Append (103, _T ("Re-Load	R"));
		m1 -> Append (102, _T ("Exit"));
		wxMenu * m7 = new wxMenu ();
		if (config -> number_of_slide_rules > 0) m7 -> AppendRadioItem (701, _T ("Front	F1"));
		if (config -> number_of_slide_rules > 1) m7 -> AppendRadioItem (702, _T ("Back	F2"));
		if (config -> number_of_slide_rules > 2) m7 -> AppendRadioItem (703, _T ("Both sides	F3"));
		if (config -> number_of_slide_rules > 3) m7 -> AppendRadioItem (704, _T ("Side 4	F4"));
		if (config -> number_of_slide_rules > 4) m7 -> AppendRadioItem (705, _T ("Side 5	F5"));
		if (config -> number_of_slide_rules > 5) m7 -> AppendRadioItem (706, _T ("Side 6	F6"));
		if (config -> number_of_slide_rules > 6) m7 -> AppendRadioItem (707, _T ("Side 7	F7"));
		if (config -> number_of_slide_rules > 7) m7 -> AppendRadioItem (708, _T ("Side 8	F8"));
		if (config -> number_of_slide_rules > 8) m7 -> AppendRadioItem (709, _T ("Side 9	F9"));
		if (config -> number_of_slide_rules > 9) m7 -> AppendRadioItem (710, _T ("Side 10	F10"));
		if (config -> number_of_slide_rules > 9) m7 -> AppendRadioItem (711, _T ("Side 11	F11"));
		if (config -> number_of_slide_rules > 10) m7 -> AppendRadioItem (712, _T ("Side 12	F12"));
		wxMenu * m2 = new wxMenu ();
		m2 -> AppendCheckItem (201, _T ("Readouts on stator left	Q"));
		m2 -> AppendCheckItem (202, _T ("Readouts on stator right	W"));
		m2 -> AppendCheckItem (203, _T ("Readouts on slide left	A"));
		m2 -> AppendCheckItem (204, _T ("Readouts on slide right	S"));
		m2 -> AppendCheckItem (205, _T ("Readouts on main hairline	Z"));
		m2 -> AppendCheckItem (206, _T ("Readouts on other hairlines	X"));
		m2 -> AppendSeparator ();
		m2 -> AppendRadioItem (209, _T ("Decimal Places not specified"));
		m2 -> AppendRadioItem (210, _T ("Decimal Places = 0"));
		m2 -> AppendRadioItem (211, _T ("Decimal Places = 1"));
		m2 -> AppendRadioItem (212, _T ("Decimal Places = 2"));
		m2 -> AppendRadioItem (213, _T ("Decimal Places = 3"));
		m2 -> AppendRadioItem (214, _T ("Decimal Places = 4"));
		m2 -> AppendRadioItem (215, _T ("Decimal Places = 5"));
		m2 -> AppendRadioItem (216, _T ("Decimal Places = 6"));
		m2 -> AppendRadioItem (217, _T ("Decimal Places = 7"));
		m2 -> AppendRadioItem (218, _T ("Decimal Places = 8"));
		m2 -> AppendRadioItem (219, _T ("Decimal Places = 9"));
		m2 -> AppendRadioItem (220, _T ("Decimal Places = 10"));
		wxMenu * m3 = new wxMenu ();
		m3 -> AppendRadioItem (301, _T ("200"));
		m3 -> AppendRadioItem (302, _T ("300"));
		m3 -> AppendRadioItem (303, _T ("400"));
		m3 -> AppendRadioItem (304, _T ("500"));
		m3 -> AppendRadioItem (305, _T ("600"));
		m3 -> AppendRadioItem (306, _T ("700"));
		m3 -> AppendRadioItem (307, _T ("800"));
		m3 -> AppendRadioItem (308, _T ("900"));
		m3 -> AppendRadioItem (309, _T ("1000"));
		m3 -> AppendRadioItem (310, _T ("1100"));
		m3 -> AppendRadioItem (311, _T ("1200"));
		m3 -> AppendRadioItem (312, _T ("1300"));
		m3 -> AppendRadioItem (313, _T ("1400"));
		m3 -> AppendRadioItem (314, _T ("1500"));
		m3 -> AppendRadioItem (315, _T ("1600"));
		m3 -> AppendRadioItem (316, _T ("1700"));
		m3 -> AppendRadioItem (317, _T ("1800"));
		m3 -> AppendCheckItem (391, _T ("Length override"));
		wxMenu * m4 = new wxMenu ();
		m4 -> AppendRadioItem (401, _T ("X 0.01"));
		m4 -> AppendRadioItem (402, _T ("X 0.02"));
		m4 -> AppendRadioItem (403, _T ("X 0.05"));
		m4 -> AppendRadioItem (404, _T ("X 0.1"));
		m4 -> AppendRadioItem (405, _T ("X 0.2"));
		m4 -> AppendRadioItem (406, _T ("X 0.5"));
		m4 -> AppendRadioItem (407, _T ("X 1.0"));
		m4 -> AppendRadioItem (408, _T ("X 1.5"));
		m4 -> AppendRadioItem (409, _T ("X 2.0"));
		m4 -> AppendRadioItem (410, _T ("X 5.0"));
		m4 -> AppendRadioItem (411, _T ("X 10.0"));
		m4 -> AppendRadioItem (412, _T ("X 20.0"));
		m4 -> AppendCheckItem (491, _T ("Motion override"));
		wxMenu * m5 = new wxMenu ();
		m5 -> AppendCheckItem (501, _T ("Animation ON"));
		m5 -> AppendRadioItem (541, _T ("5 steps"));
		m5 -> AppendRadioItem (542, _T ("10 steps"));
		m5 -> AppendRadioItem (543, _T ("20 steps"));
		m5 -> AppendRadioItem (544, _T ("50 steps"));
		m5 -> AppendRadioItem (545, _T ("100 steps"));
		m5 -> AppendRadioItem (546, _T ("150 steps"));
		m5 -> AppendRadioItem (547, _T ("200 steps"));
		m5 -> AppendCheckItem (561, _T ("Linear"));
		m5 -> AppendCheckItem (562, _T ("Variable speed"));
		m5 -> AppendRadioItem (571, _T ("delay 1 frame"));
		m5 -> AppendRadioItem (572, _T ("delay 2 frames"));
		m5 -> AppendRadioItem (573, _T ("delay 5 frames"));
		m5 -> AppendRadioItem (574, _T ("delay 10 frames"));
		m5 -> AppendRadioItem (575, _T ("delay 20 frames"));
		m5 -> AppendRadioItem (576, _T ("delay 50 frames"));
		m5 -> AppendRadioItem (577, _T ("delay 100 frames"));
		m5 -> AppendCheckItem (591, _T ("Animation override"));
		wxMenu * m6 = new wxMenu ();
		m6 -> Append (600, _T ("About"));
		m6 -> Append (601, _T ("User manual"));
		m6 -> Append (602, _T ("Buy Me a Beer"));
		m6 -> Append (603, _T ("Check the original website"));
		bar -> Append (m1, _T ("File"));
		bar -> Append (m7, _T ("Sides"));
		bar -> Append (m2, _T ("Readouts"));
		bar -> Append (m3, _T ("Length"));
		bar -> Append (m4, _T ("Motion"));
		bar -> Append (m5, _T ("Animation"));
		bar -> Append (m6, _T ("Help"));
		SetMenuBar (bar);
		internal_check_update = true;
		bar -> Check (201, config -> readouts_on_stator_left);
		bar -> Check (202, config -> readouts_on_stator_right);
		bar -> Check (203, config -> readouts_on_slide_left);
		bar -> Check (204, config -> readouts_on_slide_right);
		bar -> Check (205, config -> readouts_on_main_hairline);
		bar -> Check (206, config -> readouts_on_multi_hairlines);
//		for (int ind = 301; ind <= 317; ind++) {
//			int length = 200 + (ind - 301) * 100;
//			bar -> Check (ind, config -> length == length);
//		}
		resize_this ();
		bar -> Check (391, config -> length_override);
		bar -> Check (401, config -> motion_multiplier == 0.01);
		bar -> Check (402, config -> motion_multiplier == 0.02);
		bar -> Check (403, config -> motion_multiplier == 0.05);
		bar -> Check (404, config -> motion_multiplier == 0.1);
		bar -> Check (405, config -> motion_multiplier == 0.2);
		bar -> Check (406, config -> motion_multiplier == 0.5);
		bar -> Check (407, config -> motion_multiplier == 1.0);
		bar -> Check (408, config -> motion_multiplier == 1.5);
		bar -> Check (409, config -> motion_multiplier == 2.0);
		bar -> Check (410, config -> motion_multiplier == 5.0);
		bar -> Check (411, config -> motion_multiplier == 10.0);
		bar -> Check (412, config -> motion_multiplier == 20.0);
		bar -> Check (491, config -> motion_override);
		bar -> Check (501, ! config -> animation_off);
		bar -> Check (541, config -> animation_steps == 5);
		bar -> Check (542, config -> animation_steps == 10);
		bar -> Check (543, config -> animation_steps == 20);
		bar -> Check (544, config -> animation_steps == 50);
		bar -> Check (545, config -> animation_steps == 100);
		bar -> Check (546, config -> animation_steps == 150);
		bar -> Check (547, config -> animation_steps == 200);
		bar -> Check (561, config -> animation_linear);
		bar -> Check (562, config -> animation_variable_speed);
		bar -> Check (571, config -> animation_delays == 1);
		bar -> Check (572, config -> animation_delays == 2);
		bar -> Check (573, config -> animation_delays == 5);
		bar -> Check (574, config -> animation_delays == 10);
		bar -> Check (575, config -> animation_delays == 20);
		bar -> Check (576, config -> animation_delays == 50);
		bar -> Check (577, config -> animation_delays == 100);
		bar -> Check (591, config -> animation_override);
		if (config -> readouts_decimal_places < 0 && config -> readouts_decimal_places > 10) bar -> Check (209, true);
		else bar -> Check (210 + config -> readouts_decimal_places, true);
		internal_check_update = false;
		help = new wxHtmlHelpController ();
		help -> AddBook (wxFileName (_T ("sliderule_manual.hhp")));
		enableSideMenu ();
	}
	void LoadFile (char * file_name) {
		createSliderulesFromFile (file_name, this -> osid, config == NULL ? 0 : config -> length_override ? config -> length : 0);
		if (config != NULL) {strcpy (config -> last_loaded, file_name); config -> changed = true;}
		resize_this ();
		Refresh ();
	}
	void OnLoadFile (wxCommandEvent & event) {
		if (internal_check_update) return;
		wxFileDialog picker (this);
		picker . SetWildcard (_T ("Slide rule files (*.txt)|*.txt|All files (*.*)|*.*"));
		if (picker . ShowModal () == wxID_OK) {
			stop_threads = true;
			sliderule_window -> captured_rule = NULL;
			wxString wxfilename = picker . GetDirectory () + _T ("/") + picker . GetFilename ();
			char file_name [256];
			for (unsigned int ind = 0; ind < wxfilename . length (); ind++) {
				file_name [ind] = (char) wxfilename . c_str () [ind];
			}
			file_name [wxfilename . length ()] = '\0';
			LoadFile (file_name);
		}
		enableSideMenu ();
	}
	void OnReLoadFile (wxCommandEvent & event) {
		if (internal_check_update) return;
		stop_threads = true;
		sliderule_window -> captured_rule = NULL;
		createSliderulesFromFile (config -> last_loaded, this -> osid, config -> length_override ? config -> length : 0);
		resize_this ();
		Refresh ();
		enableSideMenu ();
	}
	void OnSeeNumbers (wxCommandEvent & event) {
		if (internal_check_update) return;
		wxMenuBar * bar = GetMenuBar ();
		if (bar == NULL) return;
		config -> changed = true;
		switch (event . GetId ()) {
		case 209: config -> readouts_decimal_places = -1; break;
		case 210: config -> readouts_decimal_places = 0; break;
		case 211: config -> readouts_decimal_places = 1; break;
		case 212: config -> readouts_decimal_places = 2; break;
		case 213: config -> readouts_decimal_places = 3; break;
		case 214: config -> readouts_decimal_places = 4; break;
		case 215: config -> readouts_decimal_places = 5; break;
		case 216: config -> readouts_decimal_places = 6; break;
		case 217: config -> readouts_decimal_places = 7; break;
		case 218: config -> readouts_decimal_places = 8; break;
		case 219: config -> readouts_decimal_places = 9; break;
		case 220: config -> readouts_decimal_places = 10; break;
		default: break;
		}
		config -> readouts_on_stator_left = bar -> IsChecked (201);
		config -> readouts_on_stator_right = bar -> IsChecked (202);
		config -> readouts_on_slide_left = bar -> IsChecked (203);
		config -> readouts_on_slide_right = bar -> IsChecked (204);
		config -> readouts_on_main_hairline = bar -> IsChecked (205);
		config -> readouts_on_multi_hairlines = bar -> IsChecked (206);
		for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {
			Sliderule * slide_rule = slide_rules [ind];
			if (slide_rule != NULL) {
				slide_rule -> draw_digital_readouts_on_stator_left = config -> readouts_on_stator_left;
				slide_rule -> draw_digital_readouts_on_stator_right = config -> readouts_on_stator_right;
				slide_rule -> draw_digital_readouts_on_slide_left = config -> readouts_on_slide_left;
				slide_rule -> draw_digital_readouts_on_slide_right = config -> readouts_on_slide_right;
				slide_rule -> draw_digital_readouts_on_main_hairline = config -> readouts_on_main_hairline;
				slide_rule -> draw_digital_readouts_on_multi_hairlines = config -> readouts_on_multi_hairlines;
				slide_rule -> decimal_string = config -> readouts_decimal_places < 0 ? _T ("%f") : wxString :: Format (_T ("%s.%if"), _T ("%"), config -> readouts_decimal_places);
			}
		}
		Refresh ();
	}
	void OnLength (wxCommandEvent & event) {
		if (internal_check_update) return;
		int length = event . GetId ();
		length -= 301;
		length *= 100;
		length += 200;
		if (config == NULL) return;
		config -> length = length;
		config -> changed = true;
		for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {
			Sliderule * slide_rule = slide_rules [ind];
			if (slide_rule != NULL) slide_rule -> resize_length (length);
		}
		resize_this ();
		Refresh ();
	}
	void OnLengthOverride (wxCommandEvent & event) {
		if (internal_check_update) return;
		wxMenuBar * bar = GetMenuBar ();
		if (bar == NULL) return;
		config -> length_override = bar -> IsChecked (391);
		config -> length = current_length;
		config -> changed = true;
	}
	void OnMotion (wxCommandEvent & event) {
		if (internal_check_update) return;
		switch (event . GetId ()) {
		case 401: config -> motion_multiplier = 0.01; break;
		case 402: config -> motion_multiplier = 0.02; break;
		case 403: config -> motion_multiplier = 0.05; break;
		case 404: config -> motion_multiplier = 0.1; break;
		case 405: config -> motion_multiplier = 0.2; break;
		case 406: config -> motion_multiplier = 0.5; break;
		case 407: config -> motion_multiplier = 1.0; break;
		case 408: config -> motion_multiplier = 1.5; break;
		case 409: config -> motion_multiplier = 2.0; break;
		case 410: config -> motion_multiplier = 5.0; break;
		case 411: config -> motion_multiplier = 10.0; break;
		case 412: config -> motion_multiplier = 20.0; break;
		default: config -> motion_multiplier = 0.2; break;
		}
		config -> changed = true;
		for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {
			Sliderule * slide_rule = slide_rules [ind];
			if (slide_rule != NULL) slide_rule -> change_motion_multipliers (config -> motion_multiplier);
		}
	}
	void OnMotionOverride (wxCommandEvent & event) {
		if (internal_check_update) return;
		wxMenuBar * bar = GetMenuBar ();
		if (bar == NULL) return;
		config -> motion_override = bar -> IsChecked (491);
		config -> changed = true;
	}
	void OnAnimationON (wxCommandEvent & event) {
		if (internal_check_update) return;
		wxMenuBar * bar = GetMenuBar ();
		if (bar == NULL) return;
		config -> animation_off = ! bar -> IsChecked (501);
		config -> changed = true;
		for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {
			Sliderule * slide_rule = slide_rules [ind];
			if (slide_rule != NULL) slide_rule -> animation_steps = config -> animation_off ? 0 : config -> animation_steps;
		}
	}
	void OnAnimationSteps (wxCommandEvent & event) {
		if (internal_check_update) return;
		switch (event . GetId ()) {
		case 541: config -> animation_steps = 5; break;
		case 542: config -> animation_steps = 10; break;
		case 543: config -> animation_steps = 20; break;
		case 544: config -> animation_steps = 50; break;
		case 545: config -> animation_steps = 100; break;
		case 546: config -> animation_steps = 150; break;
		case 547: config -> animation_steps = 200; break;
		default: config -> animation_steps = 100; break;
		}
		config -> changed = true;
		for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {
			Sliderule * slide_rule = slide_rules [ind];
			if (slide_rule != NULL) slide_rule -> animation_steps = config -> animation_off ? 0 : config -> animation_steps;
		}
	}
	void OnAnimationLinear (wxCommandEvent & event) {
		if (internal_check_update) return;
		wxMenuBar * bar = GetMenuBar ();
		if (bar == NULL) return;
		config -> animation_linear = bar -> IsChecked (561);
		config -> changed = true;
		for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {
			Sliderule * slide_rule = slide_rules [ind];
			if (slide_rule != NULL) slide_rule -> animation_linear = config -> animation_linear;
		}
	}
	void OnAnimationVariableSpeed (wxCommandEvent & event) {
		if (internal_check_update) return;
		wxMenuBar * bar = GetMenuBar ();
		if (bar == NULL) return;
		config -> animation_variable_speed = bar -> IsChecked (562);
		config -> changed = true;
		for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {
			Sliderule * slide_rule = slide_rules [ind];
			if (slide_rule != NULL) slide_rule -> animation_variable_speed = config -> animation_variable_speed;
		}
	}
	void OnAnimationDelays (wxCommandEvent & event) {
		if (internal_check_update) return;
		switch (event . GetId ()) {
		case 571: config -> animation_delays = 1; break;
		case 572: config -> animation_delays = 2; break;
		case 573: config -> animation_delays = 5; break;
		case 574: config -> animation_delays = 10; break;
		case 575: config -> animation_delays = 20; break;
		case 576: config -> animation_delays = 50; break;
		case 577: config -> animation_delays = 100; break;
		default: config -> animation_delays = 50; break;
		}
		config -> changed = true;
		for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {
			Sliderule * slide_rule = slide_rules [ind];
			if (slide_rule != NULL) slide_rule -> animation_delay = config -> animation_delays;
		}
	}
	void OnChangeSide (wxCommandEvent & event) {
		if (internal_check_update) return;
		change_side (event . GetId () - 701);
	}
	void OnAnimationOverride (wxCommandEvent & event) {
		if (internal_check_update) return;
		wxMenuBar * bar = GetMenuBar ();
		if (bar == NULL) return;
		config -> animation_override = bar -> IsChecked (591);
		config -> changed = true;
	}
	void OnAbout (wxCommandEvent & event) {
		if (internal_check_update) return;
		wxMessageDialog dialog (this,
			_T ("Slide Rule Emulator.\nVersion: 1st May 2012.\nWritten by Robert Wolf.\nContribution by Greg McClure.\nEmail: robert.wolf@hercsmusicsystems.com\nWebsite: http://www.hercsmusicsystems.com"),
			_T ("Slide Rule Emulator INFO"));
		dialog . ShowModal ();
	}
	void OnUserManual (wxCommandEvent & event) {
		if (internal_check_update) return;
		help -> Display (1);
	}
	void OnMakeDonation (wxCommandEvent & event) {
		if (internal_check_update) return;
		wxLaunchDefaultBrowser (_T ("http://www.hercsmusicsystems.com/sliderule.html#Donation"));
	}
	void OnGoToWebsite (wxCommandEvent & event) {
		if (internal_check_update) return;
		wxLaunchDefaultBrowser (_T ("http://www.hercsmusicsystems.com/sliderule.html"));
	}
	void OnExit (wxCommandEvent & event) {
		if (internal_check_update) return;
		delete this;
	}
	~ SlideruleFrame (void) {stop_threads = true; if (help != NULL) delete help; help = NULL;}
private:
	DECLARE_EVENT_TABLE()
};
BEGIN_EVENT_TABLE(SlideruleFrame, wxFrame)
EVT_MENU(101, SlideruleFrame :: OnLoadFile)
EVT_MENU(103, SlideruleFrame :: OnReLoadFile)
EVT_MENU(102, SlideruleFrame :: OnExit)
EVT_MENU(201, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(202, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(203, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(204, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(205, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(206, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(209, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(210, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(211, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(212, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(213, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(214, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(215, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(216, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(217, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(218, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(219, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(220, SlideruleFrame :: OnSeeNumbers)
EVT_MENU(301, SlideruleFrame :: OnLength)
EVT_MENU(302, SlideruleFrame :: OnLength)
EVT_MENU(303, SlideruleFrame :: OnLength)
EVT_MENU(304, SlideruleFrame :: OnLength)
EVT_MENU(305, SlideruleFrame :: OnLength)
EVT_MENU(306, SlideruleFrame :: OnLength)
EVT_MENU(307, SlideruleFrame :: OnLength)
EVT_MENU(308, SlideruleFrame :: OnLength)
EVT_MENU(309, SlideruleFrame :: OnLength)
EVT_MENU(310, SlideruleFrame :: OnLength)
EVT_MENU(311, SlideruleFrame :: OnLength)
EVT_MENU(312, SlideruleFrame :: OnLength)
EVT_MENU(313, SlideruleFrame :: OnLength)
EVT_MENU(314, SlideruleFrame :: OnLength)
EVT_MENU(315, SlideruleFrame :: OnLength)
EVT_MENU(316, SlideruleFrame :: OnLength)
EVT_MENU(317, SlideruleFrame :: OnLength)
EVT_MENU(391, SlideruleFrame :: OnLengthOverride)
EVT_MENU(401, SlideruleFrame :: OnMotion)
EVT_MENU(402, SlideruleFrame :: OnMotion)
EVT_MENU(403, SlideruleFrame :: OnMotion)
EVT_MENU(404, SlideruleFrame :: OnMotion)
EVT_MENU(405, SlideruleFrame :: OnMotion)
EVT_MENU(406, SlideruleFrame :: OnMotion)
EVT_MENU(407, SlideruleFrame :: OnMotion)
EVT_MENU(408, SlideruleFrame :: OnMotion)
EVT_MENU(409, SlideruleFrame :: OnMotion)
EVT_MENU(410, SlideruleFrame :: OnMotion)
EVT_MENU(411, SlideruleFrame :: OnMotion)
EVT_MENU(412, SlideruleFrame :: OnMotion)
EVT_MENU(491, SlideruleFrame :: OnMotionOverride)
EVT_MENU(501, SlideruleFrame :: OnAnimationON)
EVT_MENU(541, SlideruleFrame :: OnAnimationSteps)
EVT_MENU(542, SlideruleFrame :: OnAnimationSteps)
EVT_MENU(543, SlideruleFrame :: OnAnimationSteps)
EVT_MENU(544, SlideruleFrame :: OnAnimationSteps)
EVT_MENU(545, SlideruleFrame :: OnAnimationSteps)
EVT_MENU(546, SlideruleFrame :: OnAnimationSteps)
EVT_MENU(547, SlideruleFrame :: OnAnimationSteps)
EVT_MENU(561, SlideruleFrame :: OnAnimationLinear)
EVT_MENU(562, SlideruleFrame :: OnAnimationVariableSpeed)
EVT_MENU(571, SlideruleFrame :: OnAnimationDelays)
EVT_MENU(573, SlideruleFrame :: OnAnimationDelays)
EVT_MENU(573, SlideruleFrame :: OnAnimationDelays)
EVT_MENU(574, SlideruleFrame :: OnAnimationDelays)
EVT_MENU(575, SlideruleFrame :: OnAnimationDelays)
EVT_MENU(576, SlideruleFrame :: OnAnimationDelays)
EVT_MENU(577, SlideruleFrame :: OnAnimationDelays)
EVT_MENU(591, SlideruleFrame :: OnAnimationOverride)
EVT_MENU(600, SlideruleFrame :: OnAbout)
EVT_MENU(601, SlideruleFrame :: OnUserManual)
EVT_MENU(602, SlideruleFrame :: OnMakeDonation)
EVT_MENU(603, SlideruleFrame :: OnGoToWebsite)
EVT_MENU(701, SlideruleFrame :: OnChangeSide)
EVT_MENU(702, SlideruleFrame :: OnChangeSide)
EVT_MENU(703, SlideruleFrame :: OnChangeSide)
EVT_MENU(704, SlideruleFrame :: OnChangeSide)
EVT_MENU(705, SlideruleFrame :: OnChangeSide)
EVT_MENU(706, SlideruleFrame :: OnChangeSide)
EVT_MENU(707, SlideruleFrame :: OnChangeSide)
EVT_MENU(708, SlideruleFrame :: OnChangeSide)
EVT_MENU(709, SlideruleFrame :: OnChangeSide)
EVT_MENU(710, SlideruleFrame :: OnChangeSide)
EVT_MENU(711, SlideruleFrame :: OnChangeSide)
EVT_MENU(712, SlideruleFrame :: OnChangeSide)
END_EVENT_TABLE()

SlideruleFrame * slide_rule_frame = NULL;

FileReceiver :: FileReceiver (SlideruleFrame * frame) {this -> frame = frame;}

bool FileReceiver :: OnDropFiles (wxCoord x, wxCoord y, const wxArrayString & files) {
	if (frame == NULL) return true;
	if ((int) files . GetCount () != 1) return true;
	char file_name [1024];
	for (unsigned int ind = 0; ind < files [0] . Length (); ind++) {file_name [ind] = (char) files [0] . c_str () [ind];}
	file_name [files [0] . Length ()] = '\0';
	frame -> LoadFile (file_name);
	frame -> enableSideMenu ();
	return true;
}

class SlideruleApp : public wxApp {
public:
	bool createDefaultSliderule (wxOperatingSystemId id) {
		Sliderule * slide_rule = new Sliderule (1, 12, config != NULL ? config -> length : 800);
//		if (id == wxOS_UNIX_LINUX) {slide_rule -> bottom_auto_spacer = slide_rule -> top_auto_spacer = 1; slide_rule -> scale_height_compensation = 0;}
//		if (id == wxOS_WINDOWS_NT) {slide_rule -> top_auto_spacer = 1; slide_rule -> bottom_auto_spacer = 2; slide_rule -> scale_height_compensation = 1;}
//		if (id == wxOS_MAC_OSX_DARWIN) {slide_rule -> top_auto_spacer = 1; slide_rule -> bottom_auto_spacer = 2; slide_rule -> scale_height_compensation = 1;}
		if (id == wxOS_UNIX_LINUX) {slide_rule -> os_compensation = 1;}
		if (id == wxOS_WINDOWS_NT) {slide_rule -> os_compensation = 0;}
		if (id == wxOS_MAC_OSX_DARWIN) slide_rule -> draw_cursor_window = true;
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
		slide_rules [0] = slide_rule;
		current_slide_rule = 0;
		for (int ind = 1; ind < config -> number_of_slide_rules; ind++) slide_rules [ind] = NULL;
		return true;
	}
	bool tryMaking (wxOperatingSystemId id) {
		if (config == NULL) return false;
		char file_name [256];
		if (argc < 2) {
			if (config != NULL) {
				if (strlen (config -> last_loaded) > 3) {
					createSliderulesFromFile (config -> last_loaded, id, config -> length_override ? config -> length : 0);
					if (slide_rules [0] != NULL) return true;
				}
			}
			return createDefaultSliderule (id);
		}
		wxString wx_file_name = wxString :: Format (_T ("%s"), argv [1]);
		for (unsigned int ind = 0; ind < wx_file_name . length (); ind++) {
			file_name [ind] = (char) wx_file_name . c_str () [ind];
		}
		file_name [wx_file_name . length ()] = '\0';
		createSliderulesFromFile (file_name, id, config -> length_override ? config -> length : 0);
		if (slide_rules [0] == NULL) createDefaultSliderule (id);
		return true;
	}
	bool OnInit (void) {
		config = new Config ();
		if (config == NULL) return false;
		slide_rules = new Sliderule_pointer [config -> number_of_slide_rules];
		if (slide_rules == NULL) return false;
		for (int ind = 0; ind < config -> number_of_slide_rules; ind++) slide_rules [ind] = NULL;
		wxOperatingSystemId id = wxPlatformInfo :: Get () . GetOperatingSystemId ();
		tryMaking (id);
		slide_rule_frame = new SlideruleFrame (id, NULL, -1, slide_rules [0] == NULL ? _T ("Slide rule") : slide_rules [0] -> name);
		slide_rule_frame -> Show ();
		return true;
	}
	int OnExit (void) {
		stop_threads = true;
		if (slide_rules != NULL) {
			for (int ind = 0; ind < config -> number_of_slide_rules; ind++) {delete slide_rules [ind]; slide_rules [ind] = NULL;}
			delete slide_rules;
			slide_rules = NULL;
		}
		if (config != NULL) delete config; config = NULL;
		return wxApp :: OnExit ();
	}
};

IMPLEMENT_APP (SlideruleApp);
