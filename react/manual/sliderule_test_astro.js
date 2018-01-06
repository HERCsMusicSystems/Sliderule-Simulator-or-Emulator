
var astroTests = {};

astroTests ['Calculate parsec test'] = function (message) {
	var rt = crnu (60 * 60 * 180 / Math . PI, 0);
	var auf = 1.495978707;
	var aum = 149.5978707;
	var pc = crnu (149.5978707 * 60 * 60 * 180 / Math . PI, 2);
	message ("The task: calculate the distance of one parsec in kilometers.");
	sliderules . objective = function () {
		if (checkValue ('D', pc / 10000000) && checkValue  ('C', auf)) {
			message ("Mission accomplished! 1 parsec = " + pc + " million kilometers.");
			increaseCookieResult ('Calculate parsec test');
			return true;
		}
		return false;
	};
};

slideruleLessons . push (astroTests);

