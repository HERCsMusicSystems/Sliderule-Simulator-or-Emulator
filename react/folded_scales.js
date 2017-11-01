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
    var shift = Math . log10 (10 / s . fold);
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
    var shift = Math . log10 (10 / s . fold);
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
    var shift = Math . log10 (10 / 3.6);
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
    var shift = Math . log10 (10 / 3.6);
    ctx . translate (length * shift, 0);
    draw_log_1R (ctx, - length, - s . height, shift + s . left_extension, s);
    mark (ctx, s . indices [0], 0, - s . height * 0.5);
    ctx . translate (length, 0);
    draw_log_1L (ctx, - length, - s . height, shift - s . right_extension, s);
  };
  return s;
};

