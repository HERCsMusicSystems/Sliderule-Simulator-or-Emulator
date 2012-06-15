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

#define USE_PACKED_COLOURS

#include "setup_file_reader.h"
#include "config.h"
#include "lanczos.h"


#include "sliderule.h"

static Config * config = NULL;

typedef Sliderule * Sliderule_pointer;
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

extern bool check_log_scales (bool & should_skip, SetupFileReader & fr, Sliderule * slide_rule);
extern bool check_decimal_tracking_scales (SetupFileReader & fr, Sliderule * slide_rule);
extern bool check_trig_scales (SetupFileReader & fr, Sliderule * slide_rule);
extern bool check_trig_dec_scales (SetupFileReader & fr, Sliderule * slide_rule);
extern bool check_trig_other_scales (SetupFileReader & fr, Sliderule * slide_rule);
extern bool check_log_log_scales (SetupFileReader & fr, Sliderule * slide_rule);
extern bool check_pythagorean_scales (SetupFileReader & fr, Sliderule * slide_rule);
extern bool check_hyperbolic_scales (SetupFileReader & fr, Sliderule * slide_rule);
extern bool check_statistical_scales (SetupFileReader & fr, Sliderule * slide_rule);

static Sliderule * createSlideruleFromFileReader (SetupFileReader & fr, wxOperatingSystemId id, int length_override) {
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
		if (! check_log_scales (should_skip, fr, slide_rule)) return slide_rule;
		if (! check_decimal_tracking_scales (fr, slide_rule)) return slide_rule;
		if (! check_trig_scales (fr, slide_rule)) return slide_rule;
		if (! check_trig_dec_scales (fr, slide_rule)) return slide_rule;
		if (! check_trig_other_scales (fr, slide_rule)) return slide_rule;
		if (! check_pythagorean_scales (fr, slide_rule)) return slide_rule;
		if (! check_statistical_scales (fr, slide_rule)) return slide_rule;
		if (! check_log_log_scales (fr, slide_rule)) return false;
		if (! check_hyperbolic_scales (fr, slide_rule)) return false;
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
		if (config -> number_of_slide_rules > 11) m7 -> AppendRadioItem (713, _T ("Side 13"));
		if (config -> number_of_slide_rules > 12) m7 -> AppendRadioItem (714, _T ("Side 14"));
		if (config -> number_of_slide_rules > 13) m7 -> AppendRadioItem (715, _T ("Side 15"));
		if (config -> number_of_slide_rules > 14) m7 -> AppendRadioItem (716, _T ("Side 16"));
		if (config -> number_of_slide_rules > 15) m7 -> AppendRadioItem (717, _T ("Side 17"));
		if (config -> number_of_slide_rules > 16) m7 -> AppendRadioItem (718, _T ("Side 18"));
		if (config -> number_of_slide_rules > 17) m7 -> AppendRadioItem (719, _T ("Side 19"));
		if (config -> number_of_slide_rules > 18) m7 -> AppendRadioItem (720, _T ("Side 20"));
		if (config -> number_of_slide_rules > 19) m7 -> AppendRadioItem (721, _T ("Side 21"));
		if (config -> number_of_slide_rules > 20) m7 -> AppendRadioItem (722, _T ("Side 22"));
		if (config -> number_of_slide_rules > 21) m7 -> AppendRadioItem (723, _T ("Side 23"));
		if (config -> number_of_slide_rules > 22) m7 -> AppendRadioItem (724, _T ("Side 24"));
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
EVT_MENU(713, SlideruleFrame :: OnChangeSide)
EVT_MENU(714, SlideruleFrame :: OnChangeSide)
EVT_MENU(715, SlideruleFrame :: OnChangeSide)
EVT_MENU(716, SlideruleFrame :: OnChangeSide)
EVT_MENU(717, SlideruleFrame :: OnChangeSide)
EVT_MENU(718, SlideruleFrame :: OnChangeSide)
EVT_MENU(719, SlideruleFrame :: OnChangeSide)
EVT_MENU(720, SlideruleFrame :: OnChangeSide)
EVT_MENU(721, SlideruleFrame :: OnChangeSide)
EVT_MENU(722, SlideruleFrame :: OnChangeSide)
EVT_MENU(723, SlideruleFrame :: OnChangeSide)
EVT_MENU(724, SlideruleFrame :: OnChangeSide)
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

extern void create_default_sliderule (Sliderule * slide_rule);

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
		create_default_sliderule (slide_rule);
//		slide_rule -> insertRule ();
//		slide_rule -> root -> stator = true;
//		slide_rule -> insertScale (new LogK (18));
//		slide_rule -> insertSpacer (4);
//		slide_rule -> insertScale (new LogA (18));
//		slide_rule -> insertRule ();
//		slide_rule -> root -> stator = false;
//		slide_rule -> insertScale (new LogB (18));
//		slide_rule -> insertSpacer (4);
//		slide_rule -> insertScale (new LogC (18));
//		slide_rule -> insertRule ();
//		slide_rule -> root -> stator = true;
//		slide_rule -> insertScale (new LogD (18));
//		slide_rule -> insertSpacer (4);
//		slide_rule -> insertScale (new LDown (18));
//		slide_rule -> close ();
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

