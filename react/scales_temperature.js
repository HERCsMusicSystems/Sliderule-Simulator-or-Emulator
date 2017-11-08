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

var scale_Fahrenheit = function (height, options) {
  var s = new spacer (height, options);
  s . value = function (location) {return Math . pow (10, location + 2) * 1.8 - 459.67;};
  s . location = function (value) {return Math . log10 ((value + 459.67) / 1.8) - 2;};
  return s;
};