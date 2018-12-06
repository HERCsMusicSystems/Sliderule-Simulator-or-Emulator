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
scale_Pressure_Atmosphere_mmHg . prototype . value = function () {
	var threshold = (Math . log10 (1.03323) + 3) / 6;
	return function (location) {return Math . pow (10, location * 6 - 3) / (location < threshold ? 0.0013595100263597 : 1.03323);};
} ();
scale_Pressure_Atmosphere_mmHg . prototype . location = function () {
	var threshold = (Math . log10 (1.03323) + 3) / 6;
	return function (value, location) {return (Math . log10 (value * (location < threshold ? 0.0013595100263597 : 1.03323)) + 3) / 6;};
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

var scale_Pressure_PSIG_InHgVac = function (height, options) {spacer . call (this, height, options);}; inherit (scale_Pressure_PSIG_InHgVac, spacer);
scale_Pressure_PSIG_InHgVac . prototype . draw_pi = false;
scale_Pressure_PSIG_InHgVac . prototype . draw_e = false;
scale_Pressure_PSIG_InHgVac . prototype . draw_c = false;
scale_Pressure_PSIG_InHgVac . prototype . value = function () {
	var threshold = (Math . log10 (1.03323) + 3) / 6;
	return function (location) {
		if (location < threshold) return (1 - Math . pow (10, location * 6 - 3) / 1.03323) * 29.921255354893;
		return (Math . pow (10, location * 6 - 3) / 1.03323 - 1) * 14.695950254;
	};
} ();
scale_Pressure_PSIG_InHgVac . prototype . psi_location = function (value) {return (Math . log10 ((value / 14.695950254 + 1) * 1.03323) + 3) / 6;};
scale_Pressure_PSIG_InHgVac . prototype . inHgVac_location = function (value) {return (Math . log10 ((1 - value / 29.921255354893) * 1.03323) + 3) / 6;};
scale_Pressure_PSIG_InHgVac . prototype . location = function () {
	var threshold = (Math . log10 (1.03323) + 3) / 6;
	return function (value, location) {return location < threshold ? this . inHgVac_location (value) : this . psi_location (value);};
} ();
scale_Pressure_PSIG_InHgVac . prototype . draw_psiginhgvac = function (ctx, length, height) {
	var h5 = height * 0.5, h4 = height * 0.4, h3 = height * 0.3, h2 = height * 0.2;
	draw_XR (ctx, this . psi_location, length, 0, 10, 1, h4, 10, 5, 10);
	draw_XR (ctx, this . psi_location, length, 0, 10, 1, h2, 5, 1, 5);
	for (var multiplier = 1; multiplier < 1000; multiplier *= 10) {
		draw_XR (ctx, this . psi_location, length, 50 * multiplier, 100 * multiplier, 1, h2, 10 * multiplier, 5 * multiplier, 10 * multiplier);
		draw_XR (ctx, this . psi_location, length, 10 * multiplier, 30 * multiplier, 1, h4, 10 * multiplier, 5 * multiplier, 10 * multiplier);
		draw_XR (ctx, this . psi_location, length, 10 * multiplier, 30 * multiplier, 1, h2, 5 * multiplier, multiplier, 5 * multiplier);
		draw_XR (ctx, this . psi_location, length, 30 * multiplier, 50 * multiplier, 1, h2, 10 * multiplier, 2 * multiplier, 10 * multiplier);
		mark (ctx, 10 * multiplier, length * this . psi_location (10 * multiplier), h5);
		mark (ctx, '2', length * this . psi_location (20 * multiplier), h5);
		mark (ctx, '3', length * this . psi_location (30 * multiplier), h5);
		mark (ctx, '4', length * this . psi_location (40 * multiplier), h5);
		mark (ctx, '5', length * this . psi_location (50 * multiplier), h5);
		tick (ctx, length * this . psi_location (60 * multiplier), h5);
		tick (ctx, length * this . psi_location (70 * multiplier), h5);
		tick (ctx, length * this . psi_location (80 * multiplier), h5);
		tick (ctx, length * this . psi_location (90 * multiplier), h5);
	}
	mark (ctx, '10000', length * this . psi_location (10000), h5);
	for (var ind = 11000; ind < 15000; ind += 1000) tick (ctx, length * this . psi_location (ind), h2);
	ctx . fillStyle = this . marking_alt ? this . marking_alt : this . alt;
	mark (ctx, '0', length * this . psi_location (0), h5);
	mark (ctx, '10', length * this . inHgVac_location (10), h5);
	mark (ctx, '20', length * this . inHgVac_location (20), h5);
	mark (ctx, '25', length * this . inHgVac_location (25), h5);
	mark (ctx, '29', length * this . inHgVac_location (29), h5);
	mark (ctx, '29.5', length * this . inHgVac_location (29.5), h5);
	draw_XR (ctx, this . inHgVac_location, length, 0, 20, 1, h4, 10, 5, 10);
	draw_XR (ctx, this . inHgVac_location, length, 0, 25, 1, h2, 5, 1, 5);
	tick (ctx, length * this . inHgVac_location (26), h2);
	tick (ctx, length * this . inHgVac_location (27), h2);
	tick (ctx, length * this . inHgVac_location (28), h2);
};
scale_Pressure_PSIG_InHgVac . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); this . draw_psiginhgvac (ctx, length, this . height);};
scale_Pressure_PSIG_InHgVac . prototype . indices = ['0', '10', '100', '1000', '10000'];

var scale_Pressure_PSIG_InHgVac_down = function (height, options) {scale_Pressure_PSIG_InHgVac . call (this, height, options);};
inherit (scale_Pressure_PSIG_InHgVac_down, scale_Pressure_PSIG_InHgVac);
scale_Pressure_PSIG_InHgVac_down . prototype . draw = function (ctx, length) {this . draw_psiginhgvac (ctx, length, - this . height);};

var pressure = function (T) {
	if (T < 0) return 0.61115 * Math . exp ((23.036 - T / 333.7) * (T / (279.82 + T))) / 98.0665;
	return 0.61121 * Math . exp ((18.678 - T / 234.5) * (T / (257.14 + T))) / 98.0665;
};

var vapour_pressures_of_water = [];
for (var ind = 0; ind <= 400; ind++) vapour_pressures_of_water . push (pressure (ind));
var ice_pressures_of_water = [];
for (var ind = 0; ind <= 30; ind++) ice_pressures_of_water . push (pressure (- ind));
var press = function (T) {
	if (T < 0) {
		if (T <= -30) return ice_pressures_of_water [30];
		T = - T;
		var ind = Math . floor (T); T -= ind;
		return ice_pressures_of_water [ind] * (1 - T) + ice_pressures_of_water [ind + 1] * T;
	}
	if (T >= 400) return vapour_pressures_of_water [ind];
	var ind = Math . floor (T); T -= ind;
	return vapour_pressures_of_water [ind] * (1 - T) + vapour_pressures_of_water [ind + 1] * T;
};
var inverpress = function (L) {
	var ind = 0;
	if (L < vapour_pressures_of_water [0]) {
		while (ind < ice_pressures_of_water . length) {
			if (ice_pressures_of_water [ind] < L) {
				if (ind === 0) return 0;
				var d1 = ice_pressures_of_water [ind - 1], d2 = ice_pressures_of_water [ind];
				var T = (d1 - L) / (d1 - d2), TL = (L - d2) / (d1 - d2);
				return - (ind * T + (ind - 1) * TL);
			}
			ind ++;
		}
		return - ice_pressures_of_water . length;
	}
	while (ind < vapour_pressures_of_water . length) {
		if (vapour_pressures_of_water [ind] > L) {
			if (ind === 0) return 0;
			var d1 = vapour_pressures_of_water [ind - 1], d2 = vapour_pressures_of_water [ind];
			var T = (L - d1) / (d2 - d1), TL = (d2 - L) / (d2 - d1);
			return (ind - 1) * TL + ind * T;
		}
		ind ++;
	}
	return vapour_pressures_of_water . length;
};

var scale_Tw = function (height, options) {spacer . call (this, height, options);}; inherit (scale_Tw, spacer);
scale_Tw . prototype . location = function (value) {return (Math . log10 (press (value)) + 3) / 6;};
scale_Tw . prototype . value = function (location) {
	return inverpress (Math . pow (10, location * 6 - 3));
};
scale_Tw . prototype . draw = function (ctx, length) {
	var h5 = this . height * 0.5, h4 = this . height * 0.4, h2 = this . height * 0.2;
	draw_XR (ctx, this . location, length, -20, 200, 1, h4, 10, 5, 10);
	draw_XR (ctx, this . location, length, -20, 200, 1, h2, 5, 1, 5);
	mark (ctx, '-20', length * this . location (-20), h5);
	mark (ctx, '-10', length * this . location (-10), h5);
	mark (ctx, '0', length * this . location (0), h5);
	mark (ctx, '10', length * this . location (10), h5);
	mark (ctx, '2', length * this . location (20), h5);
	mark (ctx, '3', length * this . location (30), h5);
	mark (ctx, '4', length * this . location (40), h5);
	mark (ctx, '5', length * this . location (50), h5);
	mark (ctx, '6', length * this . location (60), h5);
	mark (ctx, '7', length * this . location (70), h5);
	mark (ctx, '8', length * this . location (80), h5);
	mark (ctx, '9', length * this . location (90), h5);
	mark (ctx, '100', length * this . location (100), h5);
	tick (ctx, length * this . location (110), h5);
	tick (ctx, length * this . location (120), h5);
	tick (ctx, length * this . location (130), h5);
	tick (ctx, length * this . location (140), h5);
	mark (ctx, '150', length * this . location (150), h5);
	tick (ctx, length * this . location (160), h5);
	tick (ctx, length * this . location (170), h5);
	tick (ctx, length * this . location (180), h5);
	tick (ctx, length * this . location (190), h5);
	mark (ctx, '2', length * this . location (200), h5);
};
