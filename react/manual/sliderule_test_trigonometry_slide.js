
var trigonometrySlideTests = {};

trigonometrySlideTests ['Sine of an angle using slide test'] = function (message) {
  var angle = 1;
  while (angle > 80 || angle < 6) angle = crnu (Math . floor (Math . random () * 100));
  var sine = crnu (Math . sin (angle * Math . PI / 180), 6);
  message ("The task: find sine of " + angle + " degrees.");
  sliderules . objective = function () {
    if (checkValue ("S", angle)) {
      message ("Mission accomplished! sin(" + angle + "\u00b0) = " + sine);
      increaseCookieResult ('Sine of an angle using slide test');
      return true;
    }
    return false;
  };
};

trigonometrySlideTests ['Tangent of an angle using slide test'] = function (message) {
  var angle = 1;
  while (angle > 45 || angle < 6) angle = crnu (Math . floor (Math . random () * 100));
  var tangent = crnu (Math . tan (angle * Math . PI / 180), 6);
  message ("The task: find tangent of " + angle + " degrees.");
  sliderules . objective = function () {
    if (checkValue ("T", angle)) {
      message ("Mission accomplished! tan(" + angle + "\u00b0) = " + tangent);
      increaseCookieResult ('Tangent of an angle using slide test');
      return true;
    }
    return false;
  };
};

trigonometrySlideTests ['Angle of a sine using slide test'] = function (message) {
  var sine = 0;
  while (sine > 0.95 || sine < 0.1) sine = crnu (Math . floor (Math . random () * 100) / 100);
  var angle = crnu (Math . asin (sine) * 180 / Math . PI, 5);
  var sinec = crnu (sine * 10);
  message ("The task: find an angle, which sine is " + sine + ".");
  sliderules . objective = function () {
    if (checkValue ("C", sinec)) {
      message ("Mission accomplished! sin(" + angle + "\u00b0) = " + sine);
      increaseCookieResult ('Angle of a sine using slide test');
      return true;
    }
    return false;
  };
};

trigonometrySlideTests ['Angle of a tangent using slide test'] = function (message) {
  var tangent = 0;
  while (tangent > 0.95 || tangent < 0.1) tangent = crnu (Math . floor (Math . random () * 100) / 100);
  var angle = crnu (Math . atan (tangent) * 180 / Math . PI, 5);
  var tangentc = crnu (tangent * 10);
  message ("The task: find an angle, which tangent is " + tangent + ".");
  sliderules . objective = function () {
    if (checkValue ("C", tangentc)) {
      message ("Mission accomplished! tan(" + angle + "\u00b0) = " + tangent);
      increaseCookieResult ('Angle of a tangent using slide test');
      return true;
    }
    return false;
  };
};

trigonometrySlideTests ['Small angle to radians conversion using slide test'] = function (message) {
  var angle = 0;
  while (angle > 5.7 || angle < 1) angle = crnu (Math . floor (Math . random () * 100) / 10);
  var radians = crnu (angle * Math . PI / 180, 7);
  var sine = crnu (Math . sin (angle * Math . PI / 180), 5);
  var tangent = crnu (Math . tan (angle * Math . PI / 180), 5);
  message ("The task: convert " + angle + " degrees to radians.");
  sliderules . objective = function () {
    if (checkValue ('D', radians * 100) && checkValue ('C', angle)) {
      message ("Mission accomplished! " + angle + " degrees = " + radians + " radians.");
      increaseCookieResult ('Small angle to radians conversion using slide test');
      return true;
    }
    return false;
  };
};

trigonometrySlideTests ['Calculate a using slide test'] = function (message) {
  var a = 0, c = 0;
  while (a <= 1 || c <= 1 || a >= c || c > 10) {
    a = 1 + Math . floor (Math . random () * 9);
    c = 1 + Math . floor (Math . random () * 9);
  }
  var alpha = crnu (Math . asin (a / c) * 180 / Math . PI, 5);
  message ("The task: c = " + c + ", \u03b1 = " + alpha + ", find a.");
  sliderules . objective = function () {
    if (checkValue ('S', alpha) && checkValue ('D', a)) {
      message ("Mission accomplished! a = " + a);
      increaseCookieResult ('Calculate a using slide test');
      return true;
    }
    return false;
  };
};

trigonometrySlideTests ['Calculate c and \u03b1 using slide test'] = function (message) {
  var a = 0, b = 0, c = 0;
  while (a <= 1 || b <= 1 || a > b || c > 10) {
    a = 1 + Math . floor (Math . random () * 9);
    b = 1 + Math . floor (Math . random () * 9);
    c = crnu (Math . sqrt (a * a + b * b), 5);
  }
  var alpha = crnu (Math . atan (a / b) * 180 / Math . PI, 5);
  message ("The task: a = " + a + ", b = " + b + ", find c and \u03b1.");
  sliderules . objective = function () {
    if (checkValue ("S", alpha) && checkValue ("DI", c)) {
      message ("Mission accomplished! \u03b1 = " + alpha + "\u00b0 and c = " + c);
      increaseCookieResult ('Calculate c and \u03b1 using slide test');
      return true;
    }
    return false;
  };
};

slideruleLessons . push (trigonometrySlideTests);
