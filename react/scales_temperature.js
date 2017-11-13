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
// Kelvin(_down), Centigrade(_down), Fahrenheit(_down)
//////////////////////////////////////////////////////////////////////////////

var scale_Kelvin = function (height, options) {
  var s = new scale_C (height, options);
  s . indices = [100, 1000];
  s . value = function (location) {return Math . pow (10, location + 2);};
  s . location = function (value) {return Math . log10 (value) - 2;};
  return s;
};

var scale_Kelvin_down = function (height, options) {
  var s = new scale_D (height, options);
  s . indices = [100, 1000];
  s . value = function (location) {return Math . pow (10, location + 2);};
  s . location = function (value) {return Math . log10 (value) - 2;};
  return s;
};

var draw_fahrenheits = function (ctx, length, height, s) {
  var h5 = height * 0.5, h4 = height * 0.4, h3 = height * 0.3, h2 = height * 0.2;
  var limit = 1 + s . right_extension;
  draw_MRS (ctx, s . location, length, -200, 2000, 100, limit, h5);
  draw_XR (ctx, s . location, length, -200, 1000, limit, h4, 100, 50, 100);
  draw_XR (ctx, s . location, length, 100, 1000, limit, h3, 50, 10, 50);
  draw_XR (ctx, s . location, length, 100, 1000, limit, h2, 10, 5, 10);
  draw_XR (ctx, s . location, length, 1000, 2000, limit, h3, 100, 50, 100);
  draw_XR (ctx, s . location, length, 1000, 2000, limit, h2, 50, 10, 50);
  limit = - s . left_extension;
  draw_MLS (ctx, s . location, length, -1000, -250, 50, limit, h5);
  draw_XL (ctx, s . location, length, -1000, 100, limit, h3, 50, 10, 50);
  draw_XL (ctx, s . location, length, -1000, 100, limit, h2, 10, 2, 10);
};

var scale_Fahrenheit = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location + 2) * 1.8 - 459.67;};
  s . location = function (value) {return Math . log10 ((value + 459.67) / 1.8) - 2;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_fahrenheits (ctx, length, s . height, s);};
  return s;
};

var scale_Fahrenheit_down = function (height, options) {
  var s = new scale_Fahrenheit (height, options);
  s . draw = function (ctx, length) {draw_fahrenheits (ctx, length, - s . height, s);};
  return s;
};

var draw_centigrades = function (ctx, length, height, s) {
  var h5 = height * 0.5, h4 = height * 0.4, h3 = height * 0.3, h2 = height * 0.2;
  var limit = 1 + s . right_extension;
  draw_MRS (ctx, s . location, length, -100, 1000, 100, limit, h5);
  draw_XR (ctx, s . location, length, -100, 1000, limit, h4, 100, 50, 100); 
  draw_XR (ctx, s . location, length, -100, 1000, limit, h3, 50, 10, 50);
  draw_XR (ctx, s . location, length, -100, 300, limit, h2, 10, 2, 10);
  draw_XR (ctx, s . location, length, 300, 1000, limit, h2, 10, 5, 10);
  limit = - s . left_extension;
  draw_MLS (ctx, s . location, length, -1000, -150, 50, limit, h5);
  draw_XL (ctx, s . location, length, -1000, -100, limit, h4, 50, 10, 50);
  draw_XL (ctx, s . location, length, -1000, -100, limit, h3, 10, 5, 10);
  draw_XL (ctx, s . location, length, -1000, -100, limit, h2, 5, 1, 5);
};

var scale_Centigrade = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location + 2) - 273.15;};
  s . location = function (value) {return Math . log10 (value + 273.15) - 2;};
  s . draw = function (ctx, length) {ctx . translate (0, s . height); draw_centigrades (ctx, length, s . height, s);};
  return s;
};

var scale_Centigrade_down = function (height, options) {
  var s = new scale_Centigrade (height, options);
  s . draw = function (ctx, length) {draw_centigrades (ctx, length, - s . height, s);};
  return s;
};