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

#include "setup_file_reader.h"
#include <string.h>

SetupFileReader :: SetupFileReader (char * file_name) {
	setup_file = NULL;
	setup_file = fopen (file_name, "rb");
	act_char = 0;
}

SetupFileReader :: ~ SetupFileReader (void) {
	close ();
}

void SetupFileReader :: close (void) {
	if (setup_file == NULL) return;
	fclose (setup_file);
	setup_file = NULL;
}

bool SetupFileReader :: file_not_found (void) {
	if (setup_file == NULL) return true;
	return false;
}

int SetupFileReader :: get_char (void) {
	act_char = fgetc (setup_file);
	return act_char;
}

void SetupFileReader :: get_symbol (void) {
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
			return;
		}
		symbol_control = 5;
		if (negative) int_symbol = 0 - int_symbol;
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

void SetupFileReader :: skip (void) {
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

bool SetupFileReader :: id (char * name) {
	if (symbol_control != 1) return false;
	return strcmp (symbol, name) == 0;
}

bool SetupFileReader :: get_string (void) {
	get_symbol ();
	return symbol_control == 4;
}

bool SetupFileReader :: get_int (void) {
	get_symbol ();
	return symbol_control == 5;
}

bool SetupFileReader :: get_float (void) {
	get_symbol ();
	return symbol_control == 6;
}

bool SetupFileReader :: get_id (void) {
	get_symbol ();
	if (symbol_control != 3) return false;
	get_symbol ();
	return symbol_control == 1;
}

bool SetupFileReader :: get_id (char * name) {
	get_symbol ();
	if (symbol_control != 3 || strcmp (symbol, name) != 0) return false;
	get_symbol ();
	return symbol_control == 1;
}


