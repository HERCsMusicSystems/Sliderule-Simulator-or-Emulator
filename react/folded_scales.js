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
// LL0M(_down), LL1M(_down), LL2M(_down), LL3M(_down)
// LL00M(_down), LL01M(_down), LL02M(_down), LL03M(_down)
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

var draw_LL0M = function (ctx, length, height, s) {
  var h5 = height * 0.5, h4 = height * 0.4, h3 = height * 0.3, h2 = height * 0.2;
  var limit = 1 + s . right_extension;
  draw_MRSF (ctx, s . location, length, 1.004, 1.009, 0.001, 3, limit, h5);
  draw_MRSF (ctx, s . location, length, 1.01, 1.1, 0.005, 3, limit, h5);
  draw_XR (ctx, s . location, length, 1.005, 1.01, limit, h4, 0.001, 0.0005, 0.001);
  draw_XR (ctx, s . location, length, 1.005, 1.01, limit, h3, 0.0005, 0.0001, 0.0005);
  draw_XR (ctx, s . location, length, 1.005, 1.01, limit, h2, 0.0001, 0.00005, 0.0001);
  draw_XR (ctx, s . location, length, 1.01, 1.04, limit, h4, 0.005, 0.001, 0.005);
  draw_XR (ctx, s . location, length, 1.01, 1.04, limit, h3, 0.001, 0.0005, 0.001);
  draw_XR (ctx, s . location, length, 1.01, 1.04, limit, h2, 0.0005, 0.0001, 0.0005);
  limit = - s . left_extension;
  draw_MLSF (ctx, s . location, length, 1.001, 1.003, 0.001, 3, limit, h5);
  draw_XL (ctx, s . location, length, 1.001, 1.005, limit, h4, 0.001, 0.0005, 0.001);
  draw_XL (ctx, s . location, length, 1.001, 1.005, limit, h3, 0.0005, 0.0001, 0.0005);
  draw_XL (ctx, s . location, length, 1.001, 1.005, limit, h2, 0.0001, 0.00002, 0.0001);
};

var draw_LL1M = function (ctx, length, height, s) {
  var limit = 1 + s . right_extension;
  var h5 = height * 0.5; var h2 = height * 0.2; var h3 = height * 0.3; var h4 = height * 0.4;
  draw_MRSF (ctx, s . location, length, 1.1, 1.2, 0.01, 2, limit, h5);
  draw_MRSF (ctx, s . location, length, 1.25, 1.5, 0.05, 2, limit, h5);
  draw_XR (ctx, s . location, length, 1.1, 1.2, limit, h4, 0.02, 0.01, 0.02);
	draw_XR (ctx, s . location, length, 1.1, 1.2, limit, h3, 0.01, 0.005, 0.01);
	draw_XR (ctx, s . location, length, 1.1, 1.2, limit, h2, 0.005, 0.001, 0.005);
  draw_XR (ctx, s . location, length, 1.2, 1.5, limit, h3, 0.05, 0.01, 0.05);
  draw_XR (ctx, s . location, length, 1.2, 1.5, limit, h2, 0.01, 0.002, 0.01);
  limit = - s . left_extension;
  draw_MLSF (ctx, s . location, length, 1.02, 1.05, 0.005, 3, limit, h5);
  draw_MLS (ctx, s . location, length, 1.06, 1.09, 0.01, limit, h5);
  draw_XL (ctx, s . location, length, 1.05, 1.1, limit, h2, 0.005, 0.001, 0.005);
  draw_XL (ctx, s . location, length, 1.05, 1.1, limit, h3, 0.01, 0.005, 0.01);
  draw_XL (ctx, s . location, length, 1.01, 1.05, limit, h3, 0.005, 0.001, 0.005);
  draw_XL (ctx, s . location, length, 1.04, 1.05, limit, h2, 0.001, 0.0005, 0.001);
  draw_XL (ctx, s . location, length, 1.01, 1.04, limit, h2, 0.001, 0.0002, 0.001);
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
  draw_XL (ctx, s . location, length, 1.5, 1.8, limit, h4, 0.1, 0.05, 0.1);
  draw_XL (ctx, s . location, length, 1.8, 2.5, limit, h3, 0.1, 0.05, 0.1);
  draw_XL (ctx, s . location, length, 1.8, 2.5, limit, h2, 0.05, 0.01, 0.05);
  draw_XL (ctx, s . location, length, 1.4, 1.8, limit, h3, 0.05, 0.01, 0.05);
  draw_XL (ctx, s . location, length, 1.4, 1.8, limit, h2, 0.01, 0.005, 0.01);
  draw_XL (ctx, s . location, length, 1.1, 1.4, limit, h3, 0.05, 0.01, 0.05);
  draw_XL (ctx, s . location, length, 1.1, 1.4, limit, h2, 0.01, 0.002, 0.01);
};

var draw_LL3M = function (ctx, length, height, s) {
  var h5 = height * 0.5; var h4 = height * 0.4; var h3 = height * 0.3; var h2 = height * 0.2;
	var limit = - s . left_extension;
  draw_ML (ctx, s . location, length, 1, 9, - s . left_extension, h5);
  draw_MRS (ctx, s . location, length, 10, 50, 10, 1, h5);
  draw_XL (ctx, s . location, length, 1, 3, - s . left_extension, h4, 1, 0.5, 1);
	draw_XL (ctx, s . location, length, 1, 3, - s . left_extension, h3, 0.5, 0.1, 0.5);
	draw_XL (ctx, s . location, length, 1, 3, - s . left_extension, h2, 0.1, 0.02, 0.1);
  draw_XR (ctx, s . location, length, 3, 6, limit, h4, 1, 0.5, 1);
	draw_XR (ctx, s . location, length, 3, 6, limit, h3, 0.5, 0.1, 0.5);
	draw_XL (ctx, s . location, length, 3, 6, limit, h2, 0.1, 0.05, 0.1);
	draw_XL (ctx, s . location, length, 6, 10, limit, h3, 1, 0.5, 1);
	draw_XL (ctx, s . location, length, 6, 10, limit, h2, 0.5, 0.1, 0.5);
	draw_XL (ctx, s . location, length, 10, 30, limit, h4, 10, 5, 10);
	draw_XL (ctx, s . location, length, 10, 30, limit, h3, 5, 1, 5);
	draw_XL (ctx, s . location, length, 10, 30, limit, h2, 1, 0.5, 1);
	draw_XL (ctx, s . location, length, 30, 50, limit, h3, 10, 5, 10);
	draw_XL (ctx, s . location, length, 30, 50, limit, h2, 5, 1, 5);
	draw_XL (ctx, s . location, length, 50, 100, limit, h3, 50, 10, 50);
	draw_XL (ctx, s . location, length, 50, 100, limit, h2, 10, 2, 10);
  mark (ctx, "10\u00b2", length * s . location (100), h5);
	mark (ctx, "2", length * s . location (200), h5);
	mark (ctx, "3", length * s . location (300), h5);
	mark (ctx, "4", length * s . location (400), h5);
	mark (ctx, "5", length * s . location (500), h5);
	mark (ctx, "10\u00b3", length * s . location (1000), h5);
	mark (ctx, "2", length * s . location (2000), h5);
	mark (ctx, "3", length * s . location (3000), h5);
	mark (ctx, "4", length * s . location (4000), h5);
	mark (ctx, "5", length * s . location (5000), h5);
	mark (ctx, "10\u2074", length * s . location (10000), h5);
	mark (ctx, "2", length * s . location (20000), h5);
  mark (ctx, "3", length * s . location (30000), h5);
	mark (ctx, "4", length * s . location (40000), h5);
	mark (ctx, "5", length * s . location (50000), h5);
	mark (ctx, "10\u2075", length * s . location (100000), h5);
	mark (ctx, "2", length * s . location (200000), h5);
  mark (ctx, "3", length * s . location (300000), h5);
	mark (ctx, "4", length * s . location (400000), h5);
	mark (ctx, "5", length * s . location (500000), h5);
	mark (ctx, "10\u2076", length * s . location (1000000), h5);
	mark (ctx, "2", length * s . location (2000000), h5);
  mark (ctx, "3", length * s . location (3000000), h5);
	mark (ctx, "4", length * s . location (4000000), h5);
	mark (ctx, "5", length * s . location (5000000), h5);
	mark (ctx, "10\u2077", length * s . location (10000000), h5);
	mark (ctx, "2", length * s . location (20000000), h5);
  mark (ctx, "3", length * s . location (30000000), h5);
	mark (ctx, "4", length * s . location (40000000), h5);
	mark (ctx, "5", length * s . location (50000000), h5);
	mark (ctx, "10\u2078", length * s . location (100000000), h5);
	mark (ctx, "2", length * s . location (200000000), h5);
  mark (ctx, "3", length * s . location (300000000), h5);
	mark (ctx, "4", length * s . location (400000000), h5);
	mark (ctx, "5", length * s . location (500000000), h5);
	mark (ctx, "10\u2079", length * s . location (1000000000), h5);
	mark (ctx, "2", length * s . location (2000000000), h5);
  mark (ctx, "3", length * s . location (3000000000), h5);
	mark (ctx, "4", length * s . location (4000000000), h5);
	mark (ctx, "5", length * s . location (5000000000), h5);
	mark (ctx, "10\u00b9\u2070", length * s . location (10000000000), h5);
  limit = 1 + s . right_extension;
  draw_XR (ctx, s . location, length, 100, 400, limit, h3, 100, 50, 100);
	draw_XR (ctx, s . location, length, 100, 400, limit, h2, 50, 10, 50);
	draw_XR (ctx, s . location, length, 400, 500, limit, h2, 100, 20, 100);
	draw_XR (ctx, s . location, length, 500, 1000, limit, h3, 500, 100, 500);
	draw_XR (ctx, s . location, length, 500, 1000, limit, h2, 100, 50, 100);
  draw_XR (ctx, s . location, length, 1000, 3000, limit, h3, 1000, 500, 1000);
	draw_XR (ctx, s . location, length, 1000, 3000, limit, h2, 500, 100, 500);
	draw_XR (ctx, s . location, length, 3000, 5000, limit, h2, 1000, 200, 1000);
	draw_XR (ctx, s . location, length, 5000, 10000, limit, h3, 5000, 1000, 5000);
	draw_XR (ctx, s . location, length, 5000, 10000, limit, h2, 1000, 500, 1000);
  draw_XR (ctx, s . location, length, 10000, 20000, limit, h3, 10000, 5000, 10000);
	draw_XR (ctx, s . location, length, 10000, 20000, limit, h2, 5000, 1000, 5000);
	draw_XR (ctx, s . location, length, 20000, 50000, limit, h2, 10000, 2000, 10000);
	draw_XR (ctx, s . location, length, 50000, 100000, limit, h3, 50000, 10000, 50000);
	draw_XR (ctx, s . location, length, 50000, 100000, limit, h2, 10000, 5000, 10000);
  draw_XR (ctx, s . location, length, 100000, 200000, limit, h3, 100000, 50000, 100000);
	draw_XR (ctx, s . location, length, 100000, 200000, limit, h2, 50000, 10000, 50000);
	draw_XR (ctx, s . location, length, 200000, 500000, limit, h2, 100000, 20000, 100000);
	draw_XR (ctx, s . location, length, 500000, 1000000, limit, h3, 500000, 100000, 500000);
	draw_XR (ctx, s . location, length, 500000, 1000000, limit, h2, 100000, 50000, 100000);
  draw_XR (ctx, s . location, length, 1000000, 2000000, limit, h3, 1000000, 500000, 1000000);
	draw_XR (ctx, s . location, length, 1000000, 2000000, limit, h2, 500000, 100000, 500000);
	draw_XR (ctx, s . location, length, 2000000, 5000000, limit, h2, 1000000, 200000, 1000000);
	draw_XR (ctx, s . location, length, 5000000, 10000000, limit, h3, 5000000, 1000000, 5000000);
	draw_XR (ctx, s . location, length, 5000000, 10000000, limit, h2, 1000000, 500000, 1000000);
  draw_XR (ctx, s . location, length, 10000000, 20000000, limit, h3, 10000000, 5000000, 10000000);
	draw_XR (ctx, s . location, length, 10000000, 20000000, limit, h2, 5000000, 1000000, 5000000);
	draw_XR (ctx, s . location, length, 20000000, 40000000, limit, h2, 10000000, 2000000, 10000000);
  draw_XR (ctx, s . location, length, 40000000, 50000000, limit, h2, 10000000, 5000000, 10000000);
	draw_XR (ctx, s . location, length, 50000000, 100000000, limit, h3, 50000000, 10000000, 50000000);
	draw_XR (ctx, s . location, length, 50000000, 100000000, limit, h2, 10000000, 5000000, 10000000);
  draw_XR (ctx, s . location, length, 100000000, 300000000, limit, h2, 100000000, 20000000, 100000000);
  draw_XR (ctx, s . location, length, 300000000, 500000000, limit, h2, 100000000, 50000000, 100000000);
	draw_XR (ctx, s . location, length, 500000000, 1000000000, limit, h2, 500000000, 100000000, 500000000);
  draw_XR (ctx, s . location, length, 1000000000, 2000000000, limit, h2, 1000000000, 200000000, 1000000000);
  draw_XR (ctx, s . location, length, 2000000000, 5000000000, limit, h2, 1000000000, 500000000, 1000000000);
	draw_XR (ctx, s . location, length, 5000000000, 10000000000, limit, h2, 5000000000, 1000000000, 5000000000);
};

var scale_LL0M = inherit (spacer);
scale_LL0M . prototype . value = function (location) {return Math . pow (Math . E, Math . pow (10, location - 3) / Math . log10 (Math . E));};
scale_LL0M . prototype . location = function (value) {return 3 + Math . log10 (Math . log (value) * Math . log10 (Math . E));};
scale_LL0M . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); draw_LL0M (ctx, length, this . height, this);};

var scale_LL0M_down = inherit (scale_LL0M);
scale_LL0M_down . prototype . draw = function (ctx, length) {draw_LL0M (ctx, length, - this . height, this);};

var scale_LL1M = inherit (spacer);
scale_LL1M . prototype . value = function (location) {return Math . pow (Math . E, Math . pow (10, location - 2) / Math . log10 (Math . E));};
scale_LL1M . prototype . location = function (value) {return 2 + Math . log10 (Math . log (value) * Math . log10 (Math . E));};
scale_LL1M . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); draw_LL1M (ctx, length, this . height, this);};

var scale_LL1M_down = inherit (scale_LL1M);
scale_LL1M_down . prototype . draw = function (ctx, length) {draw_LL1M (ctx, length, - this . height, this);};

var scale_LL2M = inherit (spacer);
scale_LL2M . prototype . value = function (location) {return Math . pow (Math . E, Math . pow (10, location - 1) / Math . log10 (Math . E));};
scale_LL2M . prototype . location = function (value) {return 1 + Math . log10 (Math . log (value) * Math . log10 (Math . E));};
scale_LL2M . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); draw_LL2M (ctx, length, this . height, this);};

var scale_LL2M_down = inherit (scale_LL2M);
scale_LL2M_down . prototype . draw = function (ctx, length) {draw_LL2M (ctx, length, - this . height, this);};

var scale_LL3M = inherit (spacer);
scale_LL3M . prototype . value = function (location) {return Math . pow (Math . E, Math . pow (10, location) / Math . log10 (Math . E));};
scale_LL3M . prototype . location = function (value) {return Math . log10 (Math . log (value) * Math . log10 (Math . E));};
scale_LL3M . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); draw_LL3M (ctx, length, this . height, this);};

var scale_LL3M_down = inherit (scale_LL3M);
scale_LL3M_down . prototype . draw = function (ctx, length) {draw_LL3M (ctx, length, - this . height, this);};

var draw_LL00M = function (ctx, length, height, s) {
  var h5 = height * 0.5, h4 = height * 0.4, h3 = height * 0.3, h2 = height * 0.2;
  var limit = - s . left_extension;
  draw_MLSI (ctx, s . location, length, 0.999, 0.991, 0.001, limit, h5);
  draw_XLI (ctx, s . location, length, 0.999, 0.99, limit, h4, 0.001, 0.0005, 0.001);
  draw_XLI (ctx, s . location, length, 0.999, 0.99, limit, h3, 0.0005, 0.0001, 0.0005);
  draw_XLI (ctx, s . location, length, 0.995, 0.99, limit, h2, 0.0001, 0.00005, 0.0001);
  draw_XLI (ctx, s . location, length, 0.999, 0.995, limit, h2, 0.0001, 0.00002, 0.0001);
  limit = 1 + s . right_extension;
  draw_MRSI (ctx, s . location, length, 0.99, 0.97, 0.005, limit, h5);
  draw_XRI (ctx, s . location, length, 0.99, 0.97, limit, h4, 0.005, 0.001, 0.005);
  draw_XRI (ctx, s . location, length, 0.99, 0.97, limit, h3, 0.001, 0.0005, 0.001);
  draw_XRI (ctx, s . location, length, 0.99, 0.97, limit, h2, 0.0005, 0.0001, 0.0005);
};

var draw_LL01M = function (ctx, length, height, s) {
  var h5 = height * 0.5, h4 = height * 0.4, h3 = height * 0.3, h2 = height * 0.2;
  var limit = - s . left_extension;
  draw_MLSI (ctx, s . location, length, 0.99, 0.91, 0.01, limit, h5);
  draw_XLI (ctx, s . location, length, 0.99, 0.9, limit, h4, 0.01, 0.005, 0.01);
  draw_XLI (ctx, s . location, length, 0.99, 0.9, limit, h3, 0.005, 0.001, 0.005);
  draw_XLI (ctx, s . location, length, 0.95, 0.9, limit, h2, 0.001, 0.0005, 0.001);
  draw_XLI (ctx, s . location, length, 0.99, 0.95, limit, h2, 0.001, 0.0002, 0.001);
  limit = 1 + s . right_extension;
  draw_MRSI (ctx, s . location, length, 0.9, 0.7, 0.05, 2, h5);
  draw_XRI (ctx, s . location, length, 0.9, 0.7, limit, h5, 0.05, 0.01, 0.05);
  draw_XRI (ctx, s . location, length, 0.9, 0.7, limit, h3, 0.01, 0.005, 0.01);
  draw_XRI (ctx, s . location, length, 0.9, 0.7, limit, h2, 0.005, 0.001, 0.005);
};

var draw_LL02M = function (ctx, length, height, s) {
  var h5 = height * 0.5, h4 = height * 0.4, h3 = height * 0.3, h2 = height * 0.2;
  var limit = - s . left_extension;
  draw_MLSI (ctx, s . location, length, 1, 0.55, 0.05, limit, h5);
  draw_XLI (ctx, s . location, length, 1, 0.5, limit, h3, 0.5, 0.01, 0.5);
  draw_XLI (ctx, s . location, length, 1, 0.5, limit, h2, 0.01, 0.002, 0.01);
  limit = 1 + s . right_extension;
  draw_MRSI (ctx, s . location, length, 0.5, 0, 0.05, limit, h5);
  draw_XRI (ctx, s . location, length, 0.5, 0, limit, h3, 0.5, 0.01, 0.5);
  draw_XRI (ctx, s . location, length, 0.5, 0, limit, h2, 0.01, 0.002, 0.01);
};

var draw_LL03M = function (ctx, length, height, s) {
  var h5 = height * 0.5, h4 = height * 0.4, h3 = height * 0.3, h2 = height * 0.2;
  var limit = - s . left_extension;
  draw_MLSI (ctx, s . location, length, 0.4, 0.02, 0.02, limit, h5);
  draw_XLI (ctx, s . location, length, 0.4, 0.02, limit, h4, 0.02, 0.01, 0.02);
  draw_XLI (ctx, s . location, length, 0.4, 0.01, limit, h3, 0.01, 0.005, 0.01);
  draw_XLI (ctx, s . location, length, 0.4, 0.01, limit, h2, 0.005, 0.001, 0.005);
  mark (ctx, "10\u207b\u00b2", length * s . location (0.01), h5);
  mark (ctx, "5", length * s . location (0.005), h5);
  mark (ctx, "2", length * s . location (0.002), h5);
  draw_XLI (ctx, s . location, length, 0.01, 0.005, limit, h3, 0.005, 0.001, 0.005);
  draw_XLI (ctx, s . location, length, 0.005, 0.002, limit, h3, 0.003, 0.001, 0.003);
  draw_XLI (ctx, s . location, length, 0.01, 0.001, limit, h2, 0.001, 0.0002, 0.001);
  mark (ctx, "10\u207b\u00b3", length * s . location (0.001), h5);
  mark (ctx, "5", length * s . location (0.0005), h5);
  mark (ctx, "2", length * s . location (0.0002), h5);
  draw_XLI (ctx, s . location, length, 0.001, 0.0005, limit, h3, 0.0005, 0.0001, 0.0005);
  draw_XLI (ctx, s . location, length, 0.0005, 0.0002, limit, h3, 0.0003, 0.0001, 0.0003);
  draw_XLI (ctx, s . location, length, 0.001, 0.0005, limit, h2, 0.0001, 0.00005, 0.0001);
  draw_XLI (ctx, s . location, length, 0.0005, 0.0001, limit, h2, 0.0001, 0.00002, 0.0001);
  mark (ctx, "10\u207b\u2074", length * s . location (0.0001), h5);
  limit = 1 + s . right_extension;
  mark (ctx, "5", length * s . location (0.00005), h5);
  var location = s . location (0.00002); mark (ctx, "2", length * location, h5);
  draw_XRI (ctx, s . location, length, 0.0001, 0.00005, limit, h3, 0.00005, 0.00001, 0.00005);
  draw_XRI (ctx, s . location, length, 0.00005, 0.00002, limit, h3, 0.00003, 0.00001, 0.00003);
  draw_XRI (ctx, s . location, length, 0.0001, 0.00002, limit, h2, 0.00001, 0.000005, 0.00001);
  draw_XRI (ctx, s . location, length, 0.00002, 0.00001, limit, h2, 0.00001, 0.000002, 0.000009);
  location = s . location (0.00001); mark (ctx, "10\u207b\u2075", length * location, h5);
  location = s . location (0.000005); mark (ctx, "5", length * location, h5);
  draw_XRI (ctx, s . location, length, 0.00001, 0.000005, limit, h2, 0.000005, 0.000001, 0.000005);
  draw_XRI (ctx, s . location, length, 0.000005, 0.000002, limit, h3, 0.000003, 0.000001, 0.000003);
  draw_XRI (ctx, s . location, length, 0.000005, 0.000002, limit, h2, 0.000001, 0.0000005, 0.000001);
  draw_XRI (ctx, s . location, length, 0.000002, 0.000001, limit, h2, 0.000001, 0.0000002, 0.000001);
  location = s . location (0.000002); mark (ctx, "2", length * location, h5);
  location = s . location (0.000001); mark (ctx, "10\u207b\u2076", length * location, h5);
  location = s . location (0.0000005); mark (ctx, "5", length * location, h5);
  draw_XRI (ctx, s . location, length, 0.000001, 0.0000005, limit, h2, 0.0000005, 0.0000001, 0.0000005);
  draw_XRI (ctx, s . location, length, 0.0000005, 0.0000002, limit, h3, 0.0000003, 0.0000001, 0.0000003);
  draw_XRI (ctx, s . location, length, 0.0000005, 0.0000002, limit, h2, 0.0000001, 0.00000005, 0.0000001);
  draw_XRI (ctx, s . location, length, 0.0000002, 0.0000001, limit, h2, 0.0000001, 0.00000002, 0.0000001);
  location = s . location (0.0000002); mark (ctx, "2", length * location, h5);
  location = s . location (0.0000001); mark (ctx, "10\u207b\u2077", length * location, h5);
  location = s . location (0.00000005); mark (ctx, "5", length * location, h5);
  draw_XRI (ctx, s . location, length, 0.0000001, 0.00000005, limit, h2, 0.00000005, 0.00000001, 0.00000005);
  draw_XRI (ctx, s . location, length, 0.00000005, 0.00000002, limit, h3, 0.00000003, 0.00000001, 0.00000003);
  draw_XRI (ctx, s . location, length, 0.00000005, 0.00000002, limit, h2, 0.00000001, 0.000000005, 0.00000001);
  draw_XRI (ctx, s . location, length, 0.00000002, 0.00000001, limit, h2, 0.00000001, 0.000000002, 0.00000001);
  location = s . location (0.00000002); mark (ctx, "2", length * location, h5);
  location = s . location (0.00000001); mark (ctx, "10\u207b\u2078", length * location, h5);
  location = s . location (0.000000005); mark (ctx, "5", length * location, h5);
  draw_XRI (ctx, s . location, length, 0.00000001, 0.000000005, limit, h2, 0.000000005, 0.000000001, 0.000000005);
  draw_XRI (ctx, s . location, length, 0.000000005, 0.000000002, limit, h3, 0.000000003, 0.000000001, 0.000000003);
  draw_XRI (ctx, s . location, length, 0.000000005, 0.0000000019, limit, h2, 0.000000001, 0.0000000005, 0.000000001);
  draw_XRI (ctx, s . location, length, 0.000000002, 0.0000000009, limit, h2, 0.000000001, 0.0000000002, 0.000000001);
  location = s . location (0.000000002); mark (ctx, "2", length * location, h5);
  location = s . location (0.000000001); mark (ctx, "10\u207b\u2079", length * location, h5);
  tick (ctx, s . location (0.0000000009) * length, h2);
  tick (ctx, s . location (0.0000000008) * length, h2);
  tick (ctx, s . location (0.0000000007) * length, h2);
  tick (ctx, s . location (0.0000000006) * length, h2);
  location = s . location (0.0000000005); mark (ctx, "5", length * location, h5);
  tick (ctx, s . location (0.00000000045) * length, h2);
  tick (ctx, s . location (0.0000000004) * length, h3);
  tick (ctx, s . location (0.00000000035) * length, h2);
  tick (ctx, s . location (0.0000000003) * length, h3);
  tick (ctx, s . location (0.00000000025) * length, h2);
  location = s . location (0.0000000002); mark (ctx, "2", length * location, h5);
  location = s . location (0.0000000001); mark (ctx, "10\u207b\u00b9\u2070", length * location, h5);
  tick (ctx, s . location (0.00000000018) * length, h2);
  tick (ctx, s . location (0.00000000016) * length, h2);
  tick (ctx, s . location (0.00000000014) * length, h2);
  tick (ctx, s . location (0.00000000012) * length, h2);
};

var scale_LL00M = inherit (spacer);
scale_LL00M . prototype . value = function (location) {return Math . pow (Math . E, - Math . pow (10, location - 3) / Math . log10 (Math . E));};
scale_LL00M . prototype . location = function (value) {return 3 + Math . log10 (- Math . log (value) * Math . log10 (Math . E));};
scale_LL00M . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); draw_LL00M (ctx, length, this . height, this);};

var scale_LL00M_down = inherit (scale_LL00M);
scale_LL00M_down . prototype . draw = function (ctx, length) {draw_LL00M (ctx, length, - this . height, this);};

var scale_LL01M = inherit (spacer);
scale_LL01M . prototype . value = function (location) {return Math . pow (Math . E, - Math . pow (10, location - 2) / Math . log10 (Math . E));};
scale_LL01M . prototype . location = function (value) {return 2 + Math . log10 (- Math . log (value) * Math . log10 (Math . E));};
scale_LL01M . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); draw_LL01M (ctx, length, this . height, this);};

var scale_LL01M_down = inherit (scale_LL01M);
scale_LL01M_down . prototype . draw = function (ctx, length) {draw_LL01M (ctx, length, - this . height, this);};

var scale_LL02M = inherit (spacer);
scale_LL02M . prototype . value = function (location) {return Math . pow (Math . E, - Math . pow (10, location - 1) / Math . log10 (Math . E));};
scale_LL02M . prototype . location = function (value) {return 1 + Math . log10 (- Math . log (value) * Math . log10 (Math . E));};
scale_LL02M . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); draw_LL02M (ctx, length, this . height, this);};

var scale_LL02M_down = inherit (scale_LL02M);
scale_LL02M_down . prototype . draw = function (ctx, length) {draw_LL02M (ctx, length, - this . height, this);};

var scale_LL03M = inherit (spacer);
scale_LL03M . prototype . value = function (location) {return Math . pow (Math . E, - Math . pow (10, location) / Math . log10 (Math . E));};
scale_LL03M . prototype . location = function (value) {return Math . log10 (- Math . log (value) * Math . log10 (Math . E));};
scale_LL03M . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); draw_LL03M (ctx, length, this . height, this);};

var scale_LL03M_down = inherit (scale_LL03M);
scale_LL03M_down . prototype . draw = function (ctx, length) {draw_LL03M (ctx, length, - this . height, this);};
