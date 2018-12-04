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

var scale_Pressure_Atmosphere_mmHg = function (height, options) {spacer . call (this, height, options);}; inherit (scale_Pressure_Atmosphere_mmHg, spacer);
scale_Pressure_Atmosphere_mmHg . prototype . draw_pi = false;
scale_Pressure_Atmosphere_mmHg . prototype . draw_e = false;
scale_Pressure_Atmosphere_mmHg . prototype . draw_c = false;
scale_Pressure_Atmosphere_mmHg . prototype . value = function (location) {
	ret = Math . pow (10, location * 6 - 3) / 1.03323;
	return ret < 1 ? Math . pow (10, location * 6 - 3) / 0.0013595100263597 : ret;
};
scale_Pressure_Atmosphere_mmHg . prototype . location = function () {
	var threshold = (Math . log10 (1.03323) + 3) / 6;
	return function (value, location) {
		return location < threshold ? (Math . log10 (value * 0.0013595100263597) + 3) / 6 : (Math . log10 (value * 1.03323) + 3) / 6;
	};
} ();
scale_Pressure_Atmosphere_mmHg . prototype . draw_mmhg = function (ctx, length, height) {
	ctx . save ();
	ctx . translate (length * this . location (1, 1), 0);
	var lgth = length * (this . location (10, 1) - this . location (1, 1));
	mark (ctx, this . indices [3], 0, height * 0.5); draw_log_1R (ctx, lgth, height, 1, this); ctx . translate (lgth, 0);
	mark (ctx, this . indices [4], 0, height * 0.5); draw_log_1R (ctx, lgth, height, 1, this); ctx . translate (lgth, 0);
	mark (ctx, this . indices [5], 0, height * 0.5); draw_log_1R (ctx, lgth, height, 1, this); ctx . translate (lgth, 0);
	mark (ctx, this . indices [6], 0, height * 0.5);
	ctx . restore ();
	ctx . translate (length * this . location (1, 0), 0);
	lgth = length * (this . location (10, 0) - this . location (1, 0));
	ctx . fillStyle = this . marking_alt ? this . marking_alt : this . alt;
	mark (ctx, this . indices [0], 0, height * 0.5); draw_log_1R (ctx, lgth, height, 1, this); ctx . translate (lgth, 0);
	mark (ctx, this . indices [1], 0, height * 0.5); draw_log_1R (ctx, lgth, height, 1, this); ctx . translate (lgth, 0);
	mark (ctx, this . indices [2], 0, height * 0.5); draw_log_1R (ctx, lgth, height, 0.87, this);
};
scale_Pressure_Atmosphere_mmHg . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); this . draw_mmhg (ctx, length, this . height);};
scale_Pressure_Atmosphere_mmHg . prototype . indices = ['1', '10', '100', '1', '10', '100', '1000'];

var scale_Pressure_Atmosphere_mmHg_down = function (height, options) {scale_Pressure_Atmosphere_mmHg . call (this, height, options);};
inherit (scale_Pressure_Atmosphere_mmHg_down, scale_Pressure_Atmosphere_mmHg);
scale_Pressure_Atmosphere_mmHg_down . prototype . draw = function (ctx, length) {this . draw_mmhg (ctx, length, - this . height);};

var scale_Pressure_KgPerCm2 = function (height, options) {spacer . call (this, height, options);}; inherit (scale_Pressure_KgPerCm2, spacer);
scale_Pressure_KgPerCm2 . prototype . draw_pi = false;
scale_Pressure_KgPerCm2 . prototype . draw_e = false;
scale_Pressure_KgPerCm2 . prototype . draw_c = false;
scale_Pressure_KgPerCm2 . prototype . value = function (location) {return Math . pow (10, location * 6 - 3);};
scale_Pressure_KgPerCm2 . prototype . location = function (value) {return (Math . log10 (value) + 3) / 6;};
scale_Pressure_KgPerCm2 . prototype . draw_kgpercm2 = function (ctx, length, height) {
	length /= 6;
	ctx . fillStyle = this . marking_alt ? this . marking_alt : this . alt;
	mark (ctx, this . indices [0], 0, height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
	mark (ctx, this . indices [1], 0, height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
	mark (ctx, this . indices [2], 0, height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
	ctx . fillStyle = this . marking_colour ? this . marking_colour : this . colour;
	mark (ctx, this . indices [3], 0, height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
	mark (ctx, this . indices [4], 0, height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
	mark (ctx, this . indices [5], 0, height * 0.5); draw_log_1R (ctx, length, height, 1, this); ctx . translate (length, 0);
	mark (ctx, this . indices [6], 0, height * 0.5);
};
scale_Pressure_KgPerCm2 . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); this . draw_kgpercm2 (ctx, length, this . height);};
scale_Pressure_KgPerCm2 . prototype . indices = ['.001', '.01', '.1', '1', '10', '100', '1000'];

var scale_Pressure_KgPerCm2_down = function (height, options) {scale_Pressure_KgPerCm2 . call (this, height, options);};
inherit (scale_Pressure_KgPerCm2_down, scale_Pressure_KgPerCm2);
scale_Pressure_KgPerCm2_down . prototype . draw = function (ctx, length) {this . draw_kgpercm2 (ctx, length, - this . height);};
