///////////////////////////////////////////////////////////////////////////////////
//                     Copyright (C) 2018 Dr Robert P. Wolf                      //
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
// Pressure_Atmosphere(_down)
//////////////////////////////////////////////////////////////////////////////

var scale_Pressure_Atmosphere_mmHg = function (height, options) {
	spacer . call (this, height, options);
	this . value = function (location) {return Math . pow (10, location * 6 - 3) / 1.03323;};
	this . location = function (value) {return (Math . log10 (value * 1.03323) + 3) / 6;};
};

var scale_Pressure_KgPerCm2 = function (height, options) {
	spacer . call (this, height, options);
	delete this . draw_pi; delete this . draw_c; delete this . draw_e;
	this . value = function (location) {return Math . pow (10, location * 6 - 3);};
	this . location = function (value) {return (Math . log10 (value) + 3) / 6;};
	this . draw = function (ctx, length) {
		ctx . translate (0, this . height);
		length /= 6;
		ctx . fillStyle = this . marking_alt ? this . marking_alt : this . alt;
		mark (ctx, this . indices [0], 0, this . height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
		mark (ctx, this . indices [1], 0, this . height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
		mark (ctx, this . indices [2], 0, this . height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
		ctx . fillStyle = this . marking_colour ? this . marking_colour : this . colour;
		mark (ctx, this . indices [3], 0, this . height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
		mark (ctx, this . indices [4], 0, this . height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
		mark (ctx, this . indices [5], 0, this . height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
		mark (ctx, this . indices [6], 0, this . height * 0.5);
	};
};

scale_Pressure_KgPerCm2 . prototype . indices = ['.001', '.01', '.1', '1', '10', '100', '1000'];

