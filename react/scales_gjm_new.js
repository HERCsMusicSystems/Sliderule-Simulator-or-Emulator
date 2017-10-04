///////////////////////////////////////////////////////////////////////////////////
//                     Copyright (C) 2017 Dr Robert P. Wolf                      //
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

//////////////////////////////////////////////////////////////////////////////
// GJM: Added the following scales:
//
// CFI, DFI (folded, then inverted)
// CF10, DF10, CIF10, DIF10 (fold based on sqrt(10) instead of  pi)
// KI, JI (Inverse cube scales)
// Ln(_down) (based on Ln instead of Log)
// P2 (continuation of P1 scale for .01 to .1)
// PH2 (continuation of PH1 scale for 1 to 10)
// PT1, PT2 (based on for sqrt(x^2-1) instead of sqrt(1-x^2))
// LLD scale set (same as LL scale set but based on 10 instead of e)
// Temp scale set (D being Kelvin, yields Fahrenheit and Centigrade)
// Hyperbolic scale set for standard radians
// Metric_landr (allows for left_extension)
// HTon (piano octive frequency calculations)
//////////////////////////////////////////////////////////////////////////////

var scale_CFI = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . PI / Math . pow (10, location);};
  s . location = function (value) {return Math . log10 (Math . PI / value);};
  s . draw = function (ctx, length) {
    var shift = Math . log10 (Math . PI);
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, - length, s . height, 1 - shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, s . height, shift - s . right_extension, s);
  };
  return s;
};
var scale_DFI = function (height, options) {
  var s = new scale_CFI (height, options);
  s . draw = function (ctx, length) {
    var shift = Math . log10 (Math . PI);
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, - length, - s . height, 1 - shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, - s . height, shift - s . right_extension, s);
  };
  return s;
};

var scale_CF10 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location) * Math . pow (10, 0.5) * 0.1;};
  s . location = function (value) {return Math . log10 (10 * value / Math . pow (10, 0.5));};
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, length, s . height, 1 - shift + s . right_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (- length, 0);
    draw_log_1L (ctx, length, s . height, shift - s . left_extension, s);
  };
  return s;
};
var scale_DF10 = function (height, options) {
  var s = new scale_CF10 (height, options);
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, length, - s . height, 1 - shift + s . right_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (- length, 0);
    draw_log_1L (ctx, length, - s . height, shift - s . left_extension, s);
  };
  return s;
};
var scale_CIF10 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return 10 / (Math . pow (10, location) * Math . pow (10, 0.5));};
  s . location = function (value) {return Math . log10 (1 / (Math . pow (10, 0.5) * value * 0.1));};
  s . draw = function (ctx, length) {
    var shift = 0.5;
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, - length, s . height, 1 - shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, s . height, shift - s . right_extension, s);
  };
  return s;
};
var scale_DIF10 = function (height, options) {
  var s = new scale_CIF10 (height, options);
  s . draw = function (ctx, length) {
    var shift = 0.5;
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, - length, - s . height, 1 - shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, - s . height, shift - s . right_extension, s);
  };
  return s;
};

var scale_KI = function (height, options) {
  var s = new spacer (height, options);
  if (! options || options . draw_c == undefined) s . draw_c = false;
  s . value = function (location) {return Math . pow (10, 3 - location * 3);};
  s . location = function (value) {return 1 - Math . log10 (value) / 3;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_log_log_log (ctx, - length, height, s, s . right_extension, s . left_extension);};
  return s;
};
var scale_JI = function (height, options) {
  var s = new scale_KI (height, options);
  s . draw = function (ctx, length) {ctx . translate (length, 0); draw_log_log_log (ctx, - length, - s . height, s, s . right_extension, s . left_extension);};
  return s;
};

var scale_Ln = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return location * Math . log (10);};
  s . location = function (value) {return value / Math . log (10);};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_lin_e (ctx, length, s . height, s);};
  return s;
};
var scale_Ln_down = function (height, options) {
  var s = new scale_Ln (height, options);
  s . draw = function (ctx, length) {draw_lin_e (ctx, length, - s . height, s);};
  return s;
};

var scale_P2 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {var ret = Math . pow (10, location - 2); return Math . sqrt (1 - ret * ret);};
  s . location = function (value) {return 2 + Math . log10 (Math . sqrt (1 - value * value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_pe2 (ctx, length, s. height, s);};
  return s;
};
var scale_P2_down = function (height, options) {
  var s = new scale_P2 (height, options);
  s . draw = function (ctx, length) {draw_pe2 (ctx, length, - s. height, s);};
  return s;
};

var scale_PH2 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {var ret = Math . pow (10, location); return Math . sqrt (ret * ret + 1);};
	s . location = function (value) {return Math . log10 (Math . sqrt (value * value - 1));};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_ph2 (ctx, length, s . height, s);};
	return s;
};
var scale_PH2_down = function (height, options) {
	var s = new scale_PH2 (height, options);
	s . draw = function (ctx, length) {draw_ph2 (ctx, length, - s . height, s);};
	return s;
};

var scale_PT = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {var ret = Math . pow (10, location); return Math . sqrt (ret * ret - 1);};
  s . location = function (value) {return Math . log10 (Math . sqrt (value * value + 1));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_pt (ctx, length, s. height, s);};
  return s;
};
var scale_PT_down = function (height, options) {
  var s = new scale_PT (height, options);
  s . draw = function (ctx, length) {draw_pt (ctx, length, - s. height, s);};
  return s;
};
var scale_PT2 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {var ret = Math . pow (10, location + 1); return Math . sqrt (ret * ret - 1);};
  s . location = function (value) {return Math . log10 (Math . sqrt (value * value + 1) - 1);};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_pt2 (ctx, length, s. height, s);};
  return s;
};
var scale_PT2_down = function (height, options) {
  var s = new scale_PT2 (height, options);
  s . draw = function (ctx, length) {draw_pt2 (ctx, length, - s. height, s);};
  return s;
};

var scale_LLD3 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, Math . pow (10, location));};
  s . location = function (value) {return Math . log10 (Math . log10 (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LLD3 (ctx, length, s . height, s);};
  return s;
};
var scale_LLD3_down = function (height, options) {
  var s = new scale_LLD3 (height, options);
  s . draw = function (ctx, length) {draw_LLD3 (ctx, length, - s . height, s);};
  return s;
};
var scale_LLD2 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, Math . pow (10, location - 1));};
  s . location = function (value) {return 1 + Math . log10 (Math . log10 (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LLD2 (ctx, length, s . height, s);};
  return s;
};
var scale_LLD2_down = function (height, options) {
  var s = new scale_LLD2 (height, options);
  s . draw = function (ctx, length) {draw_LLD2 (ctx, length, - s . height, s);};
  return s;
};
var scale_LLD1 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, Math . pow (10, location - 2));};
  s . location = function (value) {return 2 + Math . log10 (Math . log10 (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LLD1 (ctx, length, s . height, s);};
  return s;
};
var scale_LLD1_down = function (height, options) {
  var s = new scale_LLD1 (height, options);
  s . draw = function (ctx, length) {draw_LLD1 (ctx, length, - s . height, s);};
  return s;
};
var scale_LLD0 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, Math . pow (10, location - 3));};
  s . location = function (value) {return 3 + Math . log10 (Math . log10 (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LLD0 (ctx, length, s . height, s);};
  return s;
};
var scale_LLD0_down = function (height, options) {
  var s = new scale_LLD0 (height, options);
  s . draw = function (ctx, length) {draw_LLD0 (ctx, length, - s . height, s);};
  return s;
};

var scale_LLD03 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {return Math . pow (10, - Math . pow (10, location));};
	s . location = function (value) {return Math . log10 (- Math . log10 (value));};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LLD03 (ctx, length, s . height, s);};
	return s;
};
var scale_LLD03_down = function (height, options) {
	var s = new scale_LLD03 (height, options);
	s . draw = function (ctx, length) {draw_LLD03 (ctx, length, - s . height, s);};
	return s;
};
var scale_LLD02 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {return Math . pow (10, - Math . pow (10, location - 1));};
	s . location = function (value) {return 1 + Math . log10 (- Math . log10 (value));};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LLD02 (ctx, length, s . height, s);};
	return s;
};
var scale_LLD02_down = function (height, options) {
	var s = new scale_LLD02 (height, options);
	s . draw = function (ctx, length) {draw_LLD02 (ctx, length, - s . height, s);};
	return s;
};
var scale_LLD01 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {return Math . pow (10, - Math . pow (10, location - 2));};
	s . location = function (value) {return 2 + Math . log10 (- Math . log10 (value));};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LLD01 (ctx, length, s . height, s);};
	return s;
};
var scale_LLD01_down = function (height, options) {
	var s = new scale_LLD01 (height, options);
	s . draw = function (ctx, length) {draw_LLD01 (ctx, length, - s . height, s);};
	return s;
};
var scale_LLD00 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, - Math . pow (10, location - 3));};
  s . location = function (value) {return 3 + Math . log10 (- Math . log10 (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LLD00 (ctx, length, s . height, s);};
  return s;
};
var scale_LLD00_down = function (height, options) {
  var s = new scale_LLD00 (height, options);
  s . draw = function (ctx, length) {draw_LLD00 (ctx, length, - s . height, s);};
  return s;
};
var scale_Cent3 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, 3 + location) - 273.15;};
  s . location = function (value) {return Math . log10 (value + 273.15) - 3;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_Cent3 (ctx, length, s . height, s);};
  return s;
};
var scale_Cent3_down = function (height, options) {
  var s = new scale_Cent3 (height, options);
  s . draw = function (ctx, length) {draw_Cent3 (ctx, length, - s . height, s);};
  return s;
};
var scale_Cent2 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, 2 + location) - 273.15;};
  s . location = function (value) {return Math . log10 (value + 273.15) - 2;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_Cent2 (ctx, length, s . height, s);};
  return s;
};
var scale_Cent2_down = function (height, options) {
  var s = new scale_Cent2 (height, options);
  s . draw = function (ctx, length) {draw_Cent2 (ctx, length, - s . height, s);};
  return s;
};
var scale_Cent1 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, 1 + location) - 273.15;};
  s . location = function (value) {return Math . log10 (value + 273.15) - 1;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_Cent1 (ctx, length, s . height, s);};
  return s;
};
var scale_Cent1_down = function (height, options) {
  var s = new scale_Cent1 (height, options);
  s . draw = function (ctx, length) {draw_Cent1 (ctx, length, - s . height, s);};
  return s;
};
var scale_Cent0 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location) - 273.15;};
  s . location = function (value) {return Math . log10 (value + 273.15);};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_Cent0 (ctx, length, s . height, s);};
  return s;
};
var scale_Cent0_down = function (height, options) {
  var s = new scale_Cent0 (height, options);
  s . draw = function (ctx, length) {draw_Cent0 (ctx, length, - s . height, s);};
  return s;
};
var scale_Fahr3 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, 3 + location) * 1.8 - 459.67;};
  s . location = function (value) {return Math . log10 ( (value + 459.67) / 1.8) - 3;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_Fahr3 (ctx, length, s . height, s);};
  return s;
};
var scale_Fahr3_down = function (height, options) {
  var s = new scale_Fahr3 (height, options);
  s . draw = function (ctx, length) {draw_Fahr3 (ctx, length, - s . height, s);};
  return s;
};
var scale_Fahr2 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, 2 + location) * 1.8 - 459.67;};
  s . location = function (value) {return Math . log10 ( (value + 459.67) / 1.8) - 2;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_Fahr2 (ctx, length, s . height, s);};
  return s;
};
var scale_Fahr2_down = function (height, options) {
  var s = new scale_Fahr2 (height, options);
  s . draw = function (ctx, length) {draw_Fahr2 (ctx, length, - s . height, s);};
  return s;
};
var scale_Fahr1 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, 1 + location) * 1.8 - 459.67;};
  s . location = function (value) {return Math . log10 ( (value + 459.67) / 1.8) - 1;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_Fahr1 (ctx, length, s . height, s);};
  return s;
};
var scale_Fahr1_down = function (height, options) {
  var s = new scale_Fahr1 (height, options);
  s . draw = function (ctx, length) {draw_Fahr1 (ctx, length, - s . height, s);};
  return s;
};
var scale_Fahr0 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location) * 1.8 - 459.67;};
  s . location = function (value) {return Math . log10 ( (value + 459.67) / 1.8);};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_Fahr0 (ctx, length, s . height, s);};
  return s;
};
var scale_Fahr0_down = function (height, options) {
  var s = new scale_Fahr0 (height, options);
  s . draw = function (ctx, length) {draw_Fahr0 (ctx, length, - s . height, s);};
  return s;
};
var scale_sinh1 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {location = Math . pow (10, location - 1); return Math . log (location + Math . sqrt (location * location + 1));};
	s . location = function (value) {return 1 + Math . log10 (0.5 * (Math . pow (Math . E, value) - Math . pow (Math . E, - value)));};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_sinhrad (ctx, length, s . height, s);};
	return s;
};
var scale_sinh1_down = function (height, options) {
	var s = new scale_sinh1 (height, options);
	s . draw = function (ctx, length) {draw_sinhrad (ctx, length, - s . height, s);};
	return s;
};
var scale_sinh2 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {location = Math . pow (10, location); return Math . log (location + Math . sqrt (location * location + 1));};
	s . location = function (value) {return Math . log10 (0.5 * (Math . pow (Math . E, value) - Math . pow (Math . E, - value)));};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_sinh2rad (ctx, length, s . height, s);};
	return s;
};
var scale_sinh2_down = function (height, options) {
	var s = new scale_sinh2 (height, options);
	s . draw = function (ctx, length) {draw_sinh2rad (ctx, length, - s . height, s);};
	return s;
};
var scale_cosh = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {location = Math . pow (10, location); return Math . log (location + Math . sqrt (location - 1) * Math . sqrt (location + 1));};
	s . location = function (value) {return Math . log10 (0.5 * (Math . pow (Math . E, value) + Math . pow (Math . E, - value)));};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_coshrad (ctx, length, s . height, s);};
	return s;
};
var scale_cosh_down = function (height, options) {
	var s = new scale_cosh (height, options);
	s . draw = function (ctx, length) {draw_coshrad (ctx, length, - s . height, s);};
	return s;
};
var scale_tanh = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {location = Math . pow (10, location - 1); if (location >= 1) location = 2; Math . log ((1 + location) / (1 - location)) / 2;};
	s . location = function (value) {value *= 2; value = Math . pow (Math . E, value); return 1 + Math . log10 ((value - 1) / (value + 1));};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_tanhrad (ctx, length, s . height, s);};
	return s;
};
var scale_tanh_down = function (height, options) {
	var s = new scale_tanh (height, options);
	s . draw = function (ctx, length) {draw_tanhrad (ctx, length, - s . height, s);};
	return s;
};
var scale_Metric_randl = function (height, options) {
	var s = new spacer (height, options);
	if (! options || options . step == undefined) s . step = 1;
	if (! options || options . scale == undefined) s . scale = 25.4;
	if (! options || options . shift == undefined) s . shift = 0;
	s . value = function (location) {return location * s . scale - s . shift;};
	s . location = function (value) {return value / s . scale + s . shift / s . scale;};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_metric_randl (ctx, length, s . height, s);};
	return s;
};
var scale_Metric_randl_down = function (height, options) {
	var s = new scale_Metric_randl (height, options);
	s . draw = function (ctx, length) {draw_metric_randl (ctx, length, - s . height, s);};
	return s;
};
var scale_HTon = function (height, options) {
  var s = new spacer (height, options);
  var tones = Math . log (10) / Math . log (2);
  s . value = function (location) {return location * tones;};
  s . location = function (value) {return value / tones;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_htone (ctx, length, s . height, s);};
  return s;
};
var scale_HTon_down = function (height, options) {
	var s = new scale_HTon (height, options);
	s . draw = function (ctx, length) {draw_htone (ctx, length, - s . height, s);};
	return s;
};
var LnGamma = function (value) {
  var c = [1.0 / 12.0, -1.0 / 360.0, 1.0 / 1260.0, -1.0 / 1680.0, 1.0 / 1188.0, -691.0 / 360360.0, 1.0 / 156.0, -3617.0 / 122400.0];
  var halfLogTwoPi = Math . log(2.0 * Math . PI) / 2.0;
  if (value <= 0) {return NaN;};
  var uppity = 0;
  if (value < 20) {uppity = Math . floor(20 - value);};
  var xx = value + uppity;
  var z = 1.0 / (xx * xx);
  var sum = c[7];
  for (i = 6; i >= 0; i--) {sum *= z; sum += c[i];};
  var result = (xx - 0.5) * Math . log(xx) - xx + halfLogTwoPi + sum / xx;
  for (i = uppity; i >= 1; i--) {result -= Math . log(xx - i);};
  return result;
};
var InvLnGamma = function (value) {
  var low = 2.0;
  var high = 30.0;
  var mid = 0.0;
  var midgma = 0.0;
  var delta = 1.0;
  var loops = 0;
  var accuracy = 0.0000001;
  var maxIterations = 100;
  if (value < 0) {return NaN};
  if (value > 30) {high = value};
  var deltamax = high * accuracy;
  while ((delta > deltamax) && (loops <= maxIterations)) {
    mid = (low + high) / 2.0;
    midgma = LnGamma(mid);
    if (midgma > value) {high = mid;} else {if (midgma < value) {low = mid};};
    delta = mid - low;
    if ((high - mid) > (mid - low)) {delta = high - mid;};
    loops += 1;
  };
  return mid;  
};
var scale_LnGamma0 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLnGamma (Math . pow (10, location - 1));};
  s . location = function (value) {return Math . log10 (LnGamma(value)) + 1;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_lngamma0 (ctx, length, s . height, s);};
  return s;
};
var scale_LnGamma0_down = function (height, options) {
	var s = new scale_LnGamma0 (height, options);
	s . draw = function (ctx, length) {draw_lngamma0 (ctx, length, - s . height, s);};
	return s;
};
var scale_LnGamma1 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLnGamma (Math . pow (10, location));};
  s . location = function (value) {return Math . log10 (LnGamma(value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_lngamma1 (ctx, length, s . height, s);};
  return s;
};
var scale_LnGamma1_down = function (height, options) {
	var s = new scale_LnGamma1 (height, options);
	s . draw = function (ctx, length) {draw_lngamma1 (ctx, length, - s . height, s);};
	return s;
};
var scale_LnGamma2 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLnGamma (Math . pow (10, location + 1));};
  s . location = function (value) {return Math . log10 (LnGamma(value)) - 1;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_lngamma2 (ctx, length, s . height, s);};
  return s;
};
var scale_LnGamma2_down = function (height, options) {
	var s = new scale_LnGamma2 (height, options);
	s . draw = function (ctx, length) {draw_lngamma2 (ctx, length, - s . height, s);};
	return s;
};
