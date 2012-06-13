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

#ifndef _CONFIG_
#define _CONFIG_

#include "setup_file_reader.h"
#include <string.h>

#define DONATION_MADE "I_made_my_donation"
#define DONATION_FREQUENCY 19

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
	void save (void);
	Config (void);
	~ Config (void);
};

#endif

