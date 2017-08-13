//////////////////////////////////////////////////////////////////////////////
// A, B, AI, BI
// C, D, CI, DI
// CF, DF, CIF, DIF
// K, J
// R1, R2, W1, W2
// L, M
// Sdec(_down), S(_down), SCdec(_down), CSdec(_down)
// STdec(_down), ST(_down)
// Tdec(_down), T(_down), T1dec(_down), TCTdec(_down), TCT1dec(_down)
// P(_down)
// LL3(_down), LL2(_down), LL1(_down)
// LL03(_down), LL02(_down), LL01(_down)
//////////////////////////////////////////////////////////////////////////////

var scale_A = function (height, options) {
  var s = new spacer (height, options);
  if (! options || ! options . draw_c) s . draw_c = false;
  s . value = function (location) {return Math . pow (10, location + location);};
  s . location = function (value) {return Math . log10 (value) * 0.5;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_log_log (ctx, length, height, s, s . left_extension, s . right_extension);};
  return s;
};
var scale_B = function (height, options) {
  var s = new scale_A (height, options);
  s . draw = function (ctx, length) {draw_log_log (ctx, length, - s . height, s, s . left_extension, s . right_extension);};
  return s;
};
var scale_AI = function (height, options) {
  var s = new spacer (height, options);
  if (! options || ! options . draw_c) s . draw_c = false;
  s . value = function (location) {return Math . pow (10, 2 - location - location);};
  s . location = function (value) {return 1 - Math . log10 (value) * 0.5;};
  s . draw = function (ctx, length) {ctx . translate (length, s . height); draw_log_log (ctx, - length, height, s, s . right_extension, s . left_extension);};
  return s;
};
var scale_BI = function (height, options) {
  var s = new scale_AI (height, options);
  s . draw = function (ctx, length) {ctx . translate (length, 0); draw_log_log (ctx, - length, - s . height, s, s . right_extension, s . left_extension);};
  return s;
};
var scale_C = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location);};
  s . location = function (value) {return Math . log10 (value);};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_log (ctx, length, s . height, s, s . left_extension, s . right_extension);};
  return s;
};
var scale_D = function (height, options) {
  var s = new scale_C (height, options);
  s . draw = function (ctx, length) {draw_log (ctx, length, - s . height, s, s . left_extension, s . right_extension);};
  return s;
};
var scale_CI = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, 1 - location);};
  s . location = function (value) {return 1 - Math . log10 (value);};
  s . draw = function (ctx, length) {ctx . translate (length, s . height); draw_log (ctx, - length, s . height, s, s . right_extension, s . left_extension);};
  return s;
};
var scale_DI = function (height, options) {
  var s = new scale_CI (height, options);
  s . draw = function (ctx, length) {ctx . translate (length, 0); draw_log (ctx, - length, - s . height, s, s . right_extension, s . left_extension);};
  return s;
};
var scale_CF = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location) * Math . PI * 0.1;};
  s . location = function (value) {return Math . log10 (10 * value / Math . PI);};
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
var scale_DF = function (height, options) {
  var s = new scale_CF (height, options);
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
var scale_CIF = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return 10 / (Math . pow (10, location) * Math . PI);};
  s . location = function (value) {return Math . log10 (1 / (Math . PI * value * 0.1));};
  s . draw = function (ctx, length) {
    var shift = Math . log10 (10 / Math . PI);
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, - length, s . height, 1 - shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, s . height, shift - s . right_extension, s);
  };
  return s;
};
var scale_DIF = function (height, options) {
  var s = new scale_CIF (height, options);
  s . draw = function (ctx, length) {
    var shift = Math . log10 (10 / Math . PI);
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, - length, - s . height, 1 - shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, - s . height, shift - s . right_extension, s);
  };
  return s;
};
var scale_K = function (height, options) {
  var s = new spacer (height, options);
  if (! options || ! options . draw_c) s . draw_c = false;
  s . value = function (location) {return Math . pow (10, location * 3);};
  s . location = function (value) {return Math . log10 (value) / 3;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_log_log_log (ctx, length, s . height, s, s . left_extension, s . right_extension);};
  return s;
};
var scale_J = function (height, options) {
  var s = new scale_K (height, options);
  s . draw = function (ctx, length) {draw_log_log_log (ctx, length, - s . height, s, s . left_extension, s . right_extension);};
  return s;
};
var scale_R1 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {return Math . pow (10, location * 0.5);};
	s . location = function (value, length) {return Math . log10 (value) * 2;};
	s . draw = function (ctx, length) {
		ctx . translate (0, s . height);
		mark (ctx, s . indices [0], 0, height * 0.5);
		draw_log_1R (ctx, length * 2, height, 0.5 + s . right_extension * 0.5, s);
		ctx . translate (-2 * length, 0);
		draw_log_1L (ctx, length * 2, height, 1 - s . left_extension * 0.5, s);
	};
	return s;
};
var scale_R2 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {return Math . pow (10, location * 0.5);};
	s . location = function (value, length) {return Math . log10 (value) * 2;};
	s . draw = function (ctx, length) {
		ctx . translate (- length, s . height);
		draw_log_1L (ctx, length * 2, height, 0.5 - s . left_extension * 0.5, s);
		ctx . translate (2 * length, 0);
		mark (ctx, s . indices [1], 0, height * 0.5);
		draw_log_1R (ctx, length * 2, height, s . right_extension * 0.5, s);
	};
	return s;
};
var scale_L = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return location * 10;};
  s . location = function (value) {return value * 0.1;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_lin (ctx, length, s . height, s);};
  return s;
};
var scale_M = function (height, options) {
  var s = new scale_L (height, options);
  s . draw = function (ctx, length) {draw_lin (ctx, length, - s . height, s);};
  return s;
};
var scale_Sdec = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . asin (Math . pow (10, location - 1)) * 180 / Math . PI;};
  s . location = function (value) {return 1 + Math . log10 (Math . sin (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_sine_dec (ctx, length, s . height, s);};
  return s;
};
var scale_Sdec_down = function (height, options) {
  var s = new scale_Sdec (height, options);
  s . draw = function (ctx, length) {draw_sine_dec (ctx, length, - s . height, s);};
  return s;
};
var scale_S = function (height, options) {
  var s = new scale_Sdec (height, options);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_sine_dec (ctx, length, s . height, s);};
  return s;
};
var scale_S_down = function (height, options) {
  var s = new scale_Sdec (height, options);
  s . draw = function (ctx, length) {draw_sine_dec (ctx, length, - s . height, s);};
  return s;
};
var scale_STdec = function (height, options) {
  var s = new spacer (height, options);
  if (! options || ! options . draw_halves) s . draw_halves = true;
  if (! options || ! options . draw_st_corrections) s . draw_st_corrections = true;
  if (! options || ! options . draw_c) s . draw_c = false;
  if (! options || ! options . draw_pi) s . draw_pi = false;
  if (! options || ! options . draw_e) s . draw_e = false;
  s . value = function (location) {return Math . pow (10, location) * 1.8 / Math . PI;};
  s . location = function (value) {return Math . log10 (value * Math . PI / 1.8);};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_small_sine_dec (ctx, length, s . height, s);};
  return s;
};
var scale_STdec_down = function (height, options) {
  var s = new scale_STdec (height, options);
  s . draw = function (ctx, length) {draw_small_sine_dec (ctx, length, - s . height, s);};
  return s;
};
var scale_ST = function (height, options) {
  var s = new scale_STdec (height, options);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_small_sine_dec (ctx, length, s . height, s);};
  return s;
};
var scale_ST_down = function (height, options) {
  var s = new scale_STdec (height, options);
  s . draw = function (ctx, length) {draw_small_sine_dec (ctx, length, - s . height, s);};
  return s;
};
var scale_Tdec = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . atan (Math . pow (10, location - 1)) * 180 / Math . PI;};
  s . location = function (value) {return 1 + Math . log10 (Math . tan (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_tan_dec (ctx, length, s . height, s);};

  return s;
};
var scale_Tdec_down = function (height, options) {
  var s = new scale_Tdec (height, options);
  s . draw = function (ctx, length) {draw_tan_dec (ctx, length, - s . height, s);};
  return s;
};
var scale_T = function (height, options) {
  var s = new scale_Tdec (height, options);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_tan_dec (ctx, length, s . height, s);};
  return s;
};
var scale_T_down = function (height, options) {
  var s = new scale_Tdec (height, options);
  s . draw = function (ctx, length) {draw_tan_dec (ctx, length, - s . height, s);};
  return s;
};
var scale_TCTdec = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . atan (Math . pow (10, location - 1)) * 180 / Math . PI;};
  s . location = function (value) {return 1 + Math . log10 (Math . tan (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_tan_cotan_dec (ctx, length, s . height, s);};
  return s;
};
var scale_TCTdec_down = function (height, options) {
  var s = new scale_TCTdec (height, options);
  s . draw = function (ctx, length) {draw_tan_cotan_dec (ctx, length, - s . height, s);};
  return s;
};
var scale_T1dec = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . atan (Math . pow (10, location - 2)) * 180 / Math . PI;};
  s . location = function (value) {return 2 + Math . log10 (Math . tan (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_big_tan_dec (ctx, length, s . height, s);};
  return s;
};
var scale_T1dec_down = function (height, options) {
  var s = new scale_T1dec (height, options);
  s . draw = function (ctx, length) {draw_big_tan_dec (ctx, length, - s . height, s);};
  return s;
};
var scale_TCT1dec = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . atan (Math . pow (10, location - 2)) * 180 / Math . PI;};
  s . location = function (value) {return 2 + Math . log10 (Math . tan (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_big_tan_dec (ctx, length, s . height, s);};
  return s;
};
var scale_TCT1dec_down = function (height, options) {
  var s = new scale_TCT1dec (height, options);
  s . draw = function (ctx, length) {draw_big_tan_cotan_dec (ctx, length, - s . height, s);};
  return s;
};
var scale_SCdec = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . asin (Math . pow (10, location - 1)) * 180 / Math . PI;};
  s . location = function (value) {return 1 + Math . log10 (Math . sin (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_sine_cosine_dec (ctx, length, s . height, s, 'sc');};
  return s;
};
var scale_SCdec_down = function (height, options) {
  var s = new scale_SCdec (height, options);
  s . draw = function (ctx, length) {draw_sine_cosine_dec (ctx, length, - s . height, s, 'sc');};
  return s;
};
var scale_CSdec = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . asin (Math . pow (10, location - 1)) * 180 / Math . PI;};
  s . location = function (value) {return 1 + Math . log10 (Math . sin (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_sine_cosine_dec (ctx, length, s . height, s, 'cs');};
  return s;
};
var scale_CSdec_down = function (height, options) {
  var s = new scale_CSdec (height, options);
  s . draw = function (ctx, length) {draw_sine_cosine_dec (ctx, length, - s . height, s, 'cs');};
  return s;
};
var scale_P = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {var ret = Math . pow (10, location - 1); return Math . sqrt (1 - ret * ret);};
  s . location = function (value) {return 1 + Math . log10 (Math . sqrt (1 - value * value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_pe (ctx, length, s. height, s);};
  return s;
};
var scale_P_down = function (height, options) {
  var s = new scale_P (height, options);
  s . draw = function (ctx, length) {draw_pe (ctx, length, - s. height, s);};
  return s;
};
var scale_LL3 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (Math . E, Math . pow (10, location));};
  s . location = function (value) {return Math . log10 (Math . log (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LL3 (ctx, length, s . height, s);};
  return s;
};
var scale_LL3_down = function (height, options) {
  var s = new scale_LL3 (height, options);
  s . draw = function (ctx, length) {draw_LL3 (ctx, length, - s . height, s);};
  return s;
};
var scale_LL2 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (Math . E, Math . pow (10, location - 1));};
  s . location = function (value) {return 1 + Math . log10 (Math . log (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LL2 (ctx, length, s . height, s);};
  return s;
};
var scale_LL2_down = function (height, options) {
  var s = new scale_LL2 (height, options);
  s . draw = function (ctx, length) {draw_LL2 (ctx, length, - s . height, s);};
  return s;
};
var scale_LL1 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (Math . E, Math . pow (10, location - 2));};
  s . location = function (value) {return 2 + Math . log10 (Math . log (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LL1 (ctx, length, s . height, s);};
  return s;
};
var scale_LL1_down = function (height, options) {
  var s = new scale_LL1 (height, options);
  s . draw = function (ctx, length) {draw_LL1 (ctx, length, - s . height, s);};
  return s;
};
var scale_LL03 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {return Math . pow (Math . E, - Math . pow (10, location));};
	s . location = function (value) {return Math . log10 (- Math . log (value));};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LL03 (ctx, length, s . height, s);};
	return s;
};
var scale_LL03_down = function (height, options) {
	var s = new scale_LL03 (height, options);
	s . draw = function (ctx, length) {draw_LL03 (ctx, length, - s . height, s);};
	return s;
};
var scale_LL02 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {return Math . pow (Math . E, - Math . pow (10, location - 1));};
	s . location = function (value) {return 1 + Math . log10 (- Math . log (value));};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LL02 (ctx, length, s . height, s);};
	return s;
};
var scale_LL02_down = function (height, options) {
	var s = new scale_LL02 (height, options);
	s . draw = function (ctx, length) {draw_LL02 (ctx, length, - s . height, s);};
	return s;
};
var scale_LL01 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {return Math . pow (Math . E, - Math . pow (10, location - 2));};
	s . location = function (value) {return 2 + Math . log10 (- Math . log (value));};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LL01 (ctx, length, s . height, s);};
	return s;
};
var scale_LL01_down = function (height, options) {
	var s = new scale_LL01 (height, options);
	s . draw = function (ctx, length) {draw_LL01 (ctx, length, - s . height, s);};
	return s;
};
