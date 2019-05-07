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
// Pressure_KgPerCm2(_down)
// Pressure_KgPerCm2(_down)
// Pressure_PSIG_InHgVac(_down)
// Tw(_down)
// M1 M2
// adiabatic(_down)
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

var vapour_pressures_of_water = [
	0.6112, 0.7060, 0.8135, 0.9353, 1.0729, 1.2281, 1.4027, 1.5989, 1.8187, 2.0646,
	2.3392, 2.6452, 2.9857, 3.3638, 3.7809, 4.2452, 4.7582, 5.3240, 5.9472, 6.6324,
	7.3848, 8.2096, 9.1126, 10.100, 11.117, 12.352, 13.632, 15.023, 16.534, 18.173,
	19.948, 21.869, 23.946, 26.188, 28.605, 31.210, 34.013, 37.010, 40.240, 43.704,
	47.416, 51.388, 55.636, 60.174, 65.018, 70.183, 75.685, 81.542, 87.771, 94.390,
	101.42, 108.87, 116.78, 125.15, 134.01, 143.38, 153.28, 163.74, 174.77, 186.41,
	198.67, 211.59, 225.18, 239.47, 254.49, 270.28, 286.84, 304.22, 322.44, 341.54,
	361.53, 382.46, 404.36, 427.25, 451.17, 476.16, 502.24, 529.45, 557.83, 587.41,
	618.23, 650.32, 683.73, 718.48, 754.62, 792.18, 831.22, 871.75, 913.84, 957.51,
	1002.8, 1049.8, 1098.5, 1148.9, 1201.2, 1255.2, 1311.2, 1369.1, 1429.0, 1490.9,
	1554.9, 1621.0, 1689.3, 1759.8, 1832.6, 1907.7, 1985.1, 2065.0, 2147.3, 2232.2,
	2319.6, 2422.34, 2525.08, 2627.82, 2730.56, 2833.3, 2936.04, 3038.78, 3141.52, 3244.26,
	3347.0, 3481.53, 3616.06, 3750.59, 3885.12, 4019.65, 4154.18, 4288.71, 4423.24, 4557.77,
	4692.3, 4864.73, 5037.16, 5209.59, 5382.02, 5554.45, 5726.88, 5899.31, 6071.74, 6244.17,
	6416.6, 6633.73, 6850.86, 7067.99, 7285.12, 7502.25, 7719.38, 7936.51, 8153.64, 8370.77,
	8587.9, 8857.51, 9127.12, 9396.73, 9666.34, 9935.95, 10205.56, 10475.17, 10744.78, 11014.39,
	11284, 11615.7, 11947.4, 12279.1, 12610.8, 12942.5, 13274.2, 13605.9, 13937.6, 14269.3,
	14601, 15007.5, 15414, 15820.5, 16227, 16633.5, 17040, 17446.5, 17853, 18259.5,
	18666, 19072.5, 19479, 19885.5, 20292, 20698.5, 21105, 21511.5, 21918, 22324.5,
	22731
];
for (var ind in vapour_pressures_of_water) vapour_pressures_of_water [ind] /= 100;
var ice_pressures_of_water = [];
for (var ind = 0; ind <= 30; ind++) ice_pressures_of_water . push (pressure (- ind));
var press = function (T) {
	if (T < 0) {
		if (T <= -30) return ice_pressures_of_water [30];
		T = - T;
		var ind = Math . floor (T); T -= ind;
		return ice_pressures_of_water [ind] * (1 - T) + ice_pressures_of_water [ind + 1] * T;
	}
	T *= 0.5;
	if (T > 380) return vapour_pressures_of_water [190];
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
			return ((ind - 1) * TL + ind * T) * 2;
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
scale_Tw . prototype . drawTw = function (ctx, length, height) {
	var h5 = height * 0.5, h4 = height * 0.4, h3 = height * 0.3, h2 = height * 0.2;
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
	mark (ctx, '3', length * this . location (300), h5);
	draw_XR (ctx, this . location, length, 200, 370, 1, h4, 100, 50, 100);
	draw_XR (ctx, this . location, length, 200, 350, 1, h3, 50, 10, 50);
	draw_XR (ctx, this . location, length, 200, 370, 1, h2, 10, 2, 10);
	tick (ctx, length * this . location (360), h3);
	tick (ctx, length * this . location (370), h3);
	tick (ctx, length * this . location (372), h2);
	mark (ctx, '374.16', length * this . location (374.16), h5);
};
scale_Tw . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); this . drawTw (ctx, length, this . height);};

var scale_Tw_down = function (height, options) {scale_Tw . call (this, height, options);}; inherit (scale_Tw_down, scale_Tw);
scale_Tw_down . prototype . draw = function (ctx, length) {this . drawTw (ctx, length, - this . height);};

var scale_M1 = function (height, options) {spacer . call (this, height, options);}; inherit (scale_M1, spacer);
scale_M1 . prototype . value_m1 = function (location) {
	var value = Math . pow (10, location * 8 - 5.16);
	return value / (1 + value);
};
scale_M1 . prototype . value_log = function (location) {return Math . pow (10, location * 8);};
scale_M1 . prototype . value = function (location) {return location <= 0.25 ? this . value_log (location) : this . value_m1 (location);};
scale_M1 . prototype . location_m1 = function (value) {return (Math . log10 (value / (1 - value)) + 5.16) / 8;};
scale_M1 . prototype . location_log = function (value) {return Math . log10 (value) / 8;};
scale_M1 . prototype . location = function (value, location) {return location <= 0.25 ? this . location_log (value) : this . location_m1 (value);}
scale_M1 . prototype . draw_m1 = function (ctx, length, height) {
	var h5 = height * 0.5, h4 = height * 0.4, h3 = height * 0.3, h2 = height * 0.2;
	draw_XR (ctx, this . location_m1, length, 0.001, 0.01, 1, h2, 0.001, 0.0005, 0.001);
	draw_XR (ctx, this . location_m1, length, 0, 0.01, 1, h5, 0.005, 0.001, 0.005);
	draw_XR (ctx, this . location_m1, length, 0.01, 0.1, 1, h2, 0.01, 0.005, 0.01);
	draw_XR (ctx, this . location_m1, length, 0, 0.1, 1, h5, 0.05, 0.01, 0.05);
	draw_XR (ctx, this . location_m1, length, 0, 1, 1, h5, 0.5, 0.1, 0.5);
	draw_XR (ctx, this . location_m1, length, 0.1, 0.9, 1, h3, 0.1, 0.05, 0.1);
	draw_XR (ctx, this . location_m1, length, 0.1, 0.9, 1, h2, 0.05, 0.01, 0.05);
	draw_XR (ctx, this . location_m1, length, 0.9, 0.99, 1, h5, 0.1, 0.01, 0.1);
	draw_XR (ctx, this . location_m1, length, 0.9, 0.99, 1, h2, 0.01, 0.005, 0.01);
	draw_XR (ctx, this . location_m1, length, 0.99, 0.999, 2, h5, 0.01, 0.001, 0.01);
	draw_XR (ctx, this . location_m1, length, 0.99, 0.999, 1, h2, 0.001, 0.0005, 0.001);
	mmark (ctx, '.001', length * this . location (0.001), h5);
	mark (ctx, '5', length * this . location (0.005), h5);
	mmark (ctx, '.01', length * this . location (0.01), h5);
	mark (ctx, '5', length * this . location (0.05), h5);
	mmark (ctx, '.1', length * this . location (0.1), h5);
	mark (ctx, '5', length * this . location (0.5), h5);
	mmark (ctx, '.9', length * this . location (0.9), h5);
	mmark (ctx, '.95', length * this . location (0.95), h5);
	mmark (ctx, '.99', length * this . location (0.99), h5);
	mmark (ctx, '.995', length * this . location (0.995), h5);
	mmark (ctx, '.999', length * this . location (0.999), h5);
	ctx . fillStyle = this . marking_alt ? this . marking_alt : this . alt;
	draw_XR (ctx, this . location_log, length, 1, 2, 1, h4, 1, 0.5, 1);
	draw_XR (ctx, this . location_log, length, 1, 2, 1, h2, 0.5, 0.1, 0.5);
	draw_XR (ctx, this . location_log, length, 2, 4, 1, h2, 1, 0.2, 1);
	draw_XR (ctx, this . location_log, length, 4, 10, 1, h2, 1, 0.5, 1);
	mark (ctx, '1', 0, h5);
	mark (ctx, '2', length * this . location_log (2), h5);
	mark (ctx, '3', length * this . location_log (3), h5);
	mark (ctx, '4', length * this . location_log (4), h5);
	mark (ctx, '5', length * this . location_log (5), h5);
	tick (ctx, length * this . location_log (6), h5);
	tick (ctx, length * this . location_log (7), h5);
	tick (ctx, length * this . location_log (8), h5);
	tick (ctx, length * this . location_log (9), h5);
	draw_XR (ctx, this . location_log, length, 10, 20, 10, h4, 10, 5, 10);
	draw_XR (ctx, this . location_log, length, 10, 20, 10, h2, 5, 1, 5);
	draw_XR (ctx, this . location_log, length, 20, 40, 10, h2, 10, 2, 10);
	draw_XR (ctx, this . location_log, length, 40, 100, 10, h2, 10, 5, 10);
	mark (ctx, '1', length * this . location_log (10), h5);
	mark (ctx, '2', length * this . location_log (20), h5);
	mark (ctx, '3', length * this . location_log (30), h5);
	mark (ctx, '4', length * this . location_log (40), h5);
	mark (ctx, '5', length * this . location_log (50), h5);
	tick (ctx, length * this . location_log (60), h5);
	tick (ctx, length * this . location_log (70), h5);
	tick (ctx, length * this . location_log (80), h5);
	tick (ctx, length * this . location_log (90), h5);
	mark (ctx, '1', length * this . location_log (100), h5);
};
scale_M1 . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); this . draw_m1 (ctx, length, this . height);};
var scale_M2 = function (height, options) {scale_M1 . call (this, height, options);}; inherit (scale_M2, scale_M1);
scale_M2 . prototype . draw = function (ctx, length) {this . draw_m1 (ctx, length, - this . height);};

var scale_adiabatic = function (height, options) {spacer . call (this, height, options);}; inherit (scale_adiabatic, spacer);
scale_adiabatic . prototype . value_air = function (location) {return Math . pow (10, location * 6 * 0.285714286 + 2.1) - 273.16;};
scale_adiabatic . prototype . location_air = function (value) {return (Math . log10 (value + 273.16) - 2.12) / 6 / 0.285714286;};
scale_adiabatic . prototype . value_co2 = function (location) {return Math . pow (10, location * 6 * 0.2212 + 1.6) - 273.16;};
scale_adiabatic . prototype . location_co2 = function (value) {return (Math . log10 (value + 273.16) - 1.6) / 6 / 0.2212;};
scale_adiabatic . prototype . value = function (location) {return location <= 0.5 ? this . value_air (location) : this . value_co2 (location);};
scale_adiabatic . prototype . location = function (value, location) {return location <= 0.5 ? this . location_air (value) : this . location_co2 (value);};
scale_adiabatic . prototype . draw_adiabatic = function (ctx, length, height) {
	var h5 = height * 0.5, h4 = height * 0.4, h3 = height * 0.3, h2 = height * 0.2;
	draw_XR (ctx, this . location_air, length, -100, 200, 1, h4, 50, 10, 50);
	draw_XR (ctx, this . location_air, length, -100, 50, 1, h2, 10, 2, 10);
	draw_XR (ctx, this . location_air, length, 50, 200, 1, h2, 10, 5, 10);
	tick (ctx, length * this . location_air (150), h5);
	draw_XR (ctx, this . location_air, length, 200, 500, 1, h4, 100, 50, 100);
	draw_XR (ctx, this . location_air, length, 200, 500, 1, h2, 50, 10, 50);
	mark (ctx, '-100', length * this . location_air (-100), h5);
	mark (ctx, '-50', length * this . location_air (-50), h5);
	mark (ctx, '0', length * this . location_air (0), h5);
	mark (ctx, '50', length * this . location_air (50), h5);
	mark (ctx, '100', length * this . location_air (100), h5);
	mark (ctx, '2', length * this . location_air (200), h5);
	mark (ctx, '3', length * this . location_air (300), h5);
	mark (ctx, '4', length * this . location_air (400), h5);
	mark (ctx, '500', length * this . location_air (500), h5);
	draw_XR (ctx, this . location_co2, length, -90, -50, 1, h4, 40, 10, 40);
	draw_XR (ctx, this . location_co2, length, -50, 200, 1, h4, 50, 10, 50);
	draw_XR (ctx, this . location_co2, length, -90, 50, 1, h2, 10, 2, 10);
	draw_XR (ctx, this . location_co2, length, 50, 200, 1, h2, 10, 5, 10);
	tick (ctx, length * this . location_co2 (150), h5);
	draw_XR (ctx, this . location_co2, length, 200, 500, 2, h4, 100, 50, 100);
	draw_XR (ctx, this . location_co2, length, 200, 500, 2, h2, 50, 10, 50);
	mark (ctx, '-90', length * this . location_co2 (-90), h5);
	mark (ctx, '-50', length * this . location_co2 (-50), h5);
	mark (ctx, '0', length * this . location_co2 (0), h5);
	mark (ctx, '50', length * this . location_co2 (50), h5);
	mark (ctx, '100', length * this . location_co2 (100), h5);
	mark (ctx, '2', length * this . location_co2 (200), h5);
	mark (ctx, '3', length * this . location_co2 (300), h5);
	mark (ctx, '4', length * this . location_co2 (400), h5);
	mark (ctx, '500', length * this . location_co2 (500), h5);
};
scale_adiabatic . prototype . draw = function (ctx, length) {ctx . translate (0, this . height); this . draw_adiabatic (ctx, length, this . height);};

var scale_adiabatic_down = function (height, options) {scale_adiabatic . call (this, height, options);}; inherit (scale_adiabatic_down, scale_adiabatic);
scale_adiabatic_down . prototype . draw = function (ctx, length) {this . draw_adiabatic (ctx, length, - this . height);};

