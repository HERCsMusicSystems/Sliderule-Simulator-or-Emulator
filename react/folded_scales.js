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
// CF10, DF10, CIF10, DIF10
// CF36, DF36, CIF36, DIF36
// CFM, DFM, CIFM, DIFM
// CF1M, DF1M, CIF1M, DIF1M
// LL2M(_down)
//////////////////////////////////////////////////////////////////////////////

var scale_CF10 = function (height, options) {
  var s = new spacer (height, options);
  s . fold = Math . sqrt (10);
  s . value = function (location) {return Math . pow (10, location) * this . fold * 0.1;};
  s . location = function (value) {return Math . log10 (10 * value / this . fold);};
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, length, s . height, 1 - shift + s . right_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (- length, 0);
    draw_log_1L (ctx, length, s . height, 1 - shift - s . left_extension, s);
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
    draw_log_1L (ctx, length, - s . height, 1 - shift - s . left_extension, s);
  };
  return s;
};
var scale_CIF10 = function (height, options) {
  var s = new spacer (height, options);
  s . fold = Math . sqrt (10);
  s . value = function (location) {return 10 / (Math . pow (10, location) * this . fold);};
  s . location = function (value) {return Math . log10 (1 / (this . fold * value * 0.1));};
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, - length, s . height, shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, s . height, shift - s . right_extension, s);
  };
  return s;
};
var scale_DIF10 = function (height, options) {
  var s = new scale_CIF10 (height, options);
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, - length, - s . height, shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, - s . height, shift - s . right_extension, s);
  };
  return s;
};

var scale_CF36 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location) * 0.36;};
  s . location = function (value) {return Math . log10 (10 * value / 3.6);};
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, length, s . height, 1 - shift + s . right_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (- length, 0);
    draw_log_1L (ctx, length, s . height, 1 - shift - s . left_extension, s);
  };
  return s;
};
var scale_DF36 = function (height, options) {
  var s = new scale_CF36 (height, options);
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, length, - s . height, 1 - shift + s . right_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (- length, 0);
    draw_log_1L (ctx, length, - s . height, 1 - shift - s . left_extension, s);
  };
  return s;
};
var scale_CIF36 = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return 10 / (Math . pow (10, location) * 3.6);};
  s . location = function (value) {return Math . log10 (1 / (0.36 * value));};
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, - length, s . height, shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, s . height, shift - s . right_extension, s);
  };
  return s;
};
var scale_DIF36 = function (height, options) {
  var s = new scale_CIF36 (height, options);
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, - length, - s . height, shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, - s . height, shift - s . right_extension, s);
  };
  return s;
};

var scale_CFM = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location - 1) / Math . log10 (Math . E);};
  s . location = function (value) {return Math . log10 (10 * value * Math . log10 (Math . E));};
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, length, s . height, 1 - shift + s . right_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (- length, 0);
    draw_log_1L (ctx, length, s . height, 1 - shift - s . left_extension, s);
  };
  return s;
};
var scale_DFM = function (height, options) {
  var s = new scale_CFM (height, options);
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, length, - s . height, 1 - shift + s . right_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (- length, 0);
    draw_log_1L (ctx, length, - s . height, 1 - shift - s . left_extension, s);
  };
  return s;
};
var scale_CIFM = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, 1 - location) * Math . log10 (Math . E);};
  s . location = function (value) {return 1 - Math . log10 (value / Math . log10 (Math . E));};
  s . draw = function (ctx, length) {
    var shift = this . location (1);
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, - length, s . height, shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, s . height, shift - s . right_extension, s);
  };
  return s;
};
var scale_DIFM = function (height, options) {
  var s = new scale_CIFM (height, options);
  s . draw = function (ctx, length) {
    var shift = this . location (1);
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, - length, - s . height, shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, - s . height, shift - s . right_extension, s);
  };
  return s;
};

var scale_CF1M = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location) * Math . log10 (Math . E);};
  s . location = function (value) {return Math . log10 (10 * value / Math . log10 (Math . E)) - 1;};
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, length, s . height, 1 - shift + s . right_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (- length, 0);
    draw_log_1L (ctx, length, s . height, 1 - shift - s . left_extension, s);
  };
  return s;
};
var scale_DF1M = function (height, options) {
  var s = new scale_CF1M (height, options);
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, length, - s . height, 1 - shift + s . right_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (- length, 0);
    draw_log_1L (ctx, length, - s . height, 1 - shift - s . left_extension, s);
  };
  return s;
};
var scale_CIF1M = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, - location) / Math . log10 (Math . E);};
  s . location = function (value) {return - Math . log10 (value * Math . log10 (Math . E));};
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, s . height);
    draw_log_1R (ctx, - length, s . height, shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, s . height, shift - s . right_extension, s);
  };
  return s;
};
var scale_DIF1M = function (height, options) {
  var s = new scale_CIF1M (height, options);
  s . draw = function (ctx, length) {
    var shift = s . location (1);
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, - length, - s . height, shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, - s . height, shift - s . right_extension, s);
  };
  return s;
};

var draw_LL2M = function (ctx, length, height, s) {
  var limit = 1 + s . right_extension;
	var h5 = height * 0.5; var h2 = height * 0.2; var h3 = height * 0.3; var h4 = height * 0.4;
  draw_MRS (ctx, s . location, length, 2, 3, 0.5, limit, h5);
  draw_MRS (ctx, s . location, length, 4, 20, 1, limit, h5);
  smark (ctx, '\u{1d452}', length * s . location (Math . E), h3, h5);
  draw_XR (ctx, s . location, length, 2, 2.5, limit, h4, 0.5, 0.1, 0.5);
  draw_XR (ctx, s . location, length, 2.5, 20, limit, h3, 0.5, 0.1, 0.5);
  draw_XR (ctx, s . location, length, 2.5, 4, limit, h2, 0.1, 0.02, 0.1);
  draw_XR (ctx, s . location, length, 3, 20, limit, h4, 1, 0.5, 1);
  limit = - s . left_extension;
  draw_MLSF (ctx, s . location, length, 1.1, 1.45, 0.05, 2, limit, h5);
	draw_MLS (ctx, s . location, length, 1.5, 1.9, 0.1, limit, h5);
  draw_XL (ctx, s . location, length, 1.5, 2.5, limit, h3, 0.1, 0.05, 0.1);
  draw_XL (ctx, s . location, length, 1.8, 2.5, limit, h2, 0.05, 0.01, 0.05);
  draw_XL (ctx, s . location, length, 1.4, 1.8, limit, h3, 0.05, 0.01, 0.05);
  draw_XL (ctx, s . location, length, 1.4, 1.8, limit, h2, 0.01, 0.005, 0.01);
  draw_XL (ctx, s . location, length, 1.1, 1.4, limit, h3, 0.05, 0.01, 0.05);
  draw_XL (ctx, s . location, length, 1.1, 1.4, limit, h2, 0.01, 0.002, 0.01);
};

var scale_LL2M = inherit (spacer);
scale_LL2M . prototype . value = function (location) {return Math . pow (Math . E, Math . pow (10, location - 1) / Math . log10 (Math . E));};
scale_LL2M . prototype . location = function (value) {return 1 + Math . log10 (Math . log (value) * Math . log10 (Math . E));};
scale_LL2M . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); draw_LL2M (ctx, length, this . height, this);};

var scale_LL2M_down = inherit (scale_LL2M);
scale_LL2M_down . prototype . draw = function (ctx, length) {draw_LL2M (ctx, length, - this . height, this);};
