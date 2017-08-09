
var scale_A = function (height, options) {
  var s = new spacer (height, options);
  if (! options . draw_c) s . draw_c = false;
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
  if (! options . draw_c) s . draw_c = false;
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
  if (! options . draw_c) s . draw_c = false;
  s . value = function (location) {return Math . pow (10, location * 3);};
  s . location = function (value) {return Math . log10 (value) / 3;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_log_log_log (ctx, length, s . height, s);};
  return s;
};

var scale_J = function (height, options) {
  var s = new scale_K (height, options);
  s . draw = function (ctx, length) {draw_log_log_log (ctx, length, - s . height, s);};
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

var scale_S = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . asin (Math . pow (10, location - 1)) * 180 / Math . PI;};
  s . location = function (value) {return 1 + Math . log10 (Math . sin (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_sine_deg (ctx, length, s . height, s);};
  return s;
};

var scale_S_down = function (height, options) {
  var s = new scale_S (height, options);
  s . draw = function (ctx, length) {draw_sine_deg (ctx, length, - s . height, s);};
  return s;
};

var scale_ST = function (height, options) {
  var s = new spacer (height, options);
  if (! options . draw_halves) s . draw_halves = true;
  if (! options . draw_st_corrections) s . draw_st_corrections = true;
  if (! options . draw_c) s . draw_c = false;
  if (! options . draw_pi) s . draw_pi = false;
  if (! options . draw_e) s . draw_e = false;
  s . value = function (location) {return Math . pow (10, location) * 1.8 / Math . PI;};
  s . location = function (value) {return Math . log10 (value * Math . PI / 1.8);};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_small_sine_deg (ctx, length, s . height, s);};
  return s;
};

var scale_ST_down = function (height, options) {
  var s = new scale_ST (height, options);
  s . draw = function (ctx, length) {draw_small_sine_deg (ctx, length, - s . height, s);};
  return s;
};

var scale_T = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . atan (Math . pow (10, location - 1)) * 180 / Math . PI;};
  s . location = function (value) {return 1 + Math . log10 (Math . tan (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_tan_deg (ctx, length, s . height, s);};
  return s;
};

var scale_T_down = function (height, options) {
  var s = new scale_T (height, options);
  s . draw = function (ctx, length) {draw_tan_deg (ctx, length, - s . height, s);};
  return s;
};

var scale_TCT = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . atan (Math . pow (10, location - 1)) * 180 / Math . PI;};
  s . location = function (value) {return 1 + Math . log10 (Math . tan (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_tan_cotan_deg (ctx, length, s . height, s);};
  return s;
};

var scale_TCT_down = function (height, options) {
  var s = new scale_TCT (height, options);
  s . draw = function (ctx, length) {draw_tan_cotan_deg (ctx, length, - s . height, s);};
  return s;
};

var scale_T1 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . atan (Math . pow (10, location - 2)) * 180 / Math . PI;};
  s . location = function (value) {return 2 + Math . log10 (Math . tan (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_big_tan_deg (ctx, length, s . height, s);};
  return s;
};

var scale_T1_down = function (height, options) {
  var s = new scale_T1 (height, options);
  s . draw = function (ctx, length) {draw_big_tan_deg (ctx, length, - s . height, s);};
  return s;
};

var scale_TCT1 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . atan (Math . pow (10, location - 2)) * 180 / Math . PI;};
  s . location = function (value) {return 2 + Math . log10 (Math . tan (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_big_tan_deg (ctx, length, s . height, s);};
  return s;
};

var scale_TCT1_down = function (height, options) {
  var s = new scale_TCT1 (height, options);
  s . draw = function (ctx, length) {draw_big_tan_cotan_deg (ctx, length, - s . height, s);};
  return s;
};

var scale_SC = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . asin (Math . pow (10, location - 1)) * 180 / Math . PI;};
  s . location = function (value) {return 1 + Math . log10 (Math . sin (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_sine_cosine_deg (ctx, length, s . height, s);};
  return s;
};

var scale_SC_down = function (height, options) {
  var s = new scale_SC (height, options);
  s . draw = function (ctx, length) {draw_sine_cosine_deg (ctx, length, - s . height, s);};
  return s;
};

var scale_CS = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . asin (Math . pow (10, location - 1)) * 180 / Math . PI;};
  s . location = function (value) {return 1 + Math . log10 (Math . sin (value * Math . PI / 180));};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_cosine_sine_deg (ctx, length, s . height, s);};
  return s;
};

var scale_CS_down = function (height, options) {
  var s = new scale_CS (height, options);
  s . draw = function (ctx, length) {draw_cosine_sine_deg (ctx, length, - s . height, s);};
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



