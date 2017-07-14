
var scale_A = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location + location);};
  s . location = function (value) {return Math . log10 (value) * 0.5;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_log_log (ctx, length, height, s);};
  return s;
};

var scale_B = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location + location);};
  s . location = function (value) {return Math . log10 (value) * 0.5;};
  s . draw = function (ctx, length) {draw_log_log (ctx, length, - s . height, s);};
  return s;
};

var scale_AI = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, 2 - location - location);};
  s . location = function (value) {return 1 - Math . log10 (value) * 0.5;};
  s . draw = function (ctx, length) {ctx . translate (length, s . height); draw_log_log (ctx, - length, height, s);};
  return s;
};

var scale_BI = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, 2 - location - location);};
  s . location = function (value) {return 1 - Math . log10 (value) * 0.5;};
  s . draw = function (ctx, length) {ctx . translate (length, 0); draw_log_log (ctx, - length, - s . height, s);};
  return s;
};

var scale_C = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location);};
  s . location = function (value) {return Math . log10 (value);};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_log (ctx, length, s . height, s);};
  return s;
};

var scale_D = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location);};
  s . location = function (value) {return Math . log10 (value);};
  s . draw = function (ctx, length) {draw_log (ctx, length, - s . height, s);};
  return s;
};

var scale_K = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location * 3);};
  s . location = function (value) {return Math . log10 (value) / 3;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_log_log_log (ctx, length, s . height, s);};
  return s;
};

var scale_J = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location * 3);};
  s . location = function (value) {return Math . log10 (value) / 3;};
  s . draw = function (ctx, length) {draw_log_log_log (ctx, length, - s . height, s);};
  return s;
};

var scale_L = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return location * 10;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_lin (ctx, length, s . height, s);};
  return s;
};
var scale_M = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return location * 10;};
  s . draw = function (ctx, length) {draw_lin (ctx, length, - s . height, s);};
  return s;
};

var scale_S = function (height, options) {
  var s = new spacer (height, options);
  s . draw = function (ctx, length) {draw_deg (ctx, length, - s. height, s);};
  return s;
};
