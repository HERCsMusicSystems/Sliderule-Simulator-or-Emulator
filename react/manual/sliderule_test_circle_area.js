
var circleAreaTests = {};

circleAreaTests ['Area from radius test'] = function (message) {
  var r = 0;
  while (r <= 1 || r > 5.6) r = crnu (1 + Math . floor (Math . random () * 111) / 10);
  var area = crnu (Math . PI * r * r, 5);
  message ("The task: r = " + r + ", find the area of the circle.");
  sliderules . objective = function () {
    if (checkValue ('A', area) && checkValue ('B', Math . PI)) {
      message ("Mission accomplished! area = " + area);
      increaseCookieResult ('Area from radius test');
      return true;
    }
    return false;
  };
};

circleAreaTests ['Area from diameter test'] = function (message) {
  var d = 0;
  while (d <= 1 || d > 10) d = crnu (1 + Math . floor (Math . random () * 111) / 10);
  var area = crnu (d * d * Math . PI / 4, 5);
  message ("The task: diameter d = " + d + ", find the area of the circle.");
  sliderules . objective = function () {
    if (checkValue ('A', d * d)) {
      message ("Mission accomplished! area = " + area);
      increaseCookieResult ('Area from diameter test');
      return true;
    }
    return false;
  };
};

circleAreaTests ['Diameter from area test'] = function (message) {
  var area = 0;
  while (area <= 1 || area > 78) area = crnu (1 + Math . floor (Math . random () * 111));
  var d = crnu (Math . sqrt (area * 4 / Math . PI), 5);
  message ("The task: area = " + area + ", find the diameter of the circle.");
  sliderules . objective = function () {
    if (checkValue ('D', Math . sqrt (area))) {
      message ("Mission accomplished! diameter = " + d);
      increaseCookieResult ('Diameter from area test');
      return true;
    }
    return false;
  };
};

slideruleLessons . push (circleAreaTests);
