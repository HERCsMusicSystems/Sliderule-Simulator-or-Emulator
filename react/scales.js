//////////////////////////////////////////////////////////////////////////////
// A, B, AI, BI
// C, D, CI, DI
// CF, DF, CIF, DIF
// K, J
// R1, R2, W1, W2
// L, M, LR12, LW12
// Sdec(_down), S(_down), SCdec(_down), CSdec(_down)
// STdec(_down), ST(_down)
// Tdec(_down), T(_down), T1dec(_down), T1(_down), TCTdec(_down), TCT1dec(_down)
// P(_down)
// LL3(_down), LL2(_down), LL1(_down), CLL0, DLL0
// LL03(_down), LL02(_down), LL01(_down), LL00(_down)
//////////////////////////////////////////////////////////////////////////////

var toDeg = function (value) {
  var deg = value | 0;
  var frac = Math . abs (value - deg);
  var min = (frac * 60) | 0;
  var sec = Math . round (frac * 3600 - min * 60);
  if (sec == 60) {sec = 0; min += 1;}
  if (min == 60) {min = 0; deg += 1;}
  sec = sec | 0;
  if (min < 10) min = "0" + min;
  if (sec < 10) sec = "0" + sec;
  return deg  + ":" + min + ":" + sec;
};
var scale_A = function (height, options) {
  var s = new spacer (height, options);
  if (! options || options . draw_c == undefined) s . draw_c = false;
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
  if (! options || options . draw_c == undefined) s . draw_c = false;
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
  if (! options || options . draw_c == undefined) s . draw_c = false;
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
		mark (ctx, s . indices [0], 0, s . height * 0.5);
		draw_log_1R (ctx, length * 2, s . height, 0.5 + s . right_extension * 0.5, s);
		ctx . translate (-2 * length, 0);
		draw_log_1L (ctx, length * 2, s . height, 1 - s . left_extension * 0.5, s);
	};
	return s;
};
var scale_R2 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {return Math . pow (10, 0.5 + location * 0.5);};
	s . location = function (value, length) {return Math . log10 (value) * 2 - 1;};
	s . draw = function (ctx, length) {
		ctx . translate (- length, s . height);
		draw_log_1L (ctx, length * 2, s . height, 0.5 - s . left_extension * 0.5, s);
		ctx . translate (2 * length, 0);
		mark (ctx, s . indices [1], 0, s . height * 0.5);
		draw_log_1R (ctx, length * 2, s . height, s . right_extension * 0.5, s);
	};
	return s;
};
var scale_W1 = function (height, options) {
	var s = new scale_R1 (height, options);
	s . draw = function (ctx, length) {
		mark (ctx, s . indices [0], 0, - s . height * 0.5);
		draw_log_1R (ctx, length * 2, - s . height, 0.5 + s . right_extension * 0.5, s);
		ctx . translate (-2 * length, 0);
		draw_log_1L (ctx, length * 2, - s . height, 1 - s . left_extension * 0.5, s);
	};
	return s;
};
var scale_W2 = function (height, options) {
	var s = new scale_R2 (height, options);
	s . draw = function (ctx, length) {
		ctx . translate (- length, 0);
		draw_log_1L (ctx, length * 2, - s . height, 0.5 - s . left_extension * 0.5, s);
		ctx . translate (2 * length, 0);
		mark (ctx, s . indices [1], 0, - s . height * 0.5);
		draw_log_1R (ctx, length * 2, - s . height, s . right_extension * 0.5, s);
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
var scale_LR12 = function (height, options) {
	var s = new spacer (height, options);
	s . value = function (location) {return location * 5;};
	s . location = function (value) {return value * 0.2;};
	s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_lin_12 (ctx, length, s . height, s);};
	return s;
};
var scale_LW12 = function (height, options) {
	var s = new scale_LR12 (height, options);
	s . draw = function (ctx, length) {draw_lin_12 (ctx, length, - s . height, s);};
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
  s . display = function (location, precision) {return toDeg (this . value (location));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_sine_deg (ctx, length, s . height, s);};
  return s;
};
var scale_S_down = function (height, options) {
  var s = new scale_S (height, options);
  s . draw = function (ctx, length) {draw_sine_deg (ctx, length, - s . height, s);};
  return s;
};
var scale_STdec = function (height, options) {
  var s = new spacer (height, options);
  if (! options || options . draw_halves == undefined) s . draw_halves = true;
  if (! options || options . draw_st_corrections == undefined) s . draw_st_corrections = true;
  if (! options || options . draw_c == undefined) s . draw_c = false;
  if (! options || options . draw_pi == undefined) s . draw_pi = false;
  if (! options || options . draw_e == undefined) s . draw_e = false;
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
  s . display = function (location, precision) {return toDeg (this . value (location));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_small_sine_deg (ctx, length, s . height, s);};
  return s;
};
var scale_ST_down = function (height, options) {
  var s = new scale_ST (height, options);
  s . draw = function (ctx, length) {draw_small_sine_deg (ctx, length, - s . height, s);};
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
  s . display = function (location, precision) {return toDeg (this . value (location));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_tan_deg (ctx, length, s . height, s);};
  return s;
};
var scale_T_down = function (height, options) {
  var s = new scale_T (height, options);
  s . draw = function (ctx, length) {draw_tan_deg (ctx, length, - s . height, s);};
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
var scale_T1 = function (height, options) {
  var s = new scale_T1dec (height, options);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_big_tan_deg (ctx, length, s . height, s);};
  return s;
};
var scale_T1_down = function (height, options) {
  var s = new scale_T1 (height, options);
  s . draw = function (ctx, length) {draw_big_tan_deg (ctx, length, - s . height, s);};
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
var scale_CLL0 = function (height, options) {
  var s = new scale_C (height, options);
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_log_ll0 (ctx, length, s . height, s);};
  return s;
};
var scale_DLL0 = function (height, options) {
  var s = new scale_D (height, options);
  s . draw = function (ctx, length) {draw_log_ll0 (ctx, length, - s . height, s);};
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
var scale_LL00 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (Math . E, - Math . pow (10, location - 3));};
  s . location = function (value) {return 3 + Math . log10 (- Math . log (value));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_LL00 (ctx, length, s . height, s);};
  return s;
};
var scale_LL00_down = function (height, options) {
  var s = new scale_LL00 (height, options);
  s . draw = function (ctx, length) {draw_LL00 (ctx, length, - s . height, s);};
  return s;
};
