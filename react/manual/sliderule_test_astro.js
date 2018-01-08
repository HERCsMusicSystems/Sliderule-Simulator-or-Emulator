
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

astroTests ['Calculate light year test'] = function (message) {
  var speed_of_light = 299792458;
  var year = 60 * 60 * 24 * 365.25;
  message ("The task: calculate the length of one light year in kilometers.");
  sliderules . objective = function () {
    if (checkValue ('C', 2.99792) && checkValue ('D', 9.46073)) {
      message ("Mission accomplished! One light year = 9,460,730,000,000 kilometers.");
      increaseCookieResult ('Calculate light year test');
      return true;
    }
    return false;
  };
};

slideruleLessons . push (astroTests);

