
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

astroTests ['Parsec to light year conversion test'] = function (message) {
  var au = 149597870.7;
  var parsec = au * 60 * 60 * 180 / Math . PI;
  var year = 60 * 60 * 24 * 365.25;
  var light_speed = 299792.458;
  var light_year = year * light_speed;
  var conversion = crnu (parsec / light_year, 5);
  var parsecs = 0;
  while (parsecs < 1 || parsecs > 10) parsecs = crnu (Math . floor (Math . random () * 100) / 10);
  message ("The task: convert " + parsecs + " parsecs to light years.");
  sliderules . objective = function () {
    if (checkValue ('CF', parsecs > 3 ? parsecs / 10 : parsecs) && checkValue ('D', conversion * parsecs * (parsecs > 3 ? 0.1 : 1))) {
      message ("Mission accomplished! " + parsecs + " parsecs = " + crnu (conversion * parsecs, parsecs > 3 ? 4 : 5) + " light years.");
      increaseCookieResult ('Parsec to light year conversion test');
      return true;
    }
    return false;
  };
};

slideruleLessons . push (astroTests);

