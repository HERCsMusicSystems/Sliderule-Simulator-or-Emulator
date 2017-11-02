///////////////////////////////////////////////////////////////////////////////////
//                     Copyright (C) 2017 Gregory J. McClure                     //
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
// LnΓ (LnGamma scales), LogΓ (LogGamma scales)
// Γ(x,y) (Gamma Dist scales), B(x,y) (Beta Dist scales)
// Norm (Normal Dist scales, 1t and 2t)
// StudT(x) (Students T Dist scales)
// ChiSq(x) (Chi-Square Dist scales)
// F(x,y) (F Dist scales)
//////////////////////////////////////////////////////////////////////////////
var MAXITER = 100.0;
var ACCURACYV = 0.0000000001;

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
//////////////////////////////////////////////////////////////////////////////
// Ki and Ji scales (inverse of K and J)
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
// Ln scale
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
// Extension to the P scale for smaller value params (P closer to 1)
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
// Extension to the PH scale for larger value params (PH > 1.4)
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
// PT scales, same as P but -value instead of value
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
// LLD scales (like LL except based on 10 instead of e)
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
// Temperature scales (x is in K)
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
// Hyperbolic scales for radians (normal formulas)
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
// Metric scale that allows both left and right extensions
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
// Halftone scale (Octaves double every 2x, quartertones marked)
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
// LnGamma and LogGamma support and scales
//////////////////////////////////////////////////////////////////////////////
var LnGamma = function (value) {
  var i = 0;
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
var Gamma = function (value) {return Math . exp (LnGamma (value));};
var LogGamma = function (value) {return LnGamma (value) / Math . log(10.0);};
var InvLnGamma = function (value) {
  var low = 2.0;
  var high = 30.0;
  var mid = 0.0;
  var midgma = 0.0;
  var delta = 1.0;
  var loops = 0;
  if (value < 0) {return NaN};
  if (value > 30) {high = value};
  var deltamax = high * ACCURACYV;
  while ((delta > deltamax) && (loops <= MAXITER)) {
    mid = (low + high) / 2.0;
    midgma = LnGamma(mid);
    if (midgma > value) {high = mid;} else {if (midgma < value) {low = mid};};
    delta = mid - low;
    if ((high - mid) > (mid - low)) {delta = high - mid;};
    loops += 1;
  };
  if (loops > MAXITER) return NaN; else return mid;  
};
var InvLogGamma = function (value) {return InvLnGamma( value * Math . log( 10.0 ));};
var scale_LnGamma0 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLnGamma (Math . pow (10, location - 3));};
  s . location = function (value) {return Math . log10 (LnGamma (value)) + 3;};
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
  s . value = function (location) {return InvLnGamma (Math . pow (10, location - 2));};
  s . location = function (value) {return Math . log10 (LnGamma (value)) + 2;};
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
  s . value = function (location) {return InvLnGamma (Math . pow (10, location - 1));};
  s . location = function (value) {return Math . log10 (LnGamma (value)) + 1;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_lngamma2 (ctx, length, s . height, s);};
  return s;
};
var scale_LnGamma2_down = function (height, options) {
	var s = new scale_LnGamma2 (height, options);
	s . draw = function (ctx, length) {draw_lngamma2 (ctx, length, - s . height, s);};
	return s;
};
var scale_LnGamma3 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLnGamma (Math . pow (10, location));};
  s . location = function (value) {return Math . log10 (LnGamma (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_lngamma3 (ctx, length, s . height, s);};
  return s;
};
var scale_LnGamma3_down = function (height, options) {
	var s = new scale_LnGamma3 (height, options);
	s . draw = function (ctx, length) {draw_lngamma3 (ctx, length, - s . height, s);};
	return s;
};
var scale_LnGamma4 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLnGamma (Math . pow (10, location + 1));};
  s . location = function (value) {return Math . log10 (LnGamma (value)) - 1;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_lngamma4 (ctx, length, s . height, s);};
  return s;
};
var scale_LnGamma4_down = function (height, options) {
	var s = new scale_LnGamma4 (height, options);
	s . draw = function (ctx, length) {draw_lngamma4 (ctx, length, - s . height, s);};
	return s;
};
var scale_LnGamma5 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLnGamma (Math . pow (10, location + 2));};
  s . location = function (value) {return Math . log10 (LnGamma (value)) - 2;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_lngamma5 (ctx, length, s . height, s);};
  return s;
};
var scale_LnGamma5_down = function (height, options) {
	var s = new scale_LnGamma5 (height, options);
	s . draw = function (ctx, length) {draw_lngamma5 (ctx, length, - s . height, s);};
	return s;
};
var scale_LogGamma0 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLogGamma (Math . pow (10, location - 3));};
  s . location = function (value) {return Math . log10 (LogGamma (value)) + 3;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_loggamma0 (ctx, length, s . height, s);};
  return s;
};
var scale_LogGamma0_down = function (height, options) {
	var s = new scale_LogGamma0 (height, options);
	s . draw = function (ctx, length) {draw_loggamma0 (ctx, length, - s . height, s);};
	return s;
};
var scale_LogGamma1 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLogGamma (Math . pow (10, location - 2));};
  s . location = function (value) {return Math . log10 (LogGamma (value)) + 2;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_loggamma1 (ctx, length, s . height, s);};
  return s;
};
var scale_LogGamma1_down = function (height, options) {
	var s = new scale_LogGamma1 (height, options);
	s . draw = function (ctx, length) {draw_loggamma1 (ctx, length, - s . height, s);};
	return s;
};
var scale_LogGamma2 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLogGamma (Math . pow (10, location - 1));};
  s . location = function (value) {return Math . log10 (LogGamma (value)) + 1;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_loggamma2 (ctx, length, s . height, s);};
  return s;
};
var scale_LogGamma2_down = function (height, options) {
	var s = new scale_LogGamma2 (height, options);
	s . draw = function (ctx, length) {draw_loggamma2 (ctx, length, - s . height, s);};
	return s;
};
var scale_LogGamma3 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLogGamma (Math . pow (10, location));};
  s . location = function (value) {return Math . log10 (LogGamma (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_loggamma3 (ctx, length, s . height, s);};
  return s;
};
var scale_LogGamma3_down = function (height, options) {
	var s = new scale_LogGamma3 (height, options);
	s . draw = function (ctx, length) {draw_loggamma3 (ctx, length, - s . height, s);};
	return s;
};
var scale_LogGamma4 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLogGamma (Math . pow (10, location + 1));};
  s . location = function (value) {return Math . log10 (LogGamma (value)) - 1;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_loggamma4 (ctx, length, s . height, s);};
  return s;
};
var scale_LogGamma4_down = function (height, options) {
	var s = new scale_LogGamma4 (height, options);
	s . draw = function (ctx, length) {draw_loggamma4 (ctx, length, - s . height, s);};
	return s;
};
var scale_LogGamma5 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return InvLogGamma (Math . pow (10, location + 2));};
  s . location = function (value) {return Math . log10 (LogGamma (value)) - 2;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_loggamma5 (ctx, length, s . height, s);};
  return s;
};
var scale_LogGamma5_down = function (height, options) {
	var s = new scale_LogGamma5 (height, options);
	s . draw = function (ctx, length) {draw_loggamma5 (ctx, length, - s . height, s);};
	return s;
};
//////////////////////////////////////////////////////////////////////////////
// Support for distributions that require the local_table
//////////////////////////////////////////////////////////////////////////////
var make_SectionR = function (s, x, dxmax, dx, limit, height) {
  var value = 0.0;
  var small = 0.000000001;
  var y = 0.0;
  for (value = x; value < (x + dxmax); value += dx) {
    y = s . location (value) + small;
	if (y < limit) {
	  if (height == 5) {s . local_table . push ([5, Math.round(value*100000)/100000, y]);};
	  if (height != 5) {s . local_table . push ([height, y]);};
	};
  };
};
var make_SectionL = function (s, x, dxmax, dx, limit, height) {
  var value = 0.0;
  var small = 0.000000001;
  var y = 0.0;
  for (value = x; value < (x + dxmax); value += dx) {
    y = s . location (value) + small;
	if (y > limit) {
	  if (height == 5) {s . local_table . push ([5, Math.round(value*100000)/100000, y]);};
	  if (height != 5) {s . local_table . push ([height, y]);};
	};
  };
};
//////////////////////////////////////////////////////////////////////////////
// Gamma Distribution (Cummulative) support and scales
//////////////////////////////////////////////////////////////////////////////
var IncGammaFunc = function (a, value) {
  var result = Math . pow (value, a) * Math . exp (- value - LnGamma (a + 1));
  var sum = 1.0;
  var sum2 = 1.0;
  var prod = 1.0;
  var finished = false;
  for (var k = 1; finished != true; k++)
  {
    prod *= value / (a + k);
    sum += prod;
    if ((sum == sum2) || (k >= MAXITER)) finished = true;
    sum2 = sum;
  };
  if (k >= MAXITER) return NaN;
  result *= sum;
  return result;
};
var InvIncGammaFunc = function (a, value) {
  var low = 0.0;
  var high = 1.0;
  var mid = 0.0;
  var midgma = 0.0;
  var temp = IncGammaFunc (a, high);
  while (temp < value)
  {
    low = high;
    high = high * 2.0;
    temp = IncGammaFunc (a, high);
  }
  var deltamax = high * ACCURACYV;
  var delta = high - low;
  var loop = 0;
  while ((delta > deltamax) && (loop <= MAXITER))
  {
    mid = (low + high) / 2.0;
    midgma = IncGammaFunc (a, mid);
    if (midgma > value) high = mid;
    if (midgma < value) low = mid;
    delta = (mid - low);
    if ((high - mid) > (mid - low)) delta = (high - mid);
    loop++;
  };
  if (loop >= MAXITER) return NaN;
  return mid;
};
var GenGammaCumm = function (a, b, c, value) {return IncGammaFunc (b, Math . pow (a * value, c));};
var InvGenGammaCumm = function (a, b, c, value) {return Math . pow (InvIncGammaFunc (b, value), 1/c) / a;};
//
// GJM Working here...
//
var make_GammaDistTable = function (s) {
  var lowx = s . value (- s . left_extension);
  var highx = s . value (1 + s . right_extension);
  if (isNaN(highx)) {highx = 1.0;};
  var limit = 1 + s . right_extension;
  var diffx = 0.0;
  var firstx = 0.0;
  var x = 0.0;
  for (x = .0001; x < .00049; x += .0001) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .0001, .0005, limit, 5);
	  make_SectionR (s, x, .0001, .00005, limit, 3);
	  make_SectionR (s, x, .0001, .00001, limit, 2);
	};
  };
  for (x = .0005; x < .00099; x += .0005) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .0005, .0005, limit, 5);
	  make_SectionR (s, x, .0005, .00005, limit, 3);
	  make_SectionR (s, x, .0005, .00001, limit, 2);
	};
  };
  for (x = .001; x < .0049; x += .001) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .001, .005, limit, 5);
	  make_SectionR (s, x, .001, .0005, limit, 3);
	  make_SectionR (s, x, .001, .0001, limit, 2);
	};
  };
  for (x = .005; x < .0099; x += .005) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .005, .005, limit, 5);
	  make_SectionR (s, x, .005, .0005, limit, 3);
	  make_SectionR (s, x, .005, .0001, limit, 2);
	};
  };
  for (x = .01; x < .099; x += .01) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .01, .01, limit, 5);
	  make_SectionR (s, x, .01, .005, limit, 3);
	  make_SectionR (s, x, .01, .001, limit, 2);
	};
  };
  for (x = .1; x < .99; x += .05) {
	if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .05) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx > 0.3) {
	    make_SectionR (s, x, .05, .05, limit, 5);
	    make_SectionR (s, x, .05, .01, limit, 3);
	    make_SectionR (s, x, .05, .002, limit, 2);
	  };
	  if (diffx <= 0.3) {
	    make_SectionR (s, x, .05, .05, limit, 5);
	    make_SectionR (s, x, .05, .01, limit, 3);
	    make_SectionR (s, x, .05, .005, limit, 2);
	  };
	};
  };
  for (x = .95; x < 1; x += .01) {
	if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .01, .01, limit, 5);
	  make_SectionR (s, x, .01, .005, limit, 3);
	  make_SectionR (s, x, .01, .001, limit, 2);
	};
  };
  for (x = .99; x < .999; x += .005) {
	if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .005) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	    make_SectionR (s, x, .005, .005, limit, 5);
	    make_SectionR (s, x, .005, .001, limit, 3);
	    make_SectionR (s, x, .005, .0002, limit, 2);
	};
  };
  for (x = .995; x < 1; x += .001) {
	if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .001, .001, limit, 4);
	  make_SectionR (s, x, .001, .0005, limit, 3);
	  make_SectionR (s, x, .001, .0001, limit, 2);
	};
  };
  var limit = - s . left_extension;
  if (firstx < .0005) {
    for (x = .0001; x < firstx; x += .0001) {
	  make_SectionL (s, x, .0001, .0005, limit, 5);
	  make_SectionL (s, x, .0001, .00005, limit, 3);
	  make_SectionL (s, x, .0001, .00001, limit, 2);
    };
  };
  if ((firstx >= .0005) && (firstx < .001)) {
    for (x = .0004; x < firstx; x += .0001) {
	  make_SectionL (s, x, .0001, .0005, limit, 5);
	  make_SectionL (s, x, .0001, .00005, limit, 3);
	  make_SectionL (s, x, .0001, .00001, limit, 2);
    };
  };
  if ((firstx >= .001) && (firstx < .005)) {
    for (x = .0005; x < firstx; x += .0005) {
	  make_SectionL (s, x, .0005, .0005, limit, 5);
	  make_SectionL (s, x, .0005, .0002, limit, 3);
	  make_SectionL (s, x, .0005, .0001, limit, 2);
    };
  };
  if ((firstx >= .005) && (firstx < .01)) {
    for (x = .004; x < firstx; x += .001) {
	  make_SectionL (s, x, .001, .001, limit, 5);
	  make_SectionL (s, x, .001, .0005, limit, 3);
	  make_SectionL (s, x, .001, .0001, limit, 2);
    };
  };
  if ((firstx >= .01) && (firstx < .10)) {
    for (x = .005; x < firstx; x += .005) {
	  make_SectionL (s, x, .005, .005, limit, 5);
	  make_SectionL (s, x, .005, .001, limit, 3);
	  make_SectionL (s, x, .005, .0005, limit, 2);
    };
  };
  if ((firstx >= .1) && (firstx < .95)) {
    for (x = .05; x < firstx; x += .05) {
	  diffx = s . location (x + .05) - s . location (x);		
	  if (diffx > 0.3) {
	    make_SectionL (s, x, .05, .05, limit, 5);
	    make_SectionL (s, x, .05, .01, limit, 3);
	    make_SectionL (s, x, .05, .002, limit, 2);
	  };
	  if (diffx <= 0.3) {
	    make_SectionL (s, x, .05, .05, limit, 5);
	    make_SectionL (s, x, .05, .01, limit, 3);
	    make_SectionL (s, x, .05, .005, limit, 2);
	  };
    };
  };
  if ((firstx >= .95) && (firstx < .99)) {
    for (x = .985; x < firstx; x += .005) {
	  make_SectionL (s, x, .005, .005, limit, 5);
	  make_SectionL (s, x, .005, .001, limit, 3);
	  make_SectionL (s, x, .005, .0005, limit, 2);
    };
  };
};
var scale_GammaDist0 = function (height, a, b, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return GenGammaCumm (a, b, 1, Math . pow (10, location - 2));};
  s . location = function (value) {return Math . log10 (InvGenGammaCumm (a, b, 1, value)) + 2;};
  s . local_table = [];
  make_GammaDistTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_GammaDist0_down = function (height, a, b, options) {
	var s = new scale_GammaDist0 (height, a, b, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_GammaDist1 = function (height, a, b, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return GenGammaCumm (a, b, 1, Math . pow (10, location - 1));};
  s . location = function (value) {return Math . log10 (InvGenGammaCumm (a, b, 1, value)) + 1;};
  s . local_table = [];
  make_GammaDistTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_GammaDist1_down = function (height, a, b, options) {
	var s = new scale_GammaDist1 (height, a, b, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_GammaDist2 = function (height, a, b, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return GenGammaCumm (a, b, 1, Math . pow (10, location));};
  s . location = function (value) {return Math . log10 (InvGenGammaCumm (a, b, 1, value));};
  s . local_table = [];
  make_GammaDistTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_GammaDist2_down = function (height, a, b, options) {
	var s = new scale_GammaDist2 (height, a, b, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
//////////////////////////////////////////////////////////////////////////////
// Beta Distribution (Cummulative) support and scales
//////////////////////////////////////////////////////////////////////////////
var GenHypGeoFunc = function (arraya, arrayb, value) {
  var n = 1;
  var n1next = arraya.slice();
  var n2next = arrayb.slice();
  var n1 = n1next.length;
  var n2 = n2next.length;
  var result = value;
  var i = 0;
  for (i = 0; i < n1; i++) {result *= n1next[i];};
  for (i = 0; i < n2; i++) {result /= n2next[i];};
  var nextSum = result;
  result += 1;
  var prevValue = result + 1;
  while ((prevValue != result) && (n < MAXITER*100)) {
    nextSum *= value;
    for (i = 0; i < n1; i++) {n1next[i] += 1; nextSum *= n1next[i];};
    for (i = 0; i < n2; i++) {n2next[i] += 1; nextSum /= n2next[i];};
	n += 1;
	nextSum /= n;
	prevValue = result;
	result += nextSum;
  };
  return result;
};
var HypGeoFunc = function (a, b, c, value) {
  var arraya = [a, b];
  var arrayb = [c];
  return GenHypGeoFunc(arraya, arrayb, value);
};
var IncBetaFunc = function (a, b, value) {
  return (Math . pow (value, a) / a) * HypGeoFunc(a, 1 - b, a + 1, value);
};
var Beta = function (valuea, valueb) {return Math . exp (LnGamma (valuea) + LnGamma (valueb) - LnGamma (valuea + valueb));};
var InvIncBetaFunc = function (a, b, value) {
  var low = 0.0;
  var mid = 0.0;
  var high = 1.0;
  var midbta = 0.0;
  var delta = 1.0;
  var loops = 0;
  if (value < 0) return 0;
  var deltamax = high * ACCURACYV;
  while ((delta >= ACCURACYV) && (loops <= MAXITER)) {
    mid = (low + high) / 2.0;
    midbta = IncBetaFunc (a, b, mid);
    if (midbta > value) {high = mid;} else {if (midbta < value) {low = mid};};
    delta = mid - low;
    if ((high - mid) > (mid - low)) {delta = high - mid;};
    loops += 1;
  };
  if (loops > MAXITER) return NaN; else return mid;  
};
var BetaDistCumm = function (a, b, value) {return IncBetaFunc (a, b, value) / Beta (a, b);};
var InvBetaDistCumm = function (a, b, value) {return InvIncBetaFunc (a, b, value * Beta(a, b));};
var make_BetaDistTable = function (s) {
  var lowx = s . value (- s . left_extension);
  var highx = s . value (1 + s . right_extension);
  if (isNaN(highx)) {highx = 1.0;};
  var limit = 1 + s . right_extension;
  var diffx = 0.0;
  var firstx = 0.0;
  var x = 0.0;
  for (x = .0001; x < .00059; x += .0001) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .0001) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .0001, .00002, limit, 5);
		make_SectionR (s, x, .0001, .00001, limit, 3);
		make_SectionR (s, x, .0001, .000002, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionR (s, x, .0001, .0001, limit, 5);
		make_SectionR (s, x, .0001, .00005, limit, 3);
		make_SectionR (s, x, .0001, .00001, limit, 2);
	  };
	};	
  };
  for (x = .0006; x < .0009; x += .0002) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .0002) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .0002, .0001, limit, 5);
		make_SectionR (s, x, .0002, .00005, limit, 3);
		make_SectionR (s, x, .0002, .00002, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionR (s, x, .0002, .0002, limit, 5);
		make_SectionR (s, x, .0002, .0001, limit, 3);
		make_SectionR (s, x, .0002, .00005, limit, 2);
	  };
	};	
  };
  for (x = .001; x < .0059; x += .001) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .001) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .001, .0005, limit, 5);
		make_SectionR (s, x, .001, .0001, limit, 3);
		make_SectionR (s, x, .001, .00005, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionR (s, x, .001, .001, limit, 5);
		make_SectionR (s, x, .001, .0005, limit, 3);
		make_SectionR (s, x, .001, .0001, limit, 2);
	  };
	};	
  };
  for (x = .006; x < .009; x += .002) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .002) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .002, .001, limit, 5);
		make_SectionR (s, x, .002, .0005, limit, 3);
		make_SectionR (s, x, .002, .0002, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionR (s, x, .002, .002, limit, 5);
		make_SectionR (s, x, .002, .001, limit, 3);
		make_SectionR (s, x, .002, .0005, limit, 2);
	  };
	};	
  };
  for (x = .01; x < .049; x += .01) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .01) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .01, .002, limit, 5);
		make_SectionR (s, x, .01, .001, limit, 3);
		make_SectionR (s, x, .01, .0002, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionR (s, x, .01, .01, limit, 5);
		make_SectionR (s, x, .01, .005, limit, 3);
		make_SectionR (s, x, .01, .001, limit, 2);
	  };
	};	
  };
  for (x = .05; x < .39; x += .05) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .05) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .05, .01, limit, 5);
		make_SectionR (s, x, .05, .005, limit, 3);
		make_SectionR (s, x, .05, .001, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionR (s, x, .05, .05, limit, 5);
		make_SectionR (s, x, .05, .01, limit, 3);
		make_SectionR (s, x, .05, .005, limit, 2);
	  };
	};	
  };
  for (x = .4; x < .9; x += .1) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .1) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .1, .05, limit, 5);
		make_SectionR (s, x, .1, .01, limit, 3);
		make_SectionR (s, x, .1, .005, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionR (s, x, .1, .1, limit, 5);
		make_SectionR (s, x, .1, .05, limit, 3);
		make_SectionR (s, x, .1, .01, limit, 2);
	  };
	};	
  };
  if (highx > 0.9) {s . local_table . push ([5, 1, 1]);};
  limit = - s . left_extension;
  if ((firstx >= .0001) && (firstx < .0006)) {
    for (x = .0001; x < firstx; x += .0001) {
	  diffx = s . location (x + .0001) - s . location (x);
	  if (diffx >= 0.3) {
		make_SectionL (s, x, .0001, .00002, limit, 5);
		make_SectionL (s, x, .0001, .00001, limit, 3);
		make_SectionL (s, x, .0001, .000002, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionL (s, x, .0001, .0001, limit, 5);
		make_SectionL (s, x, .0001, .00005, limit, 3);
		make_SectionL (s, x, .0001, .00001, limit, 2);
	  };
    };
  };
  if ((firstx >= .0006) && (firstx <= .001)) {
    for (x = .0004; x < firstx; x += .0002) {
	  diffx = s . location (x + .0002) - s . location (x);
	  if (diffx >= 0.3) {
		make_SectionL (s, x, .0002, .0001, limit, 5);
		make_SectionL (s, x, .0002, .00005, limit, 3);
		make_SectionL (s, x, .0002, .00002, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionL (s, x, .0002, .0002, limit, 5);
		make_SectionL (s, x, .0002, .0001, limit, 3);
		make_SectionL (s, x, .0002, .00005, limit, 2);
	  };
    };
  };
  if ((firstx > .001) && (firstx < .006)) {
    for (x = .001; x < firstx; x += .001) {
	  diffx = s . location (x + .001) - s . location (x);
	  if (diffx >= 0.3) {
		make_SectionL (s, x, .001, .0005, limit, 5);
		make_SectionL (s, x, .001, .0001, limit, 3);
		make_SectionL (s, x, .001, .00005, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionL (s, x, .001, .001, limit, 5);
		make_SectionL (s, x, .001, .0005, limit, 3);
		make_SectionL (s, x, .001, .0001, limit, 2);
	  };
	};
  };
  if ((firstx >= .006) && (firstx <= .01)) {
    for (x = .004; x < firstx; x += .002) {
	  diffx = s . location (x + .002) - s . location (x);
	  if (diffx >= 0.3) {
		make_SectionL (s, x, .002, .001, limit, 5);
		make_SectionL (s, x, .002, .0005, limit, 3);
		make_SectionL (s, x, .002, .0002, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionL (s, x, .002, .002, limit, 5);
		make_SectionL (s, x, .002, .001, limit, 3);
		make_SectionL (s, x, .002, .0005, limit, 2);
	  };
	};
  };
  if ((firstx > .01) && (firstx <= .05)) {
    for (x = .01; x < firstx; x += .01) {
	  diffx = s . location (x + .01) - s . location (x);
	  if (diffx >= 0.3) {
		make_SectionL (s, x, .01, .002, limit, 5);
		make_SectionL (s, x, .01, .001, limit, 3);
		make_SectionL (s, x, .01, .0002, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionL (s, x, .01, .01, limit, 5);
		make_SectionL (s, x, .01, .005, limit, 3);
		make_SectionL (s, x, .01, .001, limit, 2);
	  };
	};
  };
  if ((firstx > .05) && (firstx < .4)) {
    for (x = .05; x < firstx; x += .05) {
	  diffx = s . location (x + .05) - s . location (x);
	  if (diffx >= 0.3) {
		make_SectionL (s, x, .05, .05, limit, 5);
		make_SectionL (s, x, .05, .01, limit, 3);
		make_SectionL (s, x, .05, .005, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionL (s, x, .05, .05, limit, 5);
		make_SectionL (s, x, .05, .01, limit, 3);
		make_SectionL (s, x, .05, .005, limit, 2);
	  };
	};
  };
  if (firstx >= .4) {
    for (x = .3; x < firstx; x += .1) {
	  diffx = s . location (x + .1) - s . location (x);
	  if (diffx >= 0.3) {
		make_SectionL (s, x, .1, .05, limit, 5);
		make_SectionL (s, x, .1, .01, limit, 3);
		make_SectionL (s, x, .1, .005, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionL (s, x, .1, .1, limit, 5);
		make_SectionL (s, x, .1, .05, limit, 3);
		make_SectionL (s, x, .1, .01, limit, 2);
	  };
	};
  };
};
var scale_BetaDist0 = function (height, a, b, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return BetaDistCumm (a, b, Math . pow (10, location - 3));};
  s . location = function (value) {return Math . log10 (InvBetaDistCumm (a, b, value)) + 3;};
  s . local_table = [];
  make_BetaDistTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_BetaDist0_down = function (height, a, b, options) {
	var s = new scale_BetaDist0 (height, a, b, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_BetaDist1 = function (height, a, b, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return BetaDistCumm (a, b, Math . pow (10, location - 2));};
  s . location = function (value) {return Math . log10 (InvBetaDistCumm (a, b, value)) + 2;};
  s . local_table = [];
  make_BetaDistTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_BetaDist1_down = function (height, a, b, options) {
	var s = new scale_BetaDist1 (height, a, b, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_BetaDist2 = function (height, a, b, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return BetaDistCumm (a, b, Math . pow (10, location - 1));};
  s . location = function (value) {return Math . log10 (InvBetaDistCumm (a, b, value)) + 1;};
  s . local_table = [];
  make_BetaDistTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_BetaDist2_down = function (height, a, b, options) {
	var s = new scale_BetaDist2 (height, a, b, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
//////////////////////////////////////////////////////////////////////////////
// Normal Distribution (Cummulative) support and scales
//////////////////////////////////////////////////////////////////////////////
var NormalDistCumm = function (value) {return IncGammaFunc (0.5, value * value / 2.0);};
var InvNormalDistCumm = function (value) {return Math . pow (InvIncGammaFunc (0.5, value) * 2.0, 0.5);};
var scale_NormalDist1T0 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return NormalDistCumm (Math . pow (10, location - 2));};
  s . location = function (value) {return Math . log10 (InvNormalDistCumm (value)) + 2;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_normaldist1t0 (ctx, length, s . height, s);};
  return s;
};
var scale_NormalDist1T0_down = function (height, options) {
	var s = new scale_NormalDist1T0 (height, options);
	s . draw = function (ctx, length) {draw_normaldist1t0 (ctx, length, - s . height, s);};
	return s;
};
var scale_NormalDist1T1 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return NormalDistCumm (Math . pow (10, location - 1));};
  s . location = function (value) {return Math . log10 (InvNormalDistCumm (value)) + 1;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_normaldist1t1 (ctx, length, s . height, s);};
  return s;
};
var scale_NormalDist1T1_down = function (height, options) {
	var s = new scale_NormalDist1T1 (height, options);
	s . draw = function (ctx, length) {draw_normaldist1t1 (ctx, length, - s . height, s);};
	return s;
};
var scale_NormalDist1T2 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return NormalDistCumm (Math . pow (10, location));};
  s . location = function (value) {return Math . log10 (InvNormalDistCumm (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_normaldist1t2 (ctx, length, s . height, s);};
  return s;
};
var scale_NormalDist1T2_down = function (height, options) {
	var s = new scale_NormalDist1T2 (height, options);
	s . draw = function (ctx, length) {draw_normaldist1t2 (ctx, length, - s . height, s);};
	return s;
};
var scale_NormalDist2T0 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return NormalDistCumm (Math . pow (10, location - 2)) / 2 + 0.5;};
  s . location = function (value) {return Math . log10 (InvNormalDistCumm ((value - .5) * 2)) + 2;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_normaldist2t0 (ctx, length, s . height, s);};
  return s;
};
var scale_NormalDist2T0_down = function (height, options) {
	var s = new scale_NormalDist2T0 (height, options);
	s . draw = function (ctx, length) {draw_normaldist2t0 (ctx, length, - s . height, s);};
	return s;
};
var scale_NormalDist2T1 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return NormalDistCumm (Math . pow (10, location - 1)) / 2 + 0.5;};
  s . location = function (value) {return Math . log10 (InvNormalDistCumm ((value - .5) * 2)) + 1;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_normaldist2t1 (ctx, length, s . height, s);};
  return s;
};
var scale_NormalDist2T1_down = function (height, options) {
	var s = new scale_NormalDist2T1 (height, options);
	s . draw = function (ctx, length) {draw_normaldist2t1 (ctx, length, - s . height, s);};
	return s;
};
var scale_NormalDist2T2 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return NormalDistCumm (Math . pow (10, location)) / 2 + 0.5;};
  s . location = function (value) {return Math . log10 (InvNormalDistCumm ((value - .5) * 2));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_normaldist2t2 (ctx, length, s . height, s);};
  return s;
};
var scale_NormalDist2T2_down = function (height, options) {
	var s = new scale_NormalDist2T2 (height, options);
	s . draw = function (ctx, length) {draw_normaldist2t2 (ctx, length, - s . height, s);};
	return s;
};
//////////////////////////////////////////////////////////////////////////////
// Student T Distribution (Cummulative) support and scales
//////////////////////////////////////////////////////////////////////////////
var StudTDistCumm = function (a, value) {
	return IncBetaFunc (0.5, a / 2.0, (value * value) / (a + value * value)) / Beta (0.5, a / 2.0);
};
var InvStudTDistCumm = function (a, value) {
	var y = InvIncBetaFunc (0.5, a / 2.0, value * Beta (0.5, a / 2.0));
	return Math . pow ((a * y) / (1.0 - y), 0.5);
};
var make_StudTTable = function (s) {
  var lowx = s . value (- s . left_extension);
  var highx = s . value (1 + s . right_extension);
  if (isNaN(highx)) {highx = 1.0;};
  var limit = 1 + s . right_extension;
  var diffx = 0.0;
  var firstx = 0.0;
  var x = 0.0;
  for (x = .01; x < .05; x += .01) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
		make_SectionR (s, x, .01, .01, limit, 5);
		make_SectionR (s, x, .01, .005, limit, 3);
		make_SectionR (s, x, .01, .001, limit, 2);
	};
  };
  for (x = .05; x < .95; x += .05) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .05) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .05, .01, limit, 5);
		make_SectionR (s, x, .05, .005, limit, 3);
		make_SectionR (s, x, .05, .001, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionR (s, x, .05, .05, limit, 5);
		make_SectionR (s, x, .05, .01, limit, 3);
		make_SectionR (s, x, .05, .002, limit, 2);
	  };
	};
  };
  for (x = .95; x < 0.99; x += .01) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .01) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .01, .005, limit, 5);
		make_SectionR (s, x, .01, .001, limit, 3);
		make_SectionR (s, x, .01, .0005, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionR (s, x, .01, .01, limit, 5);
		make_SectionR (s, x, .01, .005, limit, 3);
		make_SectionR (s, x, .01, .001, limit, 2);
	  };
	};
  };
  x = .99;
  if ((lowx < x) && (highx > x)) {
	diffx = s . location (x + .005) - s . location (x);
	if (firstx == 0.0) {firstx = x;};
 	make_SectionR (s, x, .005, .005, limit, 5);
	make_SectionR (s, x, .005, .001, limit, 3);
	if (diffx >= 0.3) {
	  make_SectionR (s, x, .005, .0002, limit, 2);
	};
	if (diffx < 0.3) {
	  make_SectionR (s, x, .005, .0005, limit, 2);
	};
  };
  for (x = .995; x < 0.999; x += .001) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .001, .001, limit, 5);
	  make_SectionR (s, x, .001, .0005, limit, 3);
	  make_SectionR (s, x, .001, .0001, limit, 2);
	};
  };
  for (x = .999; x < 1; x += .0005) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .0005, .0005, limit, 5);
	  make_SectionR (s, x, .0005, .0001, limit, 3);
	  make_SectionR (s, x, .0005, .00002, limit, 2);
	};
  };
  for (x = .9999; x < 1; x += .00005) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .00005, .00005, limit, 5);
	  make_SectionR (s, x, .00005, .00001, limit, 2);
	};
  };
  limit = - s . left_extension;
  if (firstx <= .05) {
	for (x = .01; x <= firstx; x += .01) {
	  make_SectionL (s, x, .01, .01, limit, 5);
	  make_SectionL (s, x, .01, .005, limit, 3);
	  make_SectionL (s, x, .01, .001, limit, 2);	  
	};
  };
  if ((firstx > .05) && (firstx <= .95)) {
    for (x = .05; x <= firstx; x += .05) {
	  diffx = s . location (x + .05) - s . location (x);
      if (diffx >= 0.3) {
		make_SectionL (s, x, .05, .01, limit, 5);
		make_SectionL (s, x, .05, .005, limit, 3);
		make_SectionL (s, x, .05, .001, limit, 2);
      };
      if (diffx < 0.3) {
		make_SectionL (s, x, .05, .05, limit, 5);
		make_SectionL (s, x, .05, .01, limit, 3);
		make_SectionL (s, x, .05, .002, limit, 2);
      };
	};
  };
  if ((firstx > .95) && (firstx <= .99)) {
    for (x = .9; x <= firstx; x += .01) {
	  diffx = s . location (x + .01) - s . location (x);
	  if (diffx >= 0.3) {
	    make_SectionL (s, x, .01, .095, limit, 5);
	    make_SectionL (s, x, .01, .001, limit, 3);
	    make_SectionL (s, x, .01, .0005, limit, 2);
	  };
	  if (diffx < 0.3) {
	    make_SectionL (s, x, .01, .01, limit, 5);
	    make_SectionL (s, x, .01, .005, limit, 3);
	    make_SectionL (s, x, .01, .001, limit, 2);
	  };
	};
  };
  if ((firstx > .99) && (firstx <= .995)) {
	x = 0.99;
	diffx = s . location (x + .005) - s . location (x);
	make_SectionL (s, x, .005, .005, limit, 5);
	make_SectionL (s, x, .005, .001, limit, 3);
	if (diffx >= 0.3) {
	  make_SectionL (s, x, .005, .0002, limit, 2);
	};
	if (diffx < 0.3) {
	  make_SectionL (s, x, .005, .0005, limit, 2);
	};
  };	
  if ((firstx > .995) && (firstx <= .999)) {
    for (x = .991; x <= firstx; x += .001) {
	  make_SectionL (s, x, .001, .001, limit, 5);
	  make_SectionL (s, x, .001, .0005, limit, 3);
	  make_SectionL (s, x, .001, .0001, limit, 2);
	};
  };	
  if ((firstx > .999) && (firstx <= .9999)) {
    for (x = .991; x <= firstx; x += .002) {
	  make_SectionL (s, x, .0005, .0005, limit, 5);
	  make_SectionL (s, x, .0005, .0001, limit, 3);
	  make_SectionL (s, x, .0005, .00002, limit, 2);
	};
  };	
  if (firstx > .9999) {
    for (x = .9995; x <= firstx; x += .00005) {
	  make_SectionL (s, x, .00005, .00005, limit, 5);
	  make_SectionL (s, x, .00005, .00001, limit, 2);
	};
  };
};
var scale_StudTDist1T0 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return StudTDistCumm (a, Math . pow (10, location - 2));};
  s . location = function (value) {return Math . log10 (InvStudTDistCumm (a, value)) + 2;};
  s . local_table = [];
  make_StudTTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_StudTDist1T0_down = function (height, a, options) {
	var s = new scale_StudTDist1T0 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_StudTDist1T1 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return StudTDistCumm (a, Math . pow (10, location - 1));};
  s . location = function (value) {return Math . log10 (InvStudTDistCumm (a, value)) + 1;};
  s . local_table = [];
  make_StudTTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_StudTDist1T1_down = function (height, a, options) {
	var s = new scale_StudTDist1T1 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_StudTDist1T2 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return StudTDistCumm (a, Math . pow (10, location));};
  s . location = function (value) {return Math . log10 (InvStudTDistCumm (a, value));};
  s . local_table = [];
  make_StudTTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_StudTDist1T2_down = function (height, a, options) {
	var s = new scale_StudTDist1T2 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_StudTDist1T3 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return StudTDistCumm (a, Math . pow (10, location + 1));};
  s . location = function (value) {return Math . log10 (InvStudTDistCumm (a, value)) - 1;};
  s . local_table = [];
  make_StudTTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_StudTDist1T3_down = function (height, a, options) {
	var s = new scale_StudTDist1T3 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_StudTDist2T0 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return StudTDistCumm (a, Math . pow (10, location - 2)) / 2 + 0.5;};
  s . location = function (value) {return Math . log10 (InvStudTDistCumm (a, (value - .5) * 2)) + 2;};
  s . local_table = [];
  make_StudTTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_StudTDist2T0_down = function (height, a, options) {
	var s = new scale_StudTDist2T0 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_StudTDist2T1 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return StudTDistCumm (a, Math . pow (10, location - 1)) / 2 + 0.5;};
  s . location = function (value) {return Math . log10 (InvStudTDistCumm (a, (value - .5) * 2)) + 1;};
  s . local_table = [];
  make_StudTTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_StudTDist2T1_down = function (height, a, options) {
	var s = new scale_StudTDist2T1 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_StudTDist2T2 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return StudTDistCumm (a, Math . pow (10, location)) / 2 + 0.5;};
  s . location = function (value) {return Math . log10 (InvStudTDistCumm (a, (value - .5) * 2));};
  s . local_table = [];
  make_StudTTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_StudTDist2T2_down = function (height, a, options) {
	var s = new scale_StudTDist2T2 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_StudTDist2T3 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return StudTDistCumm (a, Math . pow (10, location + 1)) / 2 + 0.5;};
  s . location = function (value) {return Math . log10 (InvStudTDistCumm (a, (value - .5) * 2)) - 1;};
  s . local_table = [];
  make_StudTTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_StudTDist2T3_down = function (height, a, options) {
	var s = new scale_StudTDist2T3 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
//////////////////////////////////////////////////////////////////////////////
// Chi Square Distribution (Cummulative) support and scales
//////////////////////////////////////////////////////////////////////////////
var ChiSquareDistCumm = function (a, value) {return IncGammaFunc (a / 2.0, value / 2.0);};
var InvChiSquareDistCumm = function (a, value) {return InvIncGammaFunc (a / 2.0, value) * 2.0;};
var ChiSquareDOF = function (a, value)
{
  // Need a bit more accuracy on this one...
  var OldMAXITER = MAXITER;
  MAXITER *= 10;
  var midchi = 0.0;
  if ((a < 0.0) || (a > 1.0)) return NaN;
  if ((value == 0.0) || (a == 0.0)) return 0.0;
  var low = 0.0;
  var high = 200.0;
  var deltamax = high * ACCURACYV;
  var delta = high - low;
  var loop = 0;
  var mid = 0.0;
  while ((delta > deltamax) && (loop < MAXITER))
      {
          mid = (low + high) / 2.0;
          midchi = ChiSquareDistCumm(mid, value);
          if (midchi < a) high = mid;
          if (midchi > a) low = mid;
          delta = (mid - low);
          if ((high - mid) > (mid - low)) delta = (high - mid);
          loop++;
      };
  MAXITER = OldMAXITER;
  return mid;
};
var make_ChSquareTable = function (s) {
  var lowx = s . value (- s . left_extension);
  var highx = s . value (1 + s . right_extension);
  if (isNaN(highx)) {highx = 1.0;};
  var limit = 1 + s . right_extension;
  var diffx = 0.0;
  var firstx = 0.0;
  var x = 0.0;
  for (x = .01; x < .05; x += .01) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .01) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
		make_SectionR (s, x, .01, .01, limit, 5);
		make_SectionR (s, x, .01, .005, limit, 3);
		make_SectionR (s, x, .01, .001, limit, 2);
	};
  };
  for (x = .05; x < .95; x += .05) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .05) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .05, .01, limit, 5);
		make_SectionR (s, x, .05, .005, limit, 3);
		make_SectionR (s, x, .05, .001, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionR (s, x, .05, .05, limit, 5);
		make_SectionR (s, x, .05, .01, limit, 3);
		make_SectionR (s, x, .05, .002, limit, 2);
	  };
	};
  };
  for (x = .95; x < 0.99; x += .01) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
		make_SectionR (s, x, .01, .01, limit, 5);
		make_SectionR (s, x, .01, .005, limit, 3);
		make_SectionR (s, x, .01, .001, limit, 2);
	};
  };
  x = .99;
  if ((lowx < x) && (highx > x)) {
	if (firstx == 0.0) {firstx = x;};
	make_SectionR (s, x, .005, .005, limit, 5);
	make_SectionR (s, x, .005, .001, limit, 3);
	make_SectionR (s, x, .005, .0005, limit, 2);
  };
  for (x = .995; x < 0.999; x += .002) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .002, .002, limit, 5);
	  make_SectionR (s, x, .002, .001, limit, 3);
	  make_SectionR (s, x, .002, .0005, limit, 2);
	};
  };
  for (x = .999; x < 1; x += .0005) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .0005, .0005, limit, 5);
	  make_SectionR (s, x, .0005, .0001, limit, 3);
	  make_SectionR (s, x, .0005, .00005, limit, 2);
	};
  };
  for (x = .9999; x < 1; x += .0001) {
    if ((lowx < x) && (highx > x)) {
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .0001, .0001, limit, 5);
	  make_SectionR (s, x, .0001, .00005, limit, 3);
	  make_SectionR (s, x, .0001, .00001, limit, 2);
	};
  };
  limit = - s . left_extension;
  if (firstx <= .05) {
	for (x = .01; x <= firstx; x += .01) {
	  make_SectionL (s, x, .01, .01, limit, 5);
	  make_SectionL (s, x, .01, .005, limit, 3);
	  make_SectionL (s, x, .01, .001, limit, 2);	  
	};
  };
  if ((firstx > .05) && (firstx <= .95)) {
    for (x = .05; x <= firstx; x += .05) {
	  diffx = s . location (x + .05) - s . location (x);
      if (diffx >= 0.3) {
		make_SectionL (s, x, .05, .01, limit, 5);
		make_SectionL (s, x, .05, .005, limit, 3);
		make_SectionL (s, x, .05, .001, limit, 2);
      };
      if (diffx < 0.3) {
		make_SectionL (s, x, .05, .05, limit, 5);
		make_SectionL (s, x, .05, .01, limit, 3);
		make_SectionL (s, x, .05, .002, limit, 2);
      };
	};
  };
  if ((firstx > .95) && (firstx <= .99)) {
    for (x = .9; x <= firstx; x += .01) {
	  make_SectionL (s, x, .01, .01, limit, 5);
	  make_SectionL (s, x, .01, .005, limit, 3);
	  make_SectionL (s, x, .01, .001, limit, 2);
	};
  };
  if ((firstx > .99) && (firstx <= .995)) {
    for (x = .95; x <= firstx; x += .005) {
	  make_SectionL (s, x, .005, .005, limit, 5);
	  make_SectionL (s, x, .005, .001, limit, 3);
	  make_SectionL (s, x, .005, .0005, limit, 2);
	};
  };	
  if ((firstx > .995) && (firstx <= .999)) {
    for (x = .991; x <= firstx; x += .002) {
	  make_SectionL (s, x, .002, .002, limit, 5);
	  make_SectionL (s, x, .002, .001, limit, 3);
	  make_SectionL (s, x, .002, .0005, limit, 2);
	};
  };	
  if ((firstx > .999) && (firstx <= .9999)) {
    for (x = .991; x <= firstx; x += .002) {
	  make_SectionL (s, x, .0005, .0005, limit, 5);
	  make_SectionL (s, x, .0005, .0001, limit, 3);
	  make_SectionL (s, x, .0005, .00005, limit, 2);
	};
  };	
  if (firstx > .9999) {
    for (x = .9995; x <= firstx; x += .00005) {
	  make_SectionL (s, x, .0001, .0001, limit, 5);
	  make_SectionL (s, x, .0001, .00005, limit, 3);
	  make_SectionL (s, x, .0001, .00001, limit, 2);
	};
  };
};
var scale_ChiSquare0 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return ChiSquareDistCumm (a, Math . pow (10, location - 2));};
  s . location = function (value) {return Math . log10 (InvChiSquareDistCumm (a, value)) + 2;};
  s . local_table = [];
  make_ChSquareTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_ChiSquare0_down = function (height, a, options) {
	var s = new scale_ChiSquare0 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_ChiSquare1 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return ChiSquareDistCumm (a, Math . pow (10, location - 1));};
  s . location = function (value) {return Math . log10 (InvChiSquareDistCumm (a, value)) + 1;};
  s . local_table = [];
  make_ChSquareTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_ChiSquare1_down = function (height, a, options) {
	var s = new scale_ChiSquare1 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_ChiSquare2 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return ChiSquareDistCumm (a, Math . pow (10, location));};
  s . location = function (value) {return Math . log10 (InvChiSquareDistCumm (a, value));};
  s . local_table = [];
  make_ChSquareTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_ChiSquare2_down = function (height, a, options) {
	var s = new scale_ChiSquare2 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_ChiSquare3 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return ChiSquareDistCumm (a, Math . pow (10, location + 1));};
  s . location = function (value) {return Math . log10 (InvChiSquareDistCumm (a, value)) - 1;};
  s . local_table = [];
  make_ChSquareTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_ChiSquare3_down = function (height, a, options) {
	var s = new scale_ChiSquare3 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var make_ChSquareDOFTable = function (s) {
  var lowx = s . value (- s . left_extension);
  var highx = s . value (1 + s . right_extension);
  var limit = 1 + s . right_extension;
  var x = 0.0;
  for (x = 1; x < 20; x += 1) {
    if ((lowx < x) && (highx > x)) {
	  make_SectionR (s, x, 1, 1, limit, 5);
	};
  };
  for (x = 20; x < 100; x += 5) {
    if ((lowx < x) && (highx > x)) {
	  make_SectionR (s, x, 5, 5, limit, 5);
	  make_SectionR (s, x, 5, 1, limit, 2);
	};
  };
  for (x = 100; x < 200; x += 20) {
    if ((lowx < x) && (highx > x)) {
	  make_SectionR (s, x, 20, 10, limit, 5);
	  make_SectionR (s, x, 20, 2, limit, 2);
	};
  };
};
var scale_ChiSquareDOF0 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return ChiSquareDOF(1 - a / 100, Math . pow (10, location - 1));};
  s . location = function (value) {return Math . log10 (InvChiSquareDistCumm (value, 1 - a / 100)) + 1;};
  s . local_table = [];
  make_ChSquareDOFTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_ChiSquareDOF0_down = function (height, a, options) {
	var s = new scale_ChiSquareDOF0 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_ChiSquareDOF1 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return ChiSquareDOF(1 - a / 100, Math . pow (10, location));};
  s . location = function (value) {return Math . log10 (InvChiSquareDistCumm (value, 1 - a / 100));};
  s . local_table = [];
  make_ChSquareDOFTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_ChiSquareDOF1_down = function (height, a, options) {
	var s = new scale_ChiSquareDOF1 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_ChiSquareDOF2 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return ChiSquareDOF(1 - a / 100, Math . pow (10, location + 1));};
  s . location = function (value) {return Math . log10 (InvChiSquareDistCumm (value, 1 - a / 100)) - 1;};
  s . local_table = [];
  make_ChSquareDOFTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_ChiSquareDOF2_down = function (height, a, options) {
	var s = new scale_ChiSquareDOF2 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_ChiSquareDOF3 = function (height, a, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return ChiSquareDOF(1 - a / 100, Math . pow (10, location + 2));};
  s . location = function (value) {return Math . log10 (InvChiSquareDistCumm (value, 1 - a / 100)) - 2;};
  s . local_table = [];
  make_ChSquareDOFTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_ChiSquareDOF3_down = function (height, a, options) {
	var s = new scale_ChiSquareDOF3 (height, a, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
//////////////////////////////////////////////////////////////////////////////
// F Distribution (Cummulative) support and scales
//////////////////////////////////////////////////////////////////////////////
var FDistCumm = function (a, b, value) {
	return IncBetaFunc (a/2.0, b/2.0, (a * value)/(b + a * value)) / Beta(a / 2.0, b / 2.0);
};
var InvFDistCumm = function (a, b, value) {
	var y = InvIncBetaFunc (a / 2.0, b / 2.0, value * Beta(a / 2.0, b / 2.0));
	return (b * y) / (a * (1.0 - y));
};
var make_FDistTable = function (s) {
  var lowx = s . value (- s . left_extension);
  var highx = s . value (1 + s . right_extension);
  var limit = 1 + s . right_extension;
  var diffx = 0.0;
  var firstx = 0.0;
  var x = 0.0;
  for (x = .1; x < .95; x += .05) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .05) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .05, .01, limit, 5);
		make_SectionR (s, x, .05, .005, limit, 3);
		make_SectionR (s, x, .05, .001, limit, 2);
	  };
	  if (diffx < 0.3) {
		make_SectionR (s, x, .05, .05, limit, 5);
		make_SectionR (s, x, .05, .01, limit, 3);
		make_SectionR (s, x, .05, .002, limit, 2);
	  };
	};
  };
  for (x = .95; x < 1.00; x += .005) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .005) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  if (diffx >= 0.3) {
		make_SectionR (s, x, .005, .001, limit, 5);
		make_SectionR (s, x, .005, .0005, limit, 3);
		make_SectionR (s, x, .005, .0002, limit, 2);
	  };	  
	  if (diffx < 0.3) {
		make_SectionR (s, x, .005, .005, limit, 5);
		make_SectionR (s, x, .005, .001, limit, 3);
		make_SectionR (s, x, .005, .0005, limit, 2);
	  };	  
	};
  };
  for (x = .995; x < 1.00; x += .001) {
    if ((lowx < x) && (highx > x)) {
	  diffx = s . location (x + .001) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionR (s, x, .001, .001, limit, 5);
	  make_SectionR (s, x, .001, .0005, limit, 4);
	  make_SectionR (s, x, .001, .0001, limit, 3);
	  make_SectionR (s, x, .001, .00005, limit, 2);
	};
  };
  limit = - s . left_extension;
  if (firstx <= .95) {
    for (x = .1; x <= firstx; x += .05) {
	  diffx = s . location (x + .05) - s . location (x);
      if (diffx >= 0.3) {
		make_SectionL (s, x, .05, .01, limit, 5);
		make_SectionL (s, x, .05, .005, limit, 3);
		make_SectionL (s, x, .05, .001, limit, 2);
      };
      if (diffx < 0.3) {
		make_SectionL (s, x, .05, .05, limit, 5);
		make_SectionL (s, x, .05, .01, limit, 3);
		make_SectionL (s, x, .05, .002, limit, 2);
      };
	};
  };
  if ((firstx > .95) && (firstx <= .995)) {
    for (x = .9; x <= firstx; x += .005) {
	  diffx = s . location (x + .005) - s . location (x);
      if (diffx >= 0.3) {
		make_SectionL (s, x, .005, .001, limit, 5);
		make_SectionL (s, x, .005, .0005, limit, 3);
		make_SectionL (s, x, .005, .0002, limit, 2);
      };
      if (diffx < 0.3) {
		make_SectionL (s, x, .005, .005, limit, 5);
		make_SectionL (s, x, .005, .001, limit, 3);
		make_SectionL (s, x, .005, .0005, limit, 2);
      };
	};
  };
  if (firstx > .995) {
    for (x = .995; x <= firstx; x += .001) {
	  diffx = s . location (x + .001) - s . location (x);
	  if (firstx == 0.0) {firstx = x;};
	  make_SectionL (s, x, .001, .001, limit, 5);
	  make_SectionL (s, x, .001, .0005, limit, 4);
	  make_SectionL (s, x, .001, .0001, limit, 3);
	  make_SectionL (s, x, .001, .00005, limit, 2);
	};
  };	
};
var scale_FDist0 = function (height, a, b, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return FDistCumm (a, b, Math . pow (10, location - 1));};
  s . location = function (value) {return Math . log10 (InvFDistCumm (a, b, value)) + 1;};
  s . local_table = [];
  make_FDistTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_FDist0_down = function (height, a, b, options) {
	var s = new scale_FDist0 (height, a, b, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_FDist1 = function (height, a, b, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return FDistCumm (a, b, Math . pow (10, location));};
  s . location = function (value) {return Math . log10 (InvFDistCumm (a, b, value));};
  s . local_table = [];
  make_FDistTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_FDist1_down = function (height, a, b, options) {
	var s = new scale_FDist1 (height, a, b, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_FDist2 = function (height, a, b, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return FDistCumm (a, b, Math . pow (10, location + 1));};
  s . location = function (value) {return Math . log10 (InvFDistCumm (a, b, value)) - 1;};
  s . local_table = [];
  make_FDistTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_FDist2_down = function (height, a, b, options) {
	var s = new scale_FDist2 (height, a, b, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_FDist3 = function (height, a, b, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return FDistCumm (a, b, Math . pow (10, location + 2));};
  s . location = function (value) {return Math . log10 (InvFDistCumm (a, b, value)) - 2;};
  s . local_table = [];
  make_FDistTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_FDist3_down = function (height, a, b, options) {
	var s = new scale_FDist3 (height, a, b, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
var scale_FDist4 = function (height, a, b, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return FDistCumm (a, b, Math . pow (10, location + 3));};
  s . location = function (value) {return Math . log10 (InvFDistCumm (a, b, value)) - 3;};
  s . local_table = [];
  make_FDistTable (s);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_statistic (ctx, length, s . height, s);};
  return s;
};
var scale_FDist4_down = function (height, a, b, options) {
	var s = new scale_FDist4 (height, a, b, options);
	s . draw = function (ctx, length) {draw_statistic (ctx, length, - s . height, s);};
	return s;
};
