
var circleAreaTests = {};

circleAreaTests ['Area from radius test'] = function (message) {
  var r = 0;
  while (r <= 1 || r > 5.6) r = crnu (1 + Math . floor (Math . random () * 111) / 10);
  var area = crnu (Math . PI * r * r, 5);
  message ("The task: r = " + r + ", find the area of the circle.");
  sliderules . objective = function () {
    if (checkValue ('A', area) && checkValue ('B', Math . PI)) {
      message ("Mission accomplished!");
      increaseCookieResult ('Area from radius test');
      return true;
    }
    return false;
  };
};

slideruleLessons . push (circleAreaTests);
