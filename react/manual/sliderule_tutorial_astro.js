
var astroLessons = {};

astroLessons ['Calculate parsec'] = function (message) {
	var rt = crnu (60 * 60 * 180 / Math . PI, 0);
	var auf = 1.495978707;
	var aum = 149.5978707;
	var pc = crnu (149.5978707 * 60 * 60 * 180 / Math . PI, 2);
	return [
		{action: function () {ensureSide (['C', 'D', 'CF', 'DF', 'CIF']); isolate (['C', 'D', 'CF', 'DF', 'CIF']); changeMarkings ('hairline', true); dimmm (255, 40, 8);}, delay: 100},
		{action: function () {message ("The task: calculate the distance of one parsec in kilometers.");}, delay: 500},
		{action: function () {message ("One parsec is the distance from wich one Astronomical Unit (distance from Earth to Sun = 149,597,870.7 km)");}, delay: 3000},
		{action: function () {message (".... is perceived as one arcsecond.");}, delay: 2000},
		{action: function () {message ("As tangent of one arcsecond can be approximated as \u03c0 / (180 * 60 * 60)....");}, delay: 5000},
		{action: function () {message (".... one parsec is equal to one AU multiplied by reciprocal of the tangent of one arcsecond.");}, delay: 2000},
		{action: function () {message ("1 pc = (180 * 60 * 60) / \u03c0 * " + aum + " million kilometers.");}, delay: 3000},
		{action: function () {message ("First multiply 60 * 60. Move the cursor to 0.6 on the DF scale.");}, delay: 4000},
		{action: function () {cursorTo ('DF', 0.6);}, delay: 1000},
		{action: function () {message ("Align 0.6 on the CIF scale with the cursor.");}, delay: 3000},
		{action: function () {slideTo ('CIF', 0.6);}, delay: 1000},
		{action: function () {message (".... and move cursor to 1 on the CF scale.");}, delay: 3000},
		{action: function () {cursorTo ('CF', 1);}, delay: 1000},
		{action: function () {message ("The result so far (after adjusting the decimal point) is: 3600 (on the DF scale).");}, delay: 3000},
		{action: function () {message ("Now multiply this result by 180 / \u03c0. This can be obtained by moving the cursor to the R gauge mark on the C scale.");}, delay: 4000},
		{action: function () {cursorTo ('C', 18 / Math . PI);}, delay: 1000},
		{action: function () {message ("After adjusting the decimal point you can read the result of " + rt + " on the D scale.");}, delay: 3000},
		{action: function () {message ("Now, multiply " + rt + " by Astronomical Unit (" + aum + " million kilometers).");}, delay: 3000},
		{action: function () {message ("Align 1 on the C scale with the cursor.");}, delay: 1000},
		{action: function () {slideTo ('C', 1);}, delay: 1000},
		{action: function () {message ("Move the cursor to " + crnu (auf, 5) + " on the C scale.");}, delay: 3000},
		{action: function () {cursorTo ('C', auf);}, delay: 1000},
		{action: function () {message ("Adjust the decimal point and read the result on the D scale. 1 parsec = " + pc + " million kilometers.");}, delay: 3000},
		{action: function () {cursorTo ('D', 1); slideTo ('D', 1); isolate (); dimmm (40, 255, 8);
		sliderules . objective = function () {
			if (checkValue ('D', pc / 10000000) && checkValue  ('C', auf)) {
				message ("Mission accomplished! 1 parsec = " + pc + " million kilometers.");
				increaseCookieResult ('Calculate parsec');
				return true;
			}
			return false;
		};}, delay: 6000},
		{action: function () {message ("Try these instructions again.");}, delay: 1000}
	];
};

slideruleLessons . push (astroLessons);

